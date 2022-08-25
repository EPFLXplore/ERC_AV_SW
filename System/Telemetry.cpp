/*
 * Telemetry.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */


#include "Telemetry.h"
#include "Handlers.h"
#include "tim.h"
#include <vector>

//
//static volatile uint8_t* control = (uint8_t*) 0x38001000;
//static volatile uint8_t* storage = (uint8_t*) 0x38001001;
//

#if defined(BUILD_FOR_NAVIGATION)
	STMUARTDriver UART2_driver(&huart2);
	STMUARTDriver UART1_driver(&huart1);
	STMUARTDriver UART3_driver(&huart3);
	STMUARTDriver UART6_driver(&huart6);
	//

	NetworkBus UART6_network(&UART6_driver);

	NetworkBus UART1_network(&UART1_driver);
	NetworkBus UART3_network(&UART3_driver);
	//NetworkBus network(&UART6_driver);
	NetworkBus network(&UART2_driver);
#elif defined(BUILD_FOR_SCIENCE_A)
	STMUARTDriver UART1_driver(&huart1);
	NetworkBus network(&UART1_driver);
#endif



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

static std::vector<STMUARTDriver*> STMUARTDriver_list;

STMUARTDriver* getInstance(UART_HandleTypeDef* huart) {
	for (auto & driver : STMUARTDriver_list) {
		if (driver->getHuart() == huart)
			return driver;
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
	STMUARTDriver* driver = getInstance(huart);
	xSemaphoreGiveFromISR(driver->getSemaphore(), nullptr);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart) {
	STMUARTDriver* driver = getInstance(huart);
	while(xSemaphoreTakeFromISR(driver->getSemaphore(), nullptr)); // Clear semaphore
	driver->init();
}


//static STMUARTDriver{&UART2_driver};

void test_handle(uint8_t sender_id, sc_trap_success_packet* packet) {
	volatile bool test = packet->status;
	if (test) {
		osDelay(1);
	}
}

void setupTelemetry() {
#if defined(BUILD_FOR_NAVIGATION)
	STMUARTDriver_list.push_back(&UART2_driver);
	STMUARTDriver_list.push_back(&UART1_driver);
	STMUARTDriver_list.push_back(&UART3_driver);
	STMUARTDriver_list.push_back(&UART6_driver);
	network.forward<sc_LED_packet>(&UART1_network);
	network.forward<sc_trap_packet>(&UART1_network);
	UART1_network.forward<sc_trap_success_packet>(&network);
	UART1_network.forward<sc_caching_success_packet>(&network);
	osDelay(100);
	UART1_network.handle<sc_trap_success_packet>(&test_handle);
#elif defined(BUILD_FOR_SCIENCE_A)
	STMUARTDriver_list.push_back(&UART1_driver);
	network.handle<sc_LED_packet>(&handle_led);
	network.handle<sc_trap_packet>(&handle_servo_trap);
	network.handle<sc_caching_packet>(&handle_servo_caching);
#endif
//	network.forward<PingPacket>(&network);
//	network.forward<sc_LED_packet>(&UART1_network);
//	static STMUARTDriver UART1_driver(&huart1);
//	static NetworkBus UART1_network(&UART1_driver);
//
//	static STMUARTDriver UART6_driver(&huart6);
//	static NetworkBus UART6_network(&UART6_driver);

//	network.handle<sc_LED_packet>(&handle_led);
////	network.forward<sc_LED_packet>(&UART6_network);
//	network.forward<sc_LED_packet>(&UART1_network);

}

