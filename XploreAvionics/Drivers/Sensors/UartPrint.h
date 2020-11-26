#ifndef UART_PRINT_H_
#define UART_PRINT_H_

#include <stdio.h>
#include "stm32h7xx_hal.h"

void printToUart(UART_HandleTypeDef* huart, uint8_t* data, int size);

#endif
