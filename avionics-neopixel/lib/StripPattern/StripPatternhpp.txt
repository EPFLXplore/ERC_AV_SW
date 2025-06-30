/* ========================
*
* File: StripPattern.h
*
* Author: Eliot Abramo
*
==========================*/

#ifndef __STRIP_PATTERN_H__
#define __STRIP_PATTERN_H__

#include <stdint.h>
#include <Adafruit_NeoPixel.h>

class StripPattern {
  public:
    StripPattern(Adafruit_NeoPixel* strip_);
    StripPattern(Adafruit_NeoPixel* strip_, uint8_t index_, uint16_t num_leds_, 
                float speed, uint32_t blink_period_, uint16_t pattern_period_, uint32_t update_interval_ = 100);
    virtual ~StripPattern();

    void setAll(int start, int end, byte red, byte green, byte blue);

    // Manual
    /** Recommended settings:
     * Eyesize = 4, SpeedDelay = 10, ReturnDelay = 50
     */
    void mode0(int start, int end, byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);

    // Manual Direct
    /** Recommended settings:
     * SpeedDelay = 20
     * 
     * Important note: has to be called twice. Once put pattern (color wanted), once remove (0,0,0)
     */
    void mode1(int start, int end, byte red, byte green, byte blue, int SpeedDelay);

    // Manual Inverse
    void mode2(int start, int end, byte red, byte green, byte blue);

    // Auto
    /** Recommended settings:
     * SpeedDelay = 50
     */
    void mode3(int start, int end, byte red, byte green, byte blue, int SpeedDelay);

    // Off
    void mode4(int start, int end);

    uint8_t get_index() const;
    void set_index(uint8_t index_);
    
  private:
    Adafruit_NeoPixel* strip;

    // useless
    uint8_t index;
    uint16_t num_leds;
    float speed;
    uint32_t blink_period;
    uint16_t pattern_period;
    uint32_t update_interval;
    uint32_t time = 0;
    uint32_t last_time = 0;
};

#endif