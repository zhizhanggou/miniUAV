file(GLOB_RECURSE srcs
    vfs.c
)

set(incs
    Inc
)

ex_add_library(
    LIBNAME Vfs
    SRCS ${srcs}
    INCLUDES ${incs}
    DEPENDS Freertos
)

# Some newlib syscalls are implemented in vfs.c, make sure these are always
# seen by the linker
# target_link_libraries(vfslib INTERFACE "-u vfs_include_syscalls_impl")
