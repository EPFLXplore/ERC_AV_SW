#include <ADS1234.hpp>

#include <FreeRTOS.h>
#include <stdio.h>
#include <cmsis_os.h>
#include "semphr.h"

// #define PIN_SCLK(2)
// #define PIN_PDWN(2)
// #define PIN_SPEED(2)
// #define PIN_GAIN0(2)
// #define PIN_GAIN1(2)
// #define PIN_A0(2)
// #define PIN_A1(2)
// #define PIN_DOUT(2)

/**
 * Setup Functions
 */
ADS1234::ADS1234(uint8_t PIN_SCLK, uint8_t PIN_PDWN, uint8_t PIN_SPEED, uint8_t PIN_GAIN0, 
    uint8_t PIN_GAIN1, uint8_t PIN_A0, uint8_t PIN_A1, uint8_t PIN_DOUT) :
    _PIN_SCLK(PIN_SCLK),
    _PIN_PDWN(PIN_PDWN),
    _PIN_SPEED(PIN_SPEED),
    _PIN_GAIN0(PIN_GAIN0),
    _PIN_GAIN1(PIN_GAIN1),
    _PIN_A0(PIN_A0),
    _PIN_A1(PIN_A1),
    _PIN_DOUT(PIN_DOUT)
{
}

ADS1234::~ADS1234(){}

void ADS1234::begin(Gain gain, Speed speed)
{
    // Setup pins
    pinMode(PIN_SCLK, OUTPUT);
    pinMode(PIN_PDWN, OUTPUT);
    pinMode(PIN_SPEED, OUTPUT);
    pinMode(PIN_GAIN0, OUTPUT);
    pinMode(PIN_GAIN1, OUTPUT);
    pinMode(PIN_A0, OUTPUT);
    pinMode(PIN_A1, OUTPUT);
    pinMode(PIN_DOUT, INPUT_PULLUP);

    setGain(gain);
    setSpeed(speed);
    power_up();
}

void ADS1234::power_up(void)
{
    digitalWrite(PIN_PDWN, LOW);
    digitalWrite(PIN_PDWN, HIGH);

    // Set CLK low to get the ADS1234 out of suspend
    digitalWrite(PIN_SCLK, LOW)
}

void ADS1234::power_down(void)
{
    digitalWrite(PIN_PDWN, LOW);
    digitalWrite(PIN_SCLK, HIGH);
}

/**
 * Calibration Functions
 */
void ADS1234::setGain(Gain gain)
{
    /** Gain encoding truth table
     * 
     * GAIN[1:0] | Gain
     * 00 | 1
     * 01 | 2
     * 10 | 64
     * 11 | 128
     * 
     */
    switch(gain)
    {
        case GAIN1:
        {
            digitalWrite(PIN_GAIN1, LOW);
            digitalWrite(PIN_GAIN0, LOW);
            break;
        }
        case GAIN2:
        {
            digitalWrite(PIN_GAIN1, LOW);
            digitalWrite(PIN_GAIN0, HIGH);
            break;
        }
        case GAIN64:
        {
            digitalWrite(PIN_GAIN1, HIGH);
            digitalWrite(PIN_GAIN0, LOW);
            break;
        }
        case GAIN128:
        {
            digitalWrite(PIN_GAIN1, HIGH);
            digitalWrite(PIN_GAIN0, HIGH);
            break;
        }
        default:
            break;
    }
}

void ADS1234::setSpeed(Speed speed)
{
    /** Speed pin data encoding
     * Speed | SPS
     * 0 | 10
     * 1 | 80
     */

    //set attribute of ADS1234 class
    _speed = speed

    // encode value on pin
    switch (speed)
    {
        case SLOW:
        {
            digitalWrite(PIN_SPEED, LOW);
            break;
        }
        case FAST:
        {
            digitalWrite(PIN_SPEED, HIGH);
            break;
        }
        default:
            break;
    }
}

void ADS1234::setChannel(Channel channel)
{
    /** A0 & A1 pins truth table for input encoding:
     * 
     * A1 | A0 | Channel
     * -----------------
     * 0 | 0 | CH1
     * 0 | 1 | CH2
     * 1 | 0 | CH3
     * 1 | 1 | CH4
     * 
     */

    switch(channel)
    {
        case AIN1:
        {
            digitalWrite(PIN_A1, LOW);
            digitalWrite(PIN_A0, LOW);
            break;
        }
        case AIN2:
        {
            digitalWrite(PIN_A1, LOW);
            digitalWrite(PIN_A0, HIGH);
            break;
        }
        case AIN3:
        {
            digitalWrite(PIN_A1, HIGH);
            digitalWrite(PIN_A0, LOW);
            break;
        }
        case AIN4:
        {
            digitalWrite(PIN_A1, HIGH);
            digitalWrite(PIN_A0, HIGH);
            break;
        }
        default:
            break:
    }
}

ERROR_t ADS1234::tare(Channel channel, float alpha, bool calibrating)
{
    ERROR_t err;
    float sum = 0;
    err = read_filtered(channel, sum, alpha, calibrating);

    if(err != NoERROR)
        return err;
    
    set_offset(channel, sum);
    return NoERROR;
}

void ADS1234::set_scale(Channel channel, float scale)
{
    SCALE[channel-1] = scale;
}

