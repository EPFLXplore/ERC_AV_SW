
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "LEDStrip.hpp"

// ====== USER CONFIG ======
constexpr int LED_PIN   = 26;
constexpr int NUM_LEDS  = 96;
constexpr uint32_t BAUD = 115200;
constexpr size_t  QUEUE_DEPTH = 16;
// =========================

// ====== CMD EXAMPLES ======
// 0 100 2 4 - Brown drill, full strip, chasing leds
// 0 100 1 4 - HD red, full strip, chasing leds
// 0 100 2 0 - Brown drill, full strip, solid colour
// 0 100 2 3 - Brown drill, full strip, breathing fade
// 0 100 2 5 - Brown drill, full strip, off
// 0 100 0 3 - NAV violet, full strip, breathing fade

LEDStrip strip(LED_PIN, NUM_LEDS);
QueueHandle_t cmdQueue;

// ---------- Serial parser ----------
void serialHandler() {
    if (!Serial.available()) return;
    char buf[64];
    int len = Serial.readBytesUntil('\n', buf, sizeof(buf) - 1);
    buf[len] = '\0';
    

    int low, high, system, mode;
    if (sscanf(buf, "%d %d %d %d", &low, &high, &system, &mode) == 4) {
        Command cmd;
        cmd.system        = constrain(system, 0, 2);
        cmd.mode          = constrain(mode,   0, 6);
        cmd.segment.low   = constrain(low,  0, 100);
        cmd.segment.high  = constrain(high, 0, 100);       

        switch (cmd.system) {
            case 0: cmd.segment.r = 127; cmd.segment.g = 0;   cmd.segment.b = 255; break; // NAV violet
            case 1: cmd.segment.r = 153; cmd.segment.g = 0;   cmd.segment.b = 0;   break; // HD red
            case 2: cmd.segment.r = 102; cmd.segment.g = 51;  cmd.segment.b = 0;   break; // Drill brown
        }
        xQueueSend(cmdQueue, &cmd, 0);
    } else {
        //Serial.println(F("[ERR] bad format (low high system mode)"));
        // flush bad input
        while (Serial.available() && Serial.read() != '\n');
    }
}


// ---------- Command processor task ----------
void commandTask(void* pv) {
    strip.begin();
    Command cmd;
    TickType_t delayTicks = 1 / portTICK_PERIOD_MS;
    for (;;) {
        
        while (xQueueReceive(cmdQueue, &cmd, 0) == pdTRUE) {
            strip.applyCommand(cmd);
        }
        strip.tick();                // advance animations
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
