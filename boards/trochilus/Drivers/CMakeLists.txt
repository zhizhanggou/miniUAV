file(GLOB_RECURSE srcs
    *.c
    *.s
)

set(incs
    STM32F4xx_HAL_Driver/Inc
    STM32F4xx_HAL_Driver/Inc/Legacy
    CMSIS/Device/ST/STM32F4xx/Include
    CMSIS/Include
)

ex_add_library(
    LIBNAME Drivers
    SRCS ${srcs}
    INCLUDES ${incs}
)