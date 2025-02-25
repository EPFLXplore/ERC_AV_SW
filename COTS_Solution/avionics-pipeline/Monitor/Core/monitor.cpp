#include <Arduino.h>
#include <stdarg.h>

class SerialMonitor {
public:
    // Constructor allows setting a custom baud rate (default 115200)
    SerialMonitor(unsigned long baudRate = 115200) : baudRate(baudRate) {}

    // Initialize the serial communication.
    void begin() {
        Serial.begin(baudRate);
        while (!Serial) {
            ; // Wait for the serial port to connect (necessary for some boards)
        }
    }

    // Log a simple string message.
    void log(const String &message) {
        Serial.println(message);
    }

    // Log a formatted message (like printf).
    void log(const char* format, ...) {
        char buffer[256]; // buffer size can be adjusted as needed
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        Serial.println(buffer);
    }
    
    // Log a debug message with a prefix.
    void debug(const String &message) {
        Serial.print("[DEBUG] ");
        Serial.println(message);
    }

    // Log an error message with a prefix.
    void error(const String &message) {
        Serial.print("[ERROR] ");
        Serial.println(message);
    }
    
private:
    unsigned long baudRate;
};