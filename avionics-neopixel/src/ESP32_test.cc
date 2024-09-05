#include <Wire.h>
#include <vector>
#include <Adafruit_NeoPixel.h>

#include "LEDStrip.h"
// #include "StripPattern.h"

/**
 * Message received from Rasberry: low, high, system, mode
 * Functioning: Each system has a color (blue, red, orange) and when
 *              there is a change in the mode a subsequent pattern will
 *              be displayed. Different pattern <-> different mode.
 * 
 * nav(0),hd(1),drill(2) - uint8_t
 * System: nav(0,33), hd(33,66), drill(66,100)
 * Modes: Manual(0), Manual Direct(1), Manual Inverse(2), Auto(3), Off(4)
 */

// define if we want to use GPIO instead of I2C for receiving commands

#define LED_PIN (27)
#define NUM_LEDS (96)

#define I2C_SLAVE_ADDRESS (0x50)
#define BAUD_RATE (9600)

struct Segment{
  uint8_t low;
  uint8_t high;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Command {
  Segment segment;
  uint8_t system;
  uint8_t mode;
};

static LEDStrip* strip = new LEDStrip(LED_PIN, NUM_LEDS);
static Command* commands[3] = {nullptr, nullptr, nullptr};

// Set default segments colors
void default_segments(LEDStrip* strip) {
  int segmentSize = NUM_LEDS/3;
  // NAV segment: Violet
  for (int i = 0; i < segmentSize; i++)
    strip->setPixelColor(i, strip->Color(127, 0, 255));
  // HD segment: Red
  for (int i = segmentSize; i < 2 * segmentSize; i++)
    strip->setPixelColor(i, strip->Color(153, 0, 0));
  // DRILL segment: Brown
  for (int i = 2 * segmentSize; i < NUM_LEDS; i++)
    strip->setPixelColor(i, strip->Color(102, 51, 0));

  strip->show();
  delay(500);
}

void initialize_strip(Command* commands[3]){
        // Nav init
        commands[0] = new Command();
        commands[0]->system = 0;
        commands[0]->mode = 5;

        commands[0]->segment.low = 0;
        commands[0]->segment.high = 33;
        commands[0]->segment.r = 127;
        commands[0]->segment.g = 0;
        commands[0]->segment.b = 255;

        // HD init
        commands[1] = new Command();
        commands[1]->system = 1;
        commands[1]->mode = 5;

        commands[1]->segment.low = 34;
        commands[1]->segment.high = 66;
        commands[1]->segment.r = 153;
        commands[1]->segment.g = 0;
        commands[1]->segment.b = 0;

        // Drill init
        commands[2] = new Command();
        commands[2]->system = 2;
        commands[2]->mode = 5;

        commands[2]->segment.low = 67;
        commands[2]->segment.high = 100;
        commands[2]->segment.r = 102;
        commands[2]->segment.g = 51;
        commands[2]->segment.b = 0;

}

void execute_strip(Command* commands[3], LEDStrip* strip){
  for (size_t i = 0; i < 3; i++) {
    Command* current_command = commands[i];

    uint8_t mode = current_command->mode;
    int start = (current_command->segment.low * NUM_LEDS) / 100;
    int end = (current_command->segment.high * NUM_LEDS) / 100;
    
    uint8_t r = current_command->segment.r;
    uint8_t g = current_command->segment.g;
    uint8_t b = current_command->segment.b;

    switch (mode) {
      // On
      case 0:
        strip->mode0(start,end,r,g,b);
        break;

      // Manual
      case 1:
        strip->mode1(start,end,r,g,b,4,10,50);
        break;

      // Manual Direct
      case 2:
        strip->mode2(start,end,0,0,0,20);
        strip->mode2(start,end,r,g,b,20);
        break;
      
      // Manual Inverse
      case 3:
        strip->mode3(start,end,r,g,b);
        break;
      
      // Auto
      case 4:
        strip->mode4(start,end,r,g,b,50);
        break;

      // Off
      case 5:
        strip->mode5(start,end);
        break;

      case 6:
        strip->mode6(start,end,r,g,b,10,50,1000);

      default:
        break;
    }
    // Serial.printf("Current: %d, %d, %d, %d, %d, %d \n",start,end,mode,r,g,b);
  }
}

void SerialHandler(Command* commands[3], LEDStrip* strip) {
  // Check if data is available
  if (Serial.available() > 0) {
    // Read the incoming data into a buffer
    char buffer[64];
    int len = Serial.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
    buffer[len] = '\0'; // Null-terminate the string

    // Parse the command from the buffer
    int low, high, system, mode;
    int parsed = sscanf(buffer, "%d %d %d %d", &low, &high, &system, &mode);

    // Check if the parsing was successful
    if (parsed >= 4) {
      // Update the global command variables
      commands[system]->segment.low = low;
      commands[system]->segment.high = high;
      commands[system]->mode = mode;

      // Debug print to verify the parsed values
      // Serial.printf("Parsed values - Start: %d, End: %d, Mode: %d, R: %d, G: %d, B: %d\n", low, high, system, mode);
    } else {
      Serial.println("Failed to parse command");
    }
  }
}

void setup() {
  Serial.begin(BAUD_RATE);
  initialize_strip(commands);

  pinMode(LED_BUILTIN, OUTPUT);
  strip->begin();
}

void loop(){
  // default_segments(strip);
  SerialHandler(commands,strip);
  execute_strip(commands,strip);
  delay(100);
}
