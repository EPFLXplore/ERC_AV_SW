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
		LED_ON(GPIOC, GPIO_PIN_0);
	else
		LED_OFF(GPIOC, GPIO_PIN_0);
}

void handle_servo_trap(uint8_t sender_id, sc_trap_packet* packet) {
	sc_trap_success_packet servo_status;
	if (packet->open) {
		open_servo(&htim2, 1);
//		osDelay(SERVO_CONFIRMATION_DELAY_MS);
		servo_status.status = true;
	} else {
		close_servo(&htim2, 1);
//		osDelay(SERVO_CONFIRMATION_DELAY_MS);
		servo_status.status = false;
	}
	network.send(&servo_status);
}

void handle_servo_caching(uint8_t sender_id, sc_caching_packet* packet) {
	sc_caching_success_packet servo_status;
	if (packet->open) {
		open_servo(&htim2, 2);
//		osDelay(SERVO_CONFIRMATION_DELAY_MS);
		servo_status.status = true;
	} else {
		close_servo(&htim2, 2);
//		osDelay(SERVO_CONFIRMATION_DELAY_MS);
		servo_status.status = false;
	}
	network.send(&servo_status);
}

//void handle_servo_caching(uint8_t sender_id, sc_trap_packet* packet) {
//
//}

