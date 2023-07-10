/*
 * AD7265x.cpp
 *
 *  Created on: 1 Apr 2023
 *      Author: Vincent
 */

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "stdio.h"
#include "Build.h"
#include <memory.h>
#include "../Inc/AS7265x.hpp"

AS7265x::AS7265x(I2C_HandleTypeDef* hi2c_, uint8_t i2c_address_) :
									hi2c(hi2c_), i2c_address(i2c_address_ << 1)
{}

//Initializes the sensor with basic settings
//Returns false if sensor is not detected
bool AS7265x::begin()
{
//	HAL_StatusTypeDef status;
//	if (HAL_I2C_Init(hi2c) == HAL_OK){
//		status = HAL_I2C_IsDeviceReady(hi2c, i2c_address, 10, 100);
//	} else {
//		return false;
//	}
//
//	hi2c->Init.Timing = 0x00401242;
//	hi2c->Init.OwnAddress1 = 0;
//	hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//	hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//	hi2c->Init.OwnAddress2 = 0;
//	hi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//	hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//	hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//	/** Configure Analogue filter
//	*/
//	if (HAL_I2CEx_ConfigAnalogFilter(hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//	{
//		return false;
//	}
//	/** Configure Digital filter
//	*/
//	if (HAL_I2CEx_ConfigDigitalFilter(hi2c, 0x0F) != HAL_OK)
//	{
//		return false;
//	}
//	/** I2C Enable Fast Mode Plus
//	*/
//	if (hi2c->Instance = I2C1)
//		HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
//	else if (hi2c->Instance = I2C2)
//		HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C2);
//	else if (hi2c->Instance = I2C3)
//		HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C3);


//	if(status == HAL_OK) {

		if (isConnected() == false)
		return (false); //Check for sensor presence


		//Check to see if both slaves are detected
		uint8_t value = virtualReadRegister(AS7265X_DEV_SELECT_CONTROL);
		if ((value & 0b00110000) == 0)
		return (false); //Test if Slave1 and 2 are detected. If not, bail.

		setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_WHITE);
		setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_IR);
		setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_UV);

		disableBulb(AS7265x_LED_WHITE); //Turn off bulb to avoid heating sensor
		disableBulb(AS7265x_LED_IR);
		disableBulb(AS7265x_LED_UV);

		setIndicatorCurrent(AS7265X_INDICATOR_CURRENT_LIMIT_8MA); //Set to 8mA (maximum)
		enableIndicator();

		setIntegrationCycles(50); //50 * 2.8ms = 140ms. 0 to 255 is valid.
		//If you use Mode 2 or 3 (all the colors) then integration time is double. 140*2 = 280ms between readings.

		setGain(AS7265X_GAIN_64X); //Set gain to 64x

		setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT); //One-shot reading of VBGYOR

		enableInterrupt();

//	}

	return (true); //We're all setup!
}

uint8_t AS7265x::getDeviceType()
{
  return (virtualReadRegister(AS7265X_HW_VERSION_HIGH));
}

uint8_t AS7265x::getHardwareVersion()
{
  return (virtualReadRegister(AS7265X_HW_VERSION_LOW));
}

uint8_t AS7265x::getMajorFirmwareVersion()
{
  virtualWriteRegister(AS7265X_FW_VERSION_HIGH, 0x01); //Set to 0x01 for Major
  virtualWriteRegister(AS7265X_FW_VERSION_LOW, 0x01);  //Set to 0x01 for Major

  return (virtualReadRegister(AS7265X_FW_VERSION_LOW));
}

uint8_t AS7265x::getPatchFirmwareVersion()
{
  virtualWriteRegister(AS7265X_FW_VERSION_HIGH, 0x02); //Set to 0x02 for Patch
  virtualWriteRegister(AS7265X_FW_VERSION_LOW, 0x02);  //Set to 0x02 for Patch

  return (virtualReadRegister(AS7265X_FW_VERSION_LOW));
}

uint8_t AS7265x::getBuildFirmwareVersion()
{
  virtualWriteRegister(AS7265X_FW_VERSION_HIGH, 0x03); //Set to 0x03 for Build
  virtualWriteRegister(AS7265X_FW_VERSION_LOW, 0x03);  //Set to 0x03 for Build

  return (virtualReadRegister(AS7265X_FW_VERSION_LOW));
}

//Returns true if I2C device ack's
bool AS7265x::isConnected()
{
  //Give IC 660ms for startup - max 1000ms
  for (uint8_t x = 0; x < 100; x++)
  {
	HAL_StatusTypeDef status;
#ifdef ENERGIA
    status = HAL_I2C_Master_Transmit(&hi2c, deviceAddress, 0x00, 1, 100)
#else
	status = HAL_I2C_IsDeviceReady(hi2c, i2c_address, 10, 100);
#endif

    if (status == HAL_OK)
      return (true); //Sensor ACK'd
  }
  return (false); //Sensor did not ACK
}

