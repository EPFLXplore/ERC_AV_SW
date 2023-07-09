/*
 * ADS1234.cpp
 *
 *  Created on: Apr 1, 2023
 *      Author: Yassine Bakkali
 */


#include <ADS1234.hpp>

#include <stdio.h>
#include <cmsis_os.h>

ADS1234::ADS1234(SPI_HandleTypeDef* hspi_, GPIO_TypeDef *SPI_DOUT_Port, uint16_t SPI_DOUT_Pin,
						 GPIO_TypeDef *SPI_SCLK_Port, uint16_t SPI_SCLK_Pin,
						 GPIO_TypeDef *PWDN_Port, uint16_t SPI_PWDN_Pin,
						 GPIO_TypeDef *SPEED_Port, uint16_t SPEED_Pin,
						 GPIO_TypeDef *GAIN0_Port, uint16_t GAIN0_Pin,
						 GPIO_TypeDef *GAIN1_Port, uint16_t GAIN1_Pin,
						 GPIO_TypeDef *A0_Port, uint16_t A0_Pin,
						 GPIO_TypeDef *A1_Port, uint16_t A1_Pin):
						 hspi{hspi_},
						 PIN_DOUT{SPI_DOUT_Port, SPI_DOUT_Pin},
						 PIN_SCLK{SPI_SCLK_Port, SPI_SCLK_Pin},
						 PIN_PDWN{PWDN_Port, SPI_PWDN_Pin},
						 PIN_SPEED{SPEED_Port, SPEED_Pin},
						 PIN_GAIN0{GAIN0_Port, GAIN0_Pin},
						 PIN_GAIN1{GAIN1_Port, GAIN1_Pin},
						 PIN_A0{A0_Port, A0_Pin},
						 PIN_A1{A1_Port, A1_Pin}
						 {}

ADS1234::~ADS1234() {
}
//
//void EXTI9_5_IRQHandler(void)   // <----- The ISR Function We're Looking For!
//{
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
//
//}
//
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    if(GPIO_Pin == GPIO_PIN_6) // If The INT Source Is EXTI Line9 (A9 Pin)
//    {
//    	GPIO_InitTypeDef GPIO_InitStruct;
//    	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
//    	GPIO_InitStruct.Pin = GPIO_PIN_6; // replace X with the GPIO pin number
//    	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    	GPIO_InitStruct.Pull = GPIO_NOPULL;
//    	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1; // replace GPIO_AFx_SPIx with the alternate function for SPIx
//    	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//    }
//}

void ADS1234::begin(Gain gain, Speed speed){
//  _pin_DOUT = pin_DOUT;
//  _pin_SCLK = pin_SCLK;
//  _pin_PDWN = pin_PDWN;
//  _pin_GAIN0 = pin_GAIN0;
//  _pin_GAIN1 = pin_GAIN1;
//  _pin_SPEED = pin_SPEED;
//  _pin_A0 = pin_A0;
//  _pin_A1_or_TEMP = pin_A1_or_TEMP;


//  pinMode(_pin_DOUT,  INPUT_PULLUP);
//  pinMode(_pin_SCLK, OUTPUT);
//  pinMode(_pin_PDWN, OUTPUT);
//  pinMode(_pin_GAIN0, OUTPUT);
//  pinMode(_pin_GAIN1, OUTPUT);
//  pinMode(_pin_SPEED, OUTPUT);
//  pinMode(_pin_A0, OUTPUT);
//  pinMode(_pin_A1_or_TEMP, OUTPUT);
//  HAL_SPI_DeInit(&hspi1); // replace hspiX with the handle of the SPI peripheral
//  HAL_SPI_DeInit(&hspi1); // replace hspiX with the handle of the SPI peripheral
//
//  this->GPIO_InitStruct.Pin = GPIO_PIN_6; // replace X with the GPIO pin number
//  this->GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  this->GPIO_InitStruct.Pull = GPIO_PULLUP; // replace with GPIO_PULLDOWN if needed
//  HAL_GPIO_Init(GPIOA, &this->GPIO_InitStruct);

//  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  //HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // replace GPIO_PIN_X with the GPIO pin number
  //__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6); // replace GPIO_PIN_X with the GPIO pin number
  //HAL_GPIO_EXTI_Callback(GPIO_PIN_6); // replace GPIO_PIN_X with the GPIO pin number
  //HAL_GPIO_RegisterCallback(GPIOA, GPIO_PIN_6, EXTI_Callback_Function); // replace GPIOX with the GPIO port name, GPIO_PIN_X with the GPIO pin number, and EXTI_Callback_Function with the name of your callback function

//
  setGain(gain);
  setSpeed(speed);
  setChannel(AIN3);
//  power_down();
//  osDelay(10);
  power_up();

}

