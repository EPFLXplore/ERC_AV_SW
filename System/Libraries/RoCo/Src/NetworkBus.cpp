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
	define<RequestPacket>(1);
	define<ResponsePacket>(2);
	define<ProgressPacket>(3);
	define<DataPacket>(4);
	define<Avionics_BaroTempPacket>(5);
	define<Avionics_AccelMagPacket>(6);
	define<Handling_GripperPacket>(7);
	define<Power_VoltagePacket>(8);
	define<Power_CurrentPacket>(9);
	define<Power_SystemPacket>(10);
	define<Science_MeasurePacket>(11);
	define<ErrorPacket>(63);
}


#endif /* BUILD_WITH_NETWORK_BUS */
