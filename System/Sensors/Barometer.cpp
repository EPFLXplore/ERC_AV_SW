/*
 * Barometer.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: AV Team 2020
 */

#include "Barometer.h"

#include "DataStructures.h"
#include "Debug/Debug.h"


static char cbuf[256];

void BarometerThread::init() {
	bmp280_init_default_params(&bmp280.params);
	bmp280.addr = BMP280_I2C_ADDRESS_0;
	bmp280.i2c = hi2c;

	while (!bmp280_init(&bmp280, &bmp280.params)) {
		console.printf("BMP280 initialization failed\n");
	  	osDelay(500);
	}

	console.printf("BMP280 initialized\n");
}

void BarometerThread::loop() {
	BaroData data;

	bmp280_read_float(&bmp280, &data.temperature, &data.pressure, &data.humidity);

	console.printf("%s\n", data.toString(cbuf));

	writeToRtosBuffer(data);

	osDelay(100);
}

void BarometerThread::writeToRtosBuffer(BaroData data) {

}




