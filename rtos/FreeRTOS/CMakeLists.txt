file(GLOB_RECURSE srcs
    *.c
)

set(incs
    lib/
    lib/include
    lib/include/FreeRTOS_POSIX
    lib/include/private
    lib/FreeRTOS-Plus-POSIX/include
    lib/FreeRTOS-Plus-POSIX/include/portable/st/stm32f411_v1
    lib/FreeRTOS-Plus-POSIX/include/portable/

    Source/CMSIS_RTOS_V2
    Source/include
    Source/portable/GCC/ARM_CM4F

)

ex_add_library(
    LIBNAME FreeRTOS
    SRCS ${srcs}
    INCLUDES ${incs}
)