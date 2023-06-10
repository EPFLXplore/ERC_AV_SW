#ifndef BMP280_DEFINES_H_
#define BMP280_DEFINES_H_

#define BMP280_CHIP_ID 0x58

#define BMP280_SPI_MASK_WRITE 0b01111111

#define BMP280_REG_ID         0xD0
#define BMP280_REG_RESET      0xE0
#define BMP280_REG_STATUS	  0xF3
#define BMP280_REG_CTRL_MEAS  0xF4
#define BMP280_REG_CONFIG     0xF5
#define BMP280_REG_PRESS      0xF7
#define BMP280_REG_TEMP       0xFA
#define BMP280_REG_CTRL       0xF4
#define BMP280_REG_CALIB      0x88

// For reading all data (pressure + temperature) at once
#define BMP280_REG_DATA       0xF7

#define BMP280_RESET_VALUE    0xB6

enum Oversampling
{
	oversampling_skipped = 0b000,
	oversampling_x1 = 0b001,
	oversampling_x2 = 0b010,
	oversampling_x4 = 0b011,
	oversampling_x8 = 0b100,
	oversampling_x16 = 0b101
};

enum PowerMode
{
	mode_sleep = 0b00, mode_forced = 0b01, mode_normal = 0b11
};

enum StandbyTime
{
	standby_time_500us = 0b000,
	standby_time_62500us = 0b001,
	standby_time_125ms = 0b010,
	standby_time_250ms = 0b011,
	standby_time_500ms = 0b100,
	standby_time_1000ms = 0b101,
	standby_time_2000ms = 0b110,
	standby_time_4000ms = 0b111
};

enum FilterSetting
{
	filter_off = 0b000,
	filter_coeff_2 = 0b001,
	filter_coeff_4 = 0b010,
	filter_coeff_8 = 0b011,
	filter_coeff_16 = 0b100
};

#endif /* BMP280_DEFINES_H_ */
