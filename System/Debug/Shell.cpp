/*
 * shell.c
 *
 *  Created on: 5 Sep 2020
 *      Author: AV Team 2020
 */


#include "Debug.h"


Shell::Shell(UART_HandleTypeDef* uart, Terminal* terminal) : Thread("Shell"), uart(uart), terminal(terminal) {
	cmd.components[0].component = command_buffer; // Bind command structure to buffer
}

void Shell::init() {
	console.printf("\x1b[2J\x1b[H");
	console.printf("----- EPFL Xplore Avionics Shell -----\n");
	HAL_UART_Receive_DMA(uart, dma_buffer, CMD_BUFFER_SIZE);
}

void Shell::loop() {
	endDmaStreamIndex = CMD_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(uart->hdmarx);

	while(lastDmaStreamIndex != endDmaStreamIndex) {
		receiveByte(dma_buffer[lastDmaStreamIndex]);
		lastDmaStreamIndex = (lastDmaStreamIndex + 1) % CMD_BUFFER_SIZE;
	}
}


void Shell::receiveByte(char cbuf) {
	if(cbuf == '\0') {
		return;
	}

	if(cbuf != '\n' && cbuf != '\r' && command_index < CMD_BUFFER_SIZE) {
		command_buffer[command_index++] = cbuf;

		if(cbuf == ' ') {
			uint8_t start_index = (&cmd.components[cmd.num_components].component[0] - &command_buffer[0]);

			cmd.components[cmd.num_components].length = command_index - start_index - 1;
			cmd.num_components++;
			cmd.components[cmd.num_components].component = &command_buffer[command_index];
		}
	} else {
		uint8_t start_index = (&cmd.components[cmd.num_components].component[0] - &command_buffer[0]);
		cmd.components[cmd.num_components].length = command_index - start_index;

		if(command_index - start_index > 0) {
			cmd.num_components++;
		}

		terminal->execute(&cmd, &console);

		command_index = 0;
		cmd.num_components = 0;
		cmd.components[0].length = 0;
		cmd.components[0].component = command_buffer;
	}
}
