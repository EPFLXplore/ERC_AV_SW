/*
 * system.cpp
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine
 */


#include <four_in_one_thread.h>
#include "System.h"
#include "Prober.h"
#include "i2c.h"
//#include "NPK_thread.h"


void initCortex() {
//	static ProberThread prober1(&hi2c1);
//	static ProberThread prober2(&hi2c2);
	static ProberThread prober3(&hi2c3);

}



extern "C" {

void vApplicationStackOverflowHook() {
	while(1);
}

}


