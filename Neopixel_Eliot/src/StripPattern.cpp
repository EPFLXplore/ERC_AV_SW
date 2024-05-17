/* ========================
*
* File: StripPattern.cpp
*
* Author: Vincent Nguyen
*
* This class defines a non-blocking 
* way to create and update 
* addressable LED patterns
*
==========================*/

#include "StripPattern.h"


StripPattern::StripPattern(Adafruit_NeoPixel* strip_, uint8_t index_, uint16_t num_leds_, float hue_start_, float hue_end_, 
                          float speed_, uint32_t blink_period_, uint16_t pattern_period_, uint32_t update_interval_) :
  strip(strip_), index(index_), num_leds(num_leds_), hue_start(hue_start_), hue_end(hue_end_), 
  speed(speed_), blink_period(blink_period_), pattern_period(pattern_period_), update_interval(update_interval_) {
    if (pattern_period > num_leds)
      pattern_period = num_leds;
  }

StripPattern::~StripPattern() {}

uint8_t StripPattern::get_index() const {
  return this->index;
}

void StripPattern::set_index(uint8_t index_) {
  this->index = index_;
}

void StripPattern::update() {
  static float direction = speed;
  static float distance = 0;
  static uint8_t brightness = 255;

  if (blink_period != 0)
    brightness = ((float)abs((float)(time % blink_period) - (float)(blink_period/2)) / (float)(blink_period/2))*255;
  else
    brightness = 255;

  if (millis() - time > update_interval) {
    time = millis();
    for (uint16_t i = 0; i < num_leds; ++i) {
      float j = max(0.0f, 1 - abs((distance/num_leds - (float)(i % pattern_period)/(float)pattern_period)));
      // float j = max(0, 1 - (float)abs((distance - (float)i)/num_leds));
      uint16_t hue = static_cast<uint16_t>(65535.*(hue_start + j * (hue_end - hue_start))/360.);
      strip->setPixelColor(i, strip->ColorHSV(hue, 255, brightness));
      strip->show();
    }

    distance += direction;

    if (distance > num_leds)
      direction = -speed;
    else if (distance < 0)
      direction = speed;
  }
}
