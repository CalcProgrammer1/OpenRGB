/*-----------------------------------------*\
|  CrucialController.cpp                    |
|                                           |
|  Driver for Crucial Ballistix RGB lighting|
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/19/2020   |
\*-----------------------------------------*/

#include "RGBController.h"
#include "CrucialController.h"
#include <cstring>

CrucialController::CrucialController(i2c_smbus_interface* bus, crucial_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    // We don't know what mode the sticks are in until we set one.
    this->last_mode = CRUCIAL_MODE_UNKNOWN;
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
    return("I2C: " + return_string);
}

unsigned int CrucialController::GetLEDCount()
{
    return(led_count);
}

void CrucialController::SetMode(unsigned char mode)
{
    // Don't set a mode if we've already set the same one before.
    // This is mostly useful for direct mode and updating colors often.
    if (mode == last_mode) return;
    SendEffectMode(mode, 0x10);
    last_mode = mode;
}

void CrucialController::SetAllColorsDirect(RGBColor* colors)
{
    SendDirectColors(colors);
}

void CrucialController::SetAllColorsEffect(RGBColor* colors)
{
    for(int led_idx = 0; led_idx < 8; led_idx++)
    {
        unsigned char red = RGBGetRValue(colors[led_idx]);
        unsigned char grn = RGBGetGValue(colors[led_idx]);
        unsigned char blu = RGBGetBValue(colors[led_idx]);
        SendEffectColor(led_idx, red, grn, blu);
    }
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

void CrucialController::SendDirectColors(RGBColor* color_buf)
{
    SetMode(CRUCIAL_MODE_STATIC);

    //Red Channels
    CrucialRegisterWrite(0x8300, RGBGetRValue(color_buf[0]));
    CrucialRegisterWrite(0x8301, RGBGetRValue(color_buf[1]));
    CrucialRegisterWrite(0x8302, RGBGetRValue(color_buf[2]));
    CrucialRegisterWrite(0x8303, RGBGetRValue(color_buf[3]));
    CrucialRegisterWrite(0x8304, RGBGetRValue(color_buf[4]));
    CrucialRegisterWrite(0x8305, RGBGetRValue(color_buf[5]));
    CrucialRegisterWrite(0x8306, RGBGetRValue(color_buf[6]));
    CrucialRegisterWrite(0x8307, RGBGetRValue(color_buf[7]));

    //Green Channels
    CrucialRegisterWrite(0x8340, RGBGetGValue(color_buf[0]));
    CrucialRegisterWrite(0x8341, RGBGetGValue(color_buf[1]));
    CrucialRegisterWrite(0x8342, RGBGetGValue(color_buf[2]));
    CrucialRegisterWrite(0x8343, RGBGetGValue(color_buf[3]));
    CrucialRegisterWrite(0x8344, RGBGetGValue(color_buf[4]));
    CrucialRegisterWrite(0x8345, RGBGetGValue(color_buf[5]));
    CrucialRegisterWrite(0x8346, RGBGetGValue(color_buf[6]));
    CrucialRegisterWrite(0x8347, RGBGetGValue(color_buf[7]));

    //Blue Channels
    CrucialRegisterWrite(0x8380, RGBGetBValue(color_buf[0]));
    CrucialRegisterWrite(0x8381, RGBGetBValue(color_buf[1]));
    CrucialRegisterWrite(0x8382, RGBGetBValue(color_buf[2]));
    CrucialRegisterWrite(0x8383, RGBGetBValue(color_buf[3]));
    CrucialRegisterWrite(0x8384, RGBGetBValue(color_buf[4]));
    CrucialRegisterWrite(0x8385, RGBGetBValue(color_buf[5]));
    CrucialRegisterWrite(0x8386, RGBGetBValue(color_buf[6]));
    CrucialRegisterWrite(0x8387, RGBGetBValue(color_buf[7]));
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

void CrucialController::SendEffectColor
    (
    unsigned int    led_idx,
    unsigned int    red,
    unsigned int    green,
    unsigned int    blue
    )
{
    CrucialRegisterWrite(0x82E9, (1 << led_idx));
    CrucialRegisterWrite(0x82EA, 0x00);
    CrucialRegisterWrite(0x82EB, 0x00);
    CrucialRegisterWrite(0x82EC, 0x00);
    CrucialRegisterWrite(0x82ED, red);
    CrucialRegisterWrite(0x82EE, green);
    CrucialRegisterWrite(0x82EF, blue);
    CrucialRegisterWrite(0x82F0, 0x01);
}