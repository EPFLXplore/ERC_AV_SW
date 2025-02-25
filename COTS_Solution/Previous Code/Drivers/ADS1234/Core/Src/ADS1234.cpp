/*
 * ADS1234.cpp
 *
 *  Created on: Apr 1, 2023
 *      Author: Yassine Bakkali, Vincent Nguyen
 */


#include <ADS1234.hpp>

#include <FreeRTOS.h>
#include <stdio.h>
#include <cmsis_os.h>
#include "semphr.h"

//static IOPin PIN_DOUT_static = {0, 0};

ADS1234::ADS1234(SPI_HandleTypeDef* hspi_, GPIO_TypeDef *SPI_DOUT_Port, uint16_t SPI_DOUT_Pin,
						 GPIO_TypeDef *SPI_SCLK_Port, uint16_t SPI_SCLK_Pin,
						 GPIO_TypeDef *PDWN_Port, uint16_t SPI_PDWN_Pin,
						 GPIO_TypeDef *SPEED_Port, uint16_t SPEED_Pin,
						 GPIO_TypeDef *GAIN0_Port, uint16_t GAIN0_Pin,
						 GPIO_TypeDef *GAIN1_Port, uint16_t GAIN1_Pin,
						 GPIO_TypeDef *A0_Port, uint16_t A0_Pin,
						 GPIO_TypeDef *A1_Port, uint16_t A1_Pin):
						 hspi{hspi_},
						 PIN_DOUT{SPI_DOUT_Port, SPI_DOUT_Pin},
						 PIN_SCLK{SPI_SCLK_Port, SPI_SCLK_Pin},
						 PIN_PDWN{PDWN_Port, SPI_PDWN_Pin},
						 PIN_SPEED{SPEED_Port, SPEED_Pin},
						 PIN_GAIN0{GAIN0_Port, GAIN0_Pin},
						 PIN_GAIN1{GAIN1_Port, GAIN1_Pin},
						 PIN_A0{A0_Port, A0_Pin},
						 PIN_A1{A1_Port, A1_Pin}
						 {
//							 PIN_DOUT_static = IOPin{SPI_DOUT_Port, SPI_DOUT_Pin};
						 }

ADS1234::~ADS1234() {
//  IRQn_Type IRQn;
//
//  switch(PIN_DOUT.pin) {
//  case GPIO_PIN_0:
//	  IRQn = EXTI0_IRQn;
//	  break;
//  case GPIO_PIN_1:
//	  IRQn = EXTI1_IRQn;
//	  break;
//  case GPIO_PIN_2:
//	  IRQn = EXTI2_IRQn;
//	  break;
//  case GPIO_PIN_3:
//	  IRQn = EXTI3_IRQn;
//	  break;
//  case GPIO_PIN_4:
//	  IRQn = EXTI4_IRQn;
//	  break;
//  case GPIO_PIN_5:
//  case GPIO_PIN_6:
//  case GPIO_PIN_7:
//  case GPIO_PIN_8:
//  case GPIO_PIN_9:
//	  IRQn = EXTI9_5_IRQn;
//	  break;
//  case GPIO_PIN_10:
//  case GPIO_PIN_11:
//  case GPIO_PIN_12:
//  case GPIO_PIN_13:
//  case GPIO_PIN_14:
//  case GPIO_PIN_15:
//	  IRQn = EXTI15_10_IRQn;
//	  break;
//  }
//  HAL_NVIC_DisableIRQ(IRQn);
}

void ADS1234::begin(Gain gain, Speed speed){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  HAL_SPI_DeInit(hspi); // replace hspiX with the handle of the SPI peripheral

  GPIO_InitStruct.Pin = PIN_SCLK.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_SCLK.port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PIN_PDWN.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_PDWN.port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PIN_SPEED.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_SPEED.port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PIN_GAIN0.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_GAIN0.port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PIN_GAIN1.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_GAIN1.port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PIN_A0.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_A0.port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PIN_A1.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_A1.port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PIN_DOUT.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PIN_DOUT.port, &GPIO_InitStruct);
//  GPIO_InitStruct.Pin = PIN_DOUT.pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(PIN_DOUT.port, &GPIO_InitStruct);

//  IRQn_Type IRQn;
//
//  switch(PIN_DOUT.pin) {
//  case GPIO_PIN_0:
//	  IRQn = EXTI0_IRQn;
//	  break;
//  case GPIO_PIN_1:
//	  IRQn = EXTI1_IRQn;
//	  break;
//  case GPIO_PIN_2:
//	  IRQn = EXTI2_IRQn;
//	  break;
//  case GPIO_PIN_3:
//	  IRQn = EXTI3_IRQn;
//	  break;
//  case GPIO_PIN_4:
//	  IRQn = EXTI4_IRQn;
//	  break;
//  case GPIO_PIN_5:
//  case GPIO_PIN_6:
//  case GPIO_PIN_7:
//  case GPIO_PIN_8:
//  case GPIO_PIN_9:
//	  IRQn = EXTI9_5_IRQn;
//	  break;
//  case GPIO_PIN_10:
//  case GPIO_PIN_11:
//  case GPIO_PIN_12:
//  case GPIO_PIN_13:
//  case GPIO_PIN_14:
//  case GPIO_PIN_15:
//	  IRQn = EXTI15_10_IRQn;
//	  break;
//  }
//
//  HAL_NVIC_SetPriority(IRQn, 5, 0);
//  HAL_NVIC_EnableIRQ(IRQn);

  setGain(gain);
  setSpeed(speed);
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
  HAL_GPIO_WritePin(PIN_PDWN.port, PIN_PDWN.pin, GPIO_PIN_RESET);
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

