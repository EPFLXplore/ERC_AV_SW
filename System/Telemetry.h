/*
 * Telemetry.h
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */

#ifndef TELEMETRY_TELEMETRY_H_
#define TELEMETRY_TELEMETRY_H_


#include "RoCo.h"
#include "usart.h"
//#define ROCO_BUFFER_SIZE 128
//uint8_t buffer[ROCO_BUFFER_SIZE];
//

extern STMUARTDriver UART1_driver;
extern STMUARTDriver UART2_driver;
extern STMUARTDriver UART3_driver;
extern STMUARTDriver UART6_driver;

extern NetworkBus UART1_network;
extern NetworkBus UART2_network;
extern NetworkBus UART3_network;
extern NetworkBus network;

//#if defined(BUILD_FOR_HANDLING_DEVICE)
//	extern STMUARTDriver NAV_driver;
//	extern NetworkBus HD_NAV_network;
//#elif defined(BUILD_FOR_NAVIGATION)
//	extern STMUARTDriver HD_driver;
//	extern STMUARTDriver SC1_driver;
//	extern STMUARTDriver SC2_driver;
////	STMUARTDriver CS_driver(&huart6);
//	extern NetworkBus NAV_HD_network;
//	extern NetworkBus NAV_SC1_network;
//	extern NetworkBus NAV_SC2_network;
//#elif defined(BUILD_FOR_SC1)
//	extern STMUARTDriver NAV_driver;
//	extern STMUARTDriver SC2_driver;
//	extern NetworkBus SC1_NAV_network;
//	extern NetworkBus SC1_SC2_network;
//#elif defined(BUILD_FOR_SC2)
//	extern STMUARTDriver NAV_driver;
//	extern STMUARTDriver SC1_driver;
//	extern NetworkBus SC2_NAV_network;
//	extern NetworkBus SC2_SC1_network;
//#endif

//extern STMUARTDriver driver;
//extern NetworkBus network;


//IODriver* telemtryDriver = new STMUARTDriver(&huart2); // Point to Huart2
//IOBus* network = new IOBus((IODriver*) telemtryDriver, buffer, ROCO_BUFFER_SIZE);
//void setupTelemtry();


#endif /* TELEMETRY_TELEMETRY_H_ */
