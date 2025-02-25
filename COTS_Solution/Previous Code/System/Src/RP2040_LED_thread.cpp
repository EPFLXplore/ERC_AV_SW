/*
 * RP2040_LED_thread.cpp
 *
 *  Created on: Aug 27, 2023
 *      Author: Vincent
 */

#include <RP2040_LED_thread.h>
#include "Telemetry.h"

#include "Debug.h"

#include "i2c.h"


LEDThread* LEDInstance = nullptr;

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
#ifdef USE_GPIO
	portYIELD();
#else
	if(HAL_I2C_IsDeviceReady(parent->getI2C(), RP2040_ADDR << 1, 3, 100) == HAL_OK) {
		portYIELD();
	} else {
		LEDInstance = nullptr;
		terminate();
		parent->resetProber();
	}
#endif
}

void LEDThread::gpio_command(uint8_t cmd) {
	GPIO_TypeDef* GPIOx1;
	GPIO_TypeDef* GPIOx2;
	GPIO_TypeDef* GPIOx3;
	GPIO_TypeDef* GPIOx4;
	uint16_t GPIO_Pin1;
	uint16_t GPIO_Pin2;
	uint16_t GPIO_Pin3;
	uint16_t GPIO_Pin4;

	if (parent->getI2C() == &hi2c1) {
		GPIOx1 = HAT1_P1_GPIO_Port;
		GPIOx2 = HAT1_P2_GPIO_Port;
		GPIOx3 = HAT1_P3_GPIO_Port;
		GPIOx4 = HAT1_P4_GPIO_Port;
		GPIO_Pin1 = HAT1_P1_Pin;
		GPIO_Pin2 = HAT1_P2_Pin;
		GPIO_Pin3 = HAT1_P3_Pin;
		GPIO_Pin4 = HAT1_P4_Pin;
	} else if (parent->getI2C() == &hi2c2) {
		GPIOx1 = HAT2_P1_GPIO_Port;
		GPIOx2 = HAT2_P2_GPIO_Port;
		GPIOx3 = HAT2_P3_GPIO_Port;
		GPIOx4 = HAT2_P4_GPIO_Port;
		GPIO_Pin1 = HAT2_P1_Pin;
		GPIO_Pin2 = HAT2_P2_Pin;
		GPIO_Pin3 = HAT2_P3_Pin;
		GPIO_Pin4 = HAT2_P4_Pin;
	} else if (parent->getI2C() == &hi2c3) {
		GPIOx1 = HAT3_P1_GPIO_Port;
		GPIOx2 = HAT3_P2_GPIO_Port;
		GPIOx3 = HAT3_P3_GPIO_Port;
		GPIOx4 = HAT3_P4_GPIO_Port;
		GPIO_Pin1 = HAT3_P1_Pin;
		GPIO_Pin2 = HAT3_P2_Pin;
		GPIO_Pin3 = HAT3_P3_Pin;
		GPIO_Pin4 = HAT3_P4_Pin;
	}
	switch (cmd) {
	case 0:
		HAL_GPIO_WritePin(GPIOx1, GPIO_Pin1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOx2, GPIO_Pin2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx3, GPIO_Pin3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx4, GPIO_Pin4, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOx1, GPIO_Pin1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx2, GPIO_Pin2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOx3, GPIO_Pin3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx4, GPIO_Pin4, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOx1, GPIO_Pin1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx2, GPIO_Pin2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx3, GPIO_Pin3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOx4, GPIO_Pin4, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOx1, GPIO_Pin1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx2, GPIO_Pin2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx3, GPIO_Pin3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx4, GPIO_Pin4, GPIO_PIN_SET);
		break;
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
	led_response_packet.state = 0;
	led_response_packet.success = false;
	if (LEDInstance == nullptr) {
		console.printf_error("LEDThread instance does not exist yet\r\n");
		return;
	} else {
		LEDInstance->LOG_INFO("Received LED state change request");
		if (packet->state < 4) {
#ifdef USE_GPIO
			LEDInstance->gpio_command((packet->state));
			HAL_StatusTypeDef status = HAL_OK;
#else
			HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(LEDInstance->parent->getI2C(), RP2040_ADDR << 1, &(packet->state), 1, 1000);
#endif
			if (status != HAL_OK) {
				led_response_packet.success = false;
				LEDInstance->LOG_ERROR("Thread aborted");
				MAKE_IDENTIFIABLE(led_response_packet);
				MAKE_RELIABLE_MCU(led_response_packet);
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
				MAKE_RELIABLE_MCU(led_response_packet);
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



