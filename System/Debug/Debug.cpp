/*
 * Debug.cpp
 *
 *  Created on: Jan 23, 2021
 *      Author: AV Team 2020
 */

#include "Debug.h"

#include <usart.h>


Console console(&huart3);
Monitor monitor;
Profiler profiler;
Terminal terminal;
