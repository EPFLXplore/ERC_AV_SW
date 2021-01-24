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
		sprintf(buffer, "[%ld %ld %ld]", (int32_t) (1000 * x), (int32_t) (1000 * y), (int32_t) (1000 * z));
		return buffer;
	}
};

struct IMUData {
	Vector accel;
	Vector gyro;
	Vector mag;

	char* toString(char* buffer) {
		static char buf[96];
		sprintf(buffer, "Accel: %s, Gyro: %s, Mag: %s", accel.toString(buf), gyro.toString(buf + 32), mag.toString(buf + 64));
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
};


#endif /* SENSORS_DATASTRUCTURES_H_ */
