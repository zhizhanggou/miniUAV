#ifndef __DRV_SERIAL_H
#define __DRV_SERIAL_H
#ifdef __cplusplus
extern "C" {
#endif
#include "osal.h"
#include "ringbuffer.h"
#include "stm32f4xx_hal.h"

typedef struct
{
    UART_HandleTypeDef usart;
    const ringbuf_t*   tx_dma_ringbuf;
    uint8_t*           tx_dma_trans_buf;
    size_t             tx_dma_trans_size;
    size_t             tx_dma_size;
    bool               tx_dma_busy;
    SemaphoreHandle_t  tx_sem;

    uint8_t*          rx_dma_buffer;
    size_t            rx_dma_size;
    size_t            rx_dma_pos;
    SemaphoreHandle_t rx_sem;

    bool is_initialized;
} drv_serial;

int drv_serial_init(void);
#ifdef __cplusplus
}
#endif
#endif