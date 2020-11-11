/*
 * Barometer.h
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#ifndef SENSORS_BAROMETER_H_
#define SENSORS_BAROMETER_H_

#include "Thread.h"

class BarometerThread : Thread {
public:
	BarometerThread() : Thread("Barometer"), bmp280() {}
	void init(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef* huart);
	void loop();


private:
	BMP280_HandleTypedef bmp280;
};


#endif /* SENSORS_BAROMETER_H_ */
