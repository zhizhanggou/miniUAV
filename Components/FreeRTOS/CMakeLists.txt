file(GLOB_RECURSE srcs
    croutine.c
    event_groups.c
    list.c
    queue.c
    stream_buffer.c
    tasks.c
    timers.c

    portable/GCC/ARM_CM4F/port.c
    portable/MemMang/heap_3.c
    CMSIS_RTOS_V2/cmsis_os2.c
)

set(incs
    CMSIS_RTOS_V2
    include
    portable/GCC/ARM_CM4F
    Lib/CMSIS/Include
)

ex_add_library(
    LIBNAME Freertos
    SRCS ${srcs}
    INCLUDES ${incs}
)