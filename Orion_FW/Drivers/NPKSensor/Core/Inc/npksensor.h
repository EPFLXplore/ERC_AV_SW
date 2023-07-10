/*
 * npksensor.h
 *
 *  Created on: Jul 10, 2023
 *      Author: YassineBakkali
 */

#ifndef NPKSENSOR_CORE_INC_NPKSENSOR_H_
#define NPKSENSOR_CORE_INC_NPKSENSOR_H_

#include <Modbus.h>
#include "stm32h7xx_hal.h"
#include "usart.h"


extern modbusHandler_t ModbusH;

class NPK{
public:
	NPK(UART_HandleTypeDef *handle);

	float get_nitrogen();
	float get_phosphorus();
	float get_potassium();
	float get_all();

	void initialization(uint16_t DE_PIN, GPIO_TypeDef *DE_PORT, uint16_t RE_PIN, GPIO_TypeDef *RE_PORT);
	void sensor_start();

private:
	UART_HandleTypeDef *UartHandle;

};




#endif /* NPKSENSOR_CORE_INC_NPKSENSOR_H_ */
