
#include "U087.h"

void voltmeter_init(voltmeter *dev, I2C_HandleTypeDef *i2cHandle){
	  dev->i2cHandle = i2cHandle;
	  dev->gain = PAG_512;
	  dev->mode = SINGLESHOT;
	  dev->rate = RATE_8;
	  setGain(dev, dev->gain);
	  setMode(dev, dev->mode);
	  setRate(dev, dev->rate);
	  dev->calibration_factor = 1;
	  dev->adc_raw = 0;
	  dev->resolution = getResolution(dev->gain);
	  dev->cover_time = getCoverTime(dev->rate);
	  dev->hope = PAGE512_VOLT / dev->resolution;
}


HAL_StatusTypeDef i2cReadBytes(voltmeter *dev, uint8_t addr, uint8_t reg_addr, uint8_t* buff, uint16_t len) {
	  uint8_t status = HAL_I2C_Master_Transmit(dev->i2cHandle, addr << 1, &reg_addr, 1, HAL_MAX_DELAY);
	  HAL_Delay(10);
	  if (status == HAL_OK) {
	      status = HAL_I2C_Master_Receive(dev->i2cHandle, addr << 1, buff, len, HAL_MAX_DELAY);
		  HAL_Delay(10);
	  }
	  return status;
}

HAL_StatusTypeDef i2cWriteBytes(voltmeter *dev, uint8_t addr, uint8_t reg_addr, uint8_t* buff, uint16_t len) {

  uint8_t status = HAL_I2C_Master_Transmit(dev->i2cHandle, addr << 1, &reg_addr, 1, HAL_MAX_DELAY);
  HAL_Delay(10);
  if (status == HAL_OK) {
	  status = HAL_I2C_Master_Transmit(dev->i2cHandle, addr << 1, buff, len, HAL_MAX_DELAY);
	  HAL_Delay(10);
  }
  return status;
}

HAL_StatusTypeDef i2cReadU16(voltmeter *dev, uint8_t addr, uint8_t reg_addr, uint16_t* value) {
  uint8_t read_buf[2] = {0x00, 0x00};
  uint8_t status = i2cReadBytes(dev, addr, reg_addr, read_buf, 2);
  *value = (read_buf[0] << 8) | read_buf[1];
  return status;
}

HAL_StatusTypeDef i2cWriteU16(voltmeter *dev, uint8_t addr, uint8_t reg_addr, uint16_t value) {
  uint8_t write_buf[2];
  write_buf[0] = value >> 8;
  write_buf[1] = value & 0xff;
  return i2cWriteBytes(dev, addr, reg_addr, write_buf, 2);
}

HAL_StatusTypeDef setGain(voltmeter *dev, voltmeterGain_t gain) {
  uint16_t reg_value = 0;
  uint8_t status = i2cReadU16(dev, ADS115_ADDR, ADS1115_RA_CONFIG, &reg_value);

  if (status != HAL_OK) {
    return status;
  }

  reg_value &= ~(0b0111 << 9);
  reg_value |= gain << 9;

  status = i2cWriteU16(dev, ADS115_ADDR, ADS1115_RA_CONFIG, reg_value);

  if (status == HAL_OK) {
    dev->gain = gain;
    dev->resolution = getResolution(gain);
    int16_t hope = 1;
    int16_t actual = 1;
    if (readCalibrationFromEEPROM(dev, gain, &hope, &actual)) {
      dev->calibration_factor = (double)hope / actual;
    }
  }
  return status;
}

HAL_StatusTypeDef setRate(voltmeter *dev, voltmeterRate_t rate) {
  uint16_t reg_value = 0;
  uint8_t status = i2cReadU16(dev, ADS115_ADDR, ADS1115_RA_CONFIG, &reg_value);
  if (status != HAL_OK) {
    return status;
  }

  reg_value &= ~(0b0111 << 5);
  reg_value |= rate << 5;

  status = i2cWriteU16(dev, ADS115_ADDR, ADS1115_RA_CONFIG, reg_value);

  if (status == HAL_OK) {
    dev->rate = rate;
    dev->cover_time = getCoverTime(dev->rate);
  }

  return status;
}

HAL_StatusTypeDef setMode(voltmeter *dev, voltmeterMode_t mode) {
  uint16_t reg_value = 0;
  uint8_t status = i2cReadU16(dev, ADS115_ADDR, ADS1115_RA_CONFIG, &reg_value);
  if (status != HAL_OK) {
    return status;
  }

  reg_value &= ~(0b0001 << 8);
  reg_value |= mode << 8;

  status = i2cWriteU16(dev, ADS115_ADDR, ADS1115_RA_CONFIG, reg_value);
  if (status == HAL_OK) {
    dev->mode = mode;
  }

  return status;
}


HAL_StatusTypeDef isInConversion(voltmeter *dev) {
  uint16_t value = 0x00;
  uint8_t status = i2cReadU16(dev, ADS115_ADDR,  ADS1115_RA_CONFIG, &value);

  if (status != HAL_OK) {
    return status;
  }

  return (value & (1 << 15)) ? HAL_ERROR : HAL_OK;
}

HAL_StatusTypeDef startSingleConversion(voltmeter *dev) {
  uint16_t reg_value = 0;
  uint8_t status  = i2cReadU16(dev, ADS115_ADDR, ADS1115_RA_CONFIG, &reg_value);
  if (status != HAL_OK) {
    return status;
  }

  reg_value &= ~(0b0001 << 15);
  reg_value |= 0x01 << 15;

  status = i2cWriteU16(dev, ADS115_ADDR, ADS1115_RA_CONFIG, reg_value);

  return status;
}

