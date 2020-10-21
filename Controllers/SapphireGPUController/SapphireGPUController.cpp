/*-----------------------------------------*\
|  SapphireGPUController.cpp                |
|                                           |
|  Driver for Sapphire Nitro Glow GPU RGB   |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/15/2020   |
\*-----------------------------------------*/

#include "SapphireGPUController.h"

SapphireGPUController::SapphireGPUController(i2c_smbus_interface* bus, sapphire_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

SapphireGPUController::~SapphireGPUController()
{

}

std::string SapphireGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char SapphireGPUController::GetRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_GPU_REG_RED));
}

unsigned char SapphireGPUController::GetGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_GPU_REG_GREEN));
}

unsigned char SapphireGPUController::GetBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_GPU_REG_BLUE));
}

void SapphireGPUController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_GPU_REG_RED,   red);
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_GPU_REG_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_GPU_REG_BLUE,  blue);
}

void SapphireGPUController::SetMode(unsigned char mode, unsigned char /*speed*/)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_GPU_REG_MODE, mode);
}
