/*
 * terminal.h
 *
 *  Created on: 6 Sep 2020
 *      Author: AV Team 2020
 */

#ifndef APPLICATION_HOSTBOARD_INC_DEBUG_TERMINAL_H_
#define APPLICATION_HOSTBOARD_INC_DEBUG_TERMINAL_H_


#include <stdint.h>
#include <stdbool.h>
#include "Console.h"


#define CMD_MAX_COMPONENTS 8

struct CommandComponent {
	const char* component;
	uint8_t length;

	bool matches(const char* target) {
		uint8_t i;

		for(i = 0; i < length; i++) {
			if(target[i] != component[i] || target[i] == '\0') {
				return false;
			}
		}

		return target[i] == '\0';
	}
};

typedef struct {
	struct CommandComponent components[CMD_MAX_COMPONENTS];
	uint8_t num_components;
} ShellCommand;


class Terminal {
public:
	void execute(ShellCommand* cmd, Console* feedback);
	bool isVerbose();
private:
	uint8_t location = 1;
	void update_monitors(uint8_t loc, uint8_t num_lines = 1);
	void disable_monitors(Console* feedback);
	bool verbose;
};


#endif /* APPLICATION_HOSTBOARD_INC_DEBUG_TERMINAL_H_ */
