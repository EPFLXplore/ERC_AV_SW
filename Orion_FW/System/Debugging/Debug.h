/*
 * Debug.h
 *
 *  Created on: Jan 23, 2021
 *      Author: AV Team 2020
 */

#ifndef DEBUGGING_DEBUG_H_
#define DEBUGGING_DEBUG_H_

#include "Debugging/Console.h"
#include "Debugging/Monitor.h"
#include "Debugging/Profiler.h"
#include "Debugging/Shell.h"
#include "Debugging/Terminal.h"


extern Console console;
extern Monitor monitor;
extern Profiler profiler;
extern Terminal terminal;


#endif /* DEBUGGING_DEBUG_H_ */
