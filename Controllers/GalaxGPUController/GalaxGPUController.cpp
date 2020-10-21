/*-----------------------------------------*\
|  GalaxGPUController.cpp                   |
|                                           |
|  Driver for Galax / KFA2 RGB on GPUs      |
|                                           |
|  Niels Westphal (crashniels)  12.07.2020  |
\*-----------------------------------------*/

#include "GalaxGPUController.h"
#include <cstring>

GalaxGPUController::GalaxGPUController(i2c_smbus_interface* bus, galax_gpu_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    strcpy(device_name, "Galax RTX GPU");                                               // Would be nice to get the actual GPU name. Using this as a placeholder.
}

GalaxGPUController::~GalaxGPUController()
{

}

std::string GalaxGPUController::GetDeviceName()
{
    return(device_name);
}

std::string GalaxGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char GalaxGPUController::GetLEDRed()
{
    return(GalaxGPURegisterRead(GALAX_RED_REGISTER));
}

unsigned char GalaxGPUController::GetLEDGreen()
{
    return(GalaxGPURegisterRead(GALAX_GREEN_REGISTER));
}

unsigned char GalaxGPUController::GetLEDBlue()
{
    return(GalaxGPURegisterRead(GALAX_BLUE_REGISTER));
}

void GalaxGPUController::SetLEDColorsDirect(unsigned char red, unsigned char green, unsigned char blue)                  // Direct Mode is just Static Mode without applying color changes
{
    GalaxGPURegisterWrite(GALAX_RED_REGISTER, red);
    GalaxGPURegisterWrite(GALAX_GREEN_REGISTER, green);
    GalaxGPURegisterWrite(GALAX_BLUE_REGISTER, blue);
}

void GalaxGPUController::SetLEDColorsEffect(unsigned char red, unsigned char green, unsigned char blue)
{
    GalaxGPURegisterWrite(GALAX_RED_REGISTER, red);
    GalaxGPURegisterWrite(GALAX_GREEN_REGISTER, green);
    GalaxGPURegisterWrite(GALAX_BLUE_REGISTER, blue);
}

void GalaxGPUController::SetMode(unsigned char mode)
{
    switch(mode)
    {
    case 1:
        GalaxGPURegisterWrite(GALAX_MODE_REGISTER_1, GALAX_MODE_STATIC_VALUE_1);
        GalaxGPURegisterWrite(GALAX_MODE_REGISTER_2, GALAX_MODE_STATIC_VALUE_2);
        break;

    case 2:
        GalaxGPURegisterWrite(GALAX_MODE_REGISTER_1, GALAX_MODE_BREATHING_VALUE_1);
        GalaxGPURegisterWrite(GALAX_MODE_REGISTER_2, GALAX_MODE_BREATHING_VALUE_2);
        break;

    case 3:
        GalaxGPURegisterWrite(GALAX_MODE_REGISTER_1, GALAX_MODE_RAINBOW_VALUE_1);
        GalaxGPURegisterWrite(GALAX_MODE_REGISTER_2, GALAX_MODE_RAINBOW_VALUE_2);
        break;

    case 4:
        GalaxGPURegisterWrite(GALAX_MODE_REGISTER_1, GALAX_MODE_CYCLE_BREATHING_VALUE_1);
        GalaxGPURegisterWrite(GALAX_MODE_REGISTER_2, GALAX_MODE_CYCLE_BREATHING_VALUE_2);
        break;

    default:
        break;
    }
}

unsigned char GalaxGPUController::GalaxGPURegisterRead(unsigned char reg)
{
    return(bus->i2c_smbus_read_byte_data(dev, reg));
}

void GalaxGPUController::GalaxGPURegisterWrite(unsigned char reg, unsigned char val)
{
    bus->i2c_smbus_write_byte_data(dev, reg, val);
}
