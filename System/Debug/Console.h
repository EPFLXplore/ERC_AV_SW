/*
 * console.h
 *
 *  Created on: 11 Feb 2020
 *      Author: AV Team 2020
 */

#ifndef APPLICATION_HOSTBOARD_INC_DEBUG_CONSOLE_H_
#define APPLICATION_HOSTBOARD_INC_DEBUG_CONSOLE_H_



#include <stdio.h>

#include <stm32h7xx_hal.h>
#include <cmsis_os.h>

#include "usart.h"


#define CONSOLE_BUFFER_SIZE 256
#define HARDWARE_SEMAPHORE 2

class Console {
public:
	Console(UART_HandleTypeDef* uart);
	void lock();
	void unlock();
	void print(const char* buffer);
	void printf(const char* format, ...);

private:
	UART_HandleTypeDef* console_uart;
	osSemaphoreId console_semaphore;

	char buffer[CONSOLE_BUFFER_SIZE];

	void transmit(uint8_t* buffer, uint32_t length);

};



#endif /* APPLICATION_HOSTBOARD_INC_DEBUG_CONSOLE_H_ */