//Tells IC to take all channel measurements and polls for data ready flag
void AS7265x::takeMeasurements()
{
  setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT); //Set mode to all 6-channels, one-shot

  //Wait for data to be ready
  int startTime = xTaskGetTickCount();
  while (dataAvailable() == false)
  {
    if(xTaskGetTickCount() - startTime > (maxWaitTime)) return; //Sensor failed to respond
    osDelay(AS7265X_POLLING_DELAY);
  }

  //Readings can now be accessed via getCalibratedA(), getJ(), etc
}

//Turns on all bulbs, takes measurements of all channels, turns off all bulbs
void AS7265x::takeMeasurementsWithBulb()
{
  enableBulb(AS7265x_LED_WHITE);
  enableBulb(AS7265x_LED_IR);
  enableBulb(AS7265x_LED_UV);

  takeMeasurements();

  disableBulb(AS7265x_LED_WHITE); //Turn off bulb to avoid heating sensor
  disableBulb(AS7265x_LED_IR);
  disableBulb(AS7265x_LED_UV);
}

//Get the various color readings
uint16_t AS7265x::getG()
{
  return (getChannel(AS7265X_R_G_A, AS72652_VISIBLE));
}
uint16_t AS7265x::getH()
{
  return (getChannel(AS7265X_S_H_B, AS72652_VISIBLE));
}
uint16_t AS7265x::getI()
{
  return (getChannel(AS7265X_T_I_C, AS72652_VISIBLE));
}
uint16_t AS7265x::getJ()
{
  return (getChannel(AS7265X_U_J_D, AS72652_VISIBLE));
}
uint16_t AS7265x::getK()
{
  return (getChannel(AS7265X_V_K_E, AS72652_VISIBLE));
}
uint16_t AS7265x::getL()
{
  return (getChannel(AS7265X_W_L_F, AS72652_VISIBLE));
}

//Get the various NIR readings
uint16_t AS7265x::getR()
{
  return (getChannel(AS7265X_R_G_A, AS72651_NIR));
}
uint16_t AS7265x::getS()
{
  return (getChannel(AS7265X_S_H_B, AS72651_NIR));
}
uint16_t AS7265x::getT()
{
  return (getChannel(AS7265X_T_I_C, AS72651_NIR));
}
uint16_t AS7265x::getU()
{
  return (getChannel(AS7265X_U_J_D, AS72651_NIR));
}
uint16_t AS7265x::getV()
{
  return (getChannel(AS7265X_V_K_E, AS72651_NIR));
}
uint16_t AS7265x::getW()
{
  return (getChannel(AS7265X_W_L_F, AS72651_NIR));
}

//Get the various UV readings
uint16_t AS7265x::getA()
{
  return (getChannel(AS7265X_R_G_A, AS72653_UV));
}
uint16_t AS7265x::getB()
{
  return (getChannel(AS7265X_S_H_B, AS72653_UV));
}
uint16_t AS7265x::getC()
{
  return (getChannel(AS7265X_T_I_C, AS72653_UV));
}
uint16_t AS7265x::getD()
{
  return (getChannel(AS7265X_U_J_D, AS72653_UV));
}
uint16_t AS7265x::getE()
{
  return (getChannel(AS7265X_V_K_E, AS72653_UV));
}
uint16_t AS7265x::getF()
{
  return (getChannel(AS7265X_W_L_F, AS72653_UV));
}

//A the 16-bit value stored in a given channel registerReturns
uint16_t AS7265x::getChannel(uint8_t channelRegister, uint8_t device)
{
  selectDevice(device);
  uint16_t colorData = virtualReadRegister(channelRegister) << 8; //High uint8_t
  colorData |= virtualReadRegister(channelRegister + 1);          //Low uint8_t
  return (colorData);
}

//Returns the various calibration data
float AS7265x::getCalibratedA()
{
  return (getCalibratedValue(AS7265X_R_G_A_CAL, AS72653_UV));
}
float AS7265x::getCalibratedB()
{
  return (getCalibratedValue(AS7265X_S_H_B_CAL, AS72653_UV));
}
float AS7265x::getCalibratedC()
{
  return (getCalibratedValue(AS7265X_T_I_C_CAL, AS72653_UV));
}
float AS7265x::getCalibratedD()
{
  return (getCalibratedValue(AS7265X_U_J_D_CAL, AS72653_UV));
}
float AS7265x::getCalibratedE()
{
  return (getCalibratedValue(AS7265X_V_K_E_CAL, AS72653_UV));
}
float AS7265x::getCalibratedF()
{
  return (getCalibratedValue(AS7265X_W_L_F_CAL, AS72653_UV));
}

