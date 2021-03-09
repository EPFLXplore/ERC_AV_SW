/*
 * BufferedIODriver.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */

#include "BufferedIODriver.h"
#include "Debug/Debug.h"

#include <cstring>

#ifdef BUILD_WITH_BUFFERED_IO


BufferedIODriver::BufferedIODriver(volatile uint8_t* bufferStorage, volatile uint8_t* bufferControl, size_t length) : bufferStorage(bufferStorage), bufferControl(bufferControl), bufferLength(length), readIndex(0), receiverFunc(nullptr) {
	semaphore = xSemaphoreCreateMutex();
	*bufferControl = 0;
}

void BufferedIODriver::receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver) {
	receiverFunc = receiver;
}

/*
 * Writes data to a buffer shared between Cortex-M4 and Cortex-M7
 * Any thread can call this function
 * Assumes there is no overrun
 */
void BufferedIODriver::transmit(uint8_t* data, uint32_t length) {
	if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE) {
		while(HAL_HSEM_Take(1, 1) != HAL_OK);

		uint8_t writeIndex = *bufferControl;
		uint8_t* buffer = (uint8_t*) bufferStorage;

		if(writeIndex + length < bufferLength) {
			memcpy(buffer + writeIndex, data, length);
			writeIndex += length;
		} else {
			uint16_t firstSize = bufferLength - writeIndex;
			memcpy(buffer + writeIndex, data, firstSize);
			memcpy(buffer, data + firstSize, length - firstSize);
			writeIndex = length - firstSize;
		}

		*bufferControl = writeIndex;

		HAL_HSEM_Release(1, 1);
		xSemaphoreGive(semaphore);
	}
}

/*
 * Passes the buffered data to the real IODriver
 * The flush function must be called from the thread designed to transmit the data
 */

void BufferedIODriver::flush(IODriver* driver) {
	uint8_t writeIndex = *bufferControl; // Atomic operation
	uint8_t* buffer = (uint8_t*) bufferStorage;

	if(readIndex < writeIndex) {
		driver->transmit(buffer + readIndex, writeIndex - readIndex);
	} else if(readIndex > writeIndex) {
		uint16_t firstSize = bufferLength - readIndex;
		driver->transmit(buffer + readIndex, firstSize);
		driver->transmit(buffer, writeIndex);
	}

	readIndex = writeIndex;
}

#endif
