#ifndef MONITOR_CORE_MONITOR_HPP_
#define MONITOR_CORE_MONITOR_HPP_


#include <Arduino.h>
#include <stdarg.h>

class SerialMonitor {
public:
    // Constructor allows setting a custom baud rate (default 115200)
    SerialMonitor(unsigned long baudRate = 115200);

    // Initialize the serial communication.
    void begin();
    
    // Log a simple string message.
    void log(const String &message);
    
    // Log a formatted message (like printf).
    void log(const char* format);

    // Log a debug message with a prefix.
    void debug(const String &message);

    // Log an error message with a prefix.
    void error(const String &message);
    
private:
    unsigned long baudRate;
};

#endif /* MONITOR_CORE_MONITOR_HPP_ */