/*
 * Protocol21W3.h
 *
 *  Created on: 3 May 2020
 *  Updated on: 10 May 2022
 *      Author: Aurelio Noca, AVTeam 2022
 */


#ifndef PROTOCOL_PROTOCOL22W69_H_
#define PROTOCOL_PROTOCOL22W69_H_

#include <stdint.h>
#include <stdbool.h>

//----------Avionics----------

struct avionics_ToF_packet {
  float distance;					//[mm]
} __attribute__((packed));

struct avionics_IMU_packet {
  float acceleration[3];			//[m/s^2]
  float angular_velocity[3];		//[°/s]
  float orientation[4];			//[°]
//  float quaternion[3];
} __attribute__((packed));

struct avionics_potentiometer_packet {
  float voltage;					//[V]
} __attribute__((packed));

struct avionics_massload_packet {
  float mass;						//[g]
} __attribute__((packed));

struct avionics_moisture_packet {
  float moisture;					//[%]
} __attribute__((packed));

struct avionics_voltmeter_packet {
	float voltage;					//[V]
} __attribute__((packed));

struct sc_LED_packet {
	bool on;
} __attribute__((packed));

struct sc_trap_packet {
	bool open;
} __attribute__((packed));

struct sc_caching_packet {
	bool open;
} __attribute__((packed));

struct avionics_trap_success_packet {
	bool status;
} __attribute__((packed));

struct avionics_caching_success_packet {
	bool status;
} __attribute__((packed));

struct hd_voltmeter_motor_packet {
	bool extended;
} __attribute__((packed));

struct avionics_voltmeter_motor_status_packet {
	bool status;
} __attribute__((packed));

struct hd_voltmeter_tare_packet {
	bool tare;
} __attribute__((packed));

struct avionics_voltmeter_tare_success_packet {
	bool status;
} __attribute__((packed));

struct sc_mass_calibrate_packet {
	bool tare;
} __attribute__((packed));


struct avionics_mass_calibrate_success_packet {
	bool status;
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
	uint8_t task;
	uint8_t instruction;
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
#define TARE (uint8_t)0x1  //Recalibrate sensor: ads1113, hx711, imu, stemma
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


#endif /* PROTOCOL_PROTOCOL22W69_H_ */
