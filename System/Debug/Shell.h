/*
 * shell.h
 *
 *  Created on: 5 Sep 2020
 *      Author: AV Team 2020
 */

#ifndef APPLICATION_HOSTBOARD_INC_DEBUG_SHELL_H_
#define APPLICATION_HOSTBOARD_INC_DEBUG_SHELL_H_


#include "Terminal.h"

#include "Thread.h"

#include <usart.h>
#include <stdint.h>
#include <stdbool.h>

#include <stm32h7xx_hal.h>

#define CMD_BUFFER_SIZE 512



class Shell : public Thread {
public:
	Shell(UART_HandleTypeDef* uart, Terminal* terminal);
	void init();
	void loop();

private:
	UART_HandleTypeDef* uart;
	Terminal* terminal;

	uint8_t dma_buffer[CMD_BUFFER_SIZE];

	char command_buffer[CMD_BUFFER_SIZE];
	uint32_t lastDmaStreamIndex = 0;
	uint32_t endDmaStreamIndex = 0;

	uint8_t command_index;

	ShellCommand cmd;

	void receiveByte(char cbuf);
	bool componentMatches(struct CommandComponent* comp, const char* target);
};




#endif /* APPLICATION_HOSTBOARD_INC_DEBUG_SHELL_H_ */
