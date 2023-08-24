/*
 * AS7265_thread.cpp
 *
 *      Author: Vincent Nguyen
 */

#include <AS7265_thread.h>
#include "Telemetry.h"

#include "Debug.h"


AS7265Thread* AS7265Instance = nullptr;
static char cbuf[256]; // for printf

void AS7265Thread::init() {
	AS7265Instance = this;
	// Initialize the sensor
	HAL_StatusTypeDef status;
	status = spectro.begin();
	// If the sensor was not found or incorrectly initialized, reset prober
	if(status != HAL_OK) {
		LOG_ERROR("Thread aborted");
		AS7265Instance = nullptr;
		terminate();
		parent->resetProber();
		return;
	}

	LOG_SUCCESS("Thread successfully created");

	// Sensor related configuration after successfully connected
//	spectro.disableIndicator();
}

// Declare your data with the proper data structure defined in DataStructures.h
static SpectroData spectro_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
//static ColorFilterPacket color_filter_packet;

void AS7265Thread::loop() {
	if(HAL_I2C_IsDeviceReady(parent->getI2C(), AS7265X_ADDR << 1, 3, 100) == HAL_OK) {
		if(monitor.enter(SPECTRO_MONITOR)) {
			println("%s", spectro_data.toString(cbuf));
		}
		portYIELD();
	} else {
		AS7265Instance = nullptr;
		terminate();
		parent->resetProber();
	}
}

uint8_t AS7265Thread::getPortNum() {
	return portNum;
}

void AS7265Thread::take_measurements(uint8_t sender_id) {
	uint8_t err_cnt = 0;
	HAL_StatusTypeDef status;
	status = spectro.takeMeasurementsWithBulb();
	if (status != HAL_OK)
		++err_cnt;

	status = spectro.getCalibratedA(spectro_data.data[0]); //410nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedB(spectro_data.data[1]); //435nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedC(spectro_data.data[2]); //460nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedD(spectro_data.data[3]); //485nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedE(spectro_data.data[4]); //510nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedF(spectro_data.data[5]); //535nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedG(spectro_data.data[6]); //560nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedH(spectro_data.data[7]); //585nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedR(spectro_data.data[8]); //610nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedI(spectro_data.data[9]); //645nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedS(spectro_data.data[10]); //680nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedJ(spectro_data.data[11]); //705nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedT(spectro_data.data[12]); //730nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedU(spectro_data.data[13]); //760nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedV(spectro_data.data[14]); //810nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedW(spectro_data.data[15]); //860nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedK(spectro_data.data[16]); //900nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedL(spectro_data.data[17]); //940nm
	if (status != HAL_OK)
		++err_cnt;

	if(err_cnt == 0) {
		spectro_data.success = true;
		spectro_data.max_data_val = get_max_val(spectro_data.data, 18);
		spectro_data.toArray((uint8_t*) &spectro_response_packet);
		MAKE_IDENTIFIABLE(spectro_response_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		if (sender_id == 1)
			FDCAN1_network->send(&spectro_response_packet);
		else if (sender_id == 2)
			FDCAN2_network->send(&spectro_response_packet);
		portYIELD();
	} else {
		spectro_data.max_data_val = 0.f;
		LOG_ERROR("Thread aborted");
		spectro_data.success = false;
		spectro_data.toArray((uint8_t*) &spectro_response_packet);
		MAKE_IDENTIFIABLE(spectro_response_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		if (sender_id == 1)
			FDCAN1_network->send(&spectro_response_packet);
		else if (sender_id == 2)
			FDCAN2_network->send(&spectro_response_packet);
		AS7265Instance = nullptr;
		terminate();
		parent->resetProber();
	}
}

void AS7265Thread::handle_take_measurement(uint8_t sender_id, SpectroPacket* packet) {
	if (AS7265Instance == nullptr) {
		console.printf_error("AS7265Thread instance does not exist yet\r\n");
		return;
	}
	else if (packet->measure) {
		AS7265Instance->LOG_INFO("Received measurement request");
		AS7265Instance->take_measurements(sender_id);
	}
}

