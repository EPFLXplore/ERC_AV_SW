#ifndef HX711_H_
#define HX711_H_

/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
  Instagram:  http://instagram.com/github.NimaLTD
  Youtube:    https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw
  
  Version:    1.0.0
  
  
  Reversion History:
  
  (1.0.0)
  First Release.
*/

#ifdef __cplusplus
  extern "C" {
#endif

#include "gpio.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>

#define		_HX711_USE_FREERTOS		1

void		HX711_set_pins(GPIO_TypeDef *sck_gpio, uint32_t sck_pin, GPIO_TypeDef *di_gpio, uint32_t di_pin);
void        HX711_init(void);
int32_t     HX711_value(void);
int32_t     HX711_valueAve(uint16_t sample); //returns 2s complement of V(mesured)*Gain(of channel)
int			HX711_isReady();

#ifdef __cplusplus
  }
#endif

#endif 
