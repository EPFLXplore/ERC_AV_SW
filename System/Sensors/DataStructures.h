/*
 * DataStructures.h
 *
 *  Created on: Jan 23, 2021
 *      Author: AV Team 2020
 */

#ifndef SENSORS_DATASTRUCTURES_H_
#define SENSORS_DATASTRUCTURES_H_

#include <cstdio>

struct Vector {
	float x;
	float y;
	float z;

	char* toString(char* buffer) {
		sprintf(buffer, "[%f %f %f]", x, y, z);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer) {
		*(float*)(buffer + 0) = x;
		*(float*)(buffer + 1*4) = y;
		*(float*)(buffer + 2*4) = z;
		return buffer;
	}
};

struct IMUData {
	Vector accel;
	Vector gyro;
	Vector mag;

	char* toString(char* buffer) {
		static char buf[96];
		sprintf(buffer, "Acc: %s, Ang: %s, Mag: %s", accel.toString(buf), gyro.toString(buf + 32), mag.toString(buf + 64));
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer) {
		accel.toArray(buffer);
		gyro.toArray(buffer + 3*4);
		mag.toArray(buffer + 6*4);
		return buffer;
	}
};

struct BaroData {
	float temperature;
	float pressure;
	float humidity;

	char* toString(char* buffer) {
		sprintf(buffer, "Tmp: %f, Prs: %f, Hum: %f", temperature, pressure, humidity);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer) {
		*(float*)(buffer + 0) = pressure;
		*(float*)(buffer + 1*4) = temperature;
		*(float*)(buffer + 2*4) = humidity;
		return buffer;
	}
};

struct ScienceData {
	float mass;

	char* toString(char* buffer) {
		sprintf(buffer, "Mass(g): %f", mass);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer){
		*(uint32_t*)(buffer) = mass;
		return buffer;
	}
};

struct PotentiometerData {
	uint8_t portNum;
	float voltage;

	char* toString(char* buffer) {
		sprintf(buffer, "[i2c%d] Voltage(V): %f", portNum, voltage);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer){
		*(uint8_t*)(buffer) = portNum;
		*(float*)(buffer + 1) = voltage;
		return buffer;
	}
};

#endif /* SENSORS_DATASTRUCTURES_H_ */
