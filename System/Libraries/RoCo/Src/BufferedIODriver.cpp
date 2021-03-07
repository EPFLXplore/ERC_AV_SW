/*
 * BufferedIODriver.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */

#include "BufferedIODriver.h"

#ifdef BUILD_WITH_BUFFERED_IO

void BufferedIODriver::receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver) {
	// Not implemented
}

/*
 * Writes data to a buffer shared between Cortex-M4 and Cortex-M7
 * Any thread can call this function
 */
void BufferedIODriver::transmit(uint8_t* buffer, uint32_t length) {
	// TODO
}

/*
 * Passes the buffered data to the real IODriver
 * The flush function must be called from the thread designed to transmit the data
 */
void BufferedIODriver::flush(IODriver* driver) {
	// TODO
}

#endif