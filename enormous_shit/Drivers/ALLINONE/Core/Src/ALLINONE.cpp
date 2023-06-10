/*
 * ALLINONE.C
 *
 *  Created on: May 30, 2023
 *      Author: YassineBakkali
 */

#include "ALLINONE.hpp"
#include "stm32h7xx_hal.h"

#include <stdint.h>
#include "task.h"
#include "semphr.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Modbus.h"

//modbus_t telegram[2];
//uint16_t ModbusDATA[15];
//modbusHandler_t ModbusH;
//
//static void StartTaskMaster(void *argument);
//osThreadId_t myTaskMasterHandle;
//const osThreadAttr_t myTaskMaster_attributes = {
//  .name = "myTaskMaster",
//  .stack_size = 128 * 4,
//  .priority = (osPriority_t) osPriorityLow,
//};
//
//ALLINONE::ALLINONE(UART_HandleTypeDef *handle)
//{
//	UartHandle = handle;
//}
//
//void ALLINONE::initialization(uint16_t DE_PIN, GPIO_TypeDef *DE_PORT, uint16_t RE_PIN, GPIO_TypeDef *RE_PORT){
//  /* Modbus Master initialization */
//   ModbusH.uModbusType = MB_MASTER;
//   ModbusH.port = UartHandle;
//   ModbusH.u8id = 0; // For master it must be 0
//   ModbusH.u16timeOut = 1000;
//   ModbusH.EN_Port1 = DE_PORT;
//   ModbusH.EN_Pin1 = DE_PIN;
//   ModbusH.EN_Port2 = RE_PORT;
//   ModbusH.EN_Pin2 = RE_PIN;
//   ModbusH.u16regs = ModbusDATA;
//   ModbusH.u16regsize= sizeof(ModbusDATA)/sizeof(ModbusDATA[0]);
//   ModbusH.xTypeHW = USART_HW;
//   //Initialize Modbus library
//   ModbusInit(&ModbusH);
//}
//
//void ALLINONE::sensor_start(){
//   //Start capturing traffic on serial Port
//   ModbusStart(&ModbusH);
//   myTaskMasterHandle = osThreadNew(StartTaskMaster, NULL, &myTaskMaster_attributes);
//
//}
//
//float ALLINONE::get_temperature(){
//	return ((ModbusH.xBufferRX.uxBuffer[6] < 8 ) | (ModbusH.xBufferRX.uxBuffer[7]));
//
//};
//
//float ALLINONE::get_moisturelvl(){
//
//
//};
//float ALLINONE::get_conductivity(){
//
//
//};
//float ALLINONE::get_PH(){
//
//
//};
//
//static void StartTaskMaster(void *argument){
//  /* Infinite loop */
//	 // telegram 0: read registers
//
//  uint32_t u32NotificationValue;
//
//  telegram[0].u8id = 0x01; // slave address
//  telegram[0].u8fct = (mb_functioncode_t)3; // function code (this one is registers read)
//  //telegram[0].u16RegAdd = 0x160; // start address in slave
//  telegram[0].u16RegAdd = 0x0; // start address in slave
//  telegram[0].u16CoilsNo = 4; // number of elements (coils or registers) to read
//  telegram[0].u16reg = ModbusDATA; // pointer to a memory array in the Arduino
//  int aux;
//  for(;;)
//  {
//	  ModbusQuery(&ModbusH, telegram[0]); // make a query
//	  u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
//	  if(u32NotificationValue != ERR_OK_QUERY)
//	  {
//		//handle error
//		//  while(1);
//		  aux = 1;
//	  }
//	  osDelay(10);
//  }
//}


