set(SOURCES 
    # ${VFS_PATH}/vfs.c
    ringbuffer.c
)



# set(INCLUDE_DIRS
#     ./
# )

add_library(common ${SOURCES})

target_include_directories(common PUBLIC
    ${INCLUDE_DIRS}    
)



# Some newlib syscalls are implemented in vfs.c, make sure these are always
# seen by the linker
# target_link_libraries(vfslib INTERFACE "-u vfs_include_syscalls_impl")
