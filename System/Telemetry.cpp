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
#elif defined(BUILD_FOR_SCIENCE_B)
	STMUARTDriver UART6_driver(&huart6);
	NetworkBus network(&UART6_driver);
#elif defined(BUILD_FOR_HANDLING_DEVICE)
	STMUARTDriver UART3_driver(&huart3);
	NetworkBus network(&UART3_driver);
#endif



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


void setupTelemetry() {
#if defined(BUILD_FOR_NAVIGATION)
	STMUARTDriver_list.push_back(&UART2_driver);
	STMUARTDriver_list.push_back(&UART1_driver);
	STMUARTDriver_list.push_back(&UART3_driver);
	STMUARTDriver_list.push_back(&UART6_driver);

	// DO BNO AND LIMIT SWITCHES

	// SCIENCE -> AV
	network.forward<sc_LED_packet>(&UART1_network);
	network.forward<sc_trap_packet>(&UART1_network);
	// network.forward<sc_hx711_tare_packet>(&UART1_network); //IMPORTANT
	// network.forward<sc_hx711_calibrate_packet>(&UART1_network);	 //IMPORTANT
	// AV -> SCIENCE
	UART1_network.forward<sc_trap_success_packet>(&network);
	UART1_network.forward<sc_caching_success_packet>(&network);
	UART1_network.forward<avionics_voltmeter_packet>(&network);
	UART1_network.forward<avionics_massload_packet>(&network);
	UART1_network.forward<avionics_moisture_packet>(&network);

	// HANDLING DEVICE -> AV
	// network.forward<>(&UART3_network); // tare voltmeter
	// network.forward<hd_voltmeter_motor>(&UART3_network);
	// AV -> HANDLING DEVICE
	UART3_network.forward<avionics_voltmeter_packet>(&network);
	UART3_network.forward<avionics_TOF_packet>(&network);
	// UART3_network.forward<avionics_current_packet>(&network);
#elif defined(BUILD_FOR_SCIENCE_A)
	STMUARTDriver_list.push_back(&UART1_driver);
	network.handle<sc_LED_packet>(&handle_led);
	network.handle<sc_trap_packet>(&handle_servo_trap);
	network.handle<sc_caching_packet>(&handle_servo_caching);
#elif defined(BUILD_FOR_SCIENCE_B)
	STMUARTDriver_list.push_back(&UART6_driver);
	//network.handle<sc_hx711_tare_packet(&UART6_driver)>; //IMPORTANT
	//network.handle<sc_hx711_calibrate_packet(&UART6_driver)>;
#elif defined(BUILD_FOR_HANDLING_DEVICE)
	STUMUARTDriver_list.push_back(&UART3_driver);
	//network.handle<hd_tare_packet>(&handle_voltmeter_tare);
	//network.handle<hd_motor_packet>(&handle_voltmeter_motor); //IMPORTANT
	//network.handle<avionics_current_packet>(&handle_current)

#endif

}

