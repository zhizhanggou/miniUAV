# set(target "src")
# set(INC_DIR ${CMAKE_SOURCE_DIR}/Inc)
# set(LIB_DIR ${CMAKE_SOURCE_DIR}/Lib)
# set(SRC_DIR ${CMAKE_SOURCE_DIR}/Src)
# set(STARTUP_DIR ${CMAKE_SOURCE_DIR}/Startup)
# set(TOOLS_DIR ${CMAKE_SOURCE_DIR}/tools)

# file(GLOB APP_SOURCE_FILES
#     ${SRC_DIR}/main.c
#     ${SRC_DIR}/stm32f4xx_hal_msp.c
#     ${SRC_DIR}/stm32f4xx_it.c
#     ${SRC_DIR}/syscalls.c
#     ${SRC_DIR}/sysmem.c
#     ${SRC_DIR}/system_stm32f4xx.c
    
#     ${STARTUP_DIR}/startup_stm32f411ceux.s

#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
# )
# message(STATUS "${APP_INCLUDE_DIRS}")
# set(APP_INCLUDE_DIRS
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy
#     ${LIB_DIR}/CMSIS/Include
#     ${LIB_DIR}/CMSIS/Device/ST/STM32F4xx/Include
#     ${LIB_DIR}/STM32F4xx_HAL_Driver/Inc
#     ${INC_DIR}
# )
# add_executable(${target} 
#     ${APP_SOURCE_FILES}
# )

# target_include_directories(${target} PUBLIC
#     ${APP_INCLUDE_DIRS}
# )




# set(LINKER_SCRIPT "${TOOLS_DIR}/STM32F411CEUX_FLASH.ld")
# set(link_flags
#     "-mcpu=cortex-m4"   
#     "--specs=nosys.specs"
#     "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${target}.map"
#     "-Wl,--gc-sections"
#     "-static" 
#     "--specs=nano.specs"
#     "-mfpu=fpv4-sp-d16"
#     "-mfloat-abi=hard" 
#     "-mthumb" 
#     "-Wl,--start-group -lc -lm -Wl,--end-group"
#     "-T${LINKER_SCRIPT}")
# string(REGEX REPLACE ";" " " link_flags ${link_flags})
# # message(${link_flags})
# #set(link_flags  "--specs=nosys.specs -Wl,-Map=test.map -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group")
# # target_link_options(${ELF_FILE} PUBLIC ${link_flags})
# set_target_properties(${target_name} PROPERTIES LINK_FLAGS ${link_flags})