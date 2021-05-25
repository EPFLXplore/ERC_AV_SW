/*
 * ProtocolRegisters.h
 *
 *  Created on: 8 May 2020
 *      Author: Arion
 */

#ifndef PROTOCOL_PROTOCOLREGISTERS_H_
#define PROTOCOL_PROTOCOLREGISTERS_H_

// This file may only be included by MessageBus.cpp (which exports the REGISTER macro)
#include "Protocol.h"

#ifdef PROTOCOL_20W18
REGISTER(PingPacket)
REGISTER(ConnectPacket)
REGISTER(DisconnectPacket)
REGISTER(RequestPacket)
REGISTER(AcknowledgePacket)
REGISTER(ResponsePacket)
REGISTER(ProgressPacket)
REGISTER(DataPacket)
REGISTER(MessagePacket)
REGISTER(ErrorPacket)
#endif /* PROTOCOL_20W18 */

#ifdef PROTOCOL_21W3
REGISTER(Avionics_BaroTempPacket)
REGISTER(Avionics_AccelMagPacket)
REGISTER(Avionics_ADCPacket)
REGISTER(Science_MassPacket)

REGISTER(Power_SystemPacket)
REGISTER(Power_VoltagePacket)
REGISTER(Power_CurrentPacket)

REGISTER(Reset_PowerSupplyPacket)
REGISTER(Switch_AvionicsPacket)
REGISTER(Switch_RamanPacket)
REGISTER(Switch_JetsonPacket)
REGISTER(Switch_LidarPacket)
REGISTER(Switch_EthernetPacket)

REGISTER(FsmPacket)

REGISTER(DataPacket)
REGISTER(PingPacket)
REGISTER(ErrorPacket)
REGISTER(RequestPacket)
REGISTER(ResponsePacket)
REGISTER(ProgressPacket)
#endif /* PROTOCOL_21W3 */



#endif /* PROTOCOL_PROTOCOLREGISTERS_H_ */
