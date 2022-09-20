/*
 * Stemma_thread.cpp
 *
 *  Created on: 10 Aug, 2022
 *      Author: Yassine
 */

#include "Telemetry.h"
#include "Stemma_thread.h"



void StemmaThread::init() {
	uint8_t status = HAL_ERROR;
	status = stemma_init(&stemma, parent->getI2C());
	if(status != 0) {
		terminate();
		parent->resetProber();
		return;
	}
}

static ScienceData data;
static avionics_moisture_packet packet;
void StemmaThread::loop() {
	  HAL_StatusTypeDef status = HAL_ERROR;
	  status = stemma_ReadMoisture(&stemma, 0);
		if(status == HAL_OK) {
			data.moisture = stemma.moisture;
			data.toArray((uint8_t*) &packet);
			network.send(&packet);
			portYIELD();
		} else {
			terminate();
			parent->resetProber();
		}
}

