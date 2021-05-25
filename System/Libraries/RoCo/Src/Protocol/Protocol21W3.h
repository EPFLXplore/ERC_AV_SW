/*
 * Protocol21W3.h
 *
 *  Created on: 3 May 2020
 *      Author: Aurelio Noca
 */

#ifndef PROTOCOL_PROTOCOL21W03_H_
#define PROTOCOL_PROTOCOL21W03_H_

#include <cstdint>

//----------Avionics----------

struct Avionics_BaroTempPacket {
  float pressure;					//[Pa]
  float temperature;				//[°C]
} __attribute__((packed));

struct Avionics_AccelMagPacket {
  float acceleration[3];			//[m/s^2]
  float angular[3];					//[°]
  float magneto[3];					//[mT]
} __attribute__((packed));

// Handling device + potentiometers (ads1113)
struct Avionics_ADCPacket {
  uint8_t port;
  float voltage;					//[V]
} __attribute__((packed));

// Science
struct Science_MassPacket {
  float mass;						//[g]
} __attribute__((packed));


//----------Power supply----------

struct Power_SystemPacket {
  float battery_charge;
  uint8_t state;
} __attribute__((packed));

struct Power_VoltagePacket {
  float voltages[4];
} __attribute__((packed));

struct Power_CurrentPacket {
  float currents[4];
} __attribute__((packed));

struct Reset_PowerSupplyPacket {
	bool reset;
} __attribute__((packed));

struct Switch_AvionicsPacket {
	bool on;
} __attribute__((packed));

struct Switch_RamanPacket  {
	bool on;
} __attribute__((packed));

struct Switch_JetsonPacket  {
	bool on;
} __attribute__((packed));

struct Switch_LidarPacket  {
	bool on;
} __attribute__((packed));

struct Switch_EthernetPacket  {
	bool on;
} __attribute__((packed));


//----------FSM----------

struct FsmPacket{
	uint32_t state;
}__attribute__((packed));


//----------General packets----------

struct DataPacket {
	uint32_t data;
} __attribute__((packed));

struct PingPacket {
	uint64_t time;
} __attribute__((packed));

struct ErrorPacket {
	uint8_t error_id;
} __attribute__((packed));

struct RequestPacket {
	uint16_t uuid;
	uint8_t action_id;
	uint8_t target_id;
	uint32_t payload;
} __attribute__((packed));

//Different action id definitions to be sent:
//Avionics
#define TARE (uint8_t)0x1 //Recalibrate sensor: ads1113, hx711, imu, barometer
#define RESET_SENSOR (uint8_t)0x2
#define STOP_DATA (uint8_t)0x3

//Science
#define START_ANALYSIS (uint8_t)0x4 //Random example

struct ResponsePacket {
	uint16_t uuid;
	uint8_t action_id;
	uint8_t target_id;
	uint32_t payload;
} __attribute__((packed));

struct ProgressPacket {
	uint32_t uuid;
	uint8_t progress;
} __attribute__((packed));


#endif /* PROTOCOL_PROTOCOL21W3_H_ */
