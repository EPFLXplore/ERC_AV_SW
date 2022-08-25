/*
 * Handlers.h
 *
 *  Created on: Aug 25, 2022
 *      Author: Vincent
 */

#ifndef HANDLERS_H_
#define HANDLERS_H_

#include "RoCo.h"

void handle_led(uint8_t sender_id, sc_LED_packet* packet);
void handle_servo_trap(uint8_t sender_id, sc_trap_packet* packet);
void handle_servo_caching(uint8_t sender_id, sc_caching_packet* packet);

#endif /* HANDLERS_H_ */
