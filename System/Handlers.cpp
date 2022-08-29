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
#include "Voltmeter_extender.h"

#include "Voltmeter_thread.h"
#include "HX711_thread.h"

#define SERVO_CONFIRMATION_DELAY_MS 200


void handle_led(uint8_t sender_id, sc_LED_packet* packet) {
	if (packet->on)
		LED_ON(LED_GPIO_Port, LED_Pin);
	else
		LED_OFF(LED_GPIO_Port, LED_Pin);
	//HAL_TIM_Base_Start_IT(&htim1);
}

void handle_servo_trap(uint8_t sender_id, sc_trap_packet* packet) {
	avionics_trap_success_packet  servo_status;
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

void handle_servo_caching(uint8_t sender_id, sc_caching_packet* packet) {
	avionics_caching_success_packet servo_status;
	switch (packet->open){
	case 0 :
		close_servo(&htim2, 1);
		servo_status.status = 0;
		break;
	case 1 :
		neutral_servo(&htim2, 1);
		servo_status.status = 1;
		break;
	case 2 :
		open_servo(&htim2, 1);
		servo_status.status = 2;
		break;
	}
	osDelay(SERVO_CONFIRMATION_DELAY_MS);
	network.send(&servo_status);
}

void handle_voltmeter_motor(uint8_t sender_id, hd_voltmeter_motor_packet* packet) {
//	avionics_voltmeter_motor_status_packet motor_status;
	HAL_TIM_Base_Start_IT(&htim1);
//	bool motor_status;
	if (packet->extended) {
		extend_voltmeter(GPIOA, GPIO_PIN_4, GPIOA, GPIO_PIN_5);
		set_extender_status(true);
//		motor_status = true;
	} else {
		retract_voltmeter(GPIOA, GPIO_PIN_4, GPIOA, GPIO_PIN_5);
		set_extender_status(false);
//		motor_status = false;
	}
//	osDelay(VOLTMETER_MAX_EXTENSION_INTERVAL_MS);

//	osDelay(SERVO_CONFIRMATION_DELAY_MS);
//	network.send(&motor_status);
}

void handle_voltmeter_tare(uint8_t sender_id, hd_voltmeter_tare_packet* packet) {
	avionics_voltmeter_tare_success_packet voltmeter_tare_status;
	if (packet->tare && voltmeterInstance != nullptr) {
		voltmeterInstance->tare();
		voltmeter_tare_status.status = true;
	} else {
		voltmeter_tare_status.status = false;
	}
	network.send(&voltmeter_tare_status);
}

void handle_mass_calibrate(uint8_t sender_id, sc_mass_calibrate_packet* packet) {
	avionics_mass_calibrate_success_packet* mass_calibrate_status;
	if (packet->tare && hx711Instance != nullptr) {
		hx711Instance->calibrateMultiplier();
	}

}
//void handle_servo_caching(uint8_t sender_id, sc_trap_packet* packet) {
//
//}

