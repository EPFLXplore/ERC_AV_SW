/*
 * NetworkBus.cpp
 *
 *  Created on: 8 May 2020
 *      Author: Arion
 */

#include "Build/Build.h"


#ifdef BUILD_WITH_NETWORK_BUS


#include "NetworkBus.h"
#include "Protocol/Protocol.h"

NetworkBus::NetworkBus(IODriver* driver) : IOBus(driver, network_frame, sizeof(network_frame)) {
	define<PingPacket>(0);
	define<ConnectPacket>(1);
	define<DisconnectPacket>(2);
	define<RequestPacket>(3);
	define<AcknowledgePacket>(4);
	define<ResponsePacket>(5);
	define<ProgressPacket>(6);
	define<DataPacket>(7);
	define<MessagePacket>(62);
	define<ErrorPacket>(63);
}


#endif /* BUILD_WITH_NETWORK_BUS */
