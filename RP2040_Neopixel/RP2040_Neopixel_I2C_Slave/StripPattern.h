/* ========================
*
* File: StripPattern.h
*
* Author: Vincent Nguyen
*
==========================*/

#ifndef __STRIP_PATTERN_H__
#define __STRIP_PATTERN_H__

#include <stdint.h>
#include <Adafruit_NeoPixel.h>

class StripPattern {
  public:

    StripPattern(Adafruit_NeoPixel* strip_, uint8_t index_, uint16_t num_leds_, float hue_start_, float hue_end_, 
                float speed, uint32_t blink_period_, uint16_t pattern_period_, uint32_t update_interval_ = 100);
    virtual ~StripPattern();

    uint8_t get_index() const;

    void set_index(uint8_t index_);

    void update();
    
  private:
    Adafruit_NeoPixel* strip;
    uint8_t index;
    uint16_t num_leds;
    float hue_start;
    float hue_end;
    float speed;
    uint32_t blink_period;
    uint16_t pattern_period;
    uint32_t update_interval;
    uint32_t time = 0;
    uint32_t last_time = 0;
};

#endif