/*-----------------------------------------*\
|  EVGAGPUController.cpp                    |
|                                           |
|  Driver for EVGA GPU RGB lighting         |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/11/2020   |
\*-----------------------------------------*/

#include "EVGAGPUController.h"

EVGAGPUController::EVGAGPUController(i2c_smbus_interface* bus, evga_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

EVGAGPUController::~EVGAGPUController()
{

}

std::string EVGAGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return(return_string);
}

unsigned char EVGAGPUController::GetRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_REG_RED));
}

unsigned char EVGAGPUController::GetGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_REG_GREEN));
}

unsigned char EVGAGPUController::GetBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_REG_BLUE));
}

void EVGAGPUController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_REG_RED,   red);
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_REG_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_REG_BLUE,  blue);
}

void EVGAGPUController::SetMode(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_REG_MODE, mode);
}
