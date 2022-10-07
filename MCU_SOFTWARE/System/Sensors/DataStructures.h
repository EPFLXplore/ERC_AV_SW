/*
 * DataStructures.h
 *
 *  Created on: Jan 23, 2021
 *  Updated on: Aug 5, 2022
 *      Authors: AV Team 2020, AV Team 2022
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

struct Quaternion {
	float w;
	float x;
	float y;
	float z;
	char* toString(char* buffer) {
		sprintf(buffer, "[%f %f %f %f]", w, x, y, z);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer) {
		*(float*)(buffer + 0) = w;
		*(float*)(buffer + 1*4) = x;
		*(float*)(buffer + 2*4) = y;
		*(float*)(buffer + 3*4) = z;
		return buffer;
	}

};

//struct IMUData {
//	Vector accel;
//	Vector gyro;
//	Vector mag;
//
//	char* toString(char* buffer) {
//		static char buf[96];
//		sprintf(buffer, "Acc: %s, Ang: %s, Mag: %s", accel.toString(buf), gyro.toString(buf + 32), mag.toString(buf + 64));
//		return buffer;
//	}
//
//	uint8_t* toArray(uint8_t* buffer) {
//		accel.toArray(buffer);
//		gyro.toArray(buffer + 3*4);
//		mag.toArray(buffer + 6*4);
//		return buffer;
//	}
//};

struct IMUData {
	Vector accel;
	Vector gyro;
//	Vector mag;
	Quaternion orientation;

	char* toString(char* buffer) {
		static char buf[32];
		sprintf(buffer, "Acc: %s, Gyro: %s, Mag: %s", accel.toString(buf), gyro.toString(buf), orientation.toString(buf));
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer) {
		accel.toArray(buffer);
		gyro.toArray(buffer + 3*4);
		orientation.toArray(buffer + 6*4);
//		mag.toArray(buffer + 6*4);
		return buffer;
	}
};

struct TOFData {
	float distance;

	char* toString(char* buffer) {
		static char buf[32];
		sprintf(buffer, "Tof(mm): %f", distance);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer){
		*(float*)(buffer) = distance;
		return buffer;
	}
};


struct ScienceData {
	float moisture;

	char* toString(char* buffer) {
		static char buf[32];
		sprintf(buffer, "Moisture(-): %f", moisture);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer){
		*(float*)(buffer) = moisture;

		return buffer;
	}
};

struct MassData {
    float mass;
    char* toString(char* buffer) {
        static char buf[32];
        sprintf(buffer, "Mass: %f", mass);
        return buffer;
    }

    uint8_t* toArray(uint8_t* buffer){
        *(float*)(buffer) = mass;
        return buffer;
    }
};


struct VoltmeterData {
	float voltage;

	char* toString(char* buffer) {
		sprintf(buffer, "Voltage(V): %f", voltage);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer){
		*(float*)(buffer) = voltage;
		return buffer;
	}
};


#endif /* SENSORS_DATASTRUCTURES_H_ */
