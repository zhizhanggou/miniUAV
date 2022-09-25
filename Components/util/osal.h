#ifndef _OSAL_H_
#define _OSAL_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#define OS_DELAY(x) vTaskDelay(x)
#define OS_ENTER_CRITICAL() taskENTER_CRITICAL()
#define OS_EXIT_CRITICAL() taskEXIT_CRITICAL()

#define OS_ENTER_CRITICAL_ISR() taskENTER_CRITICAL_FROM_ISR()
#define OS_EXIT_CRITICAL_ISR(x) taskEXIT_CRITICAL_FROM_ISR(x)

#define OS_DISABLE_INTERRUPTS() taskDISABLE_INTERRUPTS()
#define OS_ENABLE_INTERRUPTS() taskENABLE_INTERRUPTS()

#define OS_MALLOC(size) pvPortMalloc(size)
#define OS_FREE(ptr) vPortFree(ptr)

#ifdef __cplusplus
}
#endif
#endif