/*
 * TOF_thread.cpp
 *
 *  Created on: 13 Aug 2020
 *      Author: YassineBakkali
 */

#include "TOF_thread.h"
#include "Telemetry.h"


void TOFThread::init() {

	  VL53L1_Error status = VL53L1_ERROR_NONE;
	  Dev->I2cHandle = parent->getI2C();
	  Dev->I2cDevAddr = 0x52; // Not entirely sure, please run I2C address scanner
	  status = VL53L1_WaitDeviceBooted( Dev );
	  VL53L1_DataInit( Dev );
	  if(status != VL53L1_ERROR_NONE) {
	  //		println("[%d] BNO055 initialization failed", portNum);
	  		terminate();
	  		parent->resetProber();
	  		return;
	  	}
	  VL53L1_StaticInit( Dev );
	  VL53L1_SetDistanceMode( Dev, VL53L1_DISTANCEMODE_LONG );
	  VL53L1_SetMeasurementTimingBudgetMicroSeconds( Dev, 50000 );
	  VL53L1_SetInterMeasurementPeriodMilliSeconds( Dev, 500 );
	  VL53L1_StartMeasurement( Dev );

}

static TOFData data;
static avionics_ToF_packet packet;
void TOFThread::loop() {

		VL53L1_WaitMeasurementDataReady( Dev );
		VL53L1_Error status = VL53L1_GetRangingMeasurementData( Dev, &RangingData );
		if(status == VL53L1_ERROR_NONE) {
		//		println("[i2c%d] %s", portNum, data.toString(cbuf));
			data.distance = RangingData.RangeMilliMeter;
			data.toArray((uint8_t*) &packet);
			network.send(&packet);
			portYIELD();
		} else {
		//		println("[i2c%d] BNO055 disconnected", portNum);
			terminate();
			parent->resetProber();
		}
		VL53L1_ClearInterruptAndStartMeasurement( Dev );

}
