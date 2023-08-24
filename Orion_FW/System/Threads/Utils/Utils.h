/*
 * Utils.h
 *
 *  Created on: Aug 24, 2023
 *      Author: Vincent
 */

#ifndef THREADS_UTILS_UTILS_H_
#define THREADS_UTILS_UTILS_H_

#include <stdint.h>


uint16_t normFloatToScaledUInt16(const float float_val, const float max_float_val);

float scaledUInt16ToNormFloat(const uint16_t uint_val, const float max_float_val);

float get_max_val(float arr[], int length);

#endif /* THREADS_UTILS_UTILS_H_ */
