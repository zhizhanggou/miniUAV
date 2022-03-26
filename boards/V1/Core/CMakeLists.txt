file(GLOB_RECURSE srcs_public
    *.s
    *.c
)

set(incs
    Inc
)

list(REMOVE_ITEM srcs ${srcs_public})

ex_add_library(
    LIBNAME Core
    SRCS ${srcs_public}
    INCLUDES ${incs}
    DEPENDS Drivers
)

add_library(core_public INTERFACE)
target_sources(core_public INTERFACE ${srcs_public})
set_property(GLOBAL APPEND PROPERTY PROJECT_LIBRARIES_INTERFACE core_public)