/*
 * Telemetry.cpp
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine, Vincent
 */


#include "Telemetry.h"
#include "Handlers.h"
#include "tim.h"
#include "AS7265_thread.h"
//#include <vector>


static uint32_t can_id1 = 0x32;
ROCANDriver FDCAN1_driver(&hfdcan1, can_id1);
//STMUARTDriver UART1_driver(&huart1);
//STMUARTDriver UART3_driver(&huart3);
////STMUARTDriver UART4_driver(&huart4);
//STMUARTDriver UART5_driver(&huart5);
////STMUARTDriver UART8_driver(&huart8);
//
//
CANBus FDCAN1_network(&FDCAN1_driver);
//NetworkBus JetsonNetwork(&UART1_driver);
//NetworkBus UART3_network(&UART3_driver);
////NetworkBus UART4_network(&UART4_driver);
//NetworkBus UART5_network(&UART5_driver);
//NetworkBus UART8_network(&UART8_driver);

void setupTelemetry(){
//	UART3_network.forward<DummySystem_DummyPacket>(&JetsonNetwork);
//	UART4_network.forward<DummySystem_DummyPacket>(&JetsonNetwork);
//	UART5_network.forward<DummySystem_DummyPacket>(&JetsonNetwork);
//	UART8_network.forward<DummySystem_DummyPacket>(&JetsonNetwork);




	FDCAN1_network.handle<DummyPacket>(&handle_dummyCallback);
	FDCAN1_network.handle<SpectroPacket>(&AS7265Thread::handle_take_measurement);
//	JetsonNetwork.handle<DummySystem_DummyPacket>(&handle_dummyCallback);
#if defined(BUILD_FOR_NAVIGATION)
	STMUARTDriver_list.push_back(&UART2_driver);
	STMUARTDriver_list.push_back(&UART1_driver);
	STMUARTDriver_list.push_back(&UART3_driver);
	STMUARTDriver_list.push_back(&UART6_driver);

	// DO BNO AND LIMIT SWITCHES

	// SCIENCE -> AV
	network.forward<sc_LED_packet>(&UART1_network);
	network.forward<sc_trap_packet>(&UART1_network);
	network.forward<sc_caching_packet>(&UART1_network);
	network.forward<sc_mass_calibrate_packet>(&UART6_network); //IMPORTANT
	// network.forward<sc_hx711_calibrate_packet>(&UART1_network);	 //IMPORTANT

	// AV -> SCIENCE A
	UART1_network.forward<avionics_trap_success_packet>(&network);
	UART1_network.forward<avionics_caching_success_packet>(&network);

	// AV -> SCIENCE B
	UART6_network.forward<avionics_massload_packet>(&network);
	UART6_network.forward<avionics_moisture_packet>(&network);

	// AV -> HANDLING DEVICE
	UART3_network.forward<avionics_voltmeter_motor_status_packet>(&network);
	UART3_network.forward<avionics_voltmeter_packet>(&network);
	UART1_network.forward<avionics_voltmeter_tare_success_packet>(&network);
	UART3_network.forward<avionics_ToF_packet>(&network);
	// UART3_network.forward<avionics_current_packet>(&network);

	// HANDLING DEVICE -> AV
	 network.forward<hd_voltmeter_tare_packet>(&UART3_network);
	 network.forward<hd_voltmeter_motor_packet>(&UART3_network);

#elif defined(BUILD_FOR_SCIENCE_A)
	STMUARTDriver_list.push_back(&UART1_driver);
	network.handle<sc_LED_packet>(&handle_led);
	network.handle<sc_trap_packet>(&handle_servo_trap);
	network.handle<sc_caching_packet>(&handle_servo_caching);
#elif defined(BUILD_FOR_SCIENCE_B)
	STMUARTDriver_list.push_back(&UART1_driver);
	//network.handle<sc_hx711_tare_packet(&UART6_driver)>; //IMPORTANT
	network.handle<sc_mass_calibrate_packet>(&handle_mass_calibrate);
#elif defined(BUILD_FOR_HANDLING_DEVICE)
	STMUARTDriver_list.push_back(&UART1_driver);
	network.handle<hd_voltmeter_motor_packet>(&handle_voltmeter_motor);
	network.handle<hd_voltmeter_tare_packet>(&handle_voltmeter_tare);
	//network.handle<avionics_current_packet>(&handle_current)
#endif

}

