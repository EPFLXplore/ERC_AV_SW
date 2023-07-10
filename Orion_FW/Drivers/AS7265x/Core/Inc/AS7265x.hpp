/*
 * AD7265x.hpp
 *
 *  Created on: 1 Apr 2023
 *      Author: Vincent
 */

#ifndef AS7265X_CORE_INC_AS7265X_HPP_
#define AS7265X_CORE_INC_AS7265X_HPP_

#include <stdbool.h>
#include <stdint.h>
#include "stm32h7xx_hal.h"

#include "AS7265x_defines.h"


class AS7265x {
public:
	AS7265x(I2C_HandleTypeDef* hi2c, uint8_t i2c_address);
	bool begin();
	bool isConnected();

	uint8_t getDeviceType();
	uint8_t getHardwareVersion();
	uint8_t getMajorFirmwareVersion();
	uint8_t getPatchFirmwareVersion();
	uint8_t getBuildFirmwareVersion();

	uint8_t getTemperature(uint8_t deviceNumber = 0); //Get temp in C of the master IC
	float getTemperatureAverage();                    //Get average of all three ICs

	void takeMeasurements();
	void takeMeasurementsWithBulb();

	void enableIndicator(); //Blue status LED
	void disableIndicator();

	void enableBulb(uint8_t device);
	void disableBulb(uint8_t device);

	void setGain(uint8_t gain);            //1 to 64x
	void setMeasurementMode(uint8_t mode); //4 channel, other 4 channel, 6 chan, or 6 chan one shot
	void setIntegrationCycles(uint8_t cycleValue);

	void setBulbCurrent(uint8_t current, uint8_t device); //
	void setIndicatorCurrent(uint8_t current);            //0 to 8mA

	void enableInterrupt();
	void disableInterrupt();

	void softReset();

	bool dataAvailable(); //Returns true when data is available

	//Returns the various calibration data
	float getCalibratedA();
	float getCalibratedB();
	float getCalibratedC();
	float getCalibratedD();
	float getCalibratedE();
	float getCalibratedF();

	float getCalibratedG();
	float getCalibratedH();
	float getCalibratedI();
	float getCalibratedJ();
	float getCalibratedK();
	float getCalibratedL();

	float getCalibratedR();
	float getCalibratedS();
	float getCalibratedT();
	float getCalibratedU();
	float getCalibratedV();
	float getCalibratedW();

	//Get the various raw readings
	uint16_t getA();
	uint16_t getB();
	uint16_t getC();
	uint16_t getD();
	uint16_t getE();
	uint16_t getF();

	uint16_t getG();
	uint16_t getH();
	uint16_t getI();
	uint16_t getJ();
	uint16_t getK();
	uint16_t getL();

	uint16_t getR();
	uint16_t getS();
	uint16_t getT();
	uint16_t getU();
	uint16_t getV();
	uint16_t getW();

private:
	I2C_HandleTypeDef* hi2c;
	uint16_t i2c_address;
	uint16_t getChannel(uint8_t channelRegister, uint8_t device);
	float getCalibratedValue(uint8_t calAddress, uint8_t device);
	float convertBytesToFloat(uint32_t myLong);

	void selectDevice(uint8_t device); //Change between the x51, x52, or x53 for data and settings

	uint8_t virtualReadRegister(uint8_t virtualAddr);
	void virtualWriteRegister(uint8_t virtualAddr, uint8_t dataToWrite);

	uint8_t readRegister(uint8_t addr);
	bool writeRegister(uint8_t addr, uint8_t val);
	uint16_t maxWaitTime = 35; //Based on integration cycles
};


#endif /* AS7265X_CORE_INC_AS7265X_HPP_ */
