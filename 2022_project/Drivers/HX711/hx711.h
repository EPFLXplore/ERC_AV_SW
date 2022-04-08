#ifndef HX711_H_
#define HX711_H_

#ifdef __cplusplus
  extern "C" {
#endif

#include "gpio.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define		_HX711_USE_FREERTOS		1

struct HX711{
	//Clock pin
	GPIO_TypeDef* sck_gpio;
	uint32_t sck_pin;

	//SDA pin
	GPIO_TypeDef* di_gpio;
	uint32_t di_pin;
};

void  		HX711_begin(struct HX711 hx711); //starts communication between stm32 and HX711
void        HX711_init(struct HX711 hx711); //configures pins
int32_t     HX711_value(struct HX711 hx711); //returns 2s complement of V(mesured)*Gain(of channel)
int32_t     HX711_valueAve(struct HX711 hx711, uint16_t sample); //returns 2s complement of average V(mesured)*Gain(of channel)
int			HX711_isReady(struct HX711 hx711); //checks that HX711 is ready to return measurement
bool 		HX711_checkReadiness(struct HX711 hx711);

#ifdef __cplusplus
  }
#endif

#endif 
