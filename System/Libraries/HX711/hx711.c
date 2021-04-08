#include "hx711.h"
#if (_HX711_USE_FREERTOS == 1)
#include "cmsis_os.h"
#define HX711_delay(x)    osDelay(x)
#else
#define HX711_delay(x)    HAL_Delay(x)
#endif

const uint32_t clockPulse = 50; //ns

//Clock pin
GPIO_TypeDef *_hx711_sck_gpio;
uint32_t _hx711_sck_pin;

//SDA pin
GPIO_TypeDef *_hx711_di_gpio;
uint32_t _hx711_di_pin;

//#############################################################################################
__STATIC_INLINE void HX711_delay_ns(uint64_t nanoseconds)
{
  uint64_t clk_cycle_start = DWT->CYCCNT;
  nanoseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
  while ((DWT->CYCCNT - clk_cycle_start) < nanoseconds/1000);
}
void  HX711_set_pins(GPIO_TypeDef *sck_gpio, uint32_t sck_pin, GPIO_TypeDef *di_gpio, uint32_t di_pin){
	_hx711_sck_gpio = sck_gpio;
	_hx711_sck_pin = sck_pin;
	_hx711_di_gpio = di_gpio;
	_hx711_di_pin = di_pin;
}
//#############################################################################################
void  HX711_begin(void)
{
  HX711_init();
  //initialise tick register
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  //start first reading
  HAL_GPIO_WritePin(_hx711_sck_gpio, _hx711_sck_pin, GPIO_PIN_SET);
  HX711_delay(10);
  HAL_GPIO_WritePin(_hx711_sck_gpio, _hx711_sck_pin, GPIO_PIN_RESET);
  HX711_delay(10);
  HX711_valueAve(8);
}
//#############################################################################################
//setup HX711 pins
void  HX711_init(void)
{
  GPIO_InitTypeDef  gpio;
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_PULLUP;
  gpio.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio.Pin = _hx711_sck_pin;
  HAL_GPIO_Init(_hx711_sck_gpio, &gpio);
  gpio.Mode = GPIO_MODE_INPUT;
  gpio.Pull = GPIO_PULLUP;
  gpio.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio.Pin = _hx711_di_pin;
  HAL_GPIO_Init(_hx711_di_gpio, &gpio);

}
//#############################################################################################
int32_t HX711_value(void)
{
  uint32_t data = 0;
  uint32_t  startTime = HAL_GetTick();
  while(!HX711_isReady())
  {
    if(HAL_GetTick() - startTime > 150)
      return 0;
  }
  for(int8_t i=0; i<24 ; i++)
  {
    HAL_GPIO_WritePin(_hx711_sck_gpio, _hx711_sck_pin, GPIO_PIN_SET);
    HX711_delay_ns(clockPulse);
    data = data << 1;    
    HAL_GPIO_WritePin(_hx711_sck_gpio, _hx711_sck_pin, GPIO_PIN_RESET);
    HX711_delay_ns(clockPulse);
    if(HAL_GPIO_ReadPin(_hx711_di_gpio, _hx711_di_pin) == GPIO_PIN_SET)
      data ++;
  }
  data = data ^ 0x800000; 
  HAL_GPIO_WritePin(_hx711_sck_gpio, _hx711_sck_pin, GPIO_PIN_SET);
  HX711_delay_ns(clockPulse);
  HAL_GPIO_WritePin(_hx711_sck_gpio, _hx711_sck_pin, GPIO_PIN_RESET);
  HX711_delay_ns(clockPulse);
  return data;    
}
//#############################################################################################
int32_t HX711_valueAve(uint16_t sample)
{
  int64_t  ave = 0;
  for(uint16_t i=0 ; i<sample ; i++)
    ave += HX711_value();
  return (int32_t)(ave / sample);
}
//#############################################################################################
int HX711_isReady() {
	return HAL_GPIO_ReadPin(_hx711_di_gpio, _hx711_di_pin) == GPIO_PIN_RESET;
}
