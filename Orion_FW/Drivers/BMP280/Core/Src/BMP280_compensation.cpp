/*
 * Copyright (c) 2020 Bosch Sensortec GmbH. All rights reserved.
 *
 * BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Code origin:
 * https://github.com/BoschSensortec/BMP280_driver/tree/e3c8794ca51f7115247eb4f5e9a9206dbb09699e
 *
 */

#include <stdint.h>
#include "BMP280.hpp"
#include "BMP280_defines.h"

/**
 * Calculate sensor temperature from measurement and compensation parameters.
 * @param uncomp_temp: Raw temperature measurement.
 * @return Temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
 * */
int32_t BMP280::compensate_temperature(int32_t uncomp_temp)
{
	int32_t var1, var2;
	var1 =
			((((uncomp_temp / 8)
					- ((int32_t) compensationParameters.dig_t1 << 1)))
					* ((int32_t) compensationParameters.dig_t2)) / 2048;
	var2 = (((((uncomp_temp / 16) - ((int32_t) compensationParameters.dig_t1))
			* ((uncomp_temp / 16) - ((int32_t) compensationParameters.dig_t1)))
			/ 4096) * ((int32_t) compensationParameters.dig_t3)) / 16384;
	t_fine = var1 + var2;
	return (t_fine * 5 + 128) / 256;
}

/**
 * Calculate pressure from measurement and compensation parameters.
 * @param uncomp_pres: Raw pressure measurement.
 * @return Pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
 * Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
 * */
uint32_t BMP280::compensate_pressure(int32_t uncomp_pres)
{
	int64_t var1, var2, p;

	var1 = ((int64_t) (t_fine)) - 128000;
	var2 = var1 * var1 * (int64_t) compensationParameters.dig_p6;
	var2 = var2 + ((var1 * (int64_t) compensationParameters.dig_p5) * 131072);
	var2 = var2 + (((int64_t) compensationParameters.dig_p4) * 34359738368);
	var1 = ((var1 * var1 * (int64_t) compensationParameters.dig_p3) / 256)
			+ ((var1 * (int64_t) compensationParameters.dig_p2) * 4096);
	var1 = ((INT64_C(0x800000000000) + var1)
			* ((int64_t) compensationParameters.dig_p1)) / 8589934592;
	if (var1 == 0)
	{
		return 0;
	}
	p = 1048576 - uncomp_pres;
	p = (((((p * 2147483648U)) - var2) * 3125) / var1);
	var1 = (((int64_t) compensationParameters.dig_p9) * (p / 8192) * (p / 8192))
			/ 33554432;
	var2 = (((int64_t) compensationParameters.dig_p8) * p) / 524288;
	p = ((p + var1 + var2) / 256)
			+ (((int64_t) compensationParameters.dig_p7) * 16);
	return (uint32_t) p;
}
