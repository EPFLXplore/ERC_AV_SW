#ifndef HX711_H_
#define HX711_H_

#ifdef __cplusplus
  extern "C" {
#endif

#include "gpio.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>

#define		_HX711_USE_FREERTOS		1

void		HX711_set_pins(GPIO_TypeDef *sck_gpio, uint32_t sck_pin, GPIO_TypeDef *di_gpio, uint32_t di_pin);
void  		HX711_begin(void); //starts communication between stm32 and HX711
void        HX711_init(void); //configures pins
int32_t     HX711_value(void); //returns 2s complement of V(mesured)*Gain(of channel)
int32_t     HX711_valueAve(uint16_t sample); //returns 2s complement of average V(mesured)*Gain(of channel)
int			HX711_isReady(); //checks that HX711 is ready to return measurement

#ifdef __cplusplus
  }
#endif

#endif 
