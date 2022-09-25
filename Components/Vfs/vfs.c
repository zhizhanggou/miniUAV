#include "vfs.h"
#include "FreeRTOS.h"
#include <semphr.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define CHECK_AND_CALL(ret, pvfs, func, ...) \
    do{                                      \
        if(pvfs->vfs.func == NULL) {         \
            return -1;                       \
        }                                    \
        ret = (*pvfs->vfs.func)(__VA_ARGS__);\
    } while(0)


#define CHECK_OR_CALL(ret, pvfs, func, ...)         \
    do{                                             \
        if(pvfs->vfs.func != NULL) {                \
            ret = (*pvfs->vfs.func)(__VA_ARGS__);   \
        }                                           \
    } while(0)

#define FD_TABLE_ENTRY_UNUSED                                                                                        \
    (fd_table_t)                                                                                                     \
    {                                                                                                                \
        .permanent = false, .has_pending_close = false, .has_pending_select = false, .vfs_index = -1, .local_fd = -1 \
    }
#define LEN_PATH_PREFIX_IGNORED __SIZE_MAX__

typedef int8_t  vfs_index_t;
typedef uint8_t local_fd_t;
typedef struct
{
    bool        permanent : 1;
    bool        has_pending_close : 1;
    bool        has_pending_select : 1;
    uint8_t     _reserved : 5;
    vfs_index_t vfs_index;
    local_fd_t  local_fd;
} fd_table_t;

static vfs_entry_t*      s_vfs[VFS_MAX_COUNT] = { 0 };
static size_t            s_vfs_count          = 0;
static fd_table_t        s_fd_table[MAX_FDS]  = { [0 ... MAX_FDS - 1] = FD_TABLE_ENTRY_UNUSED };
static SemaphoreHandle_t s_fd_table_lock;

err_t vfs_init()
{
    s_fd_table_lock = xSemaphoreCreateCounting(1, 1);
    if(s_fd_table_lock == NULL) {
        return FAIL;
    }
    return OK;
}

err_t vfs_register_common(const char* base_path, size_t len, const vfs_t* vfs, void* ctx, int* vfs_index)
{

    /* empty prefix is allowed, "/" is not allowed */
    if((len <= 1) || (len > VFS_PATH_MAX)) {
        return ERR_INVALID_ARG;
    }
    /* prefix has to start with "/" and not end with "/" */
    if(len >= 2 && ((base_path[0] != '/') || (base_path[len - 1] == '/'))) {
        return ERR_INVALID_ARG;
    }

    vfs_entry_t* entry = ( vfs_entry_t* )malloc(sizeof(vfs_entry_t));
    if(entry == NULL) {
        return ERR_NO_MEM;
    }
    size_t index;
    for(index = 0; index < s_vfs_count; ++index) {
        if(s_vfs[index] == NULL) {
            break;
        }
    }
    if(index == s_vfs_count) {
        if(s_vfs_count >= VFS_MAX_COUNT) {
            free(entry);
            return ERR_NO_MEM;
        }
        ++s_vfs_count;
    }
    s_vfs[index] = entry;

    strcpy(entry->path_prefix, base_path);  // we have already verified argument length

    memcpy(&entry->vfs, vfs, sizeof(vfs_t));
    entry->path_prefix_len = len;
    entry->ctx             = ctx;
    entry->offset          = index;

    if(vfs_index) {
        *vfs_index = index;
    }

    return OK;
}

err_t vfs_register(const char* base_path, const vfs_t* vfs, void* ctx)
{
    return vfs_register_common(base_path, strlen(base_path), vfs, ctx, NULL);
}

const vfs_entry_t* get_vfs_for_index(int index)
{
    if(index < 0 || index >= s_vfs_count) {
        return NULL;
    }
    else {
        return s_vfs[index];
    }
}

static inline bool fd_valid(int fd)
{
    return (fd < MAX_FDS) && (fd >= 0);
}

static const vfs_entry_t* get_vfs_for_fd(int fd)
{
    const vfs_entry_t* vfs = NULL;
    if(fd_valid(fd)) {
        const int index = s_fd_table[fd].vfs_index;  // single read -> no locking is required
        vfs             = get_vfs_for_index(index);
    }
    return vfs;
}

static inline int get_local_fd(const vfs_entry_t* vfs, int fd)
{
    int local_fd = -1;

    if(vfs && fd_valid(fd)) {
        local_fd = s_fd_table[fd].local_fd;  // single read -> no locking is required
    }

    return local_fd;
}

static const char* translate_path(const vfs_entry_t* vfs, const char* src_path)
{
    assert(strncmp(src_path, vfs->path_prefix, vfs->path_prefix_len) == 0);
    if(strlen(src_path) == vfs->path_prefix_len) {
        // special case when src_path matches the path prefix exactly
        return "/";
    }
    return src_path + vfs->path_prefix_len;
}

