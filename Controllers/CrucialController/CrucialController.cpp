/*---------------------------------------------------------*\
| CrucialController.cpp                                     |
|                                                           |
|   Driver for Crucial Ballistix RAM                        |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "CrucialController.h"

CrucialController::CrucialController(i2c_smbus_interface* bus, crucial_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    for(int i = 0; i < 16; i++)
    {
        device_version[i] = CrucialRegisterRead(CRUCIAL_REG_DEVICE_VERSION + i);
    }
}

CrucialController::~CrucialController()
{

}

std::string CrucialController::GetDeviceVersion()
{
    return(device_version);
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

void CrucialController::SetMode(unsigned char mode)
{
    SendEffectMode(mode, 0x10);
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
    unsigned char color_blk[8];

    for(unsigned int led = 0; led < 8; led++)
    {
        color_blk[led] = RGBGetRValue(color_buf[led]);
    }

    //Red Channels
    CrucialRegisterWriteBlock(0x8300, color_blk, 8);

    for(unsigned int led = 0; led < 8; led++)
    {
        color_blk[led] = RGBGetGValue(color_buf[led]);
    }

    //Green Channels
    CrucialRegisterWriteBlock(0x8340, color_blk, 8);

    for(unsigned int led = 0; led < 8; led++)
    {
        color_blk[led] = RGBGetBValue(color_buf[led]);
    }

    //Blue Channels
    CrucialRegisterWriteBlock(0x8380, color_blk, 8);
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

void CrucialController::CrucialRegisterWriteBlock(crucial_register reg, unsigned char * data, unsigned char sz)
{
    //Write Crucial register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write Crucial block data
    bus->i2c_smbus_write_block_data(dev, 0x03, sz, data);
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