//Returns the various calibration data
float AS7265x::getCalibratedG()
{
  return (getCalibratedValue(AS7265X_R_G_A_CAL, AS72652_VISIBLE));
}
float AS7265x::getCalibratedH()
{
  return (getCalibratedValue(AS7265X_S_H_B_CAL, AS72652_VISIBLE));
}
float AS7265x::getCalibratedI()
{
  return (getCalibratedValue(AS7265X_T_I_C_CAL, AS72652_VISIBLE));
}
float AS7265x::getCalibratedJ()
{
  return (getCalibratedValue(AS7265X_U_J_D_CAL, AS72652_VISIBLE));
}
float AS7265x::getCalibratedK()
{
  return (getCalibratedValue(AS7265X_V_K_E_CAL, AS72652_VISIBLE));
}
float AS7265x::getCalibratedL()
{
  return (getCalibratedValue(AS7265X_W_L_F_CAL, AS72652_VISIBLE));
}

float AS7265x::getCalibratedR()
{
  return (getCalibratedValue(AS7265X_R_G_A_CAL, AS72651_NIR));
}
float AS7265x::getCalibratedS()
{
  return (getCalibratedValue(AS7265X_S_H_B_CAL, AS72651_NIR));
}
float AS7265x::getCalibratedT()
{
  return (getCalibratedValue(AS7265X_T_I_C_CAL, AS72651_NIR));
}
float AS7265x::getCalibratedU()
{
  return (getCalibratedValue(AS7265X_U_J_D_CAL, AS72651_NIR));
}
float AS7265x::getCalibratedV()
{
  return (getCalibratedValue(AS7265X_V_K_E_CAL, AS72651_NIR));
}
float AS7265x::getCalibratedW()
{
  return (getCalibratedValue(AS7265X_W_L_F_CAL, AS72651_NIR));
}

//Given an address, read four bytes and return the floating point calibrated value
float AS7265x::getCalibratedValue(uint8_t calAddress, uint8_t device)
{
  selectDevice(device);

  uint8_t b0, b1, b2, b3;
  b0 = virtualReadRegister(calAddress + 0);
  b1 = virtualReadRegister(calAddress + 1);
  b2 = virtualReadRegister(calAddress + 2);
  b3 = virtualReadRegister(calAddress + 3);

  //Channel calibrated values are stored big-endian
  uint32_t calBytes = 0;
  calBytes |= ((uint32_t)b0 << (8 * 3));
  calBytes |= ((uint32_t)b1 << (8 * 2));
  calBytes |= ((uint32_t)b2 << (8 * 1));
  calBytes |= ((uint32_t)b3 << (8 * 0));

  return (convertBytesToFloat(calBytes));
}

//Given 4 bytes returns the floating point value
float AS7265x::convertBytesToFloat(uint32_t myLong)
{
  float myFloat;
  memcpy(&myFloat, &myLong, 4); //Copy bytes into a float
  return (myFloat);
}

//Mode 0: 4 channels out of 6 (see datasheet)
//Mode 1: Different 4 channels out of 6 (see datasheet)
//Mode 2: All 6 channels continuously
//Mode 3: One-shot reading of all channels
void AS7265x::setMeasurementMode(uint8_t mode)
{
  if (mode > 0b11)
    mode = 0b11; //Error check

  //Read, mask/set, write
  uint8_t value = virtualReadRegister(AS7265X_CONFIG); //Read
  value &= 0b11110011;                                 //Clear BANK bits
  value |= (mode << 2);                                //Set BANK bits with user's choice
  virtualWriteRegister(AS7265X_CONFIG, value);         //Write
}

//Sets the gain value
//Gain 0: 1x (power-on default)
//Gain 1: 3.7x
//Gain 2: 16x
//Gain 3: 64x
void AS7265x::setGain(uint8_t gain)
{
  if (gain > 0b11)
    gain = 0b11;

  //Read, mask/set, write
  uint8_t value = virtualReadRegister(AS7265X_CONFIG); //Read
  value &= 0b11001111;                                 //Clear GAIN bits
  value |= (gain << 4);                                //Set GAIN bits with user's choice
  virtualWriteRegister(AS7265X_CONFIG, value);         //Write
}

