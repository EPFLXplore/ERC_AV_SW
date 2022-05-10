#include "hx711.h"

#if (_HX711_USE_FREERTOS == 1)
#include "cmsis_os.h"

/*============================================================================*/
/* Redfine local functions													  */
/*============================================================================*/

#define hx711_delay(x)    osDelay(x)
#else
#define hx711_delay(x)    HAL_Delay(x)
#endif

/*============================================================================*/
/* Constants														          */
/*============================================================================*/

#define TIMEOUT         150 	// ticks

#define CALIB_TIMEOUT   30*1000 // ms

#define CLOCK_PULSE     200 	// Pulses width in ns to send to HX711

#define DEF_N_SAMPLES   20 		// Default number of samples for averaging data

#define CALIB_WEIGHT    250.f 	// Calibration weight in grams

#define CALIB_THRESH    5000   // Calibration threshold detection (raw)

/*============================================================================*/
/* Local functions														      */
/*============================================================================*/

// =============================================================================

/**
 * @brief  		Returns 1 if data is ready for retrieval.
 * @param[in]	hx711		hx711 structure.
 * @return					1 if data is ready for retrieval, 0 otherwise.
 */
static int8_t hx711_is_ready(hx711_t *hx711)
{
	return (HAL_GPIO_ReadPin(hx711->dat_gpio, hx711->dat_pin) == GPIO_PIN_RESET);
}

/**
 * @brief  		Delay function using Cycle Counter.
 * @param[in]	nanoseconds		Delay in ns.
 */
__STATIC_INLINE void hx711_delay_ns(uint64_t nanoseconds)
{
	uint64_t clk_cycle_start = DWT->CYCCNT;
	nanoseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
	while ((DWT->CYCCNT - clk_cycle_start) < nanoseconds/1000);
}

// =============================================================================

/**
 * @brief  		Returns raw value from HX711.
 * @param[in]	hx711		hx711 structure.
 * @return		data		Raw value from sensor.
 */
int32_t hx711_value(hx711_t *hx711)
{

	uint32_t data = 0;

	uint32_t  startTime = HAL_GetTick();

	while(!hx711_is_ready(hx711))
	{
		if(HAL_GetTick() - startTime > TIMEOUT)
			return 0;
	}

	// Shift out 24 bits
	for(int8_t i = 0; i < 24 ; i++)
	{
		HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);
		hx711_delay_ns(CLOCK_PULSE);
		data = data << 1;
		HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
		hx711_delay_ns(CLOCK_PULSE);
		if(HAL_GPIO_ReadPin(hx711->dat_gpio, hx711->dat_pin) == GPIO_PIN_SET)
			data ++;
	}

	data = data ^ 0x800000;

	HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);
	hx711_delay_ns(CLOCK_PULSE);
	HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
	hx711_delay_ns(CLOCK_PULSE);

	return data;
}

// =============================================================================

/**
 * @brief  		Returns raw averaged values on n_samples number of samples
 * @param[in]	hx711		hx711 structure.
 * @return		ave			Average over n_samples samples.
 */
static int32_t hx711_value_ave(hx711_t *hx711, uint16_t n_sample)
{
	int64_t  ave = 0;
	for(uint16_t i = 0 ; i < n_sample ; i++)
		ave += hx711_value(hx711);

	return (int32_t)(ave / n_sample);
}

/*============================================================================*/
/* Exported functions														  */
/*============================================================================*/

/**
 * @brief  		Initializes the HX711
 * @param[in]	hx711		hx711 structure.
 * @param[in]	clk_gpio	GPIO port for clock line.
 * @param[in]	clk_pin		Pin number for clock line.
 * @param[in]	dat_gpio	GPIO port for data line.
 * @param[in]	dat_pin		Pin number for data line.
 */
