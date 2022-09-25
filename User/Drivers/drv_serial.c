#include "drv_serial.h"
// #include "board.h"
#include "vfs.h"
#include <fcntl.h>
#include <unistd.h>
#include "usart.h"

#define IS_IRQ_MASKED() ((__get_PRIMASK() != 0U) || ((__get_BASEPRI() != 0U)))
#define IS_IRQ_MODE() (__get_IPSR() != 0U)
#define IS_IRQ() (IS_IRQ_MODE() || IS_IRQ_MASKED())

static int serial_open(void* ctx, const char* path, int flags, int mode);
static int serial_write(void* ctx, int fd, const void* data, size_t size);

#if defined(CONFIG_USART1)
RINGBUF_DEF(m_usart1txringbuf, CONFIG_USART1_TXBUFSIZE);
static uint8_t m_usart1rxbuffer[CONFIG_USART1_RXBUFSIZE];
#endif

drv_serial m_serials[] = {
#if defined(CONFIG_USART1)
    { .usart.Instance          = &huart1,
      .usart.Init.BaudRate     = CONFIG_USART1_BAUDRATE,
      .usart.Init.WordLength   = UART_WORDLENGTH_8B,
      .usart.Init.StopBits     = UART_STOPBITS_1,
      .usart.Init.Parity       = UART_PARITY_NONE,
      .usart.Init.Mode         = UART_MODE_TX_RX,
      .usart.Init.HwFlowCtl    = UART_HWCONTROL_NONE,
      .usart.Init.OverSampling = UART_OVERSAMPLING_16,
      .tx_dma_ringbuf          = &m_usart1txringbuf,
      .tx_dma_size             = CONFIG_USART1_TXBUFSIZE,
      .tx_dma_busy             = false,
      .rx_dma_buffer           = m_usart1rxbuffer,
      .rx_dma_size             = CONFIG_USART1_RXBUFSIZE,
      .rx_dma_pos              = 0 }
#endif
};

static vfs_t m_serial_ops = {

    .write = serial_write, .read = NULL, .open = NULL, .close = NULL, .ioctl = NULL
};

static void uart_tx_callback(UART_HandleTypeDef* huart)
{
    // drv_serial* dev = NULL;
    // for(int i = 0; i < sizeof(m_serials) / sizeof(drv_serial); i++) {
    //     if(huart->Instance == m_serials[i].usart.Instance) {
    //         dev = &m_serials[i];
    //         break;
    //     }
    // }
    // if(dev != NULL) {
    //     xSemaphoreTake(dev->tx_sem, portMAX_DELAY);
    //     ringbuf_free(dev->tx_dma_ringbuf, dev->tx_dma_trans_size);

    //     dev->tx_dma_trans_size = dev->rx_dma_size;
    //     ringbuf_get(dev->tx_dma_ringbuf, &dev->tx_dma_trans_buf, &dev->tx_dma_trans_size, false);

    //     xSemaphoreGive(dev->tx_sem);
    // }
}

int drv_serial_init()
{
    char path[16] = { 0 };
    for(int i = 0; i < sizeof(m_serials) / sizeof(drv_serial); i++) {
        // vSemaphoreCreateBinary(m_serials[i].tx_sem);
        // if(m_serials[i].tx_sem == NULL) {
        //     return -1;
        // }

        // vSemaphoreCreateBinary(m_serials[i].rx_sem);
        // if(m_serials[i].rx_sem == NULL) {
        //     return -1;
        // }
        HAL_UART_Init(&(m_serials[i].usart));
        ringbuf_init(m_serials[i].tx_dma_ringbuf);
        HAL_UART_RegisterCallback(&m_serials[i].usart, HAL_UART_TX_COMPLETE_CB_ID, uart_tx_callback);

        if(m_serials[i].usart.Instance == &CONFIG_CONSOLE_UART) {
            vfs_register("/dev/console", &m_serial_ops, &m_serials[i]);
            vfs_open("/dev/console", O_RDONLY);
            vfs_open("/dev/console", O_WRONLY);
            // vfs_open("/dev/console", O_WRONLY);
            // vfs_write(STDOUT_FILENO, "hello world\n", 12);
            HAL_UART_Transmit_DMA(&m_serials[i].usart, "dev->tx_dma_trans_buf", sizeof("dev->tx_dma_trans_buf"));
        }
        else {
            sprintf(path, "/dev/ttyS%d", i);
            vfs_register(path, &m_serial_ops, &m_serials[i]);
        }
    }
    return 0;
}

// static int serial_open(void* ctx, const char* path, int flags, int mode)
// {
//     return 0;
// }

static int serial_write(void* ctx, int fd, const void* data, size_t size)
{
    BaseType_t  yield;
    drv_serial* dev      = ( drv_serial* )ctx;
    size_t      put_size = size;
    if(!dev->tx_dma_busy) {
        dev->tx_dma_busy = true;
    }

    // if(IS_IRQ()) {
    //     xSemaphoreTakeFromISR(dev->tx_sem, &yield);
    //     portYIELD_FROM_ISR(yield);
    // }
    // else {
    // xSemaphoreTake(dev->tx_sem, portMAX_DELAY);
    // }

    ringbuf_cpy_put(dev->tx_dma_ringbuf, ( uint8_t* )data, &put_size);
    if(put_size != size) {
    }

    if(!dev->tx_dma_busy) {
        dev->tx_dma_busy       = true;
        dev->tx_dma_trans_size = dev->rx_dma_size;
        ringbuf_get(dev->tx_dma_ringbuf, &dev->tx_dma_trans_buf, &dev->tx_dma_trans_size, false);

        // if(IS_IRQ()) {
        //     xSemaphoreGiveFromISR(dev->tx_sem, &yield);
        //     portYIELD_FROM_ISR(yield);
        // }
        // else {
        // xSemaphoreGive(dev->tx_sem);
        // }
    }
    else {
        // if(IS_IRQ()) {
        //     xSemaphoreGiveFromISR(dev->tx_sem, &yield);
        //     portYIELD_FROM_ISR(yield);
        // }
        // else {
        // xSemaphoreGive(dev->tx_sem);
        // }
    }

    // HAL_UART_Transmit_DMA(&dev->usart, dev->tx_dma_trans_buf, dev->tx_dma_trans_size);
    HAL_UART_Transmit_DMA(&dev->usart, "dev->tx_dma_trans_buf", sizeof("dev->tx_dma_trans_buf"));
    return size;
}