const vfs_entry_t* get_vfs_for_path(const char* path)
{
    const vfs_entry_t* best_match            = NULL;
    ssize_t            best_match_prefix_len = -1;
    size_t             len                   = strlen(path);
    for(size_t i = 0; i < s_vfs_count; ++i) {
        const vfs_entry_t* vfs = s_vfs[i];
        if(!vfs || vfs->path_prefix_len == LEN_PATH_PREFIX_IGNORED) {
            continue;
        }
        // match path prefix
        if(len < vfs->path_prefix_len || memcmp(path, vfs->path_prefix, vfs->path_prefix_len) != 0) {
            continue;
        }
        // this is the default VFS and we don't have a better match yet.
        if(vfs->path_prefix_len == 0 && !best_match) {
            best_match = vfs;
            continue;
        }
        // if path is not equal to the prefix, expect to see a path separator
        // i.e. don't match "/data" prefix for "/data1/foo.txt" path
        if(len > vfs->path_prefix_len && path[vfs->path_prefix_len] != '/') {
            continue;
        }
        // Out of all matching path prefixes, select the longest one;
        // i.e. if "/dev" and "/dev/uart" both match, for "/dev/uart/1" path,
        // choose "/dev/uart",
        // This causes all s_vfs_count VFS entries to be scanned when opening
        // a file by name. This can be optimized by introducing a table for
        // FS search order, sorted so that longer prefixes are checked first.
        if(best_match_prefix_len < ( ssize_t )vfs->path_prefix_len) {
            best_match_prefix_len = ( ssize_t )vfs->path_prefix_len;
            best_match            = vfs;
        }
    }
    return best_match;
}

int vfs_open(const char* path, int flags, int mode)
{
    const vfs_entry_t* vfs = get_vfs_for_path(path);
    if(vfs == NULL) {
        return FAIL;
    }
    const char* path_within_vfs = translate_path(vfs, path);
    int         fd_within_vfs;
    CHECK_OR_CALL(fd_within_vfs, vfs, open, path_within_vfs, flags, mode);
    if(fd_within_vfs >= 0) {
        if(xSemaphoreTake(s_fd_table_lock, portMAX_DELAY) != pdPASS) {
            return FAIL;
        }
        for(int i = 0; i < MAX_FDS; ++i) {
            if(s_fd_table[i].vfs_index == -1) {
                s_fd_table[i].permanent = false;
                s_fd_table[i].vfs_index = vfs->offset;
                s_fd_table[i].local_fd  = fd_within_vfs;
                if(xSemaphoreGive(s_fd_table_lock) != pdPASS) {
                    return FAIL;
                }
                return i;
            }
        }
        if(xSemaphoreGive(s_fd_table_lock) != pdPASS) {
            return FAIL;
        }

        int ret;
        CHECK_OR_CALL(ret, vfs, close, fd_within_vfs);
        ( void )ret;  // remove "set but not used" warning
        return FAIL;
    }
    return FAIL;
}

int vfs_close(int fd)
{
    const vfs_entry_t* vfs      = get_vfs_for_fd(fd);
    const int          local_fd = get_local_fd(vfs, fd);
    if(vfs == NULL || local_fd < 0) {
        return FAIL;
    }
    int ret;
    CHECK_OR_CALL(ret, vfs, close, local_fd);

    if(xSemaphoreTake(s_fd_table_lock, portMAX_DELAY) != pdPASS) {
        return FAIL;
    }
    if(!s_fd_table[fd].permanent) {
        if(s_fd_table[fd].has_pending_select) {
            s_fd_table[fd].has_pending_close = true;
        }
        else {
            s_fd_table[fd] = FD_TABLE_ENTRY_UNUSED;
        }
    }
    if(xSemaphoreGive(s_fd_table_lock) != pdPASS) {
        return FAIL;
    }
    return ret;
}

ssize_t vfs_read(int fd, void* dst, size_t size)
{
    const vfs_entry_t* vfs      = get_vfs_for_fd(fd);
    const int          local_fd = get_local_fd(vfs, fd);
    if(vfs == NULL || local_fd < 0) {
        return FAIL;
    }
    ssize_t ret;
    CHECK_AND_CALL(ret, vfs, read, local_fd, dst, size);
    return ret;
}

ssize_t vfs_write(int fd, const void* src, size_t size)
{
    const vfs_entry_t* vfs      = get_vfs_for_fd(fd);
    const int          local_fd = get_local_fd(vfs, fd);
    if(vfs == NULL || local_fd < 0) {
        return FAIL;
    }
    ssize_t ret;
    CHECK_AND_CALL(ret, vfs, write, local_fd, src, size);
    return ret;
}

int esp_vfs_ioctl(int fd, int cmd, ...)
{
    const vfs_entry_t* vfs      = get_vfs_for_fd(fd);
    const int          local_fd = get_local_fd(vfs, fd);
    if(vfs == NULL || local_fd < 0) {
        return FAIL;
    }
    int     ret;
    va_list args;
    va_start(args, cmd);
    CHECK_AND_CALL(ret, vfs, ioctl, local_fd, cmd, args);
    va_end(args);
    return ret;
}

void vfs_list(void)
{
    for(size_t i = 0; i < s_vfs_count; ++i) {
        printf("%s\n", s_vfs[i]->path_prefix);
    }
}
