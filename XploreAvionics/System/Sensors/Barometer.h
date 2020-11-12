/*
 * Barometer.h
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#ifndef SENSORS_BAROMETER_H_
#define SENSORS_BAROMETER_H_

#include "Thread.h"
#include "../Drivers/Sensors/bmp280.h"
#include "../Drivers/Sensors/UartPrint.h"


class BarometerThread : Thread {
public:
	BarometerThread(I2C_HandleTypeDef* hi2c) : Thread("Barometer"), bmp280(), hi2c(hi2c)  {}
	void init();
	void loop();

private:
	BMP280_HandleTypedef bmp280;
	I2C_HandleTypeDef* hi2c;
};


#endif /* SENSORS_BAROMETER_H_ */
