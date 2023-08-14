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
#include <cstdint>
#include <string.h>
#include "System.h"

#define MAKE_IDENTIFIABLE(PACKET) PACKET.id = System::get_node_id();

struct Vector {
	float x;
	float y;
	float z;

	char* toString(char* buffer) {
		sprintf(buffer, "[%+.3f %+.3f %+.3f]", x, y, z);
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
		sprintf(buffer, "[%+.3f %+.3f %+.3f %+.3f]", w, x, y, z);
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

struct EulerAngles {
	float r;
	float p;
	float y;
	char* toString(char* buffer) {
		sprintf(buffer, "[r: %+.3f p: %+.3f y: %+.3f] [deg]", r, p, y);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer) {
		*(float*)(buffer + 0) = r;
		*(float*)(buffer + 1*4) = p;
		*(float*)(buffer + 2*4) = y;
		return buffer;
	}

};

struct DummyData {
    int data;
    char* toString(char* buffer) {
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
        sprintf(buffer, "Temp: %+.3f [deg] \t Humid: %.3f [%%] \t Cond.: %.3f [us/cm] \t PH: %.3f", temperature, moisture, conductivity, ph);
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
        sprintf(buffer, "Nitrogen: %.3f [mg/kg] \t Phosphorus: %.3f [mg/kg] Potassium: %.3f [mg/kg]", nitrogen, phosphorus, potassium); // beware of the type: (%d, %f, ...)
        return buffer;
    }

    uint8_t* toArray(uint8_t* buffer){
		*(float*)(buffer + 0) = nitrogen;
		*(float*)(buffer + 1*4) = phosphorus;
		*(float*)(buffer + 2*4) = potassium;
        return buffer;
    }
};

struct SpectroData {
	float data[18] = {0};

	// Wavelength labels [nm]
	const char *labels[18] = {"410", "435", "460", "485", "510", "535", "560", "585", "610",
							  "645", "680", "705", "730", "760", "810", "860", "900", "940"};

	char* toString(char* buffer) {
	    char dataLine[18][20];  // Space for formatted data strings
	    int maxLabelLength = 0;

	    // Calculate the maximum label length
	    for (int i = 0; i < 18; ++i) {
	        int labelLength = strlen(labels[i]);
	        if (labelLength > maxLabelLength) {
	            maxLabelLength = labelLength;
	        }
	    }

	    // Format the data strings and store them in dataLine array
	    for (int i = 0; i < 18; ++i) {
	        snprintf(dataLine[i], sizeof(dataLine[i]), "%+.3f", data[i]);
	    }

	    // Construct the buffer with aligned labels and data
	    for (int i = 0; i < 18; ++i) {
	        snprintf(buffer, 100, "%-*s %s\n", maxLabelLength, labels[i], dataLine[i]);
	        buffer += strlen(buffer);  // Move the buffer pointer
	    }

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
		static char buf_accel[32];
		static char buf_gyro[32];
		static char buf_quat[32];
		sprintf(buffer, "Acc: %s \t Gyro: %s \t Quat: %s", accel.toString(buf_accel), gyro.toString(buf_gyro), orientation.toString(buf_quat));
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


struct MassData {
    float mass[4];
	char* toString(char* buffer) {
		sprintf(buffer, "CH1: %+.3f [g] \t CH2: %+.3f [g] \t CH3: %+.3f [g] \t CH4: %+.3f [g]", mass[0], mass[1], mass[2], mass[3]);
		return buffer;
	}

    uint8_t* toArray(uint8_t* buffer){
    	for(int i = 0; i < 4; ++i)
    		*(float*)(buffer + i * 4) = mass[i];
        return buffer;
    }
};


struct VoltmeterData {
	float voltage;

	char* toString(char* buffer) {
		sprintf(buffer, "Voltage: %+.3f [V]", voltage);
		return buffer;
	}

	uint8_t* toArray(uint8_t* buffer){
		*(float*)(buffer) = voltage;
		return buffer;
	}
};

struct PotentiometerData {
	float angles[4] = {0};
	char* toString(char* buffer) {
		sprintf(buffer, "CH0: %+.3f [deg] \t CH1: %+.3f [deg] \t CH2: %+.3f [deg] \t CH3: %+.3f [deg]", angles[0], angles[1], angles[2], angles[3]);
		return buffer;
	}

    uint8_t* toArray(uint8_t* buffer){
    	for(int i = 0; i < 4; ++i)
    		*(float*)(buffer + i * 4) = angles[i];
        return buffer;
    }
};

struct ServoData {
	bool success;
	char* toString(char* buffer) {
		sprintf(buffer, "%s\n", success ? "success" : "failure");
		return buffer;
	}
	uint8_t* toArray(uint8_t* buffer){
		*(bool*)(buffer) = success;
		return buffer;
	}
};

#endif /* SENSORS_DATASTRUCTURES_H_ */
