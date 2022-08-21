/*
 * system.cpp
 *
 *  Created on: Aug 1, 2022
 *      Author: Gustavo Fring
 */


#include "System.h"



void initCortex() {
//	static WatchdogThread watchdog(&hiwdg1);
//	static ProberThread prober1(&hi2c1);
//	static ProberThread prober2(&hi2c2);
	static ProberThread prober3(&hi2c4);
}



extern "C" {

void vApplicationStackOverflowHook() {
	while(1);
}

}


