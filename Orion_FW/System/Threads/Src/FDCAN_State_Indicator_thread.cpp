/*
 * FDCAN_State_Indicator_thread.cpp
 *
 *  Created on: Aug 17, 2023
 *      Author: Vincent Nguyen
 */

#include "FDCAN_State_Indicator_thread.h"
#include "System.h"

void FDCANIndicatorThread::init() {

}

void FDCANIndicatorThread::loop() {
	FDCAN_ProtocolStatusTypeDef protocol_status;
	HAL_StatusTypeDef status = HAL_FDCAN_GetProtocolStatus(fdcan, &protocol_status);

	if ((status != HAL_OK || protocol_status.ErrorPassive) && enabled == true) {
		enabled = false;
		LOG_ERROR("Connection lost on FDCAN%d", fdcan->Instance==FDCAN1 ?  1 : 2);
		if (fdcan == &hfdcan1)
			System::disable_status_led_fdcan1();
		else if (fdcan == &hfdcan2)
			System::disable_status_led_fdcan2();
	} else if (!protocol_status.ErrorPassive && enabled == false) {
		enabled = true;
		LOG_SUCCESS("Connection established on FDCAN%d", fdcan->Instance==FDCAN1 ?  1 : 2);
		if (fdcan == &hfdcan1)
			System::enable_status_led_fdcan1();
		else if (fdcan == &hfdcan2)
			System::enable_status_led_fdcan2();
	}
}

static DummyPacket dummy_packet;
struct DummyData dummy_data;

void FDCANIndicatorThread::send_dummy() {
	dummy_data.data = 0xdead; 
	dummy_data.toArray((uint8_t*) &dummy_packet);
	MAKE_IDENTIFIABLE(dummy_packet);
	MAKE_RELIABLE_MCU(dummy_packet); //MCU?=
	Telemetry::set_id(JETSON_NODE_ID); //ID?
	FDCAN1_network->send(&dummy_packet);
	FDCAN2_network->send(&dummy_packet);
	LOG_INFO("Dummy packet sent");
	portYIELD();
}
