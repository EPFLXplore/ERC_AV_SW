/*
 * ESP32_Neopixel_thread.cpp
 *
 *  Created on: Aug 25, 2024
 *      Author: eliot
 */


#include <ESP32_Neopixel_thread.h>
#include "Telemetry.h"

#include "Debug.h"

#include "i2c.h"


LEDThread* LEDInstance = nullptr;

void LEDThread::init() {
	LEDInstance = this;
	// Initialize the sensor
	HAL_StatusTypeDef status;
	status = HAL_I2C_IsDeviceReady(parent->getI2C(), ESP32_ADDR << 1, 3, 100);
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
	if(HAL_I2C_IsDeviceReady(parent->getI2C(), ESP32_ADDR << 1, 3, 100) == HAL_OK) {
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
	if(!(IS_RELIABLE_MCU(*packet))) {
		console.printf_error("Unreliable LED packet");
		return;
	}
	if(System::using_USB_5V()) {
		console.printf_error("Currently using USB 5V, which is limited to 500 mA. Not accepting LED command.");
		return;
	}
	led_response_packet.success = false;
	if (LEDInstance == nullptr) {
		console.printf_error("LEDThread instance does not exist yet\r\n");
		return;
	} else {
		LEDInstance->LOG_INFO("Received LED state change request");
		if (packet->low<packet->high) {
			uint8_t data[4] = {packet->low,packet->high,packet->system,packet->mode};
			HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(LEDInstance->parent->getI2C(), ESP32_ADDR << 1, data, 4, 1000);
			if (status != HAL_OK) {
				led_response_packet.success = false;
				LEDInstance->LOG_ERROR("Thread aborted");
				MAKE_IDENTIFIABLE(led_response_packet);
				MAKE_RELIABLE_MCU(led_response_packet);
				Telemetry::set_id(RASPBERRY_NODE_ID);
				if (sender_id == 1)
					FDCAN1_network->send(&led_response_packet);
				else if (sender_id == 2)
					FDCAN2_network->send(&led_response_packet);
				LEDInstance = nullptr;
				LEDInstance->terminate();
				LEDInstance->parent->resetProber();
			} else {
				led_response_packet.success = true;
				MAKE_IDENTIFIABLE(led_response_packet);
				MAKE_RELIABLE_MCU(led_response_packet);
				Telemetry::set_id(RASPBERRY_NODE_ID);
				if (sender_id == 1)
					FDCAN1_network->send(&led_response_packet);
				else if (sender_id == 2)
					FDCAN2_network->send(&led_response_packet);
				portYIELD();
			}
		} else {
			LEDInstance->LOG_ERROR("Invalid LED state received: \n"
									"low: %d \n high: %d",
									packet->low, packet->high);
		}
	}
}



