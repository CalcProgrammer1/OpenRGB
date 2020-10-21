/*-----------------------------------------*\
|  CorsairVengeanceController.h             |
|                                           |
|  Driver for Corsair Vengeance RGB RAM     |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/8/2019    |
\*-----------------------------------------*/

#include "CorsairVengeanceController.h"
#include <cstring>

CorsairVengeanceController::CorsairVengeanceController(i2c_smbus_interface* bus, corsair_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    strcpy(device_name, "Corsair Vengeance RGB");
    led_count = 1;
}

CorsairVengeanceController::~CorsairVengeanceController()
{

}

std::string CorsairVengeanceController::GetDeviceName()
{
    return(device_name);
}

std::string CorsairVengeanceController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned int CorsairVengeanceController::GetLEDCount()
{
    return(led_count);
}

void CorsairVengeanceController::SetLEDColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_FADE_TIME, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_RED_VAL, red);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_GREEN_VAL, green);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_BLUE_VAL, blue);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_MODE, CORSAIR_VENGEANCE_RGB_MODE_SINGLE);
}

void CorsairVengeanceController::SetMode(unsigned char /*mode*/)
{
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_VENGEANCE_RGB_CMD_MODE, CORSAIR_VENGEANCE_RGB_MODE_SINGLE);
}
