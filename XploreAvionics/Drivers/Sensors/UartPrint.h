#ifndef UART_PRINT_H_
#define UART_PRINT_H_

#include <stdio.h>
#include "usart.h"
#include "stm32h7xx_hal.h"

void printToUart(uint8_t* data, int size);

#endif /* SENSORS_BAROMETER_H_ */
