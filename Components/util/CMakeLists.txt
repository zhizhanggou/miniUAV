file(GLOB_RECURSE srcs
    *.c
)

ex_add_library(
    LIBNAME util
    SRCS ${srcs}
    DEPENDS Freertos
)

# Some newlib syscalls are implemented in vfs.c, make sure these are always
# seen by the linker
# target_link_libraries(vfslib INTERFACE "-u vfs_include_syscalls_impl")
