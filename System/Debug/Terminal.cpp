/*
 * terminal.c
 *
 *  Created on: 6 Sep 2020
 *      Author: AV Team 2020
 */

#include "Terminal.h"

#include "Debug.h"

#include <stm32h7xx_hal.h>
#include <stdlib.h> // Even though not recommended


#define EQUALS(index, str) (cmd->num_components > (index) && cmd->components[(index)].matches((str)))


void Terminal::execute(ShellCommand* cmd, Console* feedback) {
	if(cmd->num_components > 0) {
		if(EQUALS(0, "help")) {
			feedback->printf("> Xplore Avionics available commands:\r\n");
			feedback->printf("> clear: clears the screen\r\n");
			feedback->printf("> help: shows this help page\r\n");
			feedback->printf("> monitor: enables or disables a specific monitor\r\n");
			feedback->printf("> profiler: enables or disables the embedded profiler\r\n");
			feedback->printf("> reset: performs a software reset of the avionics\r\n");
			feedback->printf("> version: displays the current terminal version\r\n");
		} else if(EQUALS(0, "version")) {
			feedback->printf("> Xplore Avionics Terminal v1.0 by Arion Zimmermann\r\n");
		} else if(EQUALS(0, "reset")) {
			HAL_NVIC_SystemReset();
		} else if(EQUALS(0, "time")) {
			feedback->printf("%d\r\n", HAL_GetTick());
		} else if(EQUALS(0, "clear")) {
			feedback->printf("\x1b[2J\x1b[H\e7");
		} else if(EQUALS(0, "profiler")) {
			if(EQUALS(1, "enable")) {
				profiler.enable();
				feedback->printf("\x1b[2J");
				feedback->printf("> Profiler now enabled\r\n");
			} else if(EQUALS(1, "disable")) {
				profiler.disable();
				feedback->printf("\x1b[2J");
				feedback->printf("> Profiler now disabled\r\n");
			} else {
				feedback->printf("> Usage: profiler { enable | disable }\r\n");
			}
		} else if(EQUALS(0, "verbose")) {
			if(EQUALS(1, "on")) {
				verbose = true;
				feedback->printf("> Verbose mode enabled\r\n");
			} else if(EQUALS(1, "off")) {
				verbose = false;
				feedback->printf("> Verbose mode disabled\r\n");
			} else {
				feedback->printf("> Usage: verbose { on | off }\r\n");
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

				feedback->printf("\x1b[2J");
			} else if(EQUALS(1, "disable") && cmd->num_components == 3) {
				feedback->printf("\x1b[2J");
			} else {
				feedback->printf("> Usage: monitor { enable | disable } { sensor | state | kalman | flash | can | telemetry | airbrakes } [location] [refresh rate; default: 10]\r\n");
			}
		} else {
			feedback->printf("> %.*s: command not found\r\n", cmd->components[0].length, cmd->components[0].component);
		}
	}
}

bool Terminal::isVerbose() {
	return verbose;
}

#undef EQUALS