float ADS1234::get_scale(Channel channel)
{
    return SCALE[channel-1];   
}

void ADS1234::set_offset(Channel channel, float offset) 
{
	OFFSET[channel-1] = offset;
	prev_value[channel-1] = offset;
}

float ADS1234::get_offset(Channel channel) 
{
	return OFFSET[channel-1];
}

/*-----------------------------------------------------------------------------------------------------------------
 *-----------------------------------------------------------------------------------------------------------------
 * Get Data Functions
 *-----------------------------------------------------------------------------------------------------------------
 *-----------------------------------------------------------------------------------------------------------------
*/

// Get raw ADC value. Can block up to 100ms in normal operation. Returns 0 on sucess else error code.
ERROR_t ADS1234::read(Channel channel, long& value, bool calibrating)
{
    unsigned int waitingTime;
    unsigned int SettlingTimeAfterChangeChannel=0;

    if(channel != lastChannel)
    {
        setChannel(channel);
        if(_speed = FAST)
            SettlingTimeAfterChangeChannel = 55;
        else
            SettlingTimeAfterChangeChannel = 405;
        
        lastChannel = channel;
    }

    /* A high to low transition on the data pin means that the ADS1231
    * has finished a measurement (see datasheet page 13).
    * This can take up to 100ms (the ADS1231 runs at 10 samples per
    * second!).
    * Note that just testing for the state of the pin is unsafe.
    */
   if(calibrating)
   {
        if(_speed = FAST)
            waitingTime = 150;
        else
            waitingTime = 850;
   } else{
        if(_speed = FAST)
            waitingTime = 20;
        else
            waitingTime = 150;
   }

   waitingTime += SettlingTimeAfterChangeChannel;
   waitingTime += 600; //[ms] Add some extra time ( sometimes takes longer than what datasheet claims! )

   if(!calibrating)
        delay(100);
    else
        delay(1300);
    
    // Read 24 bits
    for(int i=23; i>=0; i--)
    {
        digitalWrite(PIN_SCLK, HIGH);
        value = (value << 1) + digitalRead(PIN_DOUT);
        digitalWrite(PIN_SCLK, LOW);
    }

    if(calibrating)
    {
        // 2 extra bits for calibrating
        for(int i=1; i>=0; i--)
        {
            digitalWrite(PIN_SCLK, HIGH);
            digitalWrite(PIN_SCLK, LOW);
        }
    }

    value = (value << 8) / 256;

    if(!calibrating)
    {
         /* The data pin now is high or low depending on the last bit that
          * was read.
          * To get it to the default state (high) we toggle the clock one
          * more time (see datasheet).
          */
         digitalWrite(PIN_SCLK, HIGH);
         digitalWrite(PIN_SCLK, LOW);
    }
    
    delay(1);
    return NoERROR;

}

ERROR_t ADS1234::read_filtered(Channel channel, float& value, float alpha, bool calibrating)
{
    long val;
    ERROR_t err = read(channel, val, calibrating);
    
    if(err != NoERROR)
        return err;

    value = prev_value[channel-1]*value + val*(1-alpha);
    last_filtered_raw[channel-1] = value;
    prev_value[channel-1] = value;
    return NoERROR;
}

float ADS1234::get_last_filtered_raw(Channel channel)
{
    return last_filtered_raw[channel-1];
}

float ADS1234::get_last_filtered_tared(Channel channel)
{
    return last_filtered_raw[channel-1] - OFFSET[channel-1];
}

ERROR_t ADS1234::read_average(Channel channel, float& value, uint16_t times, bool calibrating)
{
    long sum = 0;
    ERROR_t err;

    for(uint16_t i=0; i<times; i++)
    {
        long val;
        err = read(channel, val, calibrating);
        
        if(err != NoERROR)
            return err;
        
        value = (float)sum / times;
        return NoERROR;
    }
}

// Get Average
ERROR_t ADS1234::get_value(Channel channel, float& value, uint16_t times, bool calibrating)
{
    float val = 0;
    ERROR_t = err;
    err = read_average(channel, val, times, calibrating);
    
    if(err != NoERROR)
        return err;

    value = (val - OFFSET[channel-1]);
    return NoERROR;
}

//Get Filtered
ERROR_t ADS1234::get_value(Channel channel, float& value, float alpha, bool calibrating)
{
    float val = 0;
    ERROR_t;
    err = read_filtered(channel,val,0.8,calibrating);

    if(err != NoERROR)
        return err;
    
    value = (val - OFFSET[channel-1]);
    return NoERROR;
}

// average
ERROR_t ADS1234::get_units(Channel channel, float& value, uint16_t times, bool calibrating)
{
    float val = 0;
    ERROR_t err;
    err = get_value(channel, val, times, calibrating);

    if(err != NoERROR)
        return err;
    
    if(SCALE[channel-1]==0)
        return DIVIDED_by_ZERO;
    
    value = val / SCALE[channel-1];
    return NoERROR;
}

//filtered
ERROR_t ADS1234::get_units(Channel channel, float alpha, bool calibrating)
{
    float val = 0;
    ERROR_t err;
    err = get_value(channel,val,alpha,calibrating);

    if(err != NoERROR)
        return err;

    if(SCALE[channel-1] == 0)
        return DIVIDED_by_ZERO;

    return NoERROR;
}




