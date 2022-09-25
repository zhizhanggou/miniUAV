set(ARCH arm)
set(CPU cortex-m4)

set(plotform stm32f411xx)

add_definitions(
    -DUSE_HAL_DRIVER
    -DSTM32F411xE
)