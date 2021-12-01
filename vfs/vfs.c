#include "vfs.h"
#include "err.h"
#include <stdlib.h>
#include <string.h>

static vfs_entry_t* s_vfs[VFS_MAX_COUNT] = { 0 };
static size_t       s_vfs_count          = 0;

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
