file(GLOB_RECURSE srcs
    *.c
)

set(incs
    Source/CMSIS_RTOS_V2
    Source/include
    Source/portable/GCC/ARM_CM4F
)

ex_add_library(
    LIBNAME FreeRTOS
    SRCS ${srcs}
    INCLUDES ${incs}
)