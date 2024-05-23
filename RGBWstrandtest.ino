#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN     4
#define VALUE_GREEN 15

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  20

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Array of colors in RGB format
// int colors[][3] = {
//   {150, 18, 18},  
//   {144, 22, 22},
//   {138, 25, 25},
//   {133, 30, 30},
//   {124, 32, 32}
// };


int colors[][3] = {
  // {0, 0, 0},
  // {10, VALUE_GREEN, 0},
  // {20, 0, 0},
  // {30, VALUE_GREEN, 0},
  // {40, 0, 0},
  // {50, VALUE_GREEN, 0},
  // {60, 0, 0},
  {70, VALUE_GREEN, 0},
  {80, 0, 0},
  {110, VALUE_GREEN, 0},
  {120, 0, 0},
  {170, VALUE_GREEN, 0},
  {180, 0, 0},
  {230, VALUE_GREEN, 0},
  {240, 0, 0},
  {250, VALUE_GREEN, 0},
  {255, 0, 0}
};



const int colorCount = sizeof(colors) / sizeof(colors[0]);

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  Serial.println("Starting setup...");

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  Serial.println("Setup complete.");
}

void loop() {
  colorWave(20, 50); // Color wave with 20 pixels wave length, 50 ms delay
}

// Function to create a wave pattern with the specified color array
void colorWave(int waveLength, int delayTime) {
  static int wavePosition = 0;

  for (int i = 0; i < strip.numPixels(); i++) {
    int wave = (i + wavePosition) % waveLength;
    uint32_t color = getColorFromArray(wave, waveLength);
    strip.setPixelColor(i, color);

    // Debugging output
    Serial.print("Pixel ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(color, HEX);
  }

  strip.show();
  wavePosition++;
  delay(delayTime);
}

// Function to get the color from the array at a specific position
uint32_t getColorFromArray(int position, int waveLength) {
  int colorIndex = (position * colorCount) / waveLength;
  return strip.Color(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
}
