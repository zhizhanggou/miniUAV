set(VFS_PATH ${CMAKE_SOURCE_DIR}/vfs)
set(SOURCES 
    ${VFS_PATH}/vfs.c
    # ${VFS_PATH}/vfs_uart.c
    # ${VFS_PATH}/vfs_console.c
)



set(VFS_INCLUDE_DIRS
    ../Inc
)

add_library(vfslib ${SOURCES})

target_include_directories(vfslib PUBLIC
    ${VFS_INCLUDE_DIRS}    
)



# Some newlib syscalls are implemented in vfs.c, make sure these are always
# seen by the linker
# target_link_libraries(vfslib INTERFACE "-u vfs_include_syscalls_impl")
