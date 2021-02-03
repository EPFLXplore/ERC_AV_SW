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
 * Creates an ExternalIO interface using the given port number.
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
 * Creates a server socket and allow incoming connections
 * through the port specified by the constructor.
 * This operation is heavy and may fail.
 * Check the returned error code and set breakpoints accordingly if needed.
 */
int8_t LWIPClientIO::connectClient() {
	if(connected) {
		return -1; // Server already connected
	}

	int32_t result;

	// Creates the socket instance
	socket_id = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_id < 0) {
		return -2;
	}

	if(inet_pton(AF_INET, address_str.c_str(), &address.sin_addr) <= 0) {
		close(socket_id);
		return -3;
	}

	// Binds the client socket to the specified address and port
	result = connect(socket_id, (struct sockaddr*) &address, sizeof(address));

	if(result < 0) {
		close(socket_id);
		return -4;
	}

	// Finalises the instance's state and adds the socket server to the array of opened sockets
	this->connected = true;

	console.printf("[RoCo] [Client@%s] Client connected\r\n", ntohs(address.sin_port));

	return true;
}

/*
 * Disconnects the ExternalIO instance.
 * In particular, this function resets the ExternalIO to an initial state and closes all used IO resources.
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
	close(socket_id);
}

/*
 * Reception thread
 *
 * Allows incoming connections and adds the corresponding socket to the array of opened sockets.
 * Processes input from the remote connections and passes it to the reception handler.
 * Handles closing connections.
 */
void LWIPClientIO::update() {
	int32_t result;

	uint8_t buffer[256];

	if(connected) {
		// New data from client
		while((result = recv(socket_id, buffer, sizeof(buffer), 0)) >= 0) {
			if(result != 0) {
				if(receiver != nullptr) {
					if(ntohs(address.sin_port) == PORT_A) {
						receiver(0b10000000, buffer, result); // Sender ID marked as external
					} else {
						receiver(0b11000000, buffer, result); // Sender ID marked as internal
					}
				}
			} else {
				// Connection was closed by server
				disconnectClient();
				break;
				// Do not decrement the num_sockets field since our IDs are not linear
			}
		}
	}

	disconnectClient();
}

/*
 * Sets the receiver callback function
 */
void LWIPClientIO::receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver) {
	this->receiver = receiver;
}

/*
 * Broadcasts data to the array of connected sockets (excluding the server instance)
 * Warning: this function must be thread safe in a multithreaded environment
 */
void LWIPClientIO::transmit(uint8_t* buffer, uint32_t length) {
	if(connected) {
		int32_t result;

		while((result = send(socket_id, buffer, length, 0)) > 0) {
			length -= result;
		}
	}
}
#endif /* BUILD_WITH_LWIP_CLIENT_IO */
