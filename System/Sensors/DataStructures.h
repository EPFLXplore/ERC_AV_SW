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

	float* toArray(float* buffer) {
		buffer[0] = x;
		buffer[1] = y;
		buffer[2] = z;
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

	float* toArray(float* buffer) {
		accel.toArray(buffer);
		gyro.toArray(buffer + 3);
		mag.toArray(buffer + 6);
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

	float* toArray(float* buffer) {
		buffer[0] = pressure;
		buffer[1] = temperature;
		buffer[2] = humidity;
		return buffer;
	}
};

struct ScienceData {
	uint32_t mass;

	char* toString(char* buffer) {
		sprintf(buffer, "Mass(kg): %u", mass);
		return buffer;
	}

	uint32_t* toArray(uint32_t* buffer){
		*buffer = mass;
		return buffer;
	}
};

struct PotentiometerData {
	float voltage;

	char* toString(char* buffer) {
		sprintf(buffer, "Voltage(v): %f", voltage);
		return buffer;
	}

	float* toArray(float* buffer){
		*buffer = voltage;
		return buffer;
	}
};

#endif /* SENSORS_DATASTRUCTURES_H_ */
