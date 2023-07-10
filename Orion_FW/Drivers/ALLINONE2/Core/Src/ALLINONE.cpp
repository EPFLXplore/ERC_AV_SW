/*
 * ALLINONE.C
 *
 *  Created on: May 30, 2023
 *      Author: YassineBakkali
 */

#include "ALLINONE.hpp"
#include "stm32h7xx_hal.h"

#include <stdint.h>
#include <Modbus.h>


ALLINONE::ALLINONE(UART_HandleTypeDef *handle)
{
	UartHandle = handle;
}

void ALLINONE::initialization(uint16_t DE_PIN, GPIO_TypeDef *DE_PORT, uint16_t RE_PIN, GPIO_TypeDef *RE_PORT){
  /* Modbus Master initialization */
   ModbusH.uModbusType = MB_MASTER;
   ModbusH.port = UartHandle;
   ModbusH.u8id = 0; // For master it must be 0
   ModbusH.u16timeOut = 1000;
   ModbusH.EN_Port1 = DE_PORT;
   ModbusH.EN_Pin1 = DE_PIN;
   ModbusH.EN_Port2 = RE_PORT;
   ModbusH.EN_Pin2 = RE_PIN;
   ModbusH.u16regs = ModbusDATA;
   ModbusH.u16regsize= sizeof(ModbusDATA)/sizeof(ModbusDATA[0]);
   ModbusH.xTypeHW = USART_HW;
   //Initialize Modbus library
   ModbusInit(&ModbusH);
}

void ALLINONE::sensor_start(){
   //Start capturing traffic on serial Port
   ModbusStart(&ModbusH);
}

float ALLINONE::get_temperature(){
	uint32_t u32NotificationValue;
    ModbusQuery(&ModbusH, telegram[0]); // make a query
    u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
    if(u32NotificationValue != ERR_OK_QUERY)
    	return 999;
	return ((ModbusH.xBufferRX.uxBuffer[6] < 4 ) | (ModbusH.xBufferRX.uxBuffer[7]))/10;
};

float ALLINONE::get_moisturelvl(){


};
float ALLINONE::get_conductivity(){


};
float ALLINONE::get_PH(){
	uint32_t u32NotificationValue;
    ModbusQuery(&ModbusH, telegram[0]); // make a query
    u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
    if(u32NotificationValue != ERR_OK_QUERY)
    	return 999;
	return ((ModbusH.xBufferRX.uxBuffer[12] < 4 ) | (ModbusH.xBufferRX.uxBuffer[13]))/10;

};



