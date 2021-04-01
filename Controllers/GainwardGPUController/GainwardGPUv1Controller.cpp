/*-----------------------------------------*\
|  GainwardGPUv1Controller.cpp              |
|                                           |
|  Driver for Gainward RGB v1 on GPUs       |
|                                           |
|  TheRogueZeta 11/05/2020                  |
\*-----------------------------------------*/

#include "GainwardGPUv1Controller.h"
#include <cstring>

GainwardGPUv1Controller::GainwardGPUv1Controller(i2c_smbus_interface* bus, gainward_gpu_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

GainwardGPUv1Controller::~GainwardGPUv1Controller()
{

}

std::string GainwardGPUv1Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char GainwardGPUv1Controller::GetLEDRed()
{
    return(GainwardGPURegisterRead(GAINWARD_RED_REGISTER));
}

unsigned char GainwardGPUv1Controller::GetLEDGreen()
{
    return(GainwardGPURegisterRead(GAINWARD_GREEN_REGISTER));
}

unsigned char GainwardGPUv1Controller::GetLEDBlue()
{
    return(GainwardGPURegisterRead(GAINWARD_BLUE_REGISTER));
}

void GainwardGPUv1Controller::SetLEDColors(unsigned char red, unsigned char green, unsigned char blue)
{
    GainwardGPURegisterWrite(GAINWARD_RED_REGISTER, red);
    GainwardGPURegisterWrite(GAINWARD_GREEN_REGISTER, green);
    GainwardGPURegisterWrite(GAINWARD_BLUE_REGISTER, blue);
    GainwardGPURegisterWrite(GAINWARD_06_REGISTER, 0xFF);
}

void GainwardGPUv1Controller::SetMode()
{

}

unsigned char GainwardGPUv1Controller::GainwardGPURegisterRead(unsigned char reg)
{
    return(bus->i2c_smbus_read_byte_data(dev, reg));
}

void GainwardGPUv1Controller::GainwardGPURegisterWrite(unsigned char reg, unsigned char val)
{
    bus->i2c_smbus_write_byte_data(dev, reg, val);
}
