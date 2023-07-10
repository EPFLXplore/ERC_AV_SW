/*
 * ALLINONE.h
 *
 *  Created on: May 30, 2023
 *      Author: YassineBakkali
 */

#ifndef ALLINONE_CORE_INC_ALLINONE_HPP_
#define ALLINONE_CORE_INC_ALLINONE_HPP_


#include "stm32h7xx_hal.h"
#include "usart.h"
#include <Modbus.h>


class ALLINONE{
public:
	ALLINONE(UART_HandleTypeDef *handle);

	float get_temperature();
	float get_moisturelvl();
	float get_conductivity();
	float get_PH();

	void initialization(uint16_t DE_PIN, GPIO_TypeDef *DE_PORT, uint16_t RE_PIN, GPIO_TypeDef *RE_PORT);
	void sensor_start();

	modbus_t telegram[2];
	uint16_t ModbusDATA[15];
	modbusHandler_t ModbusH;

private:
	UART_HandleTypeDef *UartHandle;

};


#endif /* ALLINONE_CORE_INC_ALLINONE_HPP_ */
