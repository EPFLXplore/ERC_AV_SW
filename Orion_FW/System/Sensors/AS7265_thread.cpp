/*
 * AS7265_thread.cpp
 *
 *      Author: Vincent
 */

#include "AS7265_thread.h"

#include "Telemetry.h"

AS7265Thread* AS7265Instance = nullptr;
static char cbuf[256]; // for printf

void AS7265Thread::init() {
	AS7265Instance = this;
	// Initialize the sensor
	bool success = spectro.begin();
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(!success) {
		AS7265Instance = nullptr;
		terminate();
		parent->resetProber();
		return;
	}

	// Sensor related configuration after successfully connected
//	spectro.disableIndicator();
}

// Declare your data with the proper data structure defined in DataStructures.h
static DummyData dummy_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static DummySystem_DummyPacket packet;

void AS7265Thread::loop() {
	// Get the sensor data. Here we only read a differential value as an example
//	dummy_data.data = dummy_" %f \n", spectro.ADSreadADC_Differential_0_1();
	// We can print it to SVW console (optional)
//	printf("Diff value %s \n", dummy_data.toString(cbuf));
	spectro.takeMeasurementsWithBulb();
	  printf("A %f \n", spectro.getCalibratedA()); //410nm

	  printf("B %f \n", spectro.getCalibratedB()); //435nm

	  printf("C %f \n", spectro.getCalibratedC()); //460nm

	  printf("D %f \n", spectro.getCalibratedD()); //485nm

	  printf("E %f \n", spectro.getCalibratedE()); //510nm

	  printf("F %f \n", spectro.getCalibratedF()); //535nm


	  printf("G %f \n", spectro.getCalibratedG()); //560nm

	  printf("H %f \n", spectro.getCalibratedH()); //585nm

	  printf("R %f \n", spectro.getCalibratedR()); //610nm

	  printf("I %f \n", spectro.getCalibratedI()); //645nm

	  printf("S %f \n", spectro.getCalibratedS()); //680nm

	  printf("J %f \n", spectro.getCalibratedJ()); //705nm


	  printf("T %f \n", spectro.getCalibratedT()); //730nm

	  printf("U %f \n", spectro.getCalibratedU()); //760nm

	  printf("V %f \n", spectro.getCalibratedV()); //810nm

	  printf("W %f \n", spectro.getCalibratedW()); //860nm

	  printf("K %f \n", spectro.getCalibratedK()); //900nm

	  printf("K %f \n", spectro.getCalibratedL()); //940nm


//	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
//		// Send data over RoCo network
////		dummy_data.toArray((uint8_t*) &packet);
////		JetsonNetwork.send(&packet);
//		portYIELD();
//	} else {
//		AS7265Instance = nullptr;
//		terminate();
//		parent->resetProber();
//	}
	osDelay(1000);
}
