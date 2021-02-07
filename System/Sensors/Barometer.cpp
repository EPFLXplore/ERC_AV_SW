/*
 * Barometer.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: AV Team 2020
 */

#include "Barometer.h"

#include "Telemetry.h"
#include "DataStructures.h"
#include "Debug/Debug.h"


static char cbuf[256];

void BarometerThread::init() {
	bmp280_init_default_params(&bmp280.params);
	bmp280.addr = BMP280_I2C_ADDRESS_0;
	bmp280.i2c = hi2c;

	while (!bmp280_init(&bmp280, &bmp280.params)) {
		println("BMP280 initialization failed");
	  	osDelay(500);
	}

	println("BMP280 initialized");
}

void BarometerThread::loop() {
	BaroData data;

	bmp280_read_float(&bmp280, &data.temperature, &data.pressure, &data.humidity);

	println("%s", data.toString(cbuf));

	Avionics_BaroTempPacket packet;
	data.toArray((float*) &packet);

	network.send(&packet);

	osDelay(100);
}
