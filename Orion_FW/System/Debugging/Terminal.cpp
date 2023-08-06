/*
 * terminal.c
 *
 *  Created on: 6 Sep 2020
 *      Author: AV Team 2020
 */

#include "Debugging/Terminal.h"

#include <stm32h7xx_hal.h>
#include <stdlib.h> // Even though not recommended
#include "Debugging/Debug.h"

#include "AHRS_thread.h"


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
			feedback->printf("> calibrate: puts the relevant sensor into calibration mode\r\n");
			feedback->printf("> plot: puts the relevant sensor in plotting mode\r\n");
		} else if(EQUALS(0, "version")) {
			feedback->printf("> Xplore Orion Avionics Terminal v1.0 \r\n");
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
		} else if(EQUALS(0, "calibrate")) {
			if(EQUALS(1, "mag")) {
				if (AHRSInstance != nullptr) {
					disable_monitors(feedback);
					monitor.enable(MAG_CAL_MONITOR, 0, 100);
				} else {
					feedback->printf("IMU Hat is not plugged in\r\n");
				}
			}
			else if(EQUALS(1, "accel")) {
				if (AHRSInstance != nullptr) {
					disable_monitors(feedback);
					monitor.enable(ACC_CAL_MONITOR, 0, 100);
				} else {
					feedback->printf("IMU Hat is not plugged in\r\n");
				}
			}
			else if(EQUALS(1, "disable")) {
				if (AHRSInstance != nullptr) {
					disable_monitors(feedback);
					monitor.disable(MAG_CAL_MONITOR);
					feedback->printf("\x1b[2J");
					monitor.disable(ACC_CAL_MONITOR);
					feedback->printf("\x1b[2J");
				} else {
					feedback->printf("IMU Hat is not plugged in\r\n");
				}
			}
			else {
				feedback->printf("> Usage: calibrate { mag | accel | disable }\r\n");
			}
		} else if(EQUALS(0, "plot")) {
			if(EQUALS(1, "imu")) {
				if (AHRSInstance != nullptr) {
					disable_monitors(feedback);
					monitor.enable(IMU_PLOT_MONITOR, 0, 100);
				} else {
					feedback->printf("IMU Hat is not plugged in\r\n");
				}
			}
			else if(EQUALS(1, "disable")) {
				if (AHRSInstance != nullptr) {
					disable_monitors(feedback);
					monitor.disable(IMU_PLOT_MONITOR);
					feedback->printf("\x1b[2J");
				} else {
					feedback->printf("IMU Hat is not plugged in\r\n");
				}
			}
			else {
				feedback->printf("> Usage: plot { imu | disable }\r\n");
			}
		} else if(EQUALS(0, "monitor")) {
			if(EQUALS(1, "list")) {
				feedback->printf("> Currently available monitor IDs: \r\n");
				if (AHRSInstance != nullptr) {
					feedback->printf("> imu \r\n");
					feedback->printf("> accel \r\n");
					feedback->printf("> gyro \r\n");
					feedback->printf("> mag \r\n");
					feedback->printf("> quat \r\n");
					feedback->printf("> rpy \r\n");
				}
			}
			else if(EQUALS(1, "enable") && cmd->num_components >= 3) {
				uint8_t refresh_rate = 10;
				uint8_t custom_loc = 0;

				if(cmd->num_components > 3) {
					custom_loc = atoi(cmd->components[3].component);
				}

				if(cmd->num_components > 4) {
					refresh_rate = atoi(cmd->components[4].component);
				}

				if(EQUALS(2, "imu")) {
					if (AHRSInstance != nullptr) {
						if (!monitor.is_enabled(IMU_MONITOR)) {
							uint8_t chosen_loc;
							location++;
							if(cmd->num_components == 3) {
								chosen_loc = location;
							} else {
								chosen_loc = custom_loc;
							}
							monitor.enable(IMU_MONITOR, chosen_loc, refresh_rate);
							feedback->printf("\x1b[2J");
						} else {
							feedback->printf("IMU monitor already enabled\r\n");
						}
					} else {
						feedback->printf("IMU Hat is not plugged in\r\n");
					}
				}

				if(EQUALS(2, "accel")) {
					if (AHRSInstance != nullptr) {
						if (!monitor.is_enabled(ACCEL_MONITOR)) {
							uint8_t chosen_loc;
							location++;
							if(cmd->num_components == 3) {
								chosen_loc = location;
							} else {
								chosen_loc = custom_loc;
							}
							monitor.enable(ACCEL_MONITOR, chosen_loc, refresh_rate);
							feedback->printf("\x1b[2J");
						} else {
							feedback->printf("Accelerometer monitor already enabled\r\n");
						}
					} else {
						feedback->printf("IMU Hat is not plugged in\r\n");
					}
				}

				if(EQUALS(2, "gyro")) {
					if (AHRSInstance != nullptr) {
						if (!monitor.is_enabled(GYRO_MONITOR)) {
							uint8_t chosen_loc;
							location++;
							if(cmd->num_components == 3) {
								chosen_loc = location;
							} else {
								chosen_loc = custom_loc;
							}
							monitor.enable(GYRO_MONITOR, chosen_loc, refresh_rate);
							feedback->printf("\x1b[2J");
						} else {
							feedback->printf("Gyroscope monitor already enabled\r\n");
						}
					} else {
						feedback->printf("IMU Hat is not plugged in\r\n");
					}
				}

				if(EQUALS(2, "mag")) {
					if (AHRSInstance != nullptr) {
						if (!monitor.is_enabled(MAG_MONITOR)) {
							uint8_t chosen_loc;
							location++;
							if(cmd->num_components == 3) {
								chosen_loc = location;
							} else {
								chosen_loc = custom_loc;
							}
							monitor.enable(MAG_MONITOR, chosen_loc, refresh_rate);
							feedback->printf("\x1b[2J");
						} else {
							feedback->printf("Magnetometer monitor already enabled\r\n");
						}
					} else {
						feedback->printf("IMU Hat is not plugged in\r\n");
					}
				}

				if(EQUALS(2, "quat")) {
					if (AHRSInstance != nullptr) {
						if (!monitor.is_enabled(QUAT_MONITOR)) {
							uint8_t chosen_loc;
							location++;
							if(cmd->num_components == 3) {
								chosen_loc = location;
							} else {
								chosen_loc = custom_loc;
							}
							monitor.enable(QUAT_MONITOR, chosen_loc, refresh_rate);
							feedback->printf("\x1b[2J");
						} else {
							feedback->printf("Quaternion monitor already enabled\r\n");
						}
					} else {
						feedback->printf("IMU Hat is not plugged in\r\n");
					}
				}

				if(EQUALS(2, "rpy")) {
					if (AHRSInstance != nullptr) {
						if (!monitor.is_enabled(RPY_MONITOR)) {
							uint8_t chosen_loc;
							location++;
							if(cmd->num_components == 3) {
								chosen_loc = location;
							} else {
								chosen_loc = custom_loc;
							}
							monitor.enable(RPY_MONITOR, chosen_loc, refresh_rate);
							feedback->printf("\x1b[2J");
						} else {
							feedback->printf("Roll, pitch, yaw monitor already enabled\r\n");
						}
					} else {
						feedback->printf("IMU Hat is not plugged in\r\n");
					}
				}

			} else if(EQUALS(1, "enable") && cmd->num_components == 2) {
				if (AHRSInstance != nullptr) {
					monitor.enable(ACCEL_MONITOR, location, 100);
					feedback->printf("\x1b[2J");
					location++;
					monitor.enable(GYRO_MONITOR, location, 100);
					feedback->printf("\x1b[2J");
					location++;
					monitor.enable(MAG_MONITOR, location, 100);
					feedback->printf("\x1b[2J");
					location++;
					monitor.enable(RPY_MONITOR, location, 100);
					feedback->printf("\x1b[2J");
					location++;
				}
				feedback->printf("\x1b[2J");

			} else if(EQUALS(1, "disable") && cmd->num_components == 3) {
				if(EQUALS(2, "imu")) {
					if (AHRSInstance != nullptr) {
						monitor.disable(IMU_MONITOR);
						update_monitors(monitor.get_location(IMU_MONITOR));
						location--;
					} else {
						feedback->printf("IMU Hat is not plugged in\r\n");
					}
				}
				else if(EQUALS(2, "accel")) {
					if (monitor.is_enabled(ACCEL_MONITOR)) {
						monitor.disable(ACCEL_MONITOR);
						update_monitors(monitor.get_location(ACCEL_MONITOR));
						location--;
					} else {
						feedback->printf("Monitor not created yet\r\n");
					}
				}
				else if(EQUALS(2, "gyro")) {
					if (monitor.is_enabled(GYRO_MONITOR)) {
						monitor.disable(GYRO_MONITOR);
						update_monitors(monitor.get_location(GYRO_MONITOR));
						location--;
					} else {
						feedback->printf("Monitor not created yet\r\n");
					}
				}
				else if(EQUALS(2, "mag")) {
					if (monitor.is_enabled(MAG_MONITOR)) {
						monitor.disable(MAG_MONITOR);
						update_monitors(monitor.get_location(MAG_MONITOR));
						location--;
					} else {
						feedback->printf("Monitor not created yet\r\n");
					}
				}
				else if(EQUALS(2, "quat")) {
					if (monitor.is_enabled(QUAT_MONITOR)) {
						monitor.disable(QUAT_MONITOR);
						update_monitors(monitor.get_location(QUAT_MONITOR));
						location--;
					} else {
						feedback->printf("Monitor not created yet\r\n");
					}
				}
				else if(EQUALS(2, "rpy")) {
					if (monitor.is_enabled(RPY_MONITOR)) {
						monitor.disable(RPY_MONITOR);
						update_monitors(monitor.get_location(RPY_MONITOR));
						location--;
					} else {
						feedback->printf("Monitor not created yet\r\n");
					}
				}
				feedback->printf("\x1b[2J");
			}
			else if(EQUALS(1, "disable") && cmd->num_components == 2) {
				location = 0;
				disable_monitors(feedback);
			}
			else {
				feedback->printf("> Usage:\r\n");
				feedback->printf("> monitor { enable | disable } device_id [location] [refresh rate; default: 10]\r\n");
				feedback->printf("> monitor list\r\n");
			}
		} else {
			feedback->printf("> %.*s: command not found\r\n", cmd->components[0].length, cmd->components[0].component);
		}
	}
}

void Terminal::update_monitors(uint8_t loc) {
	for (uint8_t i = 0; i < MAX_MONITORS; ++i) {
		if (monitor.is_enabled(i) && monitor.get_location(i) >= loc) {
			monitor.enable(i, monitor.get_location(i)-1, monitor.get_refresh_rate(i));
		}
	}
}

void Terminal::disable_monitors(Console* feedback) {
	location = 0;
	monitor.disable(IMU_MONITOR);
	feedback->printf("\x1b[2J");
	monitor.disable(ACCEL_MONITOR);
	feedback->printf("\x1b[2J");
	monitor.disable(GYRO_MONITOR);
	feedback->printf("\x1b[2J");
	monitor.disable(MAG_MONITOR);
	feedback->printf("\x1b[2J");
	monitor.disable(QUAT_MONITOR);
	feedback->printf("\x1b[2J");
	monitor.disable(RPY_MONITOR);
	feedback->printf("\x1b[2J");
}

bool Terminal::isVerbose() {
	return verbose;
}

#undef EQUALS
