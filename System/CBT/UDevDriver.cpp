/*
 * NetworkClientIO.cpp
 *
 *  Created on: 23 March 2022
 *      Author: Arion
 */

#include "UDevDriver.h"


#ifdef BUILD_WITH_UDEV_DRIVER


#include <iostream>
#include <unistd.h>
#include <fcntl.h>

UDevDriver::UDevDriver(const char* dev) {
    this->fd = open(dev, O_RDWR | O_NONBLOCK);

    if(this->fd != -1) {
        this->connected = true;
        this->reception_thread = std::thread(&UDevDriver::receiveThread, this);
    } else {
        printf("An error occurred while opening the device\n");
        this->connected = false;
    }
}

/*
 * Releases IO resources
 */
UDevDriver::~UDevDriver() {
	closeDevice();
    this->connected = false;
}

/*
 * Closes all used IO resources
 */
void UDevDriver::closeDevice() {
    if(connected) {
        close(fd);
    }
}

/*
 * Reception thread
 *
 * Processes input from the remote connections and passes it to the reception handler.
 * Handles closing connections.
 */
void UDevDriver::receiveThread() {
	uint8_t buffer[256];
    int result;

    std::cout << "Endpoint connected" << std::endl;

	while(connected) {
		// New data from endpoint
		while((result = read(fd, buffer, 256)) > 0) {
            receiver(0, buffer, result);
		}

        std::this_thread::yield();
	}

	std::cout << "Endpoint disconnected" << std::endl;

	closeDevice();
}

/*
 * Sets the receiver callback function
 */
void UDevDriver::receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver) {
	this->receiver = receiver;
}

/*
 * Transmits the given data to the server
 */
void UDevDriver::transmit(uint8_t* buffer, uint32_t length) {
	if(connected) {
		int32_t result;

		while((result = write(fd, buffer, length)) > 0) {
			length -= result;
		}
	}
}

#endif /* BUILD_WITH_UDEV_DRIVER */
