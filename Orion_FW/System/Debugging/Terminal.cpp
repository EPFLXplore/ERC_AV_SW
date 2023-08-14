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
#include "ADS1115_Potentiometer_thread.hpp"
#include "ADS1115_Voltmeter_thread.hpp"
#include "ADS1234_Mass_thread.hpp"
#include "AS7265_thread.h"
#include "Modbus_thread.hpp"


#define EQUALS(index, str) (cmd->num_components > (index) && cmd->components[(index)].matches((str)))

void Terminal::execute(ShellCommand* cmd, Console* feedback) {
	if(cmd->num_components > 0) {
		if(EQUALS(0, "help")) {
			feedback->printf_info("> Xplore Avionics available commands:\r\n");
			feedback->printf_info("> clear: clears the screen\r\n");
			feedback->printf_info("> help: shows this help page\r\n");
			feedback->printf_info("> monitor: enables or disables a specific monitor\r\n");
			feedback->printf_info("> profiler: enables or disables the embedded profiler\r\n");
			feedback->printf_info("> reset: performs a software reset of the avionics\r\n");
			feedback->printf_info("> version: displays the current terminal version\r\n");
			feedback->printf_info("> calibrate: puts the relevant sensor into calibration mode\r\n");
			feedback->printf_info("> plot: puts the relevant sensor in plotting mode\r\n");
		} else if(EQUALS(0, "version")) {
			feedback->printf_info("> Xplore Orion Avionics Terminal v1.0 \r\n");
		} else if(EQUALS(0, "reset")) {
			HAL_NVIC_SystemReset();
		} else if(EQUALS(0, "time")) {
			feedback->printf_info("%d\r\n", HAL_GetTick());
		} else if(EQUALS(0, "clear")) {
			feedback->printf("\x1b[2J\x1b[H\e7");
		} else if(EQUALS(0, "profiler")) {
			if(EQUALS(1, "enable")) {
				profiler.enable();
				feedback->printf("\x1b[2J");
				feedback->printf_info("> Profiler now enabled\r\n");
			} else if(EQUALS(1, "disable")) {
				profiler.disable();
				feedback->printf("\x1b[2J");
				feedback->printf_info("> Profiler now disabled\r\n");
			} else {
				feedback->printf_info("> Usage: profiler { enable | disable }\r\n");
			}
		} else if(EQUALS(0, "verbose")) {
			if(EQUALS(1, "on")) {
				verbose = true;
				feedback->printf_info("> Verbose mode enabled\r\n");
			} else if(EQUALS(1, "off")) {
				verbose = false;
				feedback->printf_info("> Verbose mode disabled\r\n");
			} else {
				feedback->printf_info("> Usage: verbose { on | off }\r\n");
			}
		} else if(EQUALS(0, "calibrate")) {
			if(EQUALS(1, "mag")) {
				if (AHRSInstance != nullptr) {
					disable_monitors(feedback);
					monitor.enable(MAG_CAL_MONITOR, 0, 100);
				} else {
					feedback->printf_error("> IMU Hat is not plugged in\r\n");
				}
			}
			else if(EQUALS(1, "accel")) {
				if (AHRSInstance != nullptr) {
					disable_monitors(feedback);
					monitor.enable(ACC_CAL_MONITOR, 0, 100);
				} else {
					feedback->printf("> IMU Hat is not plugged in\r\n");
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
					feedback->printf_error("> IMU Hat is not plugged in\r\n");
				}
			}
			else {
				feedback->printf_info("> Usage: calibrate { mag | accel | disable }\r\n");
			}
		} else if(EQUALS(0, "plot")) {
			if(EQUALS(1, "imu")) {
				if (AHRSInstance != nullptr) {
					disable_monitors(feedback);
					monitor.enable(IMU_PLOT_MONITOR, 0, 100);
				} else {
					feedback->printf_error("> IMU Hat is not plugged in\r\n");
				}
			}
			else if(EQUALS(1, "disable")) {
				if (AHRSInstance != nullptr) {
					disable_monitors(feedback);
					monitor.disable(IMU_PLOT_MONITOR);
					feedback->printf("\x1b[2J");
				} else {
					feedback->printf_error("> IMU Hat is not plugged in\r\n");
				}
			}
			else {
				feedback->printf_info("> Usage: plot { imu | disable }\r\n");
			}
		} else if(EQUALS(0, "monitor")) {
			if(EQUALS(1, "list")) {
				feedback->printf_info("> Currently available monitor IDs: \r\n");
				if (AHRSInstance != nullptr) {
					feedback->printf_info("> IMU on Hat %d \r\n", AHRSInstance->getPortNum());
					feedback->printf_success("> imu \r\n");
					feedback->printf_success("> accel \r\n");
					feedback->printf_success("> gyro \r\n");
					feedback->printf_success("> mag \r\n");
					feedback->printf_success("> quat \r\n");
					feedback->printf_success("> rpy \r\n");
				}
				if (PotentiometerInstance != nullptr) {
					feedback->printf_info("> Potentiometer on Hat %d \r\n", PotentiometerInstance->getPortNum());
					feedback->printf_success("> pot \r\n");
				}
				if (VoltmeterInstance != nullptr) {
					feedback->printf_info("Voltmeter on Hat %d \r\n", VoltmeterInstance->getPortNum());
					feedback->printf_success("> volt \r\n");
				}
				if (MassSensorInstance != nullptr) {
					feedback->printf_info("> Mass sensor on Hat %d \r\n", MassSensorInstance->getPortNum());
					feedback->printf_success("> mass \r\n");
				}
				if (AS7265Instance != nullptr) {
					feedback->printf_info("> Spectrophotometer on Hat %d \r\n", AS7265Instance->getPortNum());
					feedback->printf_success("> spectro \r\n");
				}
				if (ModbusInstance != nullptr) {
					feedback->printf_info("> Modbus on Hat %d \r\n", ModbusInstance->getPortNum());
					feedback->printf_success("> modbus \r\n");
					if (ModbusInstance->four_in_one_connected()) {
						feedback->printf_success("> fourinone \r\n");
					}
					if (ModbusInstance->npk_connected()) {
						feedback->printf_success("> npk \r\n");
					}
				}
			}
			else if(EQUALS(1, "enable") && cmd->num_components >= 3) {
				uint8_t refresh_rate = 10;
				uint8_t custom_loc = 1;

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
							feedback->printf_error("> IMU monitor already enabled\r\n");
						}
					} else {
						feedback->printf_error("> IMU Hat is not plugged in\r\n");
					}
				}

				else if(EQUALS(2, "accel")) {
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
							feedback->printf_error("> Accelerometer monitor already enabled\r\n");
						}
					} else {
						feedback->printf_error("> IMU Hat is not plugged in\r\n");
					}
				}

				else if(EQUALS(2, "gyro")) {
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
							feedback->printf_error("> Gyroscope monitor already enabled\r\n");
						}
					} else {
						feedback->printf_error("> IMU Hat is not plugged in\r\n");
					}
				}

				else if(EQUALS(2, "mag")) {
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
							feedback->printf_error("> Magnetometer monitor already enabled\r\n");
						}
					} else {
						feedback->printf_error("> IMU Hat is not plugged in\r\n");
					}
				}

				else if(EQUALS(2, "quat")) {
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
							feedback->printf_error("> Quaternion monitor already enabled\r\n");
						}
					} else {
						feedback->printf_error("> IMU Hat is not plugged in\r\n");
					}
				}

				else if(EQUALS(2, "rpy")) {
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
							feedback->printf_error("> Roll, pitch, yaw monitor already enabled\r\n");
						}
					} else {
						feedback->printf_error("> IMU Hat is not plugged in\r\n");
					}
				}

				else if(EQUALS(2, "pot")) {
					if (PotentiometerInstance != nullptr) {
						if (!monitor.is_enabled(POT_MONITOR)) {
							uint8_t chosen_loc;
							location++;
							if(cmd->num_components == 3) {
								chosen_loc = location;
							} else {
								chosen_loc = custom_loc;
							}
							monitor.enable(POT_MONITOR, chosen_loc, refresh_rate);
							feedback->printf("\x1b[2J");
						} else {
							feedback->printf_error("> Potentiometer monitor already enabled\r\n");
						}
					} else {
						feedback->printf_error("> Potentiometer Hat is not plugged in\r\n");
					}
				}

				else if(EQUALS(2, "volt")) {
					if (VoltmeterInstance != nullptr) {
						if (!monitor.is_enabled(VOLTMETER_MONITOR)) {
							uint8_t chosen_loc;
							location++;
							if(cmd->num_components == 3) {
								chosen_loc = location;
							} else {
								chosen_loc = custom_loc;
							}
							monitor.enable(VOLTMETER_MONITOR, chosen_loc, refresh_rate);
							feedback->printf("\x1b[2J");
						} else {
							feedback->printf_error("> Voltmeter monitor already enabled\r\n");
						}
					} else {
						feedback->printf_error("> Voltmeter Hat is not plugged in\r\n");
					}
				}

				else if(EQUALS(2, "mass")) {
					if (MassSensorInstance != nullptr) {
						if (!monitor.is_enabled(MASS_MONITOR)) {
							uint8_t chosen_loc;
							location++;
							if(cmd->num_components == 3) {
								chosen_loc = location;
							} else {
								chosen_loc = custom_loc;
							}
							monitor.enable(MASS_MONITOR, chosen_loc, refresh_rate);
							feedback->printf("\x1b[2J");
						} else {
							feedback->printf_error("> Mass monitor already enabled\r\n");
						}
					} else {
						feedback->printf_error("> Mass Hat is not plugged in\r\n");
					}
				}

				else if(EQUALS(2, "spectro")) {
					if (AS7265Instance != nullptr) {
						if (!monitor.is_enabled(SPECTRO_MONITOR)) {
							uint8_t chosen_loc;
							location+=2; // Spectro takes 2 lines
							if(cmd->num_components == 3) {
								chosen_loc = location;
							} else {
								chosen_loc = custom_loc;
							}
							monitor.enable(SPECTRO_MONITOR, chosen_loc, refresh_rate);
							feedback->printf("\x1b[2J");
						} else {
							feedback->printf_error("> Spectro monitor already enabled\r\n");
						}
					} else {
						feedback->printf_error("> Spectrophotometer Hat is not plugged in\r\n");
					}
				}

				else if(EQUALS(2, "fourinone") or EQUALS(2, "npk") or EQUALS(2, "modbus")) {
					if (ModbusInstance != nullptr) {
						if(EQUALS(2, "fourinone") or EQUALS(2, "modbus")) {
							if (ModbusInstance->four_in_one_connected()) {
								if (!monitor.is_enabled(FOURINONE_MONITOR)) {
									uint8_t chosen_loc;
									location++;
									if(cmd->num_components == 3) {
										chosen_loc = location;
									} else {
										chosen_loc = custom_loc;
									}
									monitor.enable(FOURINONE_MONITOR, chosen_loc, refresh_rate);
									feedback->printf("\x1b[2J");
								} else {
									feedback->printf_error("> FourInOne monitor already enabled\r\n");
								}
							} else {
								feedback->printf_error("> FourInOne sensor not detected\r\n");
							}
						}

						if(EQUALS(2, "npk") or EQUALS(2, "modbus")) {
							if (ModbusInstance->four_in_one_connected()) {
								if (!monitor.is_enabled(NPK_MONITOR)) {
									uint8_t chosen_loc;
									location++;
									if(cmd->num_components == 3) {
										chosen_loc = location;
									} else {
										chosen_loc = custom_loc;
									}
									monitor.enable(NPK_MONITOR, chosen_loc, refresh_rate);
									feedback->printf("\x1b[2J");
								} else {
									feedback->printf_error("> NPK monitor already enabled\r\n");
								}
							} else {
								feedback->printf_error("> NPK sensor not detected\r\n");
							}
						}
					} else {
						feedback->printf_error("> Modbus Hat is not plugged in\r\n");
					}
				}

				else {
					feedback->printf_error("> %.*s: Invalid monitor ID\r\n", cmd->components[2].length, cmd->components[2].component);
				}

			} else if(EQUALS(1, "enable") && cmd->num_components == 2) {
				uint8_t refresh_rate = 10;
				if (AHRSInstance != nullptr) {
					monitor.enable(ACCEL_MONITOR, location, refresh_rate);
					feedback->printf("\x1b[2J");
					location++;
					monitor.enable(GYRO_MONITOR, location, refresh_rate);
					feedback->printf("\x1b[2J");
					location++;
					monitor.enable(MAG_MONITOR, location, refresh_rate);
					feedback->printf("\x1b[2J");
					location++;
					monitor.enable(RPY_MONITOR, location, refresh_rate);
					feedback->printf("\x1b[2J");
					location++;
				}
				if (PotentiometerInstance != nullptr) {
					monitor.enable(POT_MONITOR, location, refresh_rate);
					feedback->printf("\x1b[2J");
					location++;
				}
				if (VoltmeterInstance != nullptr) {
					monitor.enable(VOLTMETER_MONITOR, location, refresh_rate);
					feedback->printf("\x1b[2J");
					location++;
				}
				if (MassSensorInstance != nullptr) {
					monitor.enable(MASS_MONITOR, location, refresh_rate);
					feedback->printf("\x1b[2J");
					location++;
				}
				if (AS7265Instance != nullptr) {
					monitor.enable(SPECTRO_MONITOR, location, 1);
					feedback->printf("\x1b[2J");
					location+=2;
				}
				if (ModbusInstance != nullptr) {
					if (ModbusInstance->four_in_one_connected()) {
						monitor.enable(FOURINONE_MONITOR, location, 1);
						feedback->printf("\x1b[2J");
						location+=2;
					}
					if (ModbusInstance->npk_connected()) {
						monitor.enable(NPK_MONITOR, location, 1);
						feedback->printf("\x1b[2J");
						location+=2;
					}
				}
				feedback->printf("\x1b[2J");

			} else if(EQUALS(1, "disable") && cmd->num_components == 3) {
				if(EQUALS(2, "imu")) {
					if (AHRSInstance != nullptr) {
						monitor.disable(IMU_MONITOR);
						update_monitors(monitor.get_location(IMU_MONITOR));
						location--;
					} else {
						feedback->printf_error("> IMU Hat is not plugged in\r\n");
					}
				}
				else if(EQUALS(2, "accel")) {
					if (monitor.is_enabled(ACCEL_MONITOR)) {
						monitor.disable(ACCEL_MONITOR);
						update_monitors(monitor.get_location(ACCEL_MONITOR));
						location--;
					} else {
						feedback->printf_error("> Accel monitor not created yet\r\n");
					}
				}
				else if(EQUALS(2, "gyro")) {
					if (monitor.is_enabled(GYRO_MONITOR)) {
						monitor.disable(GYRO_MONITOR);
						update_monitors(monitor.get_location(GYRO_MONITOR));
						location--;
					} else {
						feedback->printf_error("> Gyro monitor not created yet\r\n");
					}
				}
				else if(EQUALS(2, "mag")) {
					if (monitor.is_enabled(MAG_MONITOR)) {
						monitor.disable(MAG_MONITOR);
						update_monitors(monitor.get_location(MAG_MONITOR));
						location--;
					} else {
						feedback->printf_error("> Mag monitor not created yet\r\n");
					}
				}
				else if(EQUALS(2, "quat")) {
					if (monitor.is_enabled(QUAT_MONITOR)) {
						monitor.disable(QUAT_MONITOR);
						update_monitors(monitor.get_location(QUAT_MONITOR));
						location--;
					} else {
						feedback->printf_error("> Quaternion monitor not created yet\r\n");
					}
				}
				else if(EQUALS(2, "rpy")) {
					if (monitor.is_enabled(RPY_MONITOR)) {
						monitor.disable(RPY_MONITOR);
						update_monitors(monitor.get_location(RPY_MONITOR));
						location--;
					} else {
						feedback->printf_error("> RPY monitor not created yet\r\n");
					}
				}
				else if(EQUALS(2, "pot")) {
					if (monitor.is_enabled(POT_MONITOR)) {
						monitor.disable(POT_MONITOR);
						update_monitors(monitor.get_location(POT_MONITOR));
						location--;
					} else {
						feedback->printf_error("> Potentiometer monitor not created yet\r\n");
					}
				}

				else if(EQUALS(2, "pot")) {
					if (monitor.is_enabled(POT_MONITOR)) {
						monitor.disable(POT_MONITOR);
						update_monitors(monitor.get_location(POT_MONITOR));
						location--;
					} else {
						feedback->printf_error("> Potentiometer monitor not created yet\r\n");
					}
				}

				else if(EQUALS(2, "volt")) {
					if (monitor.is_enabled(VOLTMETER_MONITOR)) {
						monitor.disable(VOLTMETER_MONITOR);
						update_monitors(monitor.get_location(VOLTMETER_MONITOR));
						location--;
					} else {
						feedback->printf_error("> Voltmeter monitor not created yet\r\n");
					}
				}

				else if(EQUALS(2, "mass")) {
					if (monitor.is_enabled(MASS_MONITOR)) {
						monitor.disable(MASS_MONITOR);
						update_monitors(monitor.get_location(MASS_MONITOR));
						location--;
					} else {
						feedback->printf_error("> Mass monitor not created yet\r\n");
					}
				}

				else if(EQUALS(2, "spectro")) {
					if (monitor.is_enabled(SPECTRO_MONITOR)) {
						monitor.disable(SPECTRO_MONITOR);
						update_monitors(monitor.get_location(SPECTRO_MONITOR), 2);
						location--;
					} else {
						feedback->printf_error("> Spectrophotometer monitor not created yet\r\n");
					}
				}

				else if(EQUALS(2, "modbus") or EQUALS(2, "fourinone") or EQUALS(2, "npk")) {

					if(EQUALS(2, "fourinone") or EQUALS(2, "modbus")) {
						if (monitor.is_enabled(FOURINONE_MONITOR)) {
							monitor.disable(FOURINONE_MONITOR);
							update_monitors(monitor.get_location(FOURINONE_MONITOR));
							location--;
						} else {
							feedback->printf_error("> FourInOne monitor not created yet\r\n");
						}
					}

					if(EQUALS(2, "npk") or EQUALS(2, "modbus")) {
						if (monitor.is_enabled(NPK_MONITOR)) {
							monitor.disable(NPK_MONITOR);
							update_monitors(monitor.get_location(NPK_MONITOR));
							location--;
						} else {
							feedback->printf_error("> NPK monitor not created yet\r\n");
						}
					}
				}

				else {
					feedback->printf_error("> %.*s: Invalid monitor ID\r\n", cmd->components[2].length, cmd->components[2].component);
				}
				feedback->printf("\x1b[2J");
			}
			else if(EQUALS(1, "disable") && cmd->num_components == 2) {
				location = 0;
				disable_monitors(feedback);
			}
			else {
				feedback->printf_info("> Usage:\r\n");
				feedback->printf_info("> monitor { enable | disable } device_id [location] [refresh rate; default: 10]\r\n");
				feedback->printf_info("> monitor list\r\n");
			}
		} else {
			feedback->printf_error("> %.*s: command not found\r\n", cmd->components[0].length, cmd->components[0].component);
		}
	}
}

void Terminal::update_monitors(uint8_t loc, uint8_t num_lines) {
	for (uint8_t i = 0; i < MAX_MONITORS; ++i) {
		if (monitor.is_enabled(i) && monitor.get_location(i) >= loc) {
			monitor.enable(i, monitor.get_location(i)-num_lines, monitor.get_refresh_rate(i));
		}
	}
}

void Terminal::disable_monitors(Console* feedback) {
	location = 1;
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
	monitor.disable(POT_MONITOR);
	feedback->printf("\x1b[2J");
	monitor.disable(VOLTMETER_MONITOR);
	feedback->printf("\x1b[2J");
	monitor.disable(MASS_MONITOR);
	feedback->printf("\x1b[2J");
	monitor.disable(SPECTRO_MONITOR);
	feedback->printf("\x1b[2J");
	monitor.disable(FOURINONE_MONITOR);
	feedback->printf("\x1b[2J");
	monitor.disable(NPK_MONITOR);
	feedback->printf("\x1b[2J");
}

bool Terminal::isVerbose() {
	return verbose;
}

#undef EQUALS
