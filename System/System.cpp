/*
 * system.cpp
 *
 *  Created on: Aug 1, 2022
 *      Author: ???
 */


#include "System.h"



void initCortex() {

}

Vector bnoVectorToVector(bno055_vector_t v) {
	Vector vector;

	vector.x = v.x;
	vector.y = v.y;
	vector.z = v.z;

	return vector;
}


extern "C" {

void vApplicationStackOverflowHook() {
	while(1);
}

}


