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
#include "Servo_thread.h"
#include "System.h"

//ROCANDriver FDCAN1_driver(&hfdcan1,8);
//ROCANDriver FDCAN2_driver(&hfdcan2);
//CANBus FDCAN1_network(&FDCAN1_driver);
//CANBus FDCAN2_network(&FDCAN2_driver);

ROCANDriver* FDCAN1_driver = nullptr;
CANBus* FDCAN1_network = nullptr;

void setupTelemetry(){

	// Setup FDCAN bus filters
//	FDCAN1_driver.filterConfig(System::get_node_id());
//	FDCAN2_driver.filterConfig(System::get_node_id());
	FDCAN1_driver = new ROCANDriver(&hfdcan1, System::get_node_id());
	FDCAN1_network = new CANBus(FDCAN1_driver);

	// Setup handles
	FDCAN1_network->handle<DummyPacket>(&handle_dummyCallback);
	FDCAN1_network->handle<SpectroPacket>(&AS7265Thread::handle_take_measurement);
	FDCAN1_network->handle<ServoPacket>(&ServoThread::handle_rotate);
	FDCAN1_network->handle<PingPacket>(&handle_ping);
}
