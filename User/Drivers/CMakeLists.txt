file(GLOB_RECURSE srcs
    drv_serial.c
)

ex_add_library(
    LIBNAME Drivers
    SRCS ${srcs}
)


# Some newlib syscalls are implemented in vfs.c, make sure these are always
# seen by the linker
# target_link_libraries(vfslib INTERFACE "-u vfs_include_syscalls_impl")
