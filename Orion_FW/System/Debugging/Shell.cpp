/*
 * shell.c
 *
 *  Created on: 5 Sep 2020
 *      Author: AV Team 2020
 */


#include <string.h>
#include "Debugging/Debug.h"


Shell::Shell(UART_HandleTypeDef* uart, Terminal* terminal) : Thread("Shell"), uart(uart), terminal(terminal) {
	cmd.components[0].component = command_buffer; // Bind command structure to buffer
}

void Shell::init() {
	console.printf("\x1b[2J\x1b[H");
	console.printf("----- EPFL Xplore Orion (rev.4) Shell -----\r\n");
}

void Shell::loop() {
	HAL_UART_Receive_DMA(uart, dma_buffer, CMD_BUFFER_SIZE);
    uint16_t bytesRead = CMD_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(uart->hdmarx);

    for (uint16_t i = 0; i < bytesRead; i++) {
        receiveByte(dma_buffer[i]);
        dma_buffer[i] = '\0'; // Clear the byte after processing
    }

    // Compact the buffer by moving unread data to the beginning
    if (bytesRead > 0) {
        uint16_t unreadBytes = bytesRead - lastProcessedIndex;
        memmove(dma_buffer, dma_buffer + lastProcessedIndex, unreadBytes);
        lastProcessedIndex = 0;
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
