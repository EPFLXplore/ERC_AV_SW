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
#include <cstdlib>

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

struct FOURINONEData {
    float temperature;
    float moisture;
    float conductivity;
    float ph;
    char* toString(char* buffer) {
        static char buf[32];
        sprintf(buffer, "Temperature: %f [deg] \t Moisture: %f [%%] \t Conductivity: %f [us/cm] \t PH: %f", temperature, moisture, conductivity, ph);
        return buffer;
    }

    uint8_t* toArray(uint8_t* buffer){
		*(float*)(buffer + 0) = temperature;
		*(float*)(buffer + 1*4) = moisture;
		*(float*)(buffer + 2*4) = conductivity;
		*(float*)(buffer + 3*4) = ph;
        return buffer;
    }
};


struct NPKData {
    float nitrogen;
    float phosphorus;
    float potassium;
    char* toString(char* buffer) {
        sprintf(buffer, "nitrogen: %f [mg/kg] phosphorus: %f [mg/kg] potassium: %f [mg/kg]", nitrogen, phosphorus, potassium); // beware of the type: (%d, %f, ...)
        return buffer;
    }

    uint8_t* toArray(uint8_t* buffer){
		*(float*)(buffer + 0) = nitrogen;
		*(float*)(buffer + 1*4) = phosphorus;
		*(float*)(buffer + 2*4) = potassium;
        return buffer;
    }
};

struct ColorFilterData {
    float data[18] = {0};
//    char refs[18] = {0};
    char* toString(char* buffer) {
//        for(int i = 0; i < 18; ++i)
//        	sprintf(buffer, "\c %f \n", refs[i], data[i]);
        return buffer;
    }

    uint8_t* toArray(uint8_t* buffer){
    	for(int i = 0; i < 18; ++i)
    		*(float*)(buffer + i * 4) = data[i];
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
		sprintf(buffer, "Acc: %s [m/s2], Gyro [rad/s]: %s, Quaternion: %s", accel.toString(buf), gyro.toString(buf), orientation.toString(buf));
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
		sprintf(buffer, "Tof: %f [mm]", distance);
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
        sprintf(buffer, "Mass: %f [g]", mass);
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
		sprintf(buffer, "Voltage: %f [V]", voltage);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer){
		*(float*)(buffer) = voltage;
		return buffer;
	}
};


#endif /* SENSORS_DATASTRUCTURES_H_ */
