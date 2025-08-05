
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "LEDStrip.hpp"

/**
 * Left:
 * System 0 = NAV
 * System 1 = HD
 * 
 * Right:
 * System 2 = DRILL
 * System 3 = Avionics
 * 
 * LOOK AT CUSTOM MSG FOR MORE INFO (Wrote everything there for sanity checks with CS)
 * 
 */

constexpr int LED_PIN_LEFT   = 26;
constexpr int LED_PIN_RIGHT   = 13;
constexpr int NUM_LEDS  = 34;
constexpr uint32_t BAUD = 115200;
constexpr size_t  QUEUE_DEPTH = 16;

LEDStrip strip_left(LED_PIN_LEFT, NUM_LEDS);
LEDStrip strip_right(LED_PIN_RIGHT, NUM_LEDS);
QueueHandle_t cmdQueue;

void serialHandler() {
    if (!Serial.available()) return;
    char buf[64];
    int len = Serial.readBytesUntil('\n', buf, sizeof(buf) - 1);
    buf[len] = '\0';
    
    int low, high, system, mode;

    if (sscanf(buf, "%d %d %d %d", &low, &high, &system, &mode) == 4) {
        Command cmd;
        cmd.system        = constrain(system, 0, 3);
        cmd.mode          = constrain(mode,   0, 6);

        switch (cmd.system) {
            case 0: cmd.segment.r = 147; cmd.segment.g = 0;   cmd.segment.b = 211; cmd.segment.low = 0, cmd.segment.high= 50; break; // NAV - Pink
            case 1: cmd.segment.r = 255; cmd.segment.g = 140; cmd.segment.b = 0;   cmd.segment.low = 51, cmd.segment.high=100; break; // HD - Yellow
            case 2: cmd.segment.r = 0;   cmd.segment.g = 255; cmd.segment.b = 0;   cmd.segment.low = 0, cmd.segment.high=50; break; // DRILL - Green
            case 3: cmd.segment.r = 20; cmd.segment.g = 56; cmd.segment.b = 50; cmd.segment.low = 51, cmd.segment.high=100; break; // Avionics - Turquoise
        }

        xQueueSend(cmdQueue, &cmd, 0);
    } else {
        while (Serial.available() && Serial.read() != '\n');
    }
}

void commandTask(void* pv) {
    strip_left.begin();
    strip_right.begin();
    Command cmd;
    TickType_t delayTicks = 1 / portTICK_PERIOD_MS;
    for (;;) {
        
        while (xQueueReceive(cmdQueue, &cmd, 0) == pdTRUE) {
            //emergency motors
            if(cmd.mode == 4){
                cmd.segment.r = 255; cmd.segment.g = 165; cmd.segment.b = 0; cmd.segment.low = 0, cmd.segment.high= 100;
                strip_left.applyCommand(cmd);
                strip_right.applyCommand(cmd);
            } 

            //emergency shutdown
            if(cmd.mode == 5){
                cmd.segment.r = 255; cmd.segment.g = 0;   cmd.segment.b = 0; cmd.segment.low = 0, cmd.segment.high= 100;
                strip_left.applyCommand(cmd);
                strip_right.applyCommand(cmd);
            }

            //all off
            if(cmd.mode == 6){
                strip_left.applyCommand(cmd);
                strip_right.applyCommand(cmd);
            }

            if(cmd.system == 0 || cmd.system == 1)
                strip_left.applyCommand(cmd);
            else
                strip_right.applyCommand(cmd);
        }
        strip_left.tick();           // advance animations
        strip_right.tick();          // advance animations
        vTaskDelay(delayTicks);      // yield
    }
}

void setup() {
    Serial.begin(BAUD);
    cmdQueue = xQueueCreate(QUEUE_DEPTH, sizeof(Command));
    xTaskCreatePinnedToCore(commandTask, "LED", 4096, nullptr, 1, nullptr, 1);
    
    // Small delay to ensure task starts
    delay(10);
    Serial.println(F("READY"));
}

void loop() {
    serialHandler();
    vTaskDelay(1);   // allow FreeRTOS scheduling
}
