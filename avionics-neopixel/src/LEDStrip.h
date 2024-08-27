/* ========================
*
* File: LEDStrip.h
*
* Author: Eliot Abramo
*
==========================*/

#ifndef __LED_STRIP_H__
#define __LED_STRIP_H__

#include <Adafruit_NeoPixel.h>
#include <vector>

class LEDStrip {
  public:
    LEDStrip(uint8_t led_pin_, uint16_t num_leds_);
    ~LEDStrip();

    void begin();
    void show();
    void setBrigtness();
    void setPixelColor(uint16_t n, uint32_t color);
    uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
    
    void disable(bool gradual = true);
    Adafruit_NeoPixel* get_strip();
    void setAll(int start, int end, byte red, byte green, byte blue);

    // On, static
    void mode0(int start, int end, byte red, byte green, byte blue);

    // Manual
    /** Recommended settings:
     * Eyesize = 4, SpeedDelay = 10, ReturnDelay = 50
     */
    void mode1(int start, int end, byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);

    // Manual Direct
    /** Recommended settings:
     * SpeedDelay = 20
     * 
     * Important note: has to be called twice. Once put pattern (color wanted), once remove (0,0,0)
     */
    void mode2(int start, int end, byte red, byte green, byte blue, int SpeedDelay);

    // Manual Inverse
    void mode3(int start, int end, byte red, byte green, byte blue);

    // Auto
    /** Recommended settings:
     * SpeedDelay = 50
     */
    void mode4(int start, int end, byte red, byte green, byte blue, int SpeedDelay);

    // Off
    void mode5(int start, int end);

  private:
    uint8_t led_pin;
    uint16_t num_leds;

    Adafruit_NeoPixel strip;
    // std::vector<StripPattern*> pattern_list;
    // StripPattern* get_pattern(uint8_t index);
};

#endif // __LED_STRIP_H__