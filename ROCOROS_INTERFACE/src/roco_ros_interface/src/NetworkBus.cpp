/*
 * NetworkBus.cpp
 *
 *  Created on: 8 May 2020
 *  Updated on : 10 Aug 2022
 *      Authors: Arion, Yassine
 */

// #include "Build/Build.h"


// #ifdef BUILD_WITH_NETWORK_BUS


#include "NetworkBus.h"
#include "Protocol/Protocol.h"

NetworkBus::NetworkBus(IODriver* driver) : IOBus(driver, network_frame, sizeof(network_frame)) {

	//---------Avionics--------------
	define<avionics_ToF_packet>(0);
	define<avionics_IMU_packet>(1);
	define<avionics_potentiometer_packet>(2);
	define<avionics_massload_packet>(3);
	define<avionics_moisture_packet>(4);
	define<avionics_voltmeter_packet>(5);
	define<sc_LED_packet>(6);
	define<sc_trap_packet>(7);
	define<sc_caching_packet>(8);
	define<avionics_trap_success_packet>(9);
	define<avionics_caching_success_packet>(10);

	//----------Power supply----------
	define<Power_SystemPacket>(11);
	define<Power_VoltagePacket>(12);
	define<Power_CurrentPacket>(13);
	define<Reset_PowerSupplyPacket>(14);
	define<Switch_AvionicsPacket>(15);
	define<Switch_RamanPacket>(16);
	define<Switch_JetsonPacket>(17);
	define<Switch_LidarPacket>(18);
	define<Switch_EthernetPacket>(19);

	//---------------FSM---------------
	define<FsmPacket>(20);

	//----------General packets--------
	define<DataPacket>(21);
	define<PingPacket>(22);
	define<ErrorPacket>(23);
	define<RequestPacket>(24);
	define<ResponsePacket>(25);
	define<ProgressPacket>(26);

	//-------------Sussy_packets------
//	define<PollosHermanos_packet>(69);
}

NetworkBus::~NetworkBus(){};


// #endif /* BUILD_WITH_NETWORK_BUS */
