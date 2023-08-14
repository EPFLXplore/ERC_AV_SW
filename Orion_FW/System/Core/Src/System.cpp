/*
 * system.cpp
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine
 */


#include <four_in_one_thread.h>
#include "System.h"
#include "Prober.h"
#include "Debug.h"
#include "i2c.h"

uint32_t System::node_id = 0x7FF;

void System::init() {

	// Configure LDO
	prioritize_3V3_buck();

	// Make sure the outputs are zero for meissa's outputs
	disable_meissa_5V_output();
	disable_meissa_3V3_output();

	// Read and update node ID from DIP switch
	update_node_id();

	static Shell shell(&huart1, &terminal);

	static ProberThread prober1(&hi2c1);
	static ProberThread prober2(&hi2c2);
	static ProberThread prober3(&hi2c3);
}

void System::prioritize_3V3_LDO() {
	HAL_GPIO_WritePin(SEL_3V3_GPIO_Port, SEL_3V3_Pin, GPIO_PIN_SET);
}

void System::prioritize_3V3_buck() {
	HAL_GPIO_WritePin(SEL_3V3_GPIO_Port, SEL_3V3_Pin, GPIO_PIN_RESET);
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

void System::update_node_id() {
	uint32_t id0 = HAL_GPIO_ReadPin(ID0_GPIO_Port, ID0_Pin);
	uint32_t id1 = HAL_GPIO_ReadPin(ID1_GPIO_Port, ID1_Pin);
	uint32_t id2 = HAL_GPIO_ReadPin(ID2_GPIO_Port, ID2_Pin);
	uint32_t id3 = HAL_GPIO_ReadPin(ID3_GPIO_Port, ID3_Pin);

	node_id = id0 | (id1 << 1) | (id2 << 2) | (id3 << 3);

	printf("Node ID: %u\n", node_id);
}

uint32_t System::get_node_id() {
	return node_id;
}

static LaserResponsePacket laser_response_packet;

void System::handle_meissa_output_cmd(uint8_t sender_id, LaserPacket* packet) {
	if (packet->enable)
		enable_meissa_5V_output();
	else
		disable_meissa_5V_output();

	laser_response_packet.success = true;

	MAKE_IDENTIFIABLE(*packet);
	Telemetry::set_id(JETSON_NODE_ID);
	FDCAN1_network->send(packet);
}

extern "C" {

void vApplicationStackOverflowHook() {
	while(1);
}

}


