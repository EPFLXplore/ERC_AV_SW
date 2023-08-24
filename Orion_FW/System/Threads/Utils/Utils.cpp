/*
 * Utils.cpp
 *
 *  Created on: Aug 24, 2023
 *      Author: Vincent
 */

#include "Utils.h"


uint16_t normFloatToScaledUInt16(const float float_val, const float max_float_val) {
	if (max_float_val == 0)
		return 0;
	else
		return (uint16_t)(float_val/max_float_val * 65535);
}

float scaledUInt16ToNormFloat(const uint16_t uint_val, const float max_float_val) {
	return (float)(max_float_val*uint_val/65535.f);
}

float get_max_val(float arr[], int length) {
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

