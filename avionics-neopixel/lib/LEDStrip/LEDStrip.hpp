
#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

struct Segment {
    uint8_t low;   // percentage 0‑100
    uint8_t high;  // percentage 0‑100
    uint8_t r, g, b;
};

struct Command {
    Segment segment;
    uint8_t system;  // 0‑2
    uint8_t mode;    // 0‑6
};

struct ModeState {
    unsigned long lastUpdate = 0;
    int step   = 0;
    int phase  = 0;
    bool initialized = false;
};

#define MAX_SYSTEMS 4

class LEDStrip {
public:
    LEDStrip(int pin, int numLeds);
    void begin();
    void applyCommand(const Command& cmd);   // queue‑safe “set and forget”
    void tick();                             // call every loop – non‑blocking

private:
    int  _pin;
    int  _numLeds;
    Adafruit_NeoPixel _strip;

    bool _processcmd = false;  // true if command received

    Command   _cmds[MAX_SYSTEMS];
    ModeState _states[MAX_SYSTEMS];

    // helpers
    int pctToIdx(uint8_t pct) const;
    void setAll(int start, int end, uint8_t r, uint8_t g, uint8_t b);
    void handleMode(uint8_t idx);

    // pattern engines – all non‑blocking
    void mode0(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b);
    void mode1(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b,
               uint8_t eye = 4, uint16_t speed = 30, uint16_t pause = 500);
    void mode2(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b, uint16_t speed = 20);
    void mode3(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b);
    void mode4(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b, uint16_t speed = 50);
    void mode5(uint8_t idx, int s, int e);
    void mode6(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b,
               uint8_t strobe = 10, uint16_t flash = 50, uint16_t endPause = 1000);
};
