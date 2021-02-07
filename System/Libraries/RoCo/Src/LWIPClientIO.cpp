/*
 * LWIPClientIO.cpp
 *
 *  Created on: Jan 24, 2021
 *      Author: arion
 */

#include "LWIPClientIO.h"


#ifdef BUILD_WITH_LWIP_CLIENT_IO


#include "Debug/Debug.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

/*
 * Creates an IODriver interface using the given port number.
 * This constructor invocation is a light operation.
 */
LWIPClientIO::LWIPClientIO(std::string address_str, uint16_t port) : address_str(address_str) {
	this->address = { 0 };
	this->socket_id = 0;
	this->connected = false;
	this->receiver = nullptr;

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
}

/*
 * Releases IO resources
 */
LWIPClientIO::~LWIPClientIO() {
	disconnectClient();
}


/*
 * Creates a client socket and connects it to a remote server
 * through the ip address and port specified by the constructor.
 * This operation is heavy and may fail.
 * Check the returned error code and set breakpoints accordingly if needed.
 */
int8_t LWIPClientIO::connectClient() {
	if(connected) {
		return -1; // Server already connected
	}

	// TODO

	this->connected = true;

	console.printf("[RoCo] [Client@%s] Client connected\r\n", ntohs(address.sin_port));

	return true;
}

/*
 * Disconnects the driver instance.
 * In particular, this function resets the IODriver to an initial state and closes all used IO resources.
 * Make sure the disconnect member function is only called in the reception thread.
 */
void LWIPClientIO::disconnectClient() {
	if(connected) {
		this->connected = false;
		closeSocket();
		console.printf("[RoCo] [Client@%s] Client disconnected\r\n", ntohs(address.sin_port));
	}
}

/*
 * Closes all used IO resources
 */
void LWIPClientIO::closeSocket() {
	// TODO
}

/*
 * Processes input from the server and passes it to the reception handler.
 */
void LWIPClientIO::update() {
	if(connected) {
		// TODO
	}
}

/*
 * Sets the receiver callback function
 */
void LWIPClientIO::receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver) {
	this->receiver = receiver;
}

/*
 * Transmits the given data to the server
 * Warning: this function must be thread safe in a multithreaded environment
 */
void LWIPClientIO::transmit(uint8_t* buffer, uint32_t length) {
	if(connected) {
		// TODO
	}
}


#endif /* BUILD_WITH_LWIP_CLIENT_IO */
