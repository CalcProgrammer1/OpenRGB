/*-----------------------------------------*\
|  CrucialController.cpp                    |
|                                           |
|  Driver for Crucial Ballistix RGB lighting|
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/19/2020   |
\*-----------------------------------------*/

#include "CrucialController.h"
#include <cstring>

CrucialController::CrucialController(i2c_smbus_interface* bus, crucial_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

CrucialController::~CrucialController()
{

}

std::string CrucialController::GetDeviceName()
{
    return(device_name);
}

std::string CrucialController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return(return_string);
}

unsigned int CrucialController::GetLEDCount()
{
    return(led_count);
}

void CrucialController::SetMode(unsigned char mode)
{
    SendEffectMode(mode, 0x10);
}

void CrucialController::SetAllColorsDirect(unsigned char red, unsigned char green, unsigned char blue)
{
    SendEffectColors(red, green, blue);
    SendEffectMode(CRUCIAL_MODE_STATIC, 0);
}

void CrucialController::SendEffectColors(unsigned char red, unsigned char green, unsigned char blue)
{
    CrucialRegisterWrite(0x82E9, 0xFF);
    CrucialRegisterWrite(0x82EA, 0x00);
    CrucialRegisterWrite(0x82EB, 0x00);
    CrucialRegisterWrite(0x82EC, 0x00);
    CrucialRegisterWrite(0x82ED, red);
    CrucialRegisterWrite(0x82EE, green);
    CrucialRegisterWrite(0x82EF, blue);
    CrucialRegisterWrite(0x82F0, 0x01);
}

void CrucialController::SendBrightness(unsigned char brightness)
{
    CrucialRegisterWrite(0x82EE, 0xFF);
    CrucialRegisterWrite(0x82EF, brightness);
    CrucialRegisterWrite(0x82F0, 0x83);
}

void CrucialController::SendEffectMode(unsigned char mode, unsigned char speed)
{
    CrucialRegisterWrite(0x820F, mode);
    CrucialRegisterWrite(0x82EE, 0x00);
    CrucialRegisterWrite(0x82EF, speed);
    CrucialRegisterWrite(0x82F0, 0x84);
}

unsigned char CrucialController::CrucialRegisterRead(crucial_register reg)
{
    //Write Crucial register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Read Crucial value
    return(bus->i2c_smbus_read_byte_data(dev, 0x81));

}

void CrucialController::CrucialRegisterWrite(crucial_register reg, unsigned char val)
{
    //Write Crucial register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write Crucial value
    bus->i2c_smbus_write_byte_data(dev, 0x01, val);

}
