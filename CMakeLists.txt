cmake_minimum_required(VERSION 3.10)
set(CMAKE_SYSTEM_NAME  "Generic")
project(trochilus LANGUAGES NONE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


# find_program(GCC_ARM_C_COMPILER arm-none-eabi-gcc
#     PATHS "${DEFAULT_GCC_ARM_PATH}" ENV PATHS NO_DEFAULT_PATH)
# find_program(GCC_ARM_C_COMPILER arm-none-eabi-objcopy
#     PATHS "${DEFAULT_GCC_ARM_PATH}" ENV PATHS NO_DEFAULT_PATH)
# find_program(GCC_ARM_C_COMPILER arm-none-eabi-size
#     PATHS "${DEFAULT_GCC_ARM_PATH}" ENV PATHS NO_DEFAULT_PATH)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_CXX_FLAGS "-mcpu=cortex-m4 -std=gnu++11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb")
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb")
set(CMAKE_ASM_FLAGS "-mcpu=cortex-m4 -g3 -x assembler-with-cpp --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb")


set(INC_DIR ${CMAKE_SOURCE_DIR}/Inc)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/Lib)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/Src)
set(VFS_DIR ${CMAKE_SOURCE_DIR}/Vfs)
set(STARTUP_DIR ${CMAKE_SOURCE_DIR}/Startup)
set(TOOLS_DIR ${CMAKE_SOURCE_DIR}/tools)
set(BOARDS_DIR ${CMAKE_SOURCE_DIR}/boards)
set(DRIVERS_DIR ${CMAKE_SOURCE_DIR}/Drivers)
set(COMMON_DIR ${CMAKE_SOURCE_DIR}/common)

include(programmer)

enable_language(CXX C ASM)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(target "trochilus")

file(GLOB APP_SOURCE_FILES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/stm32f4xx_hal_msp.c
    ${SRC_DIR}/stm32f4xx_it.c
    ${SRC_DIR}/syscalls.c
    ${SRC_DIR}/sysmem.c
    ${SRC_DIR}/system_stm32f4xx.c
    
    ${STARTUP_DIR}/startup_stm32f411ceux.s

    ${DRIVERS_DIR}/drv_serial.c

    ${BOARDS_DIR}/trochilus_v1/board_config.c

    ${COMMON_DIR}/ringbuffer.c

    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    

)

add_executable(${target} 
    ${APP_SOURCE_FILES}
)

target_link_libraries(${target} PUBLIC freertos)
target_link_libraries(${target} PUBLIC vfslib)
set(APP_INCLUDE_DIRS
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy
    ${LIB_DIR}/CMSIS/Include
    ${LIB_DIR}/CMSIS/Device/ST/STM32F4xx/Include
    ${LIB_DIR}/STM32F4xx_HAL_Driver/Inc
    
    ${INC_DIR}

    ${VFS_DIR}
    ${BOARDS_DIR}
    ${DRIVERS_DIR}
    ${COMMON_DIR}
)

target_compile_definitions(${target} PUBLIC
    
)

target_include_directories(${target} PUBLIC
    ${APP_INCLUDE_DIRS}
)

set(LINKER_SCRIPT "${TOOLS_DIR}/STM32F411CEUX_FLASH.ld")
set(link_flags
    "-mcpu=cortex-m4"
    "-T${LINKER_SCRIPT}" 
    "--specs=nosys.specs"
    "-Wl,-Map=${target}.map"
    "-Wl,--gc-sections"
    "-static"
    "-mfpu=fpv4-sp-d16"
    "-mfloat-abi=hard"
    "-mthumb"
    "-Wl,--start-group -lc -lm -Wl,--end-group")
string(REGEX REPLACE ";" " " link_flags ${link_flags})

# set_target_properties(${target} PROPERTIES LINK_FLAGS ${link_flags})


set_target_properties(${target} PROPERTIES LINK_FLAGS "-mcpu=cortex-m4 -T${TOOLS_DIR}/STM32F411CEUX_FLASH.ld --specs=nosys.specs -Wl,-Map=${target}.map -Wl,--gc-sections -static -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group")


add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND arm-none-eabi-size ${CMAKE_BINARY_DIR}/${target}.elf
    COMMAND arm-none-eabi-objcopy -O binary -S ${CMAKE_BINARY_DIR}/${target}.elf ${CMAKE_BINARY_DIR}/${target}.bin
    COMMAND arm-none-eabi-objcopy -O ihex ${CMAKE_BINARY_DIR}/${target}.elf ${CMAKE_BINARY_DIR}/${target}.hex
    BYPRODUCTS ${CMAKE_BINARY_DIR}/${target}.hex
)

add_flash_target(${target})

add_subdirectory(FreeRTOS)
add_subdirectory(Vfs)