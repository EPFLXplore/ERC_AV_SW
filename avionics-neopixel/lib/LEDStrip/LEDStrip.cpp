
#include "LEDStrip.hpp"

LEDStrip::LEDStrip(int pin, int numLeds)
    : _pin(pin), _numLeds(numLeds), _strip(numLeds, pin, NEO_GRB + NEO_KHZ800) {}

void LEDStrip::begin() {
    _strip.begin();
    _strip.setBrightness(50);
    _strip.show();
    for (uint8_t i = 0; i < MAX_SYSTEMS; ++i) {
        _cmds[i].mode = 0;   // default off
        _cmds[i].segment.low  = i * 33;
        _cmds[i].segment.high = (i + 1) * 33 - 1;
        _cmds[i].segment.r = _cmds[i].segment.g = _cmds[i].segment.b = 0;
        _states[i] = {};  // zero initialize
    }
}

// convert percentage (0‑100) to pixel index
int LEDStrip::pctToIdx(uint8_t pct) const { return (pct * _numLeds) / 100; }

void LEDStrip::applyCommand(const Command& cmd) {
    if (cmd.system >= MAX_SYSTEMS) return;                 // ignore invalid
    _cmds[cmd.system]   = cmd;                             // overwrite
    _states[cmd.system] = {};                              // reset state
}

void LEDStrip::tick() {
    for (uint8_t i = 0; i < MAX_SYSTEMS; ++i) handleMode(i);
}

void LEDStrip::setAll(int start, int end, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = start; i <= end; ++i) _strip.setPixelColor(i, _strip.Color(r, g, b));
    _strip.show();
}

void LEDStrip::handleMode(uint8_t idx) {
    const Command& cmd = _cmds[idx];
    int start = pctToIdx(cmd.segment.low);
    int end   = pctToIdx(cmd.segment.high);    
    switch (cmd.mode) {
        case 0: mode0(idx, start, end); break; // OFF
        case 1: mode1(idx, start, end, cmd.segment.r, cmd.segment.g, cmd.segment.b); break; // ON
        case 2: mode2(idx, start, end, cmd.segment.r, cmd.segment.g, cmd.segment.b); break; // BLINK
        case 3: mode3(idx, start, end, cmd.segment.r, cmd.segment.g, cmd.segment.b); break; // FAULT
        case 4: mode4(idx, cmd.segment.r, cmd.segment.g, cmd.segment.b); break; // EMERGENCY_MOTORS
        case 5: mode5(idx, cmd.segment.r, cmd.segment.g, cmd.segment.b); break; // EMERGENCY_GLOBAL
        case 6: mode6(idx); break; // ALL_OFF
        default: break;
    }

}

/******************  MODE IMPLEMENTATIONS  ******************/
void LEDStrip::mode0(uint8_t idx, int s, int e) {
    if (!_states[idx].initialized) {
        setAll(s, e, 0, 0, 255);
        _states[idx].initialized = true;
    }
}

void LEDStrip::mode1(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b) {
    if (!_states[idx].initialized) {
        setAll(s, e, r, g, b);
        _states[idx].initialized = true;
    }
}

void LEDStrip::mode2(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b,
                     uint8_t eye, uint16_t speed, uint16_t pause) {
    ModeState& st = _states[idx];
    if (millis() - st.lastUpdate < speed) return;

    // Calculate current head position
    int head = s + st.step;
    // Clear previous frame
    setAll(s, e, 0, 0, 0);

    // Draw eye
    _strip.setPixelColor(head, _strip.Color(r / 10, g / 10, b / 10));
    for (uint8_t j = 1; j <= eye; ++j)
        _strip.setPixelColor(head + j, _strip.Color(r, g, b));
    _strip.setPixelColor(head + eye + 1, _strip.Color(r / 10, g / 10, b / 10));
    _strip.show();

    st.lastUpdate = millis();
    st.step += st.phase == 0 ? 1 : -1;

    // Bounce at ends
    if (head >= e - eye - 2) { st.phase = 1; st.lastUpdate += pause; }
    else if (head <= s)      { st.phase = 0; st.lastUpdate += pause; }
}

