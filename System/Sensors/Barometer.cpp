/*
 * Barometer.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#include "Barometer.h"

extern "C" {
#include "Libraries/bmp280.h"
}

void BarometerThread::init() {
	bmp280_init_default_params(&bmp280.params);
	bmp280.addr = BMP280_I2C_ADDRESS_0;
	bmp280.i2c = hi2c;
	while (!bmp280_init(&bmp280, &bmp280.params)) {
		char data[256]; //Random size, check this
	  	int size = sprintf((char *)data, "BMP280 initialization failed\n");
	  	printToUart(huart, (uint8_t*)data, size);
	  	osDelay(500);
	 }
}

void BarometerThread::loop() {
	float temperature;
	float pressure;
	float humidity;

	bmp280_read_float(&bmp280, &temperature, &pressure, &humidity);
	osDelay(100);
}




