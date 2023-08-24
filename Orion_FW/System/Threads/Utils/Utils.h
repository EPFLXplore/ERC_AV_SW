/*
 * Utils.h
 *
 *  Created on: Aug 24, 2023
 *      Author: Vincent
 */

#ifndef THREADS_UTILS_UTILS_H_
#define THREADS_UTILS_UTILS_H_

#include <stdint.h>


uint16_t floatToScaledUInt16(const float float_val, const float max_float_val);

float scaledUInt16ToFloat(const uint16_t uint_val, const float max_float_val);

void floatArrayToScaledUInt16Array(const float floatArray[], uint16_t uint16Array[], const int length, const float max_float_val);

void scaledUInt16ArrayToFloatArray(const uint16_t uint16Array[], float floatArray[], const int length, const float max_float_val);

float get_max_val(const float* arr, int length);

#endif /* THREADS_UTILS_UTILS_H_ */
