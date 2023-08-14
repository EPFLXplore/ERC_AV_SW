/*
 * Telemetry.cpp
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine, Vincent
 */


#include "Telemetry.h"
#include "tim.h"
#include "AS7265_thread.h"
#include "Servo_thread.h"
#include "System.h"

ROCANDriver* FDCAN1_driver = nullptr;
CANBus* FDCAN1_network = nullptr;

void Telemetry::setup() {
	FDCAN1_driver = new ROCANDriver(&hfdcan1, System::get_node_id());
	FDCAN1_network = new CANBus(FDCAN1_driver);

	// Setup handles
	FDCAN1_network->handle<PingPacket>(&Telemetry::handle_ping);
	FDCAN1_network->handle<SpectroPacket>(&AS7265Thread::handle_take_measurement);
	FDCAN1_network->handle<ServoPacket>(&ServoThread::handle_rotate);
	FDCAN1_network->handle<LaserPacket>(&System::handle_meissa_output_cmd);

}

void Telemetry::handle_ping(uint8_t sender_id, PingPacket* packet) {
	MAKE_IDENTIFIABLE(*packet);
	Telemetry::set_id(JETSON_NODE_ID);
	FDCAN1_network->send(packet);
}

void Telemetry::set_id(uint32_t id) {
	dynamic_cast<ROCANDriver*>(FDCAN1_network->get_driver())->TxHeaderConfigID(id);
}
