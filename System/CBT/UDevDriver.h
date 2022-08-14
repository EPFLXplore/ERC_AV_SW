/*
 * NetworkClientIO.h
 *
 *  Created on: 23 March 2022
 *      Author: Arion
 */

#ifndef UDEVDRIVER_H_
#define UDEVDRIVER_H_

#include "Build/Build.h"


#ifdef BUILD_WITH_UDEV_DRIVER


#include "IODriver.h"
#include "NetworkIO.h"

#include <cstdint>
#include <functional>
#include <thread>


class UDevDriver : public IODriver {
public:
	UDevDriver(const char* dev);
	~UDevDriver();

	void receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver);
	void transmit(uint8_t* buffer, uint32_t length);

    bool isConnected() { return connected; }
	int getFD() { return fd; }

private:
    int fd;
    bool connected;

	std::thread reception_thread;
	std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> receiver;

	void receiveThread();
	void closeDevice();
};


#endif /* BUILD_WITH_UDEV_DRIVER */

#endif /* UDEVDRIVER_H_ */
