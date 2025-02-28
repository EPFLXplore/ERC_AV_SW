#ifndef ADS1234_INC_ADS1234_HPP_
#define ADS1234_INC_ADS1234_HPP_


//======================== which one? ========================
#include <stdint.h>
#include "stm32h7xx_hal.h"
#include <spi.h>
//============================================================

enum ERROR_t {
	NoERROR,
	TIMEOUT_HIGH,     // Timeout waiting for HIGH
	TIMEOUT_LOW,      // Timeout waiting for LOW
	WOULD_BLOCK,      // weight not measured, measuring takes too long
	STABLE_TIMEOUT,   // weight not stable within timeout
	DIVIDED_by_ZERO
};

enum Gain{
	GAIN1 = 1,
	GAIN2,
	GAIN64,
	GAIN128
};

enum Speed{
	SLOW = 0,
	FAST = 1
};

enum Channel{
	AIN1 = 1,
	AIN2 = 2,
	AIN3 = 3,
	AIN4 = 4
};

// struct IOPin
// {
//     GPIO_TypeDef *port;
//     uint16_t pin;
// };

class ADS1234
{
	public:
		ADS1234(uint8_t PIN_SCLK, uint8_t PIN_PDWN, uint8_t PIN_SPEED, 
				uint8_t PIN_GAIN0, uint8_t PIN_GAIN1, uint8_t PIN_A0, 
				uint8_t PIN_A1, uint8_t PIN_DOUT);

		~ADS1234();

        // Initialize library
        void begin(Gain = GAIN128, Speed = SLOW);


		// check if chip is ready
		// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
		// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
		bool is_ready();

		// set the gain factor; takes effect only after a call to read()
		void setGain(Gain gain);

		void setSpeed(Speed speed);

		void setChannel(Channel channel);

		// waits for the chip to be ready and returns a reading
		ERROR_t read(Channel channel, long& value, bool Calibrating = false);

		// returns an average reading; times = how many times to read
		ERROR_t read_average(Channel channel, float& value, uint16_t times, bool Calibrating);

		// returns a filtered reading (low pass filter)
		ERROR_t read_filtered(Channel channel, float& value, float alpha = 0.5, bool Calibrating = false);

		// returns last filtered reading (without offset nor scaling)
		float get_last_filtered_raw(Channel channel);

		// returns last filtered reading (with offset)
		float get_last_filtered_tared(Channel channel);

		// returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
		ERROR_t get_value(Channel channel, float& value, uint16_t times = 10, bool Calibrating = false);

		// returns (read_filtered() - OFFSET), that is the current value without the tare weight; alpha = low pass filter coefficient
		ERROR_t get_value(Channel channel, float& value, float alpha = 0.5, bool Calibrating = false);

		// returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
		// times = how many readings to do
		ERROR_t get_units(Channel channel, float& value, uint16_t times = 10, bool Calibrating = false);

		// returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
		// alpha = low pass filter coefficient
		ERROR_t get_units(Channel channel, float& value, float alpha = 0.5, bool Calibrating = false);

		// set the OFFSET value for tare weight; times = how many times to read the tare value
		ERROR_t tare(Channel channel, float alpha = 0.5, bool Calibrating = false);

		// set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
		void set_scale(Channel channel, float scale = 1.f);

		// get the current SCALE
		float get_scale(Channel channel);

		// set OFFSET, the value that's subtracted from the actual reading (tare weight)
		void set_offset(Channel channel, float offset = 0);

		// get the current OFFSET
		float get_offset(Channel channel);

		// puts the chip into power down mode
		void power_down();

		// wakes up the chip after power down mode
		void power_up();

	private:

		SPI_HandleTypeDef* hspi;
		uint8_t PIN_DOUT;
		uint8_t PIN_SCLK;
		uint8_t PIN_PDWN;
		uint8_t PIN_SPEED;
		uint8_t PIN_GAIN0;
		uint8_t PIN_GAIN1;
		uint8_t PIN_A0;
		uint8_t PIN_A1;

		float OFFSET[4] = {0, 0, 0, 0};	// used for tare weight
		float SCALE[4] = {1,1,1,1};	// used to return weight in grams, kg, ounces, whatever

		float last_filtered_raw[4] = {0, 0, 0, 0};

		Speed _speed ;
		Channel lastChannel = AIN1;
		float prev_value[4] = {0, 0, 0, 0};
};


#endif /* ADS1234_INC_ADS1234_HPP_ */
