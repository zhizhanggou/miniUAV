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


set(SRC_DIR ${CMAKE_SOURCE_DIR}/Core/Src)
set(DRIVERS_DIR ${CMAKE_SOURCE_DIR}/Drivers)
set(CORE_DIR ${CMAKE_SOURCE_DIR}/Core)

include(jlink)

enable_language(CXX C ASM)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(target "trochilus")

file(GLOB APP_SOURCE_FILES
    ${SRC_DIR}/*.c
    ${CMAKE_SOURCE_DIR}/startup_stm32f411xe.s
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    # ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    
)

add_executable(${target} 
    ${APP_SOURCE_FILES}
)

# target_link_libraries(${target} PUBLIC freertos)
# target_link_libraries(${target} PUBLIC vfslib)
set(APP_INCLUDE_DIRS
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy
    ${DRIVERS_DIR}/CMSIS/Include
    ${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Include
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc
    
    ${CORE_DIR}/Inc
)

target_compile_definitions(${target} PUBLIC
    
)

target_include_directories(${target} PUBLIC
    ${APP_INCLUDE_DIRS}
)

# set_target_properties(${target} PROPERTIES LINK_FLAGS ${link_flags})


set_target_properties(${target} PROPERTIES LINK_FLAGS "-mcpu=cortex-m4 
-T${CMAKE_SOURCE_DIR}/STM32F411CEUx_FLASH.ld 
--specs=nosys.specs 
-Wl,-Map=${target}.map 
-Wl,--gc-sections 
-static 
-mfpu=fpv4-sp-d16 
-mfloat-abi=hard 
-mthumb 
-Wl,--start-group 
-lc 
-lm 
-Wl,--end-group")

add_library(trochilus_interface INTERFACE)
target_link_libraries(${target}
    trochilus_interface
    -Wl,-Map=${target}.map 
    )

add_subdirectory(Drivers)
add_flash_target(${target})




add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND arm-none-eabi-size ${CMAKE_BINARY_DIR}/${target}.elf
    COMMAND arm-none-eabi-objcopy -O binary -S ${CMAKE_BINARY_DIR}/${target}.elf ${CMAKE_BINARY_DIR}/${target}.bin
    COMMAND arm-none-eabi-objcopy -O ihex ${CMAKE_BINARY_DIR}/${target}.elf ${CMAKE_BINARY_DIR}/${target}.hex
    BYPRODUCTS ${CMAKE_BINARY_DIR}/${target}.hex
)

