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


static char cbuf[128];

void BarometerThread::init() {
	bmp280_init_default_params(&bmp280.params);

	bmp280.params.standby = BMP280_STANDBY_05;
	bmp280.addr = BMP280_I2C_ADDRESS_0;
	bmp280.i2c = parent->getI2C();

	//portENTER_CRITICAL();
	while (!bmp280_init(&bmp280, &bmp280.params)) {
		//portEXIT_CRITICAL();
		println("BMP280 initialization failed");
		parent->resetProber();
	}
	//portEXIT_CRITICAL();

	println("BMP280 initialized");
}

static Avionics_BaroTempPacket packet;
static BaroData data;
void BarometerThread::loop() {
	if(bmp280_read_float(&bmp280, &data.temperature, &data.pressure, &data.humidity)) {
		println("%s", data.toString(cbuf));
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
		println("BMP280 disconnected");
		terminate();
		parent->resetProber();
	}
}
