/*
 * console.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Arion
 */

#include "Debug/Console.h"

#include <stdarg.h>
#include <string.h>
#include <stm32h7xx_hal.h>
#include <cmsis_os2.h>




Console::Console(UART_HandleTypeDef* uart) : console_uart(uart) {
	console_uart = uart;

	console_semaphore = osSemaphoreNew(256, 1, nullptr);

	printf("\x1b[2J\x1b[H");
}

void Console::lock() {
	osSemaphoreWait(console_semaphore, 100 * portTICK_PERIOD_MS);
}

void Console::unlock() {
	osSemaphoreRelease(console_semaphore);
}

void Console::transmit(uint8_t* buffer, uint32_t length) {
	__disable_irq();
	HAL_UART_Transmit(console_uart, buffer, length, 0xFFFFFF);
	__enable_irq();
}

int Console::printf(const char *format, ...) {
	va_list args;
	va_start(args, format);

	if(vsprintf(buffer, format, args) > 0) {
		transmit((uint8_t*) buffer, strlen(buffer));
	}

	va_end(args);

	return 0;
}
