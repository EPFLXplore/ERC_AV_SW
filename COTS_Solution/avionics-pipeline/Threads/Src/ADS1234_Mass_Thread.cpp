#include "ADS1234_Mass_Thread.hpp"
#include <iostream>

#define PIN_SCLK(2)
#define PIN_PDWN(2)
#define PIN_SPEED(2)
#define PIN_GAIN0(2)
#define PIN_GAIN1(2)
#define PIN_A0(2)
#define PIN_A1(2)
#define PIN_DOUT(2)


ADS1234Thread* MassSensorInterface = nullptr;
static char cbuf[256];

static MassData mass_data;
static MassPacket mass_packet;
static MassConfigRequestPacket mass_config_packer;

ADS1234Thread::ADS1234Thread() : mass_sensor(nullptr)
{
    mass_sensor = new ADS1234(PIN_SCLK, PIN_PDWN, PIN_SPEED, PIN_GAIN0, 
                                PIN_GAIN1, PIN_A0, PIN_A1, PIN_DOUT);
}

ADS1234Thread::~ADS1234Thread()
{
    delete mass_sensor;
    mass_sensor = nullptr;
}

ADS1234Thread::init()
{
    MassSensorInterface = this;
    
    mass_sensor->begin();
    // Channel 1
    mass_sensor->set_offset(AIN1, 263616.4375);
	mass_sensor->set_scale(AIN1, 451.8433);
    //Channel 2
    mass_sensor->set_offset(AIN2, 263616.4375);
	mass_sensor->set_scale(AIN2, 451.8433);
    //Channel 3
    mass_sensor->set_offset(AIN3, 263616.4375);
	mass_sensor->set_scale(AIN3, 451.8433);
    //Channel 4
    mass_sensor->set_offset(AIN4, 263616.4375);
	mass_sensor->set_scale(AIN4, 451.8433);

    request_config();
}

void ADS1234Thread::request_config()
{
    
}


