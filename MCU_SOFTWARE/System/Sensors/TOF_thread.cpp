/*
 * TOF_thread.cpp
 *
 *  Created on: 13 Aug, 2022
 *      Author: Yassine
 */

#include "TOF_thread.h"
#include "Telemetry.h"


void TOFThread::init() {

	  VL53L1_Error status = VL53L1_ERROR_NONE;
	  Dev->I2cHandle = parent->getI2C();
	  Dev->I2cDevAddr = 0x52;
	  status = VL53L1_WaitDeviceBooted( Dev );
	  status = VL53L1_DataInit( Dev );
	  if(status != VL53L1_ERROR_NONE) {
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
			data.distance = RangingData.RangeMilliMeter;
			data.toArray((uint8_t*) &packet);
			network.send(&packet);
			portYIELD();
		} else {
			terminate();
			parent->resetProber();
		}
		VL53L1_ClearInterruptAndStartMeasurement( Dev );

}
