/*
 * BufferedIODriver.h
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */

#ifndef LIBRARIES_ROCO_SRC_PROTOCOL_BUFFEREDIODRIVER_H_
#define LIBRARIES_ROCO_SRC_PROTOCOL_BUFFEREDIODRIVER_H_

#include "Build/Build.h"

#ifdef BUILD_WITH_BUFFERED_IO

#include "IODriver.h"

#include <message_buffer.h>
#include <cstdint>
#include <functional>


class BufferedIODriver : public IODriver {
public:
	BufferedIODriver(uint8_t* queueStorage, uint8_t* bufferStorage, size_t length);
	void receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver);
	void transmit(uint8_t* buffer, uint32_t length);
	void flush(IODriver* driver);
private:
	MessageBufferHandle_t buffer;
	StaticMessageBuffer_t staticMessageBuffer;
	uint8_t* bufferStorage;
	size_t bufferLength;
};


#endif


#endif /* LIBRARIES_ROCO_SRC_PROTOCOL_BUFFEREDIODRIVER_H_ */
