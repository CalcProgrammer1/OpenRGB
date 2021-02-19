/*-----------------------------------------*\
|  GainwardGPUController.cpp                |
|                                           |
|  Driver for Gainward RGB on GPUs          |
|                                           |
|  TheRogueZeta 11/05/2020                  |
\*-----------------------------------------*/

#include "GainwardGPUController.h"
#include <cstring>

GainwardGPUController::GainwardGPUController(i2c_smbus_interface* bus, gainward_gpu_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

GainwardGPUController::~GainwardGPUController()
{

}

std::string GainwardGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char GainwardGPUController::GetLEDRed()
{
    return(GainwardGPURegisterRead(GAINWARD_RED_REGISTER));
}

unsigned char GainwardGPUController::GetLEDGreen()
{
    return(GainwardGPURegisterRead(GAINWARD_GREEN_REGISTER));
}

unsigned char GainwardGPUController::GetLEDBlue()
{
    return(GainwardGPURegisterRead(GAINWARD_BLUE_REGISTER));
}

void GainwardGPUController::SetLEDColors(unsigned char red, unsigned char green, unsigned char blue)
{
    GainwardGPURegisterWrite(GAINWARD_RED_REGISTER, red);
    GainwardGPURegisterWrite(GAINWARD_GREEN_REGISTER, green);
    GainwardGPURegisterWrite(GAINWARD_BLUE_REGISTER, blue);
    GainwardGPURegisterWrite(GAINWARD_06_REGISTER, 0xFF);
}

void GainwardGPUController::SetMode()
{

}

unsigned char GainwardGPUController::GainwardGPURegisterRead(unsigned char reg)
{
    return(bus->i2c_smbus_read_byte_data(dev, reg));
}

void GainwardGPUController::GainwardGPURegisterWrite(unsigned char reg, unsigned char val)
{
    bus->i2c_smbus_write_byte_data(dev, reg, val);
}
