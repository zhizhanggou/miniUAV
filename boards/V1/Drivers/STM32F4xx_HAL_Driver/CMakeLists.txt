file(GLOB_RECURSE SRCS
    Src/stm32f4xx_hal_cortex.c
    Src/stm32f4xx_hal_dma_ex.c
    Src/stm32f4xx_hal_dma.c
    Src/stm32f4xx_hal_exti.c
    Src/stm32f4xx_hal_flash_ex.c
    Src/stm32f4xx_hal_flash_ramfunc.c
    Src/stm32f4xx_hal_flash.c
    Src/stm32f4xx_hal_gpio.c
    Src/stm32f4xx_hal_i2c_ex.c
    Src/stm32f4xx_hal_i2c.c
    Src/stm32f4xx_hal_pwr_ex.c
    Src/stm32f4xx_hal_pwr.c
    Src/stm32f4xx_hal_rcc_ex.c
    Src/stm32f4xx_hal_rcc.c
    Src/stm32f4xx_hal_tim_ex.c
    Src/stm32f4xx_hal_pwr.c
    Src/stm32f4xx_hal_tim.c
    Src/stm32f4xx_hal_uart.c
    Src/stm32f4xx_hal.c
)

set(incs
    Inc
)
MESSAGE(${incs})
ex_add_library ( hal_driver 
     ${SRCS} 
     ${incs} 
)

# target_sources(trochilus_interface INTERFACE
#     ${srcs}
# )

# target_include_directories(trochilus_interface INTERFACE
#     ${incs}
# )