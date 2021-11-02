set(RTOS_PATH ${CMAKE_SOURCE_DIR}/FreeRTOS)

set(SOURCES
    ${RTOS_PATH}/croutine.c
    ${RTOS_PATH}/event_groups.c
    ${RTOS_PATH}/list.c
    ${RTOS_PATH}/queue.c
    ${RTOS_PATH}/stream_buffer.c
    ${RTOS_PATH}/tasks.c
    ${RTOS_PATH}/timers.c

    ${RTOS_PATH}/portable/GCC/ARM_CM4F/port.c
    ${RTOS_PATH}/portable/MemMang/heap_4.c
    ${RTOS_PATH}/CMSIS_RTOS_V2/cmsis_os2.c
    )

set(RTOS_INCLUDE_DIRS
    ${RTOS_PATH}/CMSIS_RTOS_V2
    ${RTOS_PATH}/include
    ${RTOS_PATH}/portable/GCC/ARM_CM4F
    ${CMAKE_SOURCE_DIR}/Lib/CMSIS/Include
)

add_library(freertos ${SOURCES})

target_include_directories(freertos PUBLIC
    ${RTOS_INCLUDE_DIRS}
)