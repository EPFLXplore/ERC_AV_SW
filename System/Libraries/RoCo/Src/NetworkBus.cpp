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

	// avionics
	define<Avionics_BaroTempPacket>(0);
	define<Avionics_AccelMagPacket>(1);
	define<Avionics_ADCPacket>(2);
	define<Science_MassPacket>(3);


	// power supply
	define<Power_SystemPacket>(8);
	define<Power_VoltagePacket>(9);
	define<Power_CurrentPacket>(10);

	define<Reset_PowerSupplyPacket>(13);
	define<Switch_AvionicsPacket>(14);
	define<Switch_RamanPacket>(15);
	define<Switch_JetsonPacket>(16);
	define<Switch_LidarPacket>(17);
	define<Switch_EthernetPacket>(18);

	// finite state machine
	define<FsmPacket>(22);

	// general packets
	define<DataPacket>(58);
	define<PingPacket>(59);
	define<ErrorPacket>(60);
	define<RequestPacket>(61);
	define<ResponsePacket>(62);
	define<ProgressPacket>(63);
}


#endif /* BUILD_WITH_NETWORK_BUS */
