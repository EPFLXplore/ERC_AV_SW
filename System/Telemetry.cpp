/*
 * Telemetry.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */


#include "Telemetry.h"
#include "Build.h"

//
//static volatile uint8_t* control = (uint8_t*) 0x38001000;
//static volatile uint8_t* storage = (uint8_t*) 0x38001001;
//

STMUARTDriver UART1_driver(&huart1);
STMUARTDriver UART2_driver(&huart2);
STMUARTDriver UART3_driver(&huart3);
STMUARTDriver UART6_driver(&huart6);
//
NetworkBus UART1_network(&UART1_driver);
NetworkBus UART2_network(&UART2_driver);
NetworkBus UART3_network(&UART3_driver);
NetworkBus network(&UART6_driver);


//#if defined(BUILD_FOR_NAVIGATION)
////	UART1_network->forward<>(UART2_network);
//	UART3_network.forward<avionics_voltmeter_packet>(&UART2_network);
////	UART6_network->forward<>(UART2_network);
//#endif

//#if defined(BUILD_FOR_HANDLING_DEVICE)
//	STMUARTDriver drivers(&huart6);
////	STMUARTDriver driver = new NAV_driver(&huart6);
//#elif defined(BUILD_FOR_NAVIGATION)
//	STMUARTDriver drivers[3] =
//	{
//	STMUARTDriver(&huart6),
//	STMUARTDriver(&huart3),
//	STMUARTDriver(&huart1)
//	};
//#elif defined(BUILD_FOR_SC1)
//	STMUARTDriver drivers[2] =
//	{
//	STMUARTDriver(&huart3),
//	STMUARTDriver(&huart6)
//	};
//#elif defined(BUILD_FOR_SC2)
//	STMUARTDriver drivers[2] =
//	{
//	STMUARTDriver(&huart1),
//	STMUARTDriver(&huart6)
//	};
//#endif

//STMUARTDriver driver(&huart6);
//NetworkBus network(&driver);


//NetworkBus network(&telemtryDriver);

void setupTelemtry() {
//	network.forward<PingPacket>(&network);
}
