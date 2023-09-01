/*
 * Telemetry.cpp
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine, Vincent
 */


#include <AS7265_thread.h>
#include <Servo_thread.h>
#include <ADS1234_Mass_thread.hpp>
#include <ADS1115_Potentiometer_thread.hpp>
#include <Servo_thread.h>
#include <AHRS_thread.h>
#include <RP2040_LED_thread.h>
#include "Telemetry.h"
#include "tim.h"
#include "System.h"

#include "Debug.h"

ROCANDriver* FDCAN1_driver = nullptr;
ROCANDriver* FDCAN2_driver = nullptr;
CANBus* FDCAN1_network = nullptr;
CANBus* FDCAN2_network = nullptr;

void Telemetry::setup() {
	FDCAN1_driver = new ROCANDriver(&hfdcan1, System::get_node_id());
	FDCAN2_driver = new ROCANDriver(&hfdcan2, System::get_node_id());
	FDCAN1_network = new CANBus(FDCAN1_driver);
	FDCAN2_network = new CANBus(FDCAN2_driver);

	// Ping handles
	FDCAN1_network->handle<PingPacket>(&Telemetry::handle_ping);
	FDCAN2_network->handle<PingPacket>(&Telemetry::handle_ping);

	// Request handles
	FDCAN1_network->handle<SpectroPacket>(&AS7265Thread::handle_take_measurement);
	FDCAN2_network->handle<SpectroPacket>(&AS7265Thread::handle_take_measurement);

	FDCAN1_network->handle<ServoPacket>(&ServoThread::handle_rotate);
	FDCAN2_network->handle<ServoPacket>(&ServoThread::handle_rotate);

	FDCAN1_network->handle<LaserPacket>(&System::handle_meissa_output_cmd);
	FDCAN2_network->handle<LaserPacket>(&System::handle_meissa_output_cmd);

	FDCAN1_network->handle<LEDPacket>(&LEDThread::handle_led_request);
	FDCAN2_network->handle<LEDPacket>(&LEDThread::handle_led_request);

	// Configuration handles
	FDCAN1_network->handle<MassConfigPacket>(&ADS1234Thread::handle_set_config);
	FDCAN2_network->handle<MassConfigPacket>(&ADS1234Thread::handle_set_config);

	FDCAN1_network->handle<PotentiometerConfigPacket>(&PotentiometerThread::handle_set_config);
	FDCAN2_network->handle<PotentiometerConfigPacket>(&PotentiometerThread::handle_set_config);

	FDCAN1_network->handle<ServoConfigPacket>(&ServoThread::handle_set_config);
	FDCAN2_network->handle<ServoConfigPacket>(&ServoThread::handle_set_config);

	FDCAN1_network->handle<MagConfigPacket>(&AHRSThread::handle_set_mag_config);
	FDCAN2_network->handle<MagConfigPacket>(&AHRSThread::handle_set_mag_config);

	FDCAN1_network->handle<AccelConfigPacket>(&AHRSThread::handle_set_accel_config);
	FDCAN2_network->handle<AccelConfigPacket>(&AHRSThread::handle_set_accel_config);

	FDCAN1_network->handle<GyroConfigPacket>(&AHRSThread::handle_set_gyro_config);
	FDCAN2_network->handle<GyroConfigPacket>(&AHRSThread::handle_set_gyro_config);

	// Calibration handles
	FDCAN1_network->handle<MassCalibPacket>(&ADS1234Thread::handle_mass_calib);
	FDCAN2_network->handle<MassCalibPacket>(&ADS1234Thread::handle_mass_calib);

	FDCAN1_network->handle<ImuCalibPacket>(&AHRSThread::handle_imu_calib);
	FDCAN2_network->handle<ImuCalibPacket>(&AHRSThread::handle_imu_calib);

}

void Telemetry::handle_ping(uint8_t sender_id, PingPacket* packet) {
	if(!(IS_RELIABLE(*packet))) {
		console.printf_error("Unreliable ping packet");
		return;
	}
	MAKE_IDENTIFIABLE(*packet);
	MAKE_RELIABLE_MCU(*packet);
	Telemetry::set_id(JETSON_NODE_ID);
	if (sender_id == 1)
		FDCAN1_network->send(packet);
	else if (sender_id == 2)
		FDCAN2_network->send(packet);
}

void Telemetry::set_id(uint32_t id) {
	dynamic_cast<ROCANDriver*>(FDCAN1_network->get_driver())->TxHeaderConfigID(id);
	dynamic_cast<ROCANDriver*>(FDCAN2_network->get_driver())->TxHeaderConfigID(id);
}
