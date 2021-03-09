/*
 * LWIPClientIO.h
 *
 *  Created on: Jan 24, 2021
 *      Author: arion
 */

#ifndef LIBRARIES_ROCO_LWIPCLIENTIO_H_
#define LIBRARIES_ROCO_LWIPCLIENTIO_H_

#include "Build/Build.h"

#ifdef BUILD_WITH_LWIP_CLIENT_IO
#include "IODriver.h"
#include "NetworkIO.h"

#include <cstdint>
#include <functional>
#include <thread>

#include <arpa/inet.h>
#undef send // The inet API creates a "send" macro that must be undefined because it conflicts with MessageBus::send

class LWIPClientIO : public IODriver {
public:
	LWIPClientIO(std::string address, uint16_t port);
	~LWIPClientIO();

	int8_t connectClient();
	void disconnectClient();

	void receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver);
	void transmit(uint8_t* buffer, uint32_t length);

	void update();

	bool isConnected() { return connected; }

private:
	std::string address_str;

	sockaddr_in address;
	uint32_t socket_id;
	bool connected;
	std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> receiver;

	void closeSocket();
};
#endif /* BUILD_WITH_LWIP_CLIENT_IO */

#endif /* LIBRARIES_ROCO_LWIPCLIENTIO_H_ */
