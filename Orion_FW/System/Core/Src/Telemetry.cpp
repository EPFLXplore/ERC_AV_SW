/*
 * Telemetry.cpp
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine, Vincent
 */


#include <AS7265_thread.h>
#include <Servo_thread.h>
#include "Telemetry.h"
#include "tim.h"
#include "System.h"

ROCANDriver* FDCAN1_driver = nullptr;
ROCANDriver* FDCAN2_driver = nullptr;
CANBus* FDCAN1_network = nullptr;
CANBus* FDCAN2_network = nullptr;

void Telemetry::setup() {
	FDCAN1_driver = new ROCANDriver(&hfdcan1, System::get_node_id());
	FDCAN2_driver = new ROCANDriver(&hfdcan2, System::get_node_id());
	FDCAN1_network = new CANBus(FDCAN1_driver);
	FDCAN2_network = new CANBus(FDCAN2_driver);

	// Setup handles
	FDCAN1_network->handle<PingPacket>(&Telemetry::handle_ping);
	FDCAN1_network->handle<SpectroPacket>(&AS7265Thread::handle_take_measurement);
	FDCAN1_network->handle<ServoPacket>(&ServoThread::handle_rotate);
	FDCAN1_network->handle<LaserPacket>(&System::handle_meissa_output_cmd);

	FDCAN2_network->handle<PingPacket>(&Telemetry::handle_ping);
	FDCAN2_network->handle<SpectroPacket>(&AS7265Thread::handle_take_measurement);
	FDCAN2_network->handle<ServoPacket>(&ServoThread::handle_rotate);
	FDCAN2_network->handle<LaserPacket>(&System::handle_meissa_output_cmd);

}

void Telemetry::handle_ping(uint8_t sender_id, PingPacket* packet) {
	MAKE_IDENTIFIABLE(*packet);
	Telemetry::set_id(JETSON_NODE_ID);
	FDCAN1_network->send(packet);
	FDCAN2_network->send(packet);
}

void Telemetry::set_id(uint32_t id) {
	dynamic_cast<ROCANDriver*>(FDCAN1_network->get_driver())->TxHeaderConfigID(id);
	dynamic_cast<ROCANDriver*>(FDCAN2_network->get_driver())->TxHeaderConfigID(id);
}
