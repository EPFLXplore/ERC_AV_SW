/* ========================
*
* File: LEDStrip.cpp
*
* Author: Vincent Nguyen
*
==========================*/

#include "LEDStrip.h"

LEDStrip::LEDStrip(uint8_t led_pin_, uint16_t num_leds_) : 
  led_pin(led_pin_), num_leds(num_leds_) {
    strip = Adafruit_NeoPixel(num_leds, led_pin, NEO_GRB + NEO_KHZ800);
    strip.begin();
  };

LEDStrip::~LEDStrip() {
  for (auto pattern : pattern_list)
    pattern = nullptr;

  pattern_list.clear();
};

// Turn off all LEDs
void LEDStrip::disable(bool gradual) {
  static uint32_t time = 0;
  static uint32_t off_interval = 50;
  static uint16_t i = 0;
  if (gradual) {
    if (millis() - time > off_interval) {
      time = millis();
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
      i++;
    }
  } else {
    strip.fill(strip.Color(0, 0, 0));
    strip.show();
  }
}

void LEDStrip::add_pattern(StripPattern* pattern) {
  pattern_list.push_back(pattern);
}

StripPattern* LEDStrip::get_pattern(uint8_t index) {
  // for (auto pattern : pattern_list) {
  //   Serial.println("index");
  //   Serial.println(pattern->get_index());
  //   Serial.println(index);
  //   if (pattern->get_index() == index)
  //     return pattern;
  // }
  // Serial.println("size");
  // Serial.println(pattern_list.size());

  for (uint8_t i = 0; i < pattern_list.size(); ++i) {
    Serial.println("index");
    Serial.println(pattern_list[i]->get_index());
    Serial.println(index);
    
    if (pattern_list[i]->get_index() == index) {
        return pattern_list[i];
    }
  }
  return nullptr;
}

bool LEDStrip::update_pattern(uint8_t index) {
  StripPattern* pattern = get_pattern(index);
  if (pattern != nullptr) {
    pattern->update();
    return true;
  }
  return false;
}

Adafruit_NeoPixel* LEDStrip::get_strip() {
  return &strip;
}

