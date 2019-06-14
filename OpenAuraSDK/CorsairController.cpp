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