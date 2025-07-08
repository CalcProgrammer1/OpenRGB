/*---------------------------------------------------------*\
| TForceXtreemController.cpp                                |
|                                                           |
|   Driver for T-Force XTreem DRAM                          |
|                                                           |
|   Milan Cermak (krysmanta)                    28 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "TForceXtreemController.h"
#include "LogManager.h"

TForceXtreemController::TForceXtreemController(i2c_smbus_interface *bus, ene_dev_id dev)
{
    this->bus           = bus;
    this->dev           = dev;
}

TForceXtreemController::~TForceXtreemController()
{
}

std::string TForceXtreemController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);

    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);

    return(return_string);
}

unsigned int TForceXtreemController::GetLEDCount()
{
    return(XTREEM_LED_COUNT);
}

/*---------------------------------------------------*\
| LEDs are in a single strip that is folded in half.  |
| That makes the LED order: 0-14-1-13-2-...-7-9-8     |
\*---------------------------------------------------*/
#define XTREEM_LED_OFFSET(x) ((((x) & 0x01) > 0) ? XTREEM_LED_COUNT - 1 - ((x) >> 1) : ((x) >> 1))

unsigned char TForceXtreemController::GetLEDRed(unsigned int led)
{
    return(ENERegisterRead(XTREEM_REG_COLORS_DIRECT + ( 3 * XTREEM_LED_OFFSET(led) )));
}

unsigned char TForceXtreemController::GetLEDGreen(unsigned int led)
{
    return(ENERegisterRead(XTREEM_REG_COLORS_DIRECT + ( 3 * XTREEM_LED_OFFSET(led) ) + 2));
}

unsigned char TForceXtreemController::GetLEDBlue(unsigned int led)
{
    return(ENERegisterRead(XTREEM_REG_COLORS_DIRECT + ( 3 * XTREEM_LED_OFFSET(led) ) + 1));
}

unsigned char TForceXtreemController::GetLEDRedEffect(unsigned int led)
{
    return(ENERegisterRead(XTREEM_REG_COLORS_EFFECT + ( 3 * XTREEM_LED_OFFSET(led) )));
}

unsigned char TForceXtreemController::GetLEDGreenEffect(unsigned int led)
{
    return(ENERegisterRead(XTREEM_REG_COLORS_EFFECT + ( 3 * XTREEM_LED_OFFSET(led) ) + 2));
}

unsigned char TForceXtreemController::GetLEDBlueEffect(unsigned int led)
{
    return(ENERegisterRead(XTREEM_REG_COLORS_EFFECT + ( 3 * XTREEM_LED_OFFSET(led) ) + 1));
}

void TForceXtreemController::SetAllColorsDirect(RGBColor* colors)
{
    unsigned char* color_buf   = new unsigned char[XTREEM_LED_COUNT * 3];
    unsigned int   bytes_sent  = 0;

    for(unsigned int i = 0; i < XTREEM_LED_COUNT; i++)
    {
        unsigned int offset = 3 * XTREEM_LED_OFFSET(i);
        color_buf[offset + 0] = RGBGetRValue(colors[i]);
        color_buf[offset + 1] = RGBGetBValue(colors[i]);
        color_buf[offset + 2] = RGBGetGValue(colors[i]);
    }

    while(bytes_sent < (XTREEM_LED_COUNT * 3))
    {
        ENERegisterWriteBlock(XTREEM_REG_COLORS_DIRECT + bytes_sent, &color_buf[bytes_sent], 3);

        bytes_sent += 3;
    }

    delete[] color_buf;
}

void TForceXtreemController::SetAllColorsEffect(RGBColor* colors)
{
    unsigned char* color_buf   = new unsigned char[XTREEM_LED_COUNT * 3];
    unsigned int   bytes_sent  = 0;

    for(unsigned int i = 0; i < XTREEM_LED_COUNT; i++)
    {
        unsigned int offset = 3 * XTREEM_LED_OFFSET(i);
        color_buf[offset + 0] = RGBGetRValue(colors[i]);
        color_buf[offset + 1] = RGBGetBValue(colors[i]);
        color_buf[offset + 2] = RGBGetGValue(colors[i]);
    }

    while(bytes_sent < (XTREEM_LED_COUNT * 3))
    {
        ENERegisterWriteBlock(XTREEM_REG_COLORS_EFFECT + bytes_sent, &color_buf[bytes_sent], 3);

        bytes_sent += 3;
    }

    ENERegisterWrite(XTREEM_REG_APPLY, XTREEM_APPLY_VAL);

    delete[] color_buf;
}


void TForceXtreemController::SetDirect(unsigned char direct)
{
    ENERegisterWrite(XTREEM_REG_DIRECT, direct);
    ENERegisterWrite(XTREEM_REG_APPLY, XTREEM_APPLY_VAL);
}

void TForceXtreemController::SetLEDColorDirect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    ENERegisterWriteBlock(XTREEM_REG_COLORS_DIRECT + ( 3 * XTREEM_LED_OFFSET(led) ), colors, 3);
}

void TForceXtreemController::SetLEDColorEffect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char colors[3] = { red, blue, green };

    ENERegisterWriteBlock(XTREEM_REG_COLORS_EFFECT + (3 * XTREEM_LED_OFFSET(led)), colors, 3);

    ENERegisterWrite(XTREEM_REG_APPLY, XTREEM_APPLY_VAL);
}

void TForceXtreemController::SetMode(unsigned char mode, unsigned char speed, unsigned char direction)
{
    ENERegisterWrite(XTREEM_REG_MODE,      mode);
    ENERegisterWrite(XTREEM_REG_SPEED,     speed);
    ENERegisterWrite(XTREEM_REG_DIRECTION, direction);
    ENERegisterWrite(XTREEM_REG_APPLY,     XTREEM_APPLY_VAL);
}

unsigned char TForceXtreemController::ENERegisterRead(ene_register reg)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Read ENE value
    return(bus->i2c_smbus_read_byte_data(dev, 0x81));
}

void TForceXtreemController::ENERegisterWrite(ene_register reg, unsigned char val)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write ENE value
    bus->i2c_smbus_write_byte_data(dev, 0x01, val);
}

void TForceXtreemController::ENERegisterWriteBlock(ene_register reg, unsigned char * data, unsigned char sz)
{
    //Write ENE register
    bus->i2c_smbus_write_word_data(dev, 0x00, ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF));

    //Write ENE block data
    bus->i2c_smbus_write_block_data(dev, 0x03, sz, data);
}
