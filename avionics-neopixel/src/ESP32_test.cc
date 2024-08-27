#include <Wire.h>
#include <vector>
#include <Adafruit_NeoPixel.h>

#include "LEDStrip.h"
// #include "StripPattern.h"

/**
 * Message received from Orion: low, high, system, mode
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
#define NUM_LEDS (36)

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

/*
void set_command(Command* (&cmds)[3]) {
  cmds[0] = new Command{};
  cmds[0]->r = 127;
  cmds[0]->g = 0;
  cmds[0]->b = 255;
  cmds[0]->section = NAV;
  cmds[0]->blink = 10;

  cmds[1] = new Command{};
  cmds[1]->r = 153;
  cmds[1]->g = 0;
  cmds[1]->b = 0;
  cmds[1]->section = HD;
  cmds[1]->blink = 20;
  
  cmds[2] = new Command{};
  cmds[2]->r = 102;
  cmds[2]->g = 51;
  cmds[2]->b = 0;
  cmds[2]->section = DRILL;
  cmds[2]->blink = 30;
}
*/

/*
// for testing purposes, simulate receiving a command
void receive_command(Command* currentCommand){
  default_segments(strip)

  currentCommand->section = NAV;
  currentCommand->blink = 10;
  currentCommand->r = 127;
  currentCommand->g = 0;
  currentCommand->b = 255;

  // static int counter = 0;
  // counter++;

  // if (counter % 3 == 0) {

  // } else if (counter % 3 == 1) {
  
  // } else {
  // }
}
*/

void initialize_strip(Command* commands[3]){
        // Nav init
        commands[0] = new Command();
        commands[0]->system = 0;
        commands[0]->mode = 1;

        commands[0]->segment.low = 0;
        commands[0]->segment.high = 33;
        commands[0]->segment.r = 127;
        commands[0]->segment.g = 0;
        commands[0]->segment.b = 255;

        // HD init
        commands[1] = new Command();
        commands[1]->system = 1;
        commands[1]->mode = 2;

        commands[1]->segment.low = 34;
        commands[1]->segment.high = 66;
        commands[1]->segment.r = 153;
        commands[1]->segment.g = 0;
        commands[1]->segment.b = 0;

        // Drill init
        commands[2] = new Command();
        commands[2]->system = 2;
        commands[2]->mode = 3;

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

      default:
        break;
    }

    Serial.printf("Current: %d, %d, %d, %d, %d, %d \n",start,end,mode,r,g,b);
  }
}

void onReceiveHandler(int numBytes){
  Serial.printf("command received");
  while(Wire.available()){
    uint8_t low = Wire.read();
    uint8_t high = Wire.read();
    uint8_t system = Wire.read();
    uint8_t mode = Wire.read();

    switch (system) {
      case 0: // Nav
        commands[system]->mode = mode;
        commands[system]->segment.low = low;
        commands[system]->segment.high = high;
        break;
    
      case 1: // HD
        commands[system]->mode = mode;
        commands[system]->segment.low = low;
        commands[system]->segment.high = high;
        break;
    
      case 2: // Drill
        commands[system]->mode = mode;
        commands[system]->segment.low = low;
        commands[system]->segment.high = high;
        break;
    
      default:
        break;
    }

  }
}

void onRequestHandler(){

}

void setup() {
  // Setup serial communication
  Serial.begin(BAUD_RATE);
  initialize_strip(commands);
  // Setup I2C device as slave
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(onReceiveHandler);
  pinMode(LED_BUILTIN, OUTPUT);

  strip->begin();
}

void loop(){
  // default_segments(strip);
  // digitalWrite(LED_PIN, HIGH);
  execute_strip(commands,strip);
  // int segmentSize = NUM_LEDS / 3;
  // Serial.println("Board is working!");
  // strip->mode4(0,20,0xff, 0x77, 0x00,50);
  // strip->mode1(20,40,127,0,255,4,10,50);
  // strip->mode3(40,60,0xff, 0x77, 0x00);
  // strip->mode1(40,60,0x00,0x00,0x00,20);
  // strip->mode1(40,60,0x00,0xff,0x00,20);

  // if(commands[0] == nullptr || commands[1] == nullptr || commands[2] == nullptr) {
  //   set_command(commands);
  //   default_segments(strip, segmentSize);
  // }

  // // Update each section independently
  // for (int section = 0; section < 3; section++) {
  //   int start = section * segmentSize;
  //   int end = start + segmentSize;
  //   // updateSection(strip, commands, section, start, end);
  // }
  delay(100);
}




// // num of blinks = 1s / interval (set in command, received from Orion)
// void blink_LED(LEDStrip* strip, Command* currentCommand, int start, int end) {
//   int blinkPeriod = currentCommand->blink;
  
//   // Turn on LEDs
//   for (int i = start; i < end; i++) {
//     strip->setPixelColor(i, strip->Color(currentCommand->r, currentCommand->g, currentCommand->b));
//   }
//   strip->show();
//   delay(10000/blinkPeriod);

//   // Turn off LEDs
//   for (int i = start; i < end; i++) {
//     strip->setPixelColor(i, strip->Color(0, 0, 0));
//   }
//   strip->show();
//   delay(1000);
// }






// static StripPattern pattern_1(strip->get_strip(), 1, NUM_LEDS, 200, 350, 0.2f, 0, NUM_LEDS/1.2, 30);
// static StripPattern pattern_2(strip->get_strip(), 2, NUM_LEDS, 300, 360, 0.2F, 5000, NUM_LEDS/1.5);
// static StripPattern pattern_3(strip->get_strip(), 3, NUM_LEDS, 220, 360, 1, 750, NUM_LEDS/2);

    // pattern_1.update();

    // static uint32_t time = 0;
    // static bool state = false;
    // if (millis() - time > 1000) {
    //     time = millis();
    // }




/*
  int segmentSize = NUM_LEDS / 3;

  // First segment: Red
  for (int i = 0; i < segmentSize; i++) {
    strip->setPixelColor(i, strip->Color(255, 0, 0)); // Red
  }

  // Second segment: Green
  for (int i = segmentSize; i < 2 * segmentSize; i++) {
    strip->setPixelColor(i, strip->Color(0, 255, 0)); // Green
  }

  // Third segment: Blue
  for (int i = 2 * segmentSize; i < NUM_LEDS; i++) {
    strip->setPixelColor(i, strip->Color(0, 0, 255)); // Blue
  }

  strip->show(); // Update the strip with new colors

  delay(1000); // Wait for a second before updating again
*/