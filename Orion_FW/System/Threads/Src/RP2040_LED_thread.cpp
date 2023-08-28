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
	led_response_packet.state = 0;
	led_response_packet.success = false;
	if (LEDInstance == nullptr) {
		console.printf_error("LEDThread instance does not exist yet\r\n");
		return;
	} else {
		LEDInstance->LOG_INFO("Received LED state change request");
		if (packet->state < 4) {
			HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(LEDInstance->parent->getI2C(), RP2040_ADDR << 1, &(packet->state), 1, 1000);
			if (status != HAL_OK) {
				led_response_packet.success = false;
				LEDInstance->LOG_ERROR("Thread aborted");
				MAKE_IDENTIFIABLE(led_response_packet);
				Telemetry::set_id(JETSON_NODE_ID);
				if (sender_id == 1)
					FDCAN1_network->send(&led_response_packet);
				else if (sender_id == 2)
					FDCAN2_network->send(&led_response_packet);
				LEDInstance = nullptr;
				LEDInstance->terminate();
				LEDInstance->parent->resetProber();
			} else {
				led_response_packet.success = true;
				led_response_packet.state = packet->state;
				MAKE_IDENTIFIABLE(led_response_packet);
				Telemetry::set_id(JETSON_NODE_ID);
				if (sender_id == 1)
					FDCAN1_network->send(&led_response_packet);
				else if (sender_id == 2)
					FDCAN2_network->send(&led_response_packet);
				portYIELD();
			}
		} else {
			LEDInstance->LOG_ERROR("Invalid LED state received: %d", packet->state);
		}
	}
}