//static xSemaphoreHandle highSemaphore;
//static xSemaphoreHandle lowSemaphore;
//
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//
//    if ((GPIO_Pin == PIN_DOUT_static.pin) && (PIN_DOUT_static.pin != 0)) {
//        if (HAL_GPIO_ReadPin(PIN_DOUT_static.port, PIN_DOUT_static.pin) == GPIO_PIN_SET) {
//            xSemaphoreGiveFromISR(highSemaphore, &xHigherPriorityTaskWoken);
//        } else {
//            xSemaphoreGiveFromISR(lowSemaphore, &xHigherPriorityTaskWoken);
//        }
//
//        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//    }
//}

//void EXTI15_10_IRQHandler(void) {
//}
//
//void EXTI9_5_IRQHandler(void) {
//}
//
//void EXTI4_IRQHandler(void) {
//}
//
//void EXTI3_IRQHandler(void) {
//}
//
//void EXTI2_IRQHandler(void) {
//}
//
//void EXTI1_IRQHandler(void) {
//}
//
//void EXTI0_IRQHandler(void) {
//}


/*
 * Get the raw ADC value. Can block up to 100ms in normal operation.
 * Returns 0 on success, an error code otherwise.
 */
ERROR_t ADS1234::read(Channel channel, long& value, bool Calibrating)
{
    int i=0;
	unsigned int waitingTime;
	unsigned int SettlingTimeAfterChangeChannel=0;

	if(channel!=lastChannel){
		setChannel(channel);
		if(_speed==FAST) SettlingTimeAfterChangeChannel=55;
		else SettlingTimeAfterChangeChannel=405;
		lastChannel=channel;
	}

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

//    start=xTaskGetTickCount();
//    while(HAL_GPIO_ReadPin(PIN_DOUT.port, PIN_DOUT.pin) != 1)
//    {
//        if(xTaskGetTickCount()-start > waitingTime)
//            return TIMEOUT_HIGH; // Timeout waiting for HIGH
//    }
//
//    start=xTaskGetTickCount();
//    while(HAL_GPIO_ReadPin(PIN_DOUT.port, PIN_DOUT.pin) != 0)
//    {
//        if(xTaskGetTickCount()-start > waitingTime)
//            return TIMEOUT_LOW; // Timeout waiting for LOW
//    }
//
//	if (xSemaphoreTake(highSemaphore, pdMS_TO_TICKS(waitingTime)) != pdTRUE) {
//		return TIMEOUT_HIGH; // Timeout waiting for HIGH
//	}
//
//	if (xSemaphoreTake(lowSemaphore, pdMS_TO_TICKS(waitingTime)) != pdTRUE) {
//		return TIMEOUT_LOW; // Timeout waiting for LOW
//	}

//// VERY BAD SOLUTION BECAUSE OF LACK OF TIME PLEASE USE INTERRUPTS
	if (!Calibrating)
		osDelay(100);
	else
		osDelay(1300);

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

	osDelay(1);
	taskYIELD();

    return NoERROR; // Success
}

ERROR_t ADS1234::read_filtered(Channel channel, float& value, float alpha, bool Calibrating) {
	long val;
	ERROR_t err = read(channel, val, Calibrating);
	if (err != NoERROR) return err;

	value = prev_value[channel-1]*alpha + val*(1-alpha);
	last_filtered_raw[channel-1] = value;
	prev_value[channel-1] = value;
	return NoERROR;
}

float ADS1234::get_last_filtered_raw(Channel channel) {
	return last_filtered_raw[channel-1];
}

float ADS1234::get_last_filtered_tared(Channel channel) {
	return last_filtered_raw[channel-1] - OFFSET[channel-1];
}

ERROR_t ADS1234::read_average(Channel channel, float& value, uint16_t times, bool Calibrating) {

	long sum = 0;
	ERROR_t err;
	for (uint16_t i = 0; i < times; i++) {
		long val;
		err = read(channel, val, Calibrating);
		if(err!=NoERROR) return err;

		sum += val;
	}
	if(times==0) return DIVIDED_by_ZERO;
	value = (float)sum / times;
	return NoERROR;
}

ERROR_t ADS1234::get_value(Channel channel, float& value, uint16_t times, bool Calibrating) {
	float val = 0;
	ERROR_t err;
	err = read_average(channel, val, times, Calibrating);
	if(err!=NoERROR) return err;
	value = (val - OFFSET[channel-1]);
	return NoERROR;
}

ERROR_t ADS1234::get_value(Channel channel, float& value, float alpha, bool Calibrating) {
	float val = 0;
	ERROR_t err;
	err = read_filtered(channel, val, 0.8, Calibrating);
	if(err!=NoERROR) return err;
	value = (val - OFFSET[channel-1]);
	return NoERROR;
}

ERROR_t ADS1234::get_units(Channel channel, float& value, uint16_t times, bool Calibrating) {
	float val = 0;
	ERROR_t err;
	err = get_value(channel, val, times, Calibrating);
	if(err!=NoERROR) return err;
	if(SCALE[channel-1]==0) return DIVIDED_by_ZERO;
	value = val / SCALE[channel-1];
	return NoERROR;
}

ERROR_t ADS1234::get_units(Channel channel, float& value, float alpha, bool Calibrating) {
	float val = 0;
	ERROR_t err;
	err = get_value(channel, val, alpha, Calibrating);
	if(err!=NoERROR) return err;
	if(SCALE[channel-1]==0) return DIVIDED_by_ZERO;
	value = val / SCALE[channel-1];
	return NoERROR;
}

ERROR_t ADS1234::tare(Channel channel, float alpha, bool Calibrating) {
	ERROR_t err;
	float sum = 0;
	err = read_filtered(channel, sum, alpha, Calibrating);
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
	prev_value[channel-1] = offset;
}

float ADS1234::get_offset(Channel channel) {
	return OFFSET[channel-1];
}
