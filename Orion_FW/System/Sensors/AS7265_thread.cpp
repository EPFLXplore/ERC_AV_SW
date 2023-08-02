/*
 * AS7265_thread.cpp
 *
 *      Author: Vincent
 */

#include "AS7265_thread.h"

#include "Telemetry.h"

AS7265Thread* AS7265Instance = nullptr;

void AS7265Thread::init() {
	AS7265Instance = this;
	// Initialize the sensor
	bool success = spectro.begin();
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(!success) {
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
//static ColorFilterPacket packet;

void AS7265Thread::loop() {

	spectro.takeMeasurementsWithBulb();
	colorFilter_data.data[0] =spectro.getCalibratedA(); //410nm
	colorFilter_data.data[1] =spectro.getCalibratedB(); //435nm
	colorFilter_data.data[2] =spectro.getCalibratedC(); //460nm
	colorFilter_data.data[3] =spectro.getCalibratedD(); //485nm
	colorFilter_data.data[4] =spectro.getCalibratedE(); //510nm
	colorFilter_data.data[5] =spectro.getCalibratedF(); //535nm
	colorFilter_data.data[6] =spectro.getCalibratedG(); //560nm
	colorFilter_data.data[7] =spectro.getCalibratedH(); //585nm
	colorFilter_data.data[8] =spectro.getCalibratedR(); //610nm
	colorFilter_data.data[9] =spectro.getCalibratedI(); //645nm
	colorFilter_data.data[10]=spectro.getCalibratedS(); //680nm
	colorFilter_data.data[11]=spectro.getCalibratedJ(); //705nm
	colorFilter_data.data[12]=spectro.getCalibratedT(); //730nm
	colorFilter_data.data[13]=spectro.getCalibratedU(); //760nm
	colorFilter_data.data[14]=spectro.getCalibratedV(); //810nm
	colorFilter_data.data[15]=spectro.getCalibratedW(); //860nm
	colorFilter_data.data[16]=spectro.getCalibratedK(); //900nm
	colorFilter_data.data[17]=spectro.getCalibratedL(); //940nm

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		// Send data over RoCo network
//		colorFilter_data.toArray((uint8_t*) &packet);
//		FDCAN1_network.send(&packet);
		portYIELD();
	} else {
		AS7265Instance = nullptr;
		terminate();
		parent->resetProber();
	}
}
