/*
 * Barometer.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */


#include "Barometer.h"

void BarometerThread::init(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef* huart) {
	bmp280_init_default_params(&bmp280.params);
	bmp280.addr = BMP280_I2C_ADDRESS_0;
	bmp280.i2c = hi2c;
	while (!bmp280_init(&bmp280, &bmp280.params)) {
	  	size = sprintf((char *)Data, "BMP280 initialization failed\n");
	  	HAL_UART_Transmit(&huart3, Data, size, 1000);
	  	osDelay(2000);
	 }
}
void BarometerThread::loop() {
	float *temperature;
	float *pressure;
	float *humidity;

	bmp280_read_float(bmp280, temperature, pressure, humidity);

	osDelay(100);

}



