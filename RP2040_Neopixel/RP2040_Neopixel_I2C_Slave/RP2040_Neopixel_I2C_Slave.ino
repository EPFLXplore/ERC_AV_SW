// /* ===================================
// *
// * File: RP2040_Neopixel_I2C_Slave.ino
// *
// * Author: Vincent Nguyen
// *
// ======================================*/

#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#include "LEDStrip.h"
#include "StripPattern.h"

// #define USE_GPIO // define if we want to use GPIO instead of I2C for receiving commands

#define I2C_SLAVE_ADDRESS (0x50)
#define LED_PIN (D6)
#define NUM_LEDS (20)

#define BAUD_RATE (9600)

typedef enum Command {
  OFF = 0x00,
  MANUAL = 0x01,
  AUTONOMOUS = 0x02,
  BLINK = 0x03
} Command;

// LEDStrip class not working for some reason, there are memory issues with pointers
// Instead we will be creating the patterns directly

static LEDStrip* strip = new LEDStrip(LED_PIN, NUM_LEDS);

static StripPattern pattern_manual(strip->get_strip(), 1, NUM_LEDS, 220, 300, 0.2f, 0, NUM_LEDS/1.5);
static StripPattern pattern_autonomous(strip->get_strip(), 2, NUM_LEDS, 300, 360, 0.2F, 5000, NUM_LEDS/1.5);
static StripPattern pattern_blink(strip->get_strip(), 3, NUM_LEDS, 220, 360, 1, 750, NUM_LEDS/2);

static Command cmd = OFF;

void setup() {
  // Setup serial communication
  Serial.begin(BAUD_RATE);

  // Start watchdog timer
  rp2040.wdt_begin(1000);

  // strip->add_pattern(pattern_manual);
  // strip->add_pattern(pattern_autonomous);
  // strip->add_pattern(pattern_blink);


  // Setup I2C device as slave
  Wire.begin(I2C_SLAVE_ADDRESS);
  pinMode(LED_BUILTIN, OUTPUT);

#ifdef USE_GPIO
  pinMode(D0, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
#else
  Wire.onReceive(receiveCommandEvent);
#endif
}

#ifndef USE_GPIO
void receiveCommandEvent(int byte_cnt) {
  if (byte_cnt >= 1) {
    Serial.println("index");
    Command cmd_temp = static_cast<Command>(Wire.read());
    Serial.println("Received command");
    Serial.println(cmd_temp);
    if (cmd_temp > 0x03) {
      Serial.println("Invalid command");
    } else {
      cmd = cmd_temp;
    }
  }
}
#endif

void loop() {
  rp2040.wdt_reset();

#ifdef USE_GPIO
  if (digitalRead(D0) == HIGH) {
    strip->disable(false);
  } else if (digitalRead(D1) == HIGH) {
    pattern_manual.update();
  } else if (digitalRead(D2) == HIGH) {
    pattern_autonomous.update();
  } else if (digitalRead(D3) == HIGH) {
    pattern_blink.update();
  } else {
    strip->disable(false);
  }
#else

  static Command cmd = OFF;
  static uint32_t time_led = 0;
  switch (cmd) {
    case OFF:
      strip->disable(false);
      if (millis() - time_led > 2000) {
        time_led = millis();
        cmd = MANUAL;
      }
      break;
    case MANUAL:
      Serial.println("manual");
      pattern_manual.update();
      if (millis() - time_led > 2000) {
        time_led = millis();
        cmd = AUTONOMOUS;
      }
      break;
    case AUTONOMOUS:
      Serial.println("auto");
      pattern_autonomous.update();
      if (millis() - time_led > 2000) {
        time_led = millis();
        cmd = BLINK;
      }
      break;
    case BLINK:
      Serial.println("blink");
      pattern_blink.update();
      if (millis() - time_led > 2000) {
        time_led = millis();
        cmd = OFF;
      }
      break;
  }
#endif

  static uint32_t time = 0;
  static bool state = false;
  if (millis() - time > 1000) {
    time = millis();
    if (state == false) {
    digitalWrite(LED_BUILTIN, HIGH);
    state = true;
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      state = false;
    }
  }
}

void set_led_hue(float hue) {
  for (uint16_t i = 0; i < NUM_LEDS; ++i) {
    strip->get_strip()->setPixelColor(i, strip->get_strip()->ColorHSV(65535*hue/360., 255, 100));
  }
}