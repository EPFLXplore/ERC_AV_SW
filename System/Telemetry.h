/*
 * Telemetry.h
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */

#ifndef TELEMETRY_TELEMETRY_H_
#define TELEMETRY_TELEMETRY_H_


#include "RoCo.h"
#include "usart.h"
//#define ROCO_BUFFER_SIZE 128
//uint8_t buffer[ROCO_BUFFER_SIZE];
//
//IODriver* telemtryDriver = new STMUARTDriver(&huart2); // Point to Huart2
//IOBus* network = new IOBus((IODriver*) telemtryDriver, buffer, ROCO_BUFFER_SIZE);
extern "C"{
void setupTelemtry();
}

#endif /* TELEMETRY_TELEMETRY_H_ */
