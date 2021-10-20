/*-----------------------------------------*\
|  PNYGPUController.cpp                     |
|                                           |
|  Driver for PNY GPU RGB (Turing)          |
|  lighting controller                      |
|                                           |
|  KendallMorgan   10/17/2021               |
\*-----------------------------------------*/

#include "PNYGPUController.h"

PNYGPUController::PNYGPUController(i2c_smbus_interface* bus, pny_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

PNYGPUController::~PNYGPUController()
{

}

std::string PNYGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char PNYGPUController::GetMode()
{
    return(bus->i2c_smbus_read_byte_data(dev, PNY_GPU_MODE_OFF));
}

unsigned char PNYGPUController::GetRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, PNY_GPU_REG_COLOR_RED));
}

unsigned char PNYGPUController::GetGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, PNY_GPU_REG_COLOR_GREEN));
}

unsigned char PNYGPUController::GetBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, PNY_GPU_REG_COLOR_BLUE));
}

void PNYGPUController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, 0xE0,                               0x00);
    bus->i2c_smbus_write_byte_data(dev, PNY_GPU_REG_MODE,                   0x01);
    bus->i2c_smbus_write_byte_data(dev, PNY_GPU_REG_COLOR_RED,              red);
    bus->i2c_smbus_write_byte_data(dev, PNY_GPU_REG_COLOR_BLUE,             blue);
    bus->i2c_smbus_write_byte_data(dev, PNY_GPU_REG_COLOR_GREEN,            green);
    bus->i2c_smbus_write_byte_data(dev, PNY_GPU_REG_COLOR_BRIGHTNESS,       0x64);
}

void PNYGPUController::SetMode(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, PNY_GPU_REG_MODE, mode);
}
