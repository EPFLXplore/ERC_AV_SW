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
	define<RequestPacket>(3);
	define<ResponsePacket>(5);
	define<ProgressPacket>(6);
	define<DataPacket>(7);
	define<ErrorPacket>(8);
	define<Avionics_BaroTempPacket>(9);
	define<Avionics_AccelMagPacket>(10);
	define<Handling_GripperPacket>(11);
	define<Power_VoltagePacket>(12);
	define<Power_CurrentPacket>(13);
	define<Power_SystemPacket>(14);
	define<Science_MeasurePacket>(15);
}


#endif /* BUILD_WITH_NETWORK_BUS */
