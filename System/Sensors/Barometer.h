/*
 * Barometer.h
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#ifndef SENSORS_BAROMETER_H_
#define SENSORS_BAROMETER_H_

#include "../Thread.hpp"
#include "../Libraries/bmp280.h"
#include "../Libraries/UartPrint.h"


class BarometerThread : Thread {
public:
	BarometerThread(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef* huart) : Thread("Barometer"), bmp280(), hi2c(hi2c), huart(huart)  {}
	void init();
	void loop();

private:
	BMP280_HandleTypedef bmp280;
	I2C_HandleTypeDef* hi2c;
	UART_HandleTypeDef* huart;
};


#endif /* SENSORS_BAROMETER_H_ */
