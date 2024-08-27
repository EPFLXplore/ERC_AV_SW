/* ========================
*
* File: StripPattern.cpp
*
* Author: Eliot Abramo
*
* This class defines a non-blocking 
* way to create and update 
* addressable LED patterns
*
==========================*/

#include "StripPattern.h"


StripPattern::StripPattern(Adafruit_NeoPixel* strip_) :
  strip(strip_) {
  }

StripPattern::StripPattern(Adafruit_NeoPixel* strip_, uint8_t index_, uint16_t num_leds_,
                          float speed_, uint32_t blink_period_, uint16_t pattern_period_, uint32_t update_interval_) :
  strip(strip_), index(index_), num_leds(num_leds_),
  speed(speed_), blink_period(blink_period_), pattern_period(pattern_period_), update_interval(update_interval_) {
    if (pattern_period > num_leds)
      pattern_period = num_leds;
  }

StripPattern::~StripPattern() {}

void StripPattern::setAll(int start, int end, byte red, byte green, byte blue) {
  for(int i = start; i < end; i++ ) {
    strip->setPixelColor(i, strip->Color(red, green, blue));
  }
  strip->show();
}

void StripPattern::mode0(int start, int end, byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = start; i < end-EyeSize-2; i++) {
    setAll(start, end, 0,0,0);
    strip->setPixelColor(i, strip->Color(red/10, green/10, blue/10));

    for(int j = 1; j <= EyeSize; j++) {
      strip->setPixelColor(i+j, strip->Color(red, green, blue));
    }
    strip->setPixelColor(i+EyeSize+1, strip->Color(red/10, green/10, blue/10));
    strip->show();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = end-EyeSize-2; i > start; i--) {
    setAll(start, end, 0,0,0);
    strip->setPixelColor(i, strip->Color(red/10, green/10, blue/10));

    for(int j = 1; j <= EyeSize; j++) {
      strip->setPixelColor(i+j, strip->Color(red, green, blue));
    }
    strip->setPixelColor(i+EyeSize+1, strip->Color(red/10, green/10, blue/10));
    strip->show();
    delay(SpeedDelay);
  }
  
  delay(ReturnDelay);
}

void StripPattern::mode1(int start, int end, byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=start; i<end; i++) {
      strip->setPixelColor(i, strip->Color(red, green, blue));
      strip->show();
      delay(SpeedDelay);
  }

}

void StripPattern::mode2(int start, int end, byte red, byte green, byte blue){
  float r, g, b;
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(start,end,r,g,b);
    strip->show();
  }

  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(start,end,r,g,b);
    strip->show();
  }

}

void StripPattern::mode3(int start, int end, byte red, byte green, byte blue, int SpeedDelay) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=start; i < end; i=i+2) {
        strip->setPixelColor(i+q, strip->Color(red, green, blue));
      }
      strip->show();
     
      delay(SpeedDelay);
     
      for (int i=start; i < end; i=i+2) {
        strip->setPixelColor(i+q, strip->Color(0,0,0));
      }
    }
  }
  setAll(start,end,red,green,blue);
}

void StripPattern::mode4(int start, int end){
  setAll(start, end, 0x00,0x00,0x00);
}

uint8_t StripPattern::get_index() const {
  return this->index;
}

void StripPattern::set_index(uint8_t index_) {
  this->index = index_;
}

