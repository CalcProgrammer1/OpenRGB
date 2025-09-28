/*---------------------------------------------------------*\
| GigabyteRGBFusion2DRAMController.cpp                      |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion 2 RAM              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                07 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "GigabyteRGBFusion2DRAMController.h"

RGBFusion2DRAMController::RGBFusion2DRAMController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev)
{
    /*-----------------------------------------------------*\
    | Initialize pointers                                   |
    \*-----------------------------------------------------*/
    this->bus = bus;
    this->dev = dev;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(led_data, 0, sizeof(led_data));

    /*-----------------------------------------------------*\
    | Initialize controller with 6 LEDs                     |
    | This is hard coded for Aorus RGB RAM                  |
    \*-----------------------------------------------------*/
    led_count = 6;

    direct_initialized = false;
}

RGBFusion2DRAMController::~RGBFusion2DRAMController()
{

}

unsigned int RGBFusion2DRAMController::GetLEDCount()
{
    return(led_count);
}

std::string RGBFusion2DRAMController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

void RGBFusion2DRAMController::Apply()
{
    bus->i2c_smbus_write_byte_data(dev, RGB_FUSION_2_DRAM_APPLY_ADDR, RGB_FUSION_2_DRAM_ACTION_APPLY);
}

void RGBFusion2DRAMController::SetLEDEffect
    (
    unsigned int    led,
    int             mode,
    unsigned int    brightness,
    unsigned int    /*speed*/,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    bool truncate_packet = false;

    if(mode == RGB_FUSION_2_DRAM_MODE_DIRECT)
    {
        /*-----------------------------------------------------*\
        | In Direct mode, set one LED at a time                 |
        \*-----------------------------------------------------*/
        led_data[RGB_FUSION_2_DRAM_LED_EN_MASK] = (1 << led);

        /*-----------------------------------------------------*\
        | Hack for Direct mode                                  |
        \*-----------------------------------------------------*/
        led_data[16] = 1;
        led_data[22] = 2;
        led_data[29] = 1;
        led_data[30] = 1;

        /*-----------------------------------------------------*\
        | If bytes 15-31 have already been set, we can speed up |
        | repeat direct mode packets by only sending the portion|
        | that changes, in this case bytes 0-14.  If direct mode|
        | has already been initialized, truncate the packet by  |
        | only sending the first 15 bytes.                      |
        \*-----------------------------------------------------*/
        if(direct_initialized)
        {
            truncate_packet = true;
        }

        direct_initialized = true;

        /*-----------------------------------------------------*\
        | Direct mode is implemented using Pulse mode           |
        \*-----------------------------------------------------*/
        mode = RGB_FUSION_2_DRAM_MODE_PULSE;
    }
    else
    {
        /*-----------------------------------------------------*\
        | In all other modes, set all LEDs at once              |
        \*-----------------------------------------------------*/
        led_data[RGB_FUSION_2_DRAM_LED_EN_MASK] = 0x3F;

        /*-----------------------------------------------------*\
        | Clear direct mode initialized flag when setting a non-|
        | Direct mode                                           |
        \*-----------------------------------------------------*/
        direct_initialized = false;
    }

    led_data[RGB_FUSION_2_DRAM_IDX_MODE]        = mode;
    led_data[RGB_FUSION_2_DRAM_IDX_BRIGHTNESS]  = brightness;
    led_data[RGB_FUSION_2_DRAM_IDX_RED]         = red;
    led_data[RGB_FUSION_2_DRAM_IDX_GREEN]       = green;
    led_data[RGB_FUSION_2_DRAM_IDX_BLUE]        = blue;

    if(truncate_packet)
    {
        bus->i2c_smbus_write_block_data(dev, RGB_FUSION_2_DRAM_LED_START_ADDR, 15, led_data);
    }
    else
    {
        bus->i2c_smbus_write_block_data(dev, RGB_FUSION_2_DRAM_LED_START_ADDR, 32, led_data);
    }

    Apply();
}
