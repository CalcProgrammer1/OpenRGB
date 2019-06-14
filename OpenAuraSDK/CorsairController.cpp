/*-----------------------------------------*\
|  CorsairController.h                      |
|                                           |
|  Driver for Corsair Vengeance RGB RAM     |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/8/2019    |
\*-----------------------------------------*/

#include "CorsairController.h"
#include <cstring>

CorsairController::CorsairController(i2c_smbus_interface* bus, corsair_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    strcpy(device_name, "Corsair Vengeance RGB");
    led_count = 1;
}

CorsairController::~CorsairController()
{

}

char * CorsairController::GetDeviceName()
{
    return(device_name);
}

unsigned int CorsairController::GetLEDCount()
{
    return(led_count);
}

void CorsairController::SetAllColorsDirect(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_FADE_TIME, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_RED_VAL, red);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_GREEN_VAL, green);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_BLUE_VAL, blue);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_MODE, CORSAIR_VENGEANCE_RGB_MODE_SINGLE);
}

void CorsairController::SetMode(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_MODE, CORSAIR_VENGEANCE_RGB_MODE_SINGLE);
}

int RGBController_Corsair::GetMode()
{
    return(CORSAIR_VENGEANCE_RGB_MODE_SINGLE);
}

void RGBController_Corsair::SetMode(int mode)
{
    corsair->SetMode(mode);
}

void RGBController_Corsair::SetAllLEDs(unsigned char red, unsigned char green, unsigned char blue)
{
    corsair->SetAllColorsDirect(red, green, blue);
}

RGBController_Corsair::RGBController_Corsair(CorsairController* corsair_ptr)
{
    corsair = corsair_ptr;

    name = corsair->GetDeviceName();

    mode corsair_modes[CORSAIR_NUMBER_MODES];

    corsair_modes[0].name = "Static";
    corsair_modes[1].name = "Fade";
    corsair_modes[2].name = "Pulse";

    for (int i = 0; i < CORSAIR_NUMBER_MODES; i++)
    {
        modes.push_back(corsair_modes[i]);
    }

    for (int i = 0; i < corsair->GetLEDCount(); i++)
    {
        led* new_led = new led();

        new_led->name = "Corsair LED";

        leds.push_back(*new_led);
    }

    zone new_zone;

    new_zone.name = "Corsair Zone";
    new_zone.type = ZONE_TYPE_SINGLE;

    std::vector<int> zone_row;

    for (int i = 0; i < corsair->GetLEDCount(); i++)
    {
        zone_row.push_back(i);
    }

    new_zone.map.push_back(zone_row);

    zones.push_back(new_zone);
}