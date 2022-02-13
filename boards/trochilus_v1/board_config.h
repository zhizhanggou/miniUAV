#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

#define TROCHILUS_V1

#define CONFIG_USART1
#define CONFIG_USART1_BAUDRATE 115200
#define CONFIG_USART1_TXBUFSIZE 1024
#define CONFIG_USART1_RXBUFSIZE 256

#define CONFIG_CONSOLE_UART USART1

void board_init(void);
void clock_init(void);

#ifdef __cplusplus
}
#endif

#endif