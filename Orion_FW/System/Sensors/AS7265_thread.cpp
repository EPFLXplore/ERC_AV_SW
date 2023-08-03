/*
 * AS7265_thread.cpp
 *
 *      Author: Vincent Nguyen
 */

#include "AS7265_thread.h"

//#include "Telemetry.h"

AS7265Thread* AS7265Instance = nullptr;

void AS7265Thread::init() {
	AS7265Instance = this;
	// Initialize the sensor
	HAL_StatusTypeDef status;
	status = spectro.begin();
	// If the sensor was not found or incorrectly initialized, reset prober
	if(status != HAL_OK) {
		AS7265Instance = nullptr;
		terminate();
		parent->resetProber();
		return;
	}

	// Sensor related configuration after successfully connected
//	spectro.disableIndicator();
}

// Declare your data with the proper data structure defined in DataStructures.h
static ColorFilterData colorFilter_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
//static ColorFilterPacket color_filter_packet;

void AS7265Thread::loop() {
	uint8_t err_cnt = 0;
	HAL_StatusTypeDef status;
	status = spectro.takeMeasurementsWithBulb();
	if (status != HAL_OK)
		++err_cnt;

	status = spectro.getCalibratedA(colorFilter_data.data[0]); //410nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedB(colorFilter_data.data[1]); //435nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedC(colorFilter_data.data[2]); //460nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedD(colorFilter_data.data[3]); //485nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedE(colorFilter_data.data[4]); //510nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedF(colorFilter_data.data[5]); //535nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedG(colorFilter_data.data[6]); //560nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedH(colorFilter_data.data[7]); //585nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedR(colorFilter_data.data[8]); //610nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedI(colorFilter_data.data[9]); //645nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedS(colorFilter_data.data[10]); //680nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedJ(colorFilter_data.data[11]); //705nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedT(colorFilter_data.data[12]); //730nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedU(colorFilter_data.data[13]); //760nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedV(colorFilter_data.data[14]); //810nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedW(colorFilter_data.data[15]); //860nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedK(colorFilter_data.data[16]); //900nm
	if (status != HAL_OK)
		++err_cnt;
	status = spectro.getCalibratedL(colorFilter_data.data[17]); //940nm
	if (status != HAL_OK)
		++err_cnt;

	if(err_cnt == 0) {
		// Send data over RoCo network
		colorFilter_data.toArray((uint8_t*) &color_filter_packet);
		FDCAN1_network.send(&color_filter_packet);
		portYIELD();
	} else {
		AS7265Instance = nullptr;
		terminate();
		parent->resetProber();
	}
}

void AS7265Thread::handle_take_measurement(uint8_t sender_id, ColorFilterPacket* packet) {
	if (AS7265Instance == nullptr) {
		printf("AS7265 instance does not exist yet. \n");
		return;
	}
	printf("Dummy packet received from [%d] with value %d \n", sender_id, packet->data);
}
