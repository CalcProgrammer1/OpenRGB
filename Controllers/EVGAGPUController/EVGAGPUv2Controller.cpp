/*-----------------------------------------*\
|  EVGAGPUv2Controller.cpp                  |
|                                           |
|  Driver for EVGA GPU RGB V2 (Turing)      |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/13/2020   |
\*-----------------------------------------*/

#include "EVGAGPUv2Controller.h"

EVGAGPUv2Controller::EVGAGPUv2Controller(i2c_smbus_interface* bus, evga_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

EVGAGPUv2Controller::~EVGAGPUv2Controller()
{

}

std::string EVGAGPUv2Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char EVGAGPUv2Controller::GetRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_RED));
}

unsigned char EVGAGPUv2Controller::GetGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_GREEN));
}

unsigned char EVGAGPUv2Controller::GetBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_BLUE));
}

void EVGAGPUv2Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, 0x0E,                               0xE5);
    bus->i2c_smbus_write_byte_data(dev, 0x0E,                               0xE9);
    bus->i2c_smbus_write_byte_data(dev, 0x0E,                               0xF5);
    bus->i2c_smbus_write_byte_data(dev, 0x0E,                               0xF9);
    
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_RED,        red);
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_GREEN,      green);
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_BLUE,       blue);
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_COLOR_A_BRIGHTNESS, 0x64);

    bus->i2c_smbus_write_byte_data(dev, 0x08,                               0x01);
    bus->i2c_smbus_write_byte_data(dev, 0x0E,                               0xF0);
    bus->i2c_smbus_write_byte_data(dev, 0x0E,                               0xE0);
}

void EVGAGPUv2Controller::SetMode(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V2_REG_MODE, mode);
}