//Sets the integration cycle amount
//Give this function a byte from 0 to 255.
//Time will be 2.8ms * [integration cycles + 1]
void AS7265x::setIntegrationCycles(uint8_t cycleValue)
{
  maxWaitTime = (int)(cycleValue * 2.8 * 1.5); //Wait for integration time + 50%
  virtualWriteRegister(AS7265X_INTERGRATION_TIME, cycleValue); //Write
}

void AS7265x::enableInterrupt()
{
  //Read, mask/set, write
  uint8_t value = virtualReadRegister(AS7265X_CONFIG); //Read
  value |= (1 << 6);                                   //Set INT bit
  virtualWriteRegister(AS7265X_CONFIG, value);         //Write
}

//Disables the interrupt pin
void AS7265x::disableInterrupt()
{
  //Read, mask/set, write
  uint8_t value = virtualReadRegister(AS7265X_CONFIG); //Read
  value &= ~(1 << 6);                                  //Clear INT bit
  virtualWriteRegister(AS7265X_CONFIG, value);         //Write
}

//Checks to see if DRDY flag is set in the control setup register
bool AS7265x::dataAvailable()
{
  uint8_t value = virtualReadRegister(AS7265X_CONFIG);
  return (value & (1 << 1)); //Bit 1 is DATA_RDY
}

//Enable the LED or bulb on a given device
void AS7265x::enableBulb(uint8_t device)
{
  selectDevice(device);

  //Read, mask/set, write
  uint8_t value = virtualReadRegister(AS7265X_LED_CONFIG);
  value |= (1 << 3); //Set the bit
  virtualWriteRegister(AS7265X_LED_CONFIG, value);
}

//Disable the LED or bulb on a given device
void AS7265x::disableBulb(uint8_t device)
{
  selectDevice(device);

  //Read, mask/set, write
  uint8_t value = virtualReadRegister(AS7265X_LED_CONFIG);
  value &= ~(1 << 3); //Clear the bit
  virtualWriteRegister(AS7265X_LED_CONFIG, value);
}

//Set the current limit of bulb/LED.
//Current 0: 12.5mA
//Current 1: 25mA
//Current 2: 50mA
//Current 3: 100mA
void AS7265x::setBulbCurrent(uint8_t current, uint8_t device)
{
  selectDevice(device);

  // set the current
  if (current > 0b11)
    current = 0b11;                                        //Limit to two bits
  uint8_t value = virtualReadRegister(AS7265X_LED_CONFIG); //Read
  value &= 0b11001111;                                     //Clear ICL_DRV bits
  value |= (current << 4);                                 //Set ICL_DRV bits with user's choice
  virtualWriteRegister(AS7265X_LED_CONFIG, value);         //Write
}

//As we read various registers we have to point at the master or first/second slave
void AS7265x::selectDevice(uint8_t device)
{
  //Set the bits 0:1. Just overwrite whatever is there because masking in the correct value doesn't work.
  virtualWriteRegister(AS7265X_DEV_SELECT_CONTROL, device);

  //This fails
  //uint8_t value = virtualReadRegister(AS7265X_DEV_SELECT_CONTROL);
  //value &= 0b11111100; //Clear lower two bits
  //if(device < 3) value |= device; //Set the bits
  //virtualWriteRegister(AS7265X_DEV_SELECT_CONTROL, value);
}

//Enable the onboard indicator LED
void AS7265x::enableIndicator()
{
  selectDevice(AS72651_NIR);

  //Read, mask/set, write
  uint8_t value = virtualReadRegister(AS7265X_LED_CONFIG);
  value |= (1 << 0); //Set the bit

  virtualWriteRegister(AS7265X_LED_CONFIG, value);
}

//Disable the onboard indicator LED
void AS7265x::disableIndicator()
{
  selectDevice(AS72651_NIR);

  //Read, mask/set, write
  uint8_t value = virtualReadRegister(AS7265X_LED_CONFIG);
  value &= ~(1 << 0); //Clear the bit

  virtualWriteRegister(AS7265X_LED_CONFIG, value);
}

//Set the current limit of onboard LED. Default is max 8mA = 0b11.
void AS7265x::setIndicatorCurrent(uint8_t current)
{
  selectDevice(AS72651_NIR);

  if (current > 0b11)
    current = 0b11;
  //Read, mask/set, write
  uint8_t value = virtualReadRegister(AS7265X_LED_CONFIG); //Read
  value &= 0b11111001;                                     //Clear ICL_IND bits
  value |= (current << 1);                                 //Set ICL_IND bits with user's choice

  virtualWriteRegister(AS7265X_LED_CONFIG, value); //Write
}

