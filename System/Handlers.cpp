/*
 * Handlers.cpp
 *
 *  Created on: Aug 25, 2022
 *      Author: Vincent
 */

#include "Handlers.h"
#include "Telemetry.h"
#include "LED_Switch.h"
#include "Servo.h"

#define SERVO_CONFIRMATION_DELAY_MS 200

void handle_led(uint8_t sender_id, sc_LED_packet* packet) {
	if (packet->on)
		LED_ON(LED_GPIO_Port, LED_Pin);
	else
		LED_OFF(LED_GPIO_Port, LED_Pin);
}

void handle_servo_trap(uint8_t sender_id, sc_trap_packet* packet) {
	sc_trap_success_packet servo_status;
	if (packet->open) {
		open_servo(&htim2, 1);
		servo_status.status = true;
	} else {
		close_servo(&htim2, 1);
		servo_status.status = false;
	}
	osDelay(SERVO_CONFIRMATION_DELAY_MS);
	network.send(&servo_status);
}

void handle_servo_caching(uint8_t sender_id, sc_caching_packet* packet) {
	sc_caching_success_packet servo_status;
	if (packet->open) {
		open_servo(&htim2, 2);
		servo_status.status = true;
	} else {
		close_servo(&htim2, 2);
		servo_status.status = false;
	}
	osDelay(SERVO_CONFIRMATION_DELAY_MS);
	network.send(&servo_status);
}

//void handle_servo_caching(uint8_t sender_id, sc_trap_packet* packet) {
//
//}

