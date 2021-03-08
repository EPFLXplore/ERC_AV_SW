/*
 * BufferedIODriver.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */

#include "BufferedIODriver.h"
#include "Debug/Debug.h"

#ifdef BUILD_WITH_BUFFERED_IO


BufferedIODriver::BufferedIODriver(uint8_t* queueStorage, uint8_t* bufferStorage, size_t length) : bufferStorage(bufferStorage), bufferLength(length) {
	buffer = xMessageBufferCreateStatic(length, queueStorage, &staticMessageBuffer);
}

void BufferedIODriver::receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver) {
	// Not implemented
}

/*
 * Writes data to a buffer shared between Cortex-M4 and Cortex-M7
 * Any thread can call this function
 */
void BufferedIODriver::transmit(uint8_t* data, uint32_t length) {
	xMessageBufferSend(buffer, data, length, pdMS_TO_TICKS(5));
}

/*
 * Passes the buffered data to the real IODriver
 * The flush function must be called from the thread designed to transmit the data
 */

void BufferedIODriver::flush(IODriver* driver) {
	size_t size = 0;

	do {
		size = xMessageBufferReceive(buffer, bufferStorage, bufferLength, pdMS_TO_TICKS(5));
		driver->transmit(bufferStorage, size);
	} while(size > 0);
}

#endif