void hx711_init(hx711_t *hx711,
				GPIO_TypeDef *clk_gpio, uint16_t clk_pin,
				GPIO_TypeDef *dat_gpio, uint16_t dat_pin)
{
	// Enable DWT Cycle Counter
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	// Initialize counter
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	// Initialize HX711 pins
	hx711->clk_gpio = clk_gpio;
	hx711->clk_pin = clk_pin;
	hx711->dat_gpio = dat_gpio;
	hx711->dat_pin = dat_pin;

	// Initialize GPIO pins (clock and data lines)
	GPIO_InitTypeDef  gpio = {0};

	// Clock line
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Pin = clk_pin;
	HAL_GPIO_Init(clk_gpio, &gpio);

	// Data line
	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Pin = dat_pin;
	HAL_GPIO_Init(dat_gpio, &gpio);

	HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);
	hx711_delay(10);
	HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
	hx711_delay(10);
	hx711_value_ave(hx711, 2);
	hx711_delay(100);
}

// =============================================================================

/**
 * @brief  		Returns 1 if data is ready for retrieval.
 * @param[in]	hx711		hx711 structure.
 * @return					1 if data is ready for retrieval, 0 otherwise.
 */
bool hx711_is_online(hx711_t *hx711)
{
	uint32_t  startTime = HAL_GetTick();

	while(!hx711_is_ready(hx711))
	{
		if(HAL_GetTick() - startTime > TIMEOUT)
			return false;
	}
	return true;
}

// =============================================================================

/**
 * @brief  		Adjust offset to adjust output value to 0
 * @param[in]	hx711		hx711 structure.
 * @param[in]	n_sample	Number of samples to average over.
 */
void hx711_tare(hx711_t *hx711, uint16_t n_sample)
{
	int32_t ave = hx711_value_ave(hx711, n_sample);
	hx711->offset = ave;
}

// =============================================================================

/**
 * @brief  		Returns average weight over n_samples in grams.
 * @param[in]	hx711		hx711 structure.
 * @param[in]	n_sample	Number of samples to average over.
 */
float hx711_weight_ave(hx711_t *hx711, uint16_t n_sample)
{
	int32_t raw_ave = hx711_value_ave(hx711, n_sample);
	return (raw_ave - hx711->offset) / hx711->coef;
}

// =============================================================================

/**
 * @brief  		Returns weight in grams.
 * @param[in]	hx711		hx711 structure.
 */
float hx711_weight(hx711_t *hx711)
{
	int32_t raw_val = hx711_value(hx711);
	return ((float)raw_val - hx711->offset) / hx711->coef;
}

// =============================================================================

/**
 * @brief  		Sets HX711 weight coefficient.
 * @param[in]	hx711		hx711 structure.
 * @param[in]	coef		Weight coefficient.
 */
void hx711_coef_set(hx711_t *hx711, float coef)
{
	hx711->coef = coef;
}

// =============================================================================

/**
 * @brief  		Power down HX711.
 * @param[in]	hx711		hx711 structure.
 */
void hx711_power_down(hx711_t *hx711)
{
	HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);
	hx711_delay(1);
}

// =============================================================================

/**
 * @brief  		Power up HX711.
 * @param[in]	hx711		hx711 structure.
 */
void hx711_power_up(hx711_t *hx711)
{
	HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
}

// =============================================================================

/**
 * @brief  		Calibrates weight coefficient for a given weight.
 * 				We assume that the scale is empty beforehand.
 * @param[in]	hx711		hx711 structure.
 * @param[in]	hx711		hx711 structure.
 */
bool hx711_calibration(hx711_t *hx711)
{
//	hx711_delay(200);
	hx711_tare(hx711, DEF_N_SAMPLES);

	uint32_t startTime = HAL_GetTick();

	while((HAL_GetTick() - startTime) < CALIB_TIMEOUT) {

		int32_t current_val = hx711_value_ave(hx711, DEF_N_SAMPLES);
		int32_t diff = current_val - hx711->offset;

		if(diff > CALIB_THRESH){
			hx711_delay(200); // Give the weight some time to settle
			hx711->coef = (hx711_value_ave(hx711, DEF_N_SAMPLES) - hx711->offset)/CALIB_WEIGHT;
			return true;
		}
	}
	return false;
}

