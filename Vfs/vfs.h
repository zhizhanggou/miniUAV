#ifndef __VFS_H__
#define __VFS_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "err.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define VFS_PATH_MAX 20
#define VFS_MAX_COUNT 64
#define MAX_FDS 1024
typedef struct
{
    int flags; /*!< ESP_VFS_FLAG_CONTEXT_PTR or ESP_VFS_FLAG_DEFAULT */
    union {
        ssize_t (*write_p)(void* p, int fd, const void* data, size_t size); /*!< Write with context pointer */
        ssize_t (*write)(int fd, const void* data, size_t size);            /*!< Write without context pointer */
    };
    // union {
    //     off_t (*lseek_p)(void* p, int fd, off_t size, int mode);                                     /*!< Seek with context pointer */
    //     off_t (*lseek)(int fd, off_t size, int mode);                                                /*!< Seek without context pointer */
    // };
    union {
        ssize_t (*read_p)(void* ctx, int fd, void* dst, size_t size); /*!< Read with context pointer */
        ssize_t (*read)(int fd, void* dst, size_t size);              /*!< Read without context pointer */
    };
    // union {
    //     ssize_t (*pread_p)(void* ctx, int fd, void* dst, size_t size, off_t offset); /*!< pread with context pointer */
    //     ssize_t (*read)(int fd, void* dst, size_t size, off_t offset);               /*!< pread without context pointer */
    // };
    // union {
    //     ssize_t (*pwrite_p)(void* ctx, int fd, const void* src, size_t size, off_t offset); /*!< pwrite with context pointer */
    //     ssize_t (*write)(int fd, const void* src, size_t size, off_t offset);               /*!< pwrite without context pointer */
    // };
    union {
        int (*open_p)(void* ctx, const char* path, int flags, int mode); /*!< open with context pointer */
        int (*open)(const char* path, int flags, int mode);              /*!< open without context pointer */
    };
    union {
        int (*close_p)(void* ctx, int fd); /*!< close with context pointer */
        int (*close)(int fd);              /*!< close without context pointer */
    };
    // union {
    //     int ( *fstat_p )( void* ctx, int fd, struct stat* st ); /*!< fstat with context pointer */
    //     int ( *fstat )( int fd, struct stat* st );              /*!< fstat without context pointer */
    // };
    // #ifdef CONFIG_VFS_SUPPORT_DIR
    //     union {
    //         int ( *stat_p )( void* ctx, const char* path, struct stat* st ); /*!< stat with context pointer */
    //         int ( *stat )( const char* path, struct stat* st );              /*!< stat without context pointer */
    //     };
    //     union {
    //         int ( *link_p )( void* ctx, const char* n1, const char* n2 ); /*!< link with context pointer */
    //         int ( *link )( const char* n1, const char* n2 );              /*!< link without context pointer */
    //     };
    //     union {
    //         int ( *unlink_p )( void* ctx, const char* path ); /*!< unlink with context pointer */
    //         int ( *unlink )( const char* path );              /*!< unlink without context pointer */
    //     };
    //     union {
    //         int ( *rename_p )( void* ctx, const char* src, const char* dst ); /*!< rename with context pointer */
    //         int ( *rename )( const char* src, const char* dst );              /*!< rename without context pointer */
    //     };
    //     union {
    //         DIR* ( *opendir_p )( void* ctx, const char* name ); /*!< opendir with context pointer */
    //         DIR* ( *opendir )( const char* name );              /*!< opendir without context pointer */
    //     };
    //     union {
    //         struct dirent* ( *readdir_p )( void* ctx, DIR* pdir ); /*!< readdir with context pointer */
    //         struct dirent* ( *readdir )( DIR* pdir );              /*!< readdir without context pointer */
    //     };
    //     union {
    //         int ( *readdir_r_p )( void* ctx, DIR* pdir, struct dirent* entry, struct dirent** out_dirent ); /*!< readdir_r with context pointer */
    //         int ( *readdir_r )( DIR* pdir, struct dirent* entry, struct dirent** out_dirent );              /*!< readdir_r without context pointer */
    //     };
    //     union {
    //         long ( *telldir_p )( void* ctx, DIR* pdir ); /*!< telldir with context pointer */
    //         long ( *telldir )( DIR* pdir );              /*!< telldir without context pointer */
    //     };
    //     union {
    //         void ( *seekdir_p )( void* ctx, DIR* pdir, long offset ); /*!< seekdir with context pointer */
    //         void ( *seekdir )( DIR* pdir, long offset );              /*!< seekdir without context pointer */
    //     };
    //     union {
    //         int ( *closedir_p )( void* ctx, DIR* pdir ); /*!< closedir with context pointer */
    //         int ( *closedir )( DIR* pdir );              /*!< closedir without context pointer */
    //     };
    //     union {
    //         int ( *mkdir_p )( void* ctx, const char* name, mode_t mode ); /*!< mkdir with context pointer */
    //         int ( *mkdir )( const char* name, mode_t mode );              /*!< mkdir without context pointer */
    //     };
    //     union {
    //         int ( *rmdir_p )( void* ctx, const char* name ); /*!< rmdir with context pointer */
    //         int ( *rmdir )( const char* name );              /*!< rmdir without context pointer */
    //     };
    // #endif  // CONFIG_VFS_SUPPORT_DIR
    union {
        int (*ioctl_p)(void* ctx, int fd, int cmd, va_list args); /*!< ioctl with context pointer */
        int (*ioctl)(int fd, int cmd, va_list args);              /*!< ioctl without context pointer */
    };

} vfs_t;

typedef struct vfs_entry_
{
    vfs_t  vfs;                        // contains pointers to VFS functions
    char   path_prefix[VFS_PATH_MAX];  // path prefix mapped to this VFS
    size_t path_prefix_len;            // micro-optimization to avoid doing extra strlen
    void*  ctx;                        // optional pointer which can be passed to VFS
    int    offset;                     // index of this structure in s_vfs array
} vfs_entry_t;

err_t vfs_init();
err_t vfs_register(const char* base_path, const vfs_t* vfs, void* ctx);
void  vfs_list(void);

#ifdef __cplusplus
}
#endif

#endif