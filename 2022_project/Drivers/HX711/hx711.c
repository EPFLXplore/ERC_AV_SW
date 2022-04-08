#include "hx711.h"
#if (_HX711_USE_FREERTOS == 1)
#include "cmsis_os.h"
#define HX711_delay(x)    osDelay(x)
#else
#define HX711_delay(x)    HAL_Delay(x)
#endif

#define	TIMEOUT 150 //cpu ticks
const uint32_t clockPulse = 200; //ns

//#############################################################################################
__STATIC_INLINE void HX711_delay_ns(uint64_t nanoseconds)
{
  uint64_t clk_cycle_start = DWT->CYCCNT;
  nanoseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
  while ((DWT->CYCCNT - clk_cycle_start) < nanoseconds/1000);
}

//#############################################################################################
void  HX711_begin(struct HX711 hx711)
{
  HX711_init(hx711);
  //initialise tick register
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  //start first reahx711.ding
  HAL_GPIO_WritePin(hx711.sck_gpio, hx711.sck_pin, GPIO_PIN_SET);
  HX711_delay(10);
  HAL_GPIO_WritePin(hx711.sck_gpio, hx711.sck_pin, GPIO_PIN_RESET);
  HX711_delay(10);
  HX711_valueAve(hx711, 8);
  HX711_delay(100);
}
//#############################################################################################
//setup HX711 pins
void  HX711_init(struct HX711 hx711)
{
  GPIO_InitTypeDef  gpio;
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio.Pin = hx711.sck_pin;
  HAL_GPIO_Init(hx711.sck_gpio, &gpio);
  gpio.Mode = GPIO_MODE_INPUT;
  gpio.Pull = GPIO_PULLUP;
  gpio.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio.Pin = hx711.di_pin;
  HAL_GPIO_Init(hx711.di_gpio, &gpio);

}

bool HX711_checkReadiness(struct HX711 hx711)
{
	uint32_t startTime = HAL_GetTick();

	while(!HX711_isReady(hx711)) {
		if(HAL_GetTick() - startTime > TIMEOUT) {
			return false;
		}
	}

	return true;
}
//#############################################################################################
int32_t HX711_value(struct HX711 hx711)
{
  uint32_t data = 0;
  uint32_t  startTime = HAL_GetTick();
  while(!HX711_isReady(hx711))
    {
      if(HAL_GetTick() - startTime > TIMEOUT)
        return 0;
    }
  for(int8_t i=0; i<24 ; i++)
  {
    HAL_GPIO_WritePin(hx711.sck_gpio, hx711.sck_pin, GPIO_PIN_SET);
    HX711_delay_ns(clockPulse);
    data = data << 1;    
    HAL_GPIO_WritePin(hx711.sck_gpio, hx711.sck_pin, GPIO_PIN_RESET);
    HX711_delay_ns(clockPulse);
    if(HAL_GPIO_ReadPin(hx711.di_gpio, hx711.di_pin) == GPIO_PIN_SET)
      data ++;
  }
  data = data ^ 0x800000; 
  HAL_GPIO_WritePin(hx711.sck_gpio, hx711.sck_pin, GPIO_PIN_SET);
  HX711_delay_ns(clockPulse);
  HAL_GPIO_WritePin(hx711.sck_gpio, hx711.sck_pin, GPIO_PIN_RESET);
  HX711_delay_ns(clockPulse);
  return data;    
}
//#############################################################################################
int32_t HX711_valueAve(struct HX711 hx711, uint16_t sample)
{
  int64_t  ave = 0;
  for(uint16_t i=0 ; i<sample ; i++)
    ave += HX711_value(hx711);

  return (int32_t)(ave / sample);
}
//#############################################################################################
int HX711_isReady(struct HX711 hx711) {
	return HAL_GPIO_ReadPin(hx711.di_gpio, hx711.di_pin) == GPIO_PIN_RESET;
}
