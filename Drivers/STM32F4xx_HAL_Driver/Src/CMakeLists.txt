file(GLOB_RECURSE srcs
    stm32f4xx_hal_cortex.c
    stm32f4xx_hal_dma_ex.c
    stm32f4xx_hal_dma.c
    stm32f4xx_hal_exti.c
    stm32f4xx_hal_flash_ex.c
    stm32f4xx_hal_flash_ramfunc.c
    stm32f4xx_hal_flash.c
    stm32f4xx_hal_gpio.c
    stm32f4xx_hal_i2c_ex.c
    stm32f4xx_hal_i2c.c
    stm32f4xx_hal_pwr_ex.c
    stm32f4xx_hal_pwr.c
    stm32f4xx_hal_rcc_ex.c
    stm32f4xx_hal_rcc.c
    stm32f4xx_hal_tim_ex.c
    stm32f4xx_hal_pwr.c
    stm32f4xx_hal_tim.c
    stm32f4xx_hal_uart.c
    stm32f4xx_hal.c
)

set(incs
    ../Inc
)

target_sources(trochilus_interface INTERFACE
    ${srcs}
)

target_include_directories(trochilus_interface INTERFACE
    ${incs}
)