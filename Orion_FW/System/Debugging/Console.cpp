/*
 * console.c
 *
 *  Created on: Feb 17, 2020
 *      Author: AV Team 2020
 */

#include "Debugging/Console.h"

#include <stdarg.h>
#include <string.h>
#include <stm32h7xx_hal.h>


Console::Console(UART_HandleTypeDef* uart) : console_uart(uart) {
	console_uart = uart;
//	console_semaphore = xSemaphoreCreateBinary();
}

void Console::lock() {
//	xSemaphoreTake(console_semaphore, portMAX_DELAY);
}

void Console::unlock() {
//	xSemaphoreGive(console_semaphore);
}

void Console::transmit(uint8_t* buffer, uint32_t length) {
	HAL_UART_Transmit(console_uart, buffer, length, 0xFFFFFF);
}

void Console::print(const char* buffer) {
	vTaskSuspendAll();
	transmit((uint8_t*) buffer, strlen(buffer));
	xTaskResumeAll();
}

void Console::printf(const char *format, ...) {
	va_list args;
	va_start(args, format);

	if(vsprintf(buffer, format, args) > 0) {
		print(buffer);
	}

	va_end(args);
}
