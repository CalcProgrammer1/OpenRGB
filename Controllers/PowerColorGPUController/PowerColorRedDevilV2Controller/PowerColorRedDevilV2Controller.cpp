/*---------------------------------------------------------*\
| PowerColorRedDevilV2Controller.cpp                        |
|                                                           |
|   Driver for PowerColor Red Devil GPU                     |
|                                                           |
|   Nexrem                                      15 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "PowerColorRedDevilV2Controller.h"


PowerColorRedDevilV2Controller::PowerColorRedDevilV2Controller(i2c_smbus_interface* bus, red_devil_v2_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

PowerColorRedDevilV2Controller::~PowerColorRedDevilV2Controller()
{

}

std::string PowerColorRedDevilV2Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C:" + return_string);
}

std::string PowerColorRedDevilV2Controller::GetDeviceName()
{
    return(name);
}

bool PowerColorRedDevilV2Controller::GetSync()
{
    unsigned char data[3];
    RegisterRead(RED_DEVIL_V2_READ_REG_SYNC, data);

    return data[0] && data[1] && data[2];
}

void PowerColorRedDevilV2Controller::SetSync(bool sync)
{
    if(sync)
    {
        unsigned char data[3] = {0x01, 0x01, 0x01};
        RegisterWrite(RED_DEVIL_V2_WRITE_REG_SYNC, data);
    }
    else
    {
        unsigned char data[3] = {0x00, 0x00, 0x00};
        RegisterWrite(RED_DEVIL_V2_WRITE_REG_SYNC, data);
    }
}

/*------------------------------------------------------------------*\
| Mode returns MMBBSS                                                |
\*------------------------------------------------------------------*/
red_devil_v2_mode PowerColorRedDevilV2Controller::GetMode()
{
    unsigned char data[3];
    red_devil_v2_mode mode;

    RegisterRead(RED_DEVIL_V2_READ_REG_MODE, data);

    mode.mode       = data[0];
    mode.brightness = data[1];
    mode.speed      = data[2];

    return mode;
}

void PowerColorRedDevilV2Controller::SetMode(red_devil_v2_mode mode)
{
    if(mode.mode == RED_DEVIL_V2_MODE_SYNC)
    {
        SetSync(true);
        return;
    }

    SetSync(false);

    unsigned char data[3] =
    {
        mode.mode,
        mode.brightness,
        mode.speed
    };

    RegisterWrite(RED_DEVIL_V2_WRITE_REG_MODE, data);
}

RGBColor PowerColorRedDevilV2Controller::GetLedColor(int led)
{
    /*------------------------------------------------------------------*\
    | On overflow read the first LED                                      |
    \*------------------------------------------------------------------*/
    if(led >= RED_DEVIL_V2_NUM_LEDS)
    {
        led = 0;
    }

    unsigned char data[3];
    RegisterRead(RED_DEVIL_V2_READ_REG_RGBX + led, data);

    return ToRGBColor(data[0], data[1], data[2]);
}

void PowerColorRedDevilV2Controller::SetLedColor(int led, RGBColor color)
{
    /*------------------------------------------------------------------*\
    | Skip writing to invalid LEDs                                       |
    \*------------------------------------------------------------------*/
    if(led >= RED_DEVIL_V2_NUM_LEDS)
    {
        return;
    }

    unsigned char data[3] =
    {
        (unsigned char) RGBGetRValue(color),
        (unsigned char) RGBGetGValue(color),
        (unsigned char) RGBGetBValue(color)
    };

    RegisterWrite(RED_DEVIL_V2_WRITE_REG_RGBX+led, data);
}

void PowerColorRedDevilV2Controller::SetLedColorAll(RGBColor color)
{
    unsigned char data[3] =
    {
        (unsigned char) RGBGetRValue(color),
        (unsigned char) RGBGetGValue(color),
        (unsigned char) RGBGetBValue(color)
    };

    /*------------------------------------------------------------------*\
    | Factory firmware writes both, but we only need 1 of them.          |
    | Write both anyways just in case...                                 |
    \*------------------------------------------------------------------*/
    RegisterWrite(RED_DEVIL_V2_WRITE_REG_RGB1, data);
    RegisterWrite(RED_DEVIL_V2_WRITE_REG_RGB2, data);
}

int PowerColorRedDevilV2Controller::RegisterRead(unsigned char reg, unsigned char *data)
{
    int ret = bus->i2c_smbus_read_i2c_block_data(dev, reg, 3, data);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return ret;
}

int PowerColorRedDevilV2Controller::RegisterWrite(unsigned char reg, unsigned char *data)
{
    int ret = bus->i2c_smbus_write_i2c_block_data(dev, reg, 3, data);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return ret;
}