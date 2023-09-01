/*
 * system.cpp
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine, Vincent
 */

#include <Prober.h>
#include <Watchdog_thread.h>
#include <FDCAN_State_Indicator_thread.h>
#include "System.h"
#include "Debug.h"
#include "i2c.h"
#include "iwdg.h"

#include "Datastructures.h"

uint16_t System::node_id = DEFAULT_NODE_ID;

// Warning: do NOT put any delay in system init. This is because
// the scheduler only starts the threads AFTER osKernelStart() is called
// We don't want to delay the creation of the Watchdog thread
void System::init() {
	// Create shell
	static Shell shell(&huart1, &terminal);

	// Create watchdog thread first (10 Hz)
	static WatchdogThread watchdog(&hiwdg1);
	watchdog.setTickDelay(100);

	// Disable yellow error LED
	disable_yellow_led();

	// Disable FDCAN status LEDs
	disable_status_led_fdcan1();
	disable_status_led_fdcan2();

	// Disable FDCAN Silent mode
	disable_silent_mode_fdcan1();
	disable_silent_mode_fdcan2();

	// Configure LDO
	prioritize_3V3_buck();

	// Make sure the outputs are zero for meissa's outputs
	disable_meissa_5V_output();
	disable_meissa_3V3_output();

	// Read and update node ID from DIP switch
	update_node_id();

	// Create prober threads
	static ProberThread prober1(&hi2c1);
	prober1.setTickDelay(100);
	static ProberThread prober2(&hi2c2);
	prober2.setTickDelay(100);
	static ProberThread prober3(&hi2c3);
	prober3.setTickDelay(100);

	// Create indicator LED threads
	static FDCANIndicatorThread fdcan1_indicator(&hfdcan1);
	fdcan1_indicator.setTickDelay(100);
	static FDCANIndicatorThread fdcan2_indicator(&hfdcan2);
	fdcan2_indicator.setTickDelay(100);

}

void System::prioritize_3V3_LDO() {
	HAL_GPIO_WritePin(SEL_3V3_GPIO_Port, SEL_3V3_Pin, GPIO_PIN_SET);
}

void System::prioritize_3V3_buck() {
	HAL_GPIO_WritePin(SEL_3V3_GPIO_Port, SEL_3V3_Pin, GPIO_PIN_RESET);
}

bool System::using_USB_5V() {
	return HAL_GPIO_ReadPin(STATUS_3V3_GPIO_Port, STATUS_3V3_Pin) == GPIO_PIN_RESET;
}

bool System::using_LDO_3V3() {
	return HAL_GPIO_ReadPin(STATUS_5V_GPIO_Port, STATUS_5V_Pin) == GPIO_PIN_RESET;
}

void System::enable_meissa_5V_output() {
	HAL_GPIO_WritePin(CTL_5V_GPIO_Port, CTL_5V_Pin, GPIO_PIN_SET);
}

void System::disable_meissa_5V_output() {
	HAL_GPIO_WritePin(CTL_5V_GPIO_Port, CTL_5V_Pin, GPIO_PIN_RESET);
}

void System::enable_meissa_3V3_output() {
	HAL_GPIO_WritePin(CTL_3V3_GPIO_Port, CTL_3V3_Pin, GPIO_PIN_SET);
}

void System::disable_meissa_3V3_output() {
	HAL_GPIO_WritePin(CTL_3V3_GPIO_Port, CTL_3V3_Pin, GPIO_PIN_RESET);
}

void System::enable_yellow_led() {
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
}

void System::disable_yellow_led() {
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
}

void System::blink_yellow_led(uint8_t times, float freq) {
	for (uint8_t i = 0; i < times; ++i) {
		enable_yellow_led();
		osDelay((uint32_t)1000.f/(freq));
		disable_yellow_led();
		osDelay((uint32_t)1000.f/(freq));
	}
}

void System::enable_status_led_fdcan1() {
	HAL_GPIO_WritePin(FDCAN1_STATUS_GPIO_Port, FDCAN1_STATUS_Pin, GPIO_PIN_SET);
}

void System::disable_status_led_fdcan1() {
	HAL_GPIO_WritePin(FDCAN1_STATUS_GPIO_Port, FDCAN1_STATUS_Pin, GPIO_PIN_RESET);
}

void System::enable_status_led_fdcan2() {
	HAL_GPIO_WritePin(FDCAN2_STATUS_GPIO_Port, FDCAN2_STATUS_Pin, GPIO_PIN_SET);
}

void System::disable_status_led_fdcan2() {
	HAL_GPIO_WritePin(FDCAN2_STATUS_GPIO_Port, FDCAN2_STATUS_Pin, GPIO_PIN_RESET);
}

void System::enable_silent_mode_fdcan1() {
	HAL_GPIO_WritePin(CAN1_SILENT_GPIO_Port, CAN1_SILENT_Pin, GPIO_PIN_SET);
}

void System::disable_silent_mode_fdcan1() {
	HAL_GPIO_WritePin(CAN1_SILENT_GPIO_Port, CAN1_SILENT_Pin, GPIO_PIN_RESET);
}

void System::enable_silent_mode_fdcan2() {
	HAL_GPIO_WritePin(CAN2_SILENT_GPIO_Port, CAN2_SILENT_Pin, GPIO_PIN_SET);
}

void System::disable_silent_mode_fdcan2() {
	HAL_GPIO_WritePin(CAN2_SILENT_GPIO_Port, CAN2_SILENT_Pin, GPIO_PIN_RESET);
}

void System::update_node_id() {
	uint16_t id0 = HAL_GPIO_ReadPin(ID0_GPIO_Port, ID0_Pin);
	uint16_t id1 = HAL_GPIO_ReadPin(ID1_GPIO_Port, ID1_Pin);
	uint16_t id2 = HAL_GPIO_ReadPin(ID2_GPIO_Port, ID2_Pin);
	uint16_t id3 = HAL_GPIO_ReadPin(ID3_GPIO_Port, ID3_Pin);

	node_id = id0 | (id1 << 1) | (id2 << 2) | (id3 << 3);
}

uint32_t System::get_node_id() {
	return node_id;
}

static LaserResponsePacket laser_response_packet;

void System::handle_meissa_output_cmd(uint8_t sender_id, LaserPacket* packet) {
	if(!(IS_RELIABLE_MCU(*packet))) {
		console.printf_error("Unreliable laser packet");
		return;
	}
	if (packet->enable)
		enable_meissa_5V_output();
	else
		disable_meissa_5V_output();

	// Laser is not correctly activated if Meissa is not being powered
	if (using_USB_5V())
		laser_response_packet.success = false;
	else
		laser_response_packet.success = true;

	MAKE_IDENTIFIABLE(laser_response_packet);
	MAKE_RELIABLE_MCU(laser_response_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	if (sender_id == 1)
		FDCAN1_network->send(&laser_response_packet);
	else if (sender_id == 2)
		FDCAN2_network->send(&laser_response_packet);
}

extern "C" {

void vApplicationStackOverflowHook() {
	while(1);
}

}


