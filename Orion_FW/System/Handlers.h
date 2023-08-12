/*
 * Handlers.h
 *
 *  Created on: Aug 25, 2022
 *      Author: Vincent
 */

#ifndef HANDLERS_H_
#define HANDLERS_H_

#include "RoCo.h"

void handle_dummyCallback(uint8_t sender_id, DummyPacket* packet);
void handle_ping(uint8_t sender_id, PingPacket* packet);

#endif /* HANDLERS_H_ */
