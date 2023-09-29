/* ========================
*
* File: LEDStrip.h
*
* Author: Vincent Nguyen
*
==========================*/

#ifndef __LED_STRIP_H__
#define __LED_STRIP_H__

#include <Adafruit_NeoPixel.h>
#include "StripPattern.h"
#include <vector>

class LEDStrip {
  public:
    LEDStrip(uint8_t led_pin_, uint16_t num_leds_);
    ~LEDStrip();
    void disable(bool gradual = true);
    void add_pattern(StripPattern* pattern);
    bool update_pattern(uint8_t index);

    Adafruit_NeoPixel* get_strip();

  private:
    uint8_t led_pin;
    uint16_t num_leds;

    Adafruit_NeoPixel strip;

    std::vector<StripPattern*> pattern_list;

    StripPattern* get_pattern(uint8_t index);
};

#endif // __LED_STRIP_H__