bool ADS1234::is_ready(void)
{
  return HAL_GPIO_ReadPin(PIN_DOUT.port, PIN_DOUT.pin) == 1;
}

void ADS1234::setGain(Gain gain)
{
  switch(gain)
  {
    case GAIN1:
    {
      HAL_GPIO_WritePin(PIN_GAIN1.port, PIN_GAIN1.pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PIN_GAIN0.port, PIN_GAIN0.pin, GPIO_PIN_RESET);
      break;
    }
    case GAIN2:
    {
      HAL_GPIO_WritePin(PIN_GAIN1.port, PIN_GAIN1.pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PIN_GAIN0.port, PIN_GAIN0.pin, GPIO_PIN_SET);
      break;
    }
    case GAIN64:
    {
      HAL_GPIO_WritePin(PIN_GAIN1.port, PIN_GAIN1.pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(PIN_GAIN0.port, PIN_GAIN0.pin, GPIO_PIN_RESET);
      break;
    }
    case GAIN128:
    {
      HAL_GPIO_WritePin(PIN_GAIN1.port, PIN_GAIN1.pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(PIN_GAIN0.port, PIN_GAIN0.pin, GPIO_PIN_SET);
      break;
    }
  }
}

void ADS1234::power_up(void)
{
  HAL_GPIO_WritePin(PIN_PDWN.port, PIN_PDWN.pin, GPIO_PIN_SET);

  // Set CLK low to get the ADS1231 out of suspend
  HAL_GPIO_WritePin(PIN_SCLK.port, PIN_SCLK.pin, GPIO_PIN_RESET);
}

void ADS1234::power_down(void)
{
  HAL_GPIO_WritePin(PIN_PDWN.port, PIN_PDWN.pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PIN_SCLK.port, PIN_SCLK.pin, GPIO_PIN_SET);
}


void ADS1234::setSpeed(Speed speed)
{
  _speed = speed;
  switch(speed)
  {
    case SLOW:
    {
      HAL_GPIO_WritePin(PIN_SPEED.port, PIN_SPEED.pin, GPIO_PIN_RESET);
      break;
    }
    case FAST:
    {
      HAL_GPIO_WritePin(PIN_SPEED.port, PIN_SPEED.pin, GPIO_PIN_SET);
      break;
    }
  }
}

void ADS1234::setChannel(Channel channel)
{
  switch(channel)
  {
    case AIN1:
    {
      HAL_GPIO_WritePin(PIN_A1.port, PIN_A1.pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PIN_A0.port, PIN_A0.pin, GPIO_PIN_RESET);
      break;
    }
    case AIN2:
    {
      HAL_GPIO_WritePin(PIN_A1.port, PIN_A1.pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PIN_A0.port, PIN_A0.pin, GPIO_PIN_SET);
      break;
    }
	case AIN3:
    {
      HAL_GPIO_WritePin(PIN_A1.port, PIN_A1.pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(PIN_A0.port, PIN_A0.pin, GPIO_PIN_RESET);
      break;
    }
	case AIN4:
    {
      HAL_GPIO_WritePin(PIN_A1.port, PIN_A1.pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(PIN_A0.port, PIN_A0.pin, GPIO_PIN_SET);
      break;
    }
  }
}


/*
 * Get the raw ADC value. Can block up to 100ms in normal operation.
 * Returns 0 on success, an error code otherwise.
 */
ERROR_t ADS1234::read(Channel channel,long& value, bool Calibrating)
{
    int i=0;
    unsigned long start;
	unsigned int waitingTime;
	unsigned int SettlingTimeAfterChangeChannel=0;

//	if(channel!=lastChannel){
//		setChannel(channel);
//
//		if(_speed==FAST) SettlingTimeAfterChangeChannel=55;
//		else SettlingTimeAfterChangeChannel=405;
//		lastChannel=channel;
//	}

    /* A high to low transition on the data pin means that the ADS1231
     * has finished a measurement (see datasheet page 13).
     * This can take up to 100ms (the ADS1231 runs at 10 samples per
     * second!).
     * Note that just testing for the state of the pin is unsafe.
     */

	if(Calibrating){
		if(_speed==FAST) waitingTime=150;
		else waitingTime=850;
	}
	else{
		if(_speed==FAST) waitingTime=20;
		else waitingTime=150;
	}
	waitingTime+=SettlingTimeAfterChangeChannel;

	waitingTime+=600; //[ms] Add some extra time ( sometimes takes longer than what datasheet claims! )

	// see this to replace millis
	//https://www.freertos.org/FreeRTOS_Support_Forum_Archive/September_2018/freertos_Measure_Time_within_a_Task_with_xTaskGetTickCount_6d2307a0j.html

    start=xTaskGetTickCount();
    while(HAL_GPIO_ReadPin(PIN_DOUT.port, PIN_DOUT.pin) != 1)
    {
        if(xTaskGetTickCount()-start > waitingTime)
            return TIMEOUT_HIGH; // Timeout waiting for HIGH
    }

    start=xTaskGetTickCount();
    while(HAL_GPIO_ReadPin(PIN_DOUT.port, PIN_DOUT.pin) != 0)
    {
        if(xTaskGetTickCount()-start > waitingTime)
            return TIMEOUT_LOW; // Timeout waiting for LOW
    }

    // Read 24 bits
    uint8_t buf[3] = {0, 0, 0};


    //for(i=23 ; i >= 0; i--) {
    //    digitalWrite(_pin_SCLK, HIGH);
    //    value = (value << 1) + digitalRead(_pin_DOUT);
    //    digitalWrite(_pin_SCLK, LOW);
    //}


    // Read 24 bits
    for(i=23 ; i >= 0; i--) {
        HAL_GPIO_WritePin(PIN_SCLK.port, PIN_SCLK.pin, GPIO_PIN_SET);
        value = (value << 1) + HAL_GPIO_ReadPin(PIN_DOUT.port, PIN_DOUT.pin);
        HAL_GPIO_WritePin(PIN_SCLK.port, PIN_SCLK.pin, GPIO_PIN_RESET);
    }


	if(Calibrating){
	// 2 extra bits for calibrating
		for(i=1 ; i >= 0; i--) {
			HAL_GPIO_WritePin(PIN_SCLK.port, PIN_SCLK.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PIN_SCLK.port, PIN_SCLK.pin, GPIO_PIN_RESET);
		}
	}

    value = (value << 8) / 256;

	if(!Calibrating){
		/* The data pin now is high or low depending on the last bit that
		 * was read.
		 * To get it to the default state (high) we toggle the clock one
		 * more time (see datasheet).
		 */
		HAL_GPIO_WritePin(PIN_SCLK.port, PIN_SCLK.pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(PIN_SCLK.port, PIN_SCLK.pin, GPIO_PIN_RESET);
	}
    return NoERROR; // Success
}

ERROR_t ADS1234::read_average(Channel channel, float& value, uint8_t times, bool Calibrating) {

	long sum = 0;
	ERROR_t err;
	for (uint8_t i = 0; i < times; i++) {
		long val;
		err = read(channel, val, Calibrating);
		if(err!=NoERROR) return err;

		sum += val;
		//yield();

	}
	if(times==0) return DIVIDED_by_ZERO;
	value = (float)sum / times;
	return NoERROR;
}

ERROR_t ADS1234::get_value(Channel channel, float& value, uint8_t times, bool Calibrating) {
	float val = 0;
	ERROR_t err;
	err = read_average(channel, val, times, Calibrating);
	if(err!=NoERROR) return err;
	value = val - OFFSET[channel-1];
	return NoERROR;
}

ERROR_t ADS1234::get_units(Channel channel, float& value, uint8_t times, bool Calibrating) {
	float val = 0;
	ERROR_t err;
	err = get_value(channel, val, times, Calibrating);
	if(err!=NoERROR) return err;
	if(SCALE[channel-1]==0) return DIVIDED_by_ZERO;
	value = val / SCALE[channel-1];
	return NoERROR;
}

ERROR_t ADS1234::tare(Channel channel, uint8_t times, bool Calibrating) {
	ERROR_t err;
	float sum = 0;
	err = read_average(channel, sum, times, Calibrating);
	if(err!=NoERROR) return err;
	set_offset(channel, sum);
	return NoERROR;
}

void ADS1234::set_scale(Channel channel, float scale) {
	SCALE[channel-1] = scale;
}

float ADS1234::get_scale(Channel channel) {
	return SCALE[channel-1];
}

void ADS1234::set_offset(Channel channel, float offset) {
	OFFSET[channel-1] = offset;
}

float ADS1234::get_offset(Channel channel) {
	return OFFSET[channel-1];
}
