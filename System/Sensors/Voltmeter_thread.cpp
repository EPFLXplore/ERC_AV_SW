/*
 * IMU.cpp
 *
 *  Created on: 10 Aug 2020
 *      Author: YassineBakkali
 */

#include "Voltmeter_thread.h"

#include "Telemetry.h"


void VoltmeterThread::init() {

	// Declare the structure where we using GND as address.
	// Look at the top of the header file for addresses.
	bool success = ADS1113_init(&voltmeter, parent->getI2C(), ADS_ADDR_GND); // Or ADS1015(&i2c, &hi2c1, ADS_ADDR_GND);
	if(!success) {
//		println("[%d] BNO055 initialization failed", portNum);
		terminate();
		parent->resetProber();
		return;
	}
	ADSsetGain(&voltmeter, GAIN_ONE);
}


static VoltmeterData data;
static avionics_voltmeter_packet packet;
void VoltmeterThread::loop() {
	data.voltage = ADSreadADC_Voltage(&voltmeter);
	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
//		println("[i2c%d] %s", portNum, data.toString(cbuf));
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
//		println("[i2c%d] BNO055 disconnected", portNum);
		terminate();
		parent->resetProber();
	}
}

//
//
//static void ADC1115_test(void){
//	ads1113_t i2c;
//	char MSG[100];
//	// Declare the structure where we using GND as address.
//	// Look at the top of the header file for addresses.
//	bool success = ADS1113_init(&i2c, &hi2c4, ADS_ADDR_GND); // Or ADS1015(&i2c, &hi2c1, ADS_ADDR_GND);
//	ADSsetGain(&i2c, GAIN_ONE);
//	int16_t adc_diff;
//	float adc_voltage;
//
//	while(1){
//	   // Get ADC values
//	   adc_diff = ADSreadADC_Differential_0_1(&i2c);
//	   adc_voltage = ADSreadADC_Voltage(&i2c);
//	//
//	   sprintf(MSG, "ADC diff reading \t = %d \t Voltage \t = %.2f \r\n", adc_diff, adc_voltage);
//	//       sprintf(MSG, "ADC diff reading \t = %d \n", adc_diff);
////	   sprintf(MSG, "ADC diff reading \t = %.2f \r\n", adc_voltage);
//	   HAL_UART_Transmit(&huart2, MSG, strlen((char*) MSG), 100);
//	   HAL_Delay(100);
//   }
//
//}