//Returns the temperature of a given device in C
uint8_t AS7265x::getTemperature(uint8_t deviceNumber)
{
  selectDevice(deviceNumber);
  return (virtualReadRegister(AS7265X_DEVICE_TEMP));
}

//Returns an average of all the sensor temps in C
float AS7265x::getTemperatureAverage()
{
  float average = 0;

  for (uint8_t x = 0; x < 3; x++)
    average += getTemperature(x);

  return (average / 3);
}

//Does a soft reset
//Give sensor at least 1000ms to reset
void AS7265x::softReset()
{
  //Read, mask/set, write
  uint8_t value = virtualReadRegister(AS7265X_CONFIG); //Read
  value |= (1 << 7);                                   //Set RST bit, automatically cleared after reset
  virtualWriteRegister(AS7265X_CONFIG, value);         //Write
}

//Read a virtual register from the AS7265x
uint8_t AS7265x::virtualReadRegister(uint8_t virtualAddr)
{
  uint8_t status;

  //Do a prelim check of the read register
  status = readRegister(AS7265X_STATUS_REG);
  if ((status & AS7265X_RX_VALID) != 0) //There is data to be read
  {
    readRegister(AS7265X_READ_REG); //Read the byte but do nothing with it
  }

  //Wait for WRITE flag to clear
  int startTime = xTaskGetTickCount();
  while (1)
  {
    if(xTaskGetTickCount() - startTime > maxWaitTime) return(0); //Sensor failed to respond
    status = readRegister(AS7265X_STATUS_REG);
    if ((status & AS7265X_TX_VALID) == 0)
      break; // If TX bit is clear, it is ok to write
    osDelay(AS7265X_POLLING_DELAY);
  }

  // Send the virtual register address (bit 7 should be 0 to indicate we are reading a register).
  writeRegister(AS7265X_WRITE_REG, virtualAddr);

  //Wait for READ flag to be set
  startTime = xTaskGetTickCount();
  while (1)
  {
	volatile int count = xTaskGetTickCount();
    if(count - startTime > maxWaitTime) return(0); //Sensor failed to respond
    status = readRegister(AS7265X_STATUS_REG);
    if ((status & AS7265X_RX_VALID) != 0)
      break; // Read data is ready.
    osDelay(AS7265X_POLLING_DELAY);
  }
  uint8_t incoming = readRegister(AS7265X_READ_REG);
  return (incoming);
}


//Write to a virtual register in the AS726x
void AS7265x::virtualWriteRegister(uint8_t virtualAddr, uint8_t dataToWrite)
{
  uint8_t status;

  //Wait for WRITE register to be empty
  unsigned long startTime = xTaskGetTickCount();
  while (1)
  {
    if(xTaskGetTickCount() - startTime > maxWaitTime) return; //Sensor failed to respond
    status = readRegister(AS7265X_STATUS_REG);
    if ((status & AS7265X_TX_VALID) == 0)
      break; // No inbound TX pending at slave. Okay to write now.
    osDelay(AS7265X_POLLING_DELAY);
  }

  // Send the virtual register address (setting bit 7 to indicate we are writing to a register).
  writeRegister(AS7265X_WRITE_REG, (virtualAddr | 1 << 7));

  //Wait for WRITE register to be empty
  startTime = xTaskGetTickCount();
  while (1)
  {
	volatile int count = xTaskGetTickCount();
    if(count - startTime > maxWaitTime) return; //Sensor failed to respond
    status = readRegister(AS7265X_STATUS_REG);
    if ((status & AS7265X_TX_VALID) == 0)
      break; // No inbound TX pending at slave. Okay to write now.
    osDelay(AS7265X_POLLING_DELAY);
  }

  // Send the data to complete the operation.
  writeRegister(AS7265X_WRITE_REG, dataToWrite);
}


uint8_t AS7265x::readRegister(uint8_t addr)
{
  uint8_t data = 0;
//  if (HAL_I2C_Mem_Write(hi2c, i2c_address, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000) != HAL_OK)
//  {
//    //Serial.println("No ack!");
//    return (0); //Device failed to ack
//  }

  if (HAL_I2C_Mem_Read(hi2c, i2c_address, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000) != HAL_OK)
  {
    //Serial.println("No ack!");
    return (0); //Device failed to respond
  }

  return data;
}

bool AS7265x::writeRegister(uint8_t addr, uint8_t value) {
	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, i2c_address, 10, 100);
	status = HAL_I2C_IsDeviceReady(hi2c, i2c_address, 10, 100);
	if(status != HAL_OK) {
		return false;
	}

	return HAL_I2C_Mem_Write(hi2c, i2c_address, addr, I2C_MEMADD_SIZE_8BIT, &value, 1, 10) == HAL_OK;
}

