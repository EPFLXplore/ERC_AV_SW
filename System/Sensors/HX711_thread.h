/*
 * HX711_thread.h
 *
 *  Created on: 13 Aug 2022
 *      Author: AV Team 2022
 */

#ifndef SENSORS_HX711_THREAD_H_
#define SENSORS_HX711_THREAD_H_

#include "Thread.h"
#include "DataStructures.h"
#include "Prober.h"

#include "hx711.h"

class HX711Thread : public Thread {
public:
	HX711Thread(ProberThread* parent, GPIO_TypeDef *sck_gpioa, uint32_t sck_pina, GPIO_TypeDef *di_gpioa, uint32_t di_pina,
	           GPIO_TypeDef *sck_gpiob, uint32_t sck_pinb, GPIO_TypeDef *di_gpiob, uint32_t di_pinb);
	void init();
	void loop();
	void calibrateMultiplier(void);
//	void tare(int32_t zero);
	void tare(void);
private:
	ProberThread* parent;
	uint8_t portNum;
	uint16_t _nSamples;
	float _multiplier;
	int32_t _zero;
	struct HX711 _hx711a;
	struct HX711 _hx711b;
};

extern HX711Thread* hx711Instance;

#endif /* SENSORS_HX711_H_ */