void LEDStrip::mode3(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b, uint16_t speed) {
    ModeState& st = _states[idx];
    if (millis() - st.lastUpdate < speed) return;

    for (int i = s + st.phase; i < e; i += 3)
        _strip.setPixelColor(i, _strip.Color(r, g, b));
    _strip.show();

    // clear old
    for (int i = s + st.phase; i < e; i += 3)
        _strip.setPixelColor(i, _strip.Color(0, 0, 0));

    st.phase = (st.phase + 1) % 3;
    st.lastUpdate = millis();
}

void LEDStrip::mode4(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    if (!_states[idx].initialized) {
        setAll(0, 100, r, g, b);
        _states[idx].initialized = true;
    }
}

void LEDStrip::mode5(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    if (!_states[idx].initialized) {
        setAll(0, 100, r, g, b);
        _states[idx].initialized = true;
    }
}

void LEDStrip::mode6(uint8_t idx) {
    if (!_states[idx].initialized) {
        setAll(0, 100, 0, 0, 255);
        _states[idx].initialized = true;
    }
}


// // Solid colour – set once.
// void LEDStrip::mode0(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b) {
//     if (!_states[idx].initialized) {
//         setAll(s, e, r, g, b);
//         _states[idx].initialized = true;
//     }
// }

// // Wipe (forward then colour)
// void LEDStrip::mode2(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b, uint16_t speed) {
//     ModeState& st = _states[idx];
//     if (millis() - st.lastUpdate < speed) return;

//     int pos = s + st.step;
//     if (st.phase == 0) {                    // clearing phase
//         _strip.setPixelColor(pos, _strip.Color(0, 0, 0));
//     } else {                                // colouring phase
//         _strip.setPixelColor(pos, _strip.Color(r, g, b));
//     }
//     _strip.show();
//     st.step++;
//     st.lastUpdate = millis();

//     // phase switching
//     if (pos >= e) {
//         st.step  = 0;
//         st.phase = st.phase == 0 ? 1 : 0;
//     }
// }

// // Breathing fade
// void LEDStrip::mode3(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b) {
    
//     ModeState& st = _states[idx];
//     uint16_t interval = 20;
//     if (millis() - st.lastUpdate < interval) return;
//     float intensity = st.phase == 0 ? (255 - st.step) / 255.0f : st.step / 255.0f;
//     uint8_t rr = r * intensity;
//     uint8_t gg = g * intensity;
//     uint8_t bb = b * intensity;
//     setAll(s, e, rr, gg, bb);
//     st.step += 2;
//     if (st.step >= 255) { st.step = 0; st.phase ^= 1; }
//     st.lastUpdate = millis();
// }

// // Chase lights
// void LEDStrip::mode4(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b, uint16_t speed) {
//     ModeState& st = _states[idx];
//     if (millis() - st.lastUpdate < speed) return;

//     for (int i = s + st.phase; i < e; i += 3)
//         _strip.setPixelColor(i, _strip.Color(r, g, b));
//     _strip.show();

//     // clear old
//     for (int i = s + st.phase; i < e; i += 3)
//         _strip.setPixelColor(i, _strip.Color(0, 0, 0));

//     st.phase = (st.phase + 1) % 3;
//     st.lastUpdate = millis();
// }

// // Off (blue indicator)
// void LEDStrip::mode5(uint8_t idx, int s, int e) {
//     if (!_states[idx].initialized) {
//         setAll(s, e, 0, 0, 255);
//         _states[idx].initialized = true;
//     }
// }

// // Strobe
// void LEDStrip::mode6(uint8_t idx, int s, int e, uint8_t r, uint8_t g, uint8_t b,
//                      uint8_t strobe, uint16_t flash, uint16_t endPause) {
//     ModeState& st = _states[idx];
//     uint16_t delayTime = st.phase == 0 ? flash : flash;
//     if (millis() - st.lastUpdate < delayTime) return;

//     if (st.phase == 0) {  // ON
//         setAll(s, e, r, g, b);
//         st.step++;
//         st.phase = 1;
//     } else {              // OFF
//         setAll(s, e, 0, 0, 0);
//         st.phase = 0;
//     }

//     // After N strobes, pause then repeat
//     if (st.step >= strobe * 2) {             // each strobe = 2 phases
//         st.step = 0;
//         st.lastUpdate = millis() + endPause; // long pause
//     } else {
//         st.lastUpdate = millis();
//     }
// }
