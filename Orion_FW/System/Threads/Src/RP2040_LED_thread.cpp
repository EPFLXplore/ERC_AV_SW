/*
 * RP2040_LED_thread.cpp
 *
 *  Created on: Aug 27, 2023
 *      Author: Vincent
 */

#include <RP2040_LED_thread.h>
#include "Telemetry.h"

#include "Debug.h"


LEDThread* LEDInstance = nullptr;
static char cbuf[256]; // for printf

void LEDThread::init() {
	LEDInstance = this;
	// Initialize the sensor
	HAL_StatusTypeDef status;
	status = HAL_I2C_IsDeviceReady(parent->getI2C(), RP2040_ADDR << 1, 3, 100);
	// If the sensor was not found or incorrectly initialized, reset prober
	if(status != HAL_OK) {
		LOG_ERROR("Thread aborted");
		LEDInstance = nullptr;
		terminate();
		parent->resetProber();
		return;
	}

	LOG_SUCCESS("Thread successfully created");
}

void LEDThread::loop() {
	if(HAL_I2C_IsDeviceReady(parent->getI2C(), RP2040_ADDR << 1, 3, 100) == HAL_OK) {
		portYIELD();
	} else {
		LEDInstance = nullptr;
		terminate();
		parent->resetProber();
	}
}

uint8_t LEDThread::getPortNum() {
	return portNum;
}

static LEDResponsePacket led_response_packet;

void LEDThread::handle_led_request(uint8_t sender_id, LEDPacket* packet) {
	if (LEDInstance == nullptr) {
		console.printf_error("LEDThread instance does not exist yet\r\n");
		return;
	} else {
		// write to rp2040
	}
}



