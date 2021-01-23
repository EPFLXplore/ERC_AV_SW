/*
 * terminal.c
 *
 *  Created on: 6 Sep 2020
 *      Author: arion
 */

#include "Terminal.h"

#include "Debug.h"

#include <stm32h7xx_hal.h>
#include <stdlib.h> // Even though not recommended


#define EQUALS(index, str) (cmd->num_components > (index) && cmd->components[(index)].matches((str)))


void Terminal::execute(ShellCommand* cmd, void (*respond)(const char* format, ...)) {
	if(cmd->num_components > 0) {
		if(EQUALS(0, "help")) {
			respond("> Xplore Avionics available commands:\n");
			respond("> clear: clears the screen");
			respond("> help: shows this help page\n");
			respond("> monitor: enables or disables a specific monitor\n");
			respond("> profiler: enables or disables the embedded profiler\n");
			respond("> reset: performs a software reset of the avionics\n");
			respond("> version: displays the current terminal version\n");
		} else if(EQUALS(0, "shutdown")) {
			respond("> Xplore Avionics shutting down immediately\n");
			while(1);
		} else if(EQUALS(0, "version")) {
			respond("> Xplore Avionics Terminal v1.0 by Arion Zimmermann\n");
		} else if(EQUALS(0, "reset")) {
			HAL_NVIC_SystemReset();
		} else if(EQUALS(0, "time")) {
			respond("%d\n", HAL_GetTick());
		} else if(EQUALS(0, "clear")) {
			respond("\x1b[2J\x1b[H\e7");
		} else if(EQUALS(0, "profiler")) {
			if(EQUALS(1, "enable")) {
				profiler.enable();
				respond("\x1b[2J");
				respond("> Profiler now enabled\n");
			} else if(EQUALS(1, "disable")) {
				profiler.disable();
				respond("\x1b[2J");
				respond("> Profiler now disabled\n");
			} else {
				respond("> Usage: profiler { enable | disable }\n");
			}
		} else if(EQUALS(0, "verbose")) {
			if(EQUALS(1, "on")) {
				verbose = true;
				respond("> Verbose mode enabled\n");
			} else if(EQUALS(1, "off")) {
				verbose = false;
				respond("> Verbose mode disabled\n");
			} else {
				respond("> Usage: verbose { on | off }\n");
			}
		} else if(EQUALS(0, "monitor")) {
			if(EQUALS(1, "enable") && cmd->num_components >= 3) {
				uint8_t location = 0;
				uint8_t refresh_rate = 1;

				if(cmd->num_components > 3) {
					location = atoi(cmd->components[3].component);
				}

				if(cmd->num_components > 4) {
					refresh_rate = atoi(cmd->components[4].component);
				}

				respond("\x1b[2J");
			} else if(EQUALS(1, "disable") && cmd->num_components == 3) {
				respond("\x1b[2J");
			} else {
				respond("> Usage: monitor { enable | disable } { sensor | state | kalman | flash | can | telemetry | airbrakes } [location] [refresh rate; default: 10]\n");
			}
		} else {
			respond("> %.*s: command not found\n", cmd->components[0].length, cmd->components[0].component);
		}
	}
}

bool Terminal::isVerbose() {
	return verbose;
}

#undef EQUALS
