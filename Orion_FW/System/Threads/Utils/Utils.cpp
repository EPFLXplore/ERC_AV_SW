/*
 * Utils.cpp
 *
 *  Created on: Aug 24, 2023
 *      Author: Vincent
 */

#include "Utils.h"


uint16_t floatToScaledUInt16(const float float_val, const float max_float_val) {
	if (max_float_val == 0)
		return 0;
	else
		return (uint16_t)(float_val/max_float_val * 65535);
}

float scaledUInt16ToFloat(const uint16_t uint_val, const float max_float_val) {
	return (float)(max_float_val*uint_val/65535.f);
}

void floatArrayToScaledUInt16Array(const float floatArray[], uint16_t uint16Array[], const int length, const float max_float_val) {
    for (int i = 0; i < length; i++) {
        uint16Array[i] = floatToScaledUInt16(floatArray[i], max_float_val);
    }
}

void scaledUInt16ArrayToFloatArray(const uint16_t uint16Array[], float floatArray[], const int length, const float max_float_val) {
    for (int i = 0; i < length; i++) {
        floatArray[i] = scaledUInt16ToFloat(uint16Array[i], max_float_val);
    }
}

float get_max_val(const float* arr, int length) {
    if (length <= 0) {
        return 0.0;
    }
    float max = arr[0]; // Assume the first element is the maximum

    // Iterate through the array to find the maximum value
    for (int i = 1; i < length; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

