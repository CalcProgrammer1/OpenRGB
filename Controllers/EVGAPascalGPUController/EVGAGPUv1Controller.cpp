/*-----------------------------------------*\
|  EVGAGPUv1Controller.cpp                  |
|                                           |
|  Driver for EVGA GPU RGB V1 (Pascal)      |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/11/2020   |
\*-----------------------------------------*/

#include "EVGAGPUv1Controller.h"

EVGAGPUv1Controller::EVGAGPUv1Controller(i2c_smbus_interface* bus, evga_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

EVGAGPUv1Controller::~EVGAGPUv1Controller()
{

}

std::string EVGAGPUv1Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char EVGAGPUv1Controller::GetMode()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V1_REG_MODE));
}

unsigned char EVGAGPUv1Controller::GetRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V1_REG_RED));
}

unsigned char EVGAGPUv1Controller::GetGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V1_REG_GREEN));
}

unsigned char EVGAGPUv1Controller::GetBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V1_REG_BLUE));
}

void EVGAGPUv1Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V1_REG_RED,   red);
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V1_REG_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V1_REG_BLUE,  blue);
}

void EVGAGPUv1Controller::SetMode(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V1_REG_MODE, mode);
}

void EVGAGPUv1Controller::SaveSettings()
{
    bus->i2c_smbus_write_byte_data(dev, 0x23,   0xE5);
}
