file(GLOB_RECURSE srcs
    *.c
    *.s
)
file(GLOB_RECURSE srcs_public
    Src/main.c
    Src/stm32f4xx_hal_msp.c
    Src/stm32f4xx_it.c
    Src/syscalls.c
    Src/sysmem.c
    Src/system_stm32f4xx.c
)

set(incs
    Inc
)

list(REMOVE_ITEM srcs ${srcs_public})

ex_add_library(
    LIBNAME core
    SRCS ${srcs}
    INCLUDES ${incs}
)

add_library(core_public INTERFACE)
target_sources(core_public INTERFACE ${srcs_public})
set_property(GLOBAL APPEND PROPERTY PROJECT_LIBRARIES core_public)