HAL_StatusTypeDef getVoltage(voltmeter *dev, bool calibration) {
	uint8_t status;
  if (calibration) {
	status = getConversion(dev, DEFAULT_TIMEOUT);
	if (status != HAL_OK){
		return HAL_ERROR;
	}
    dev->voltage = dev->resolution * dev->calibration_factor * dev->adc_raw * VOLTMETER_MEASURING_DIR;
  } else {
	  dev->voltage = dev->resolution * dev->adc_raw * VOLTMETER_MEASURING_DIR;
  }
}

HAL_StatusTypeDef getAdcRaw(voltmeter *dev) {
  uint16_t value = 0x01;
  uint8_t status = i2cReadU16(dev, ADS115_ADDR, ADS1115_RA_CONVERSION, &value);
  dev->adc_raw = value;
  return status;
}

HAL_StatusTypeDef getConversion(voltmeter *dev, uint16_t timeout) {
  uint8_t status = 3;
  if (dev->mode == SINGLESHOT) {
	while(status != HAL_OK){
		status = startSingleConversion(dev);
	}
    HAL_Delay(dev->cover_time);
    uint64_t time = HAL_GetTick() + timeout;
    while (time > HAL_GetTick() && (isInConversion(dev) == HAL_OK));
//    dev->value_test = time;
  }
  return getAdcRaw(dev);
}


HAL_StatusTypeDef EEPROMWrite(voltmeter *dev, uint8_t address, uint8_t* buff, uint8_t len) {
  return i2cWriteBytes(dev, EEPROM_ADDR, address, buff, len);
}

HAL_StatusTypeDef EEPROMRead(voltmeter *dev, uint8_t address, uint8_t* buff, uint8_t len) {
  return i2cReadBytes(dev, EEPROM_ADDR, address, buff, len);
}

HAL_StatusTypeDef saveCalibration2EEPROM(voltmeter *dev, voltmeterGain_t gain, int16_t hope, int16_t actual) {
  if (hope == 0 || actual == 0) {
    return HAL_ERROR;
  }

  uint8_t buff[8];
  memset(buff, 0, 8);
  buff[0] = gain;
  buff[1] = hope >> 8;
  buff[2] = hope & 0xFF;

  buff[3] = actual >> 8;
  buff[4] = actual & 0xFF;

  for (uint8_t i = 0; i < 5; i++) {
    buff[5] ^= buff[i];
  }

  uint8_t addr = getPGAEEEPROMAddr(gain);
  return EEPROMWrite(dev, addr, buff, 8);
}

uint8_t getPGAEEEPROMAddr(voltmeterGain_t gain) {
  switch (gain) {
    case PAG_6144:
      return VOLTMETER_PAG_6144_CAL_ADDR;
    case PAG_4096:
      return VOLTMETER_PAG_4096_CAL_ADDR;
    case PAG_2048:
      return VOLTMETER_PAG_2048_CAL_ADDR;
    case PAG_1024:
      return VOLTMETER_PAG_1024_CAL_ADDR;
    case PAG_512:
      return VOLTMETER_PAG_512_CAL_ADDR;
    case PAG_256:
      return VOLTMETER_PAG_256_CAL_ADDR;
    default:
      return 0x00;
  };
}

uint16_t getCoverTime(voltmeterRate_t rate) {
  switch (rate) {
    case RATE_8:
      return 1000 / 8;
    case RATE_16:
      return 1000 / 16;
    case RATE_32:
      return 1000 / 32;
    case RATE_64:
      return 1000 / 64;
    case RATE_128:
      return 1000 / 128;
    case RATE_250:
      return 1000 / 250;
    case RATE_475:
      return 1000 / 475;
    case RATE_860:
      return 1000 / 860;
    default:
      return 1000 / 128;
  };
}

float getResolution(voltmeterGain_t gain) {
  switch (gain) {
    case PAG_6144:
      return ADS1115_MV_6144 / VOLTMETER_PRESSURE_COEFFICIENT;
    case PAG_4096:
      return ADS1115_MV_4096 / VOLTMETER_PRESSURE_COEFFICIENT;
    case PAG_2048:
      return ADS1115_MV_2048 / VOLTMETER_PRESSURE_COEFFICIENT;
    case PAG_1024:
      return ADS1115_MV_1024 / VOLTMETER_PRESSURE_COEFFICIENT;
    case PAG_512:
      return ADS1115_MV_512 / VOLTMETER_PRESSURE_COEFFICIENT;
    case PAG_256:
      return ADS1115_MV_256 / VOLTMETER_PRESSURE_COEFFICIENT;
    default:
      return ADS1115_MV_256 / VOLTMETER_PRESSURE_COEFFICIENT;
  };
}

HAL_StatusTypeDef readCalibrationFromEEPROM(voltmeter *dev, voltmeterGain_t gain, int16_t* hope, int16_t* actual) {
  uint8_t addr = getPGAEEEPROMAddr(gain);
  uint8_t buff[8];
  memset(buff, 0, 8);

  *hope = 1;
  *actual = 1;

  uint8_t status = EEPROMRead(dev, addr, buff, 8);

  if (status != HAL_OK) {
    return status;
  }

  uint8_t xor_result = 0x00;
  for (uint8_t i = 0; i < 5; i++) {
    xor_result ^= buff[i];
  }

  if (xor_result != buff[5]) {
    return HAL_ERROR;
  }

  *hope = (buff[1] << 8) | buff[2];
  *actual = (buff[3] << 8) | buff[4];
  return HAL_OK;
}
