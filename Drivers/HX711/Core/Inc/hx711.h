#ifndef HX711_H_
#define HX711_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
//#include "main.h"

//####################################################################################################################

typedef struct
{
  GPIO_TypeDef  *clk_gpio;
  GPIO_TypeDef  *dat_gpio;
  uint16_t      clk_pin;
  uint16_t      dat_pin;
  int32_t       offset;
  float         coef;
} hx711_t;

//####################################################################################################################

void        hx711_init(hx711_t *hx711, GPIO_TypeDef *clk_gpio, uint16_t clk_pin, GPIO_TypeDef *dat_gpio, uint16_t dat_pin);
bool 		hx711_is_online(hx711_t *hx711);

void        hx711_coef_set(hx711_t *hx711, float coef);
void        hx711_tare(hx711_t *hx711, uint16_t sample);
int32_t hx711_value(hx711_t *hx711);
float       hx711_weight_ave(hx711_t *hx711, uint16_t sample);
float       hx711_weight(hx711_t *hx711);
void        hx711_power_down(hx711_t *hx711);
void        hx711_power_up(hx711_t *hx711);
bool        hx711_calibration(hx711_t *hx711);

//####################################################################################################################

#ifdef __cplusplus
}
#endif

#endif 
