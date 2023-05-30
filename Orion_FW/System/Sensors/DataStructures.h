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

struct DummyData {
    int data;
    char* toString(char* buffer) {
        static char buf[32];
        sprintf(buffer, "Data: %d", data); // beware of the type: (%d, %f, ...)
        return buffer;
    }

    uint8_t* toArray(uint8_t* buffer){
        *(int*)(buffer) = data;
        return buffer;
    }
};

struct ALLINONEData {
    float temp;
    float moist;
    float conduct;
    float PH;
    char* toString(char* buffer) {
        static char buf[32];
        sprintf(buffer, "temperature: %f  moisture level: %f  conductivity: %f  PH level: %f", temp, moist, conduct, PH); // beware of the type: (%d, %f, ...)
        return buffer;
    }

    uint8_t* toArray(uint8_t* buffer){
		*(float*)(buffer + 0) = temp;
		*(float*)(buffer + 1*4) = moist;
		*(float*)(buffer + 2*4) = conduct;
		*(float*)(buffer + 3*4) = PH;
        return buffer;
    }
};

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
