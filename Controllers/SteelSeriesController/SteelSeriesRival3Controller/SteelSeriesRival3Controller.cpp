/*---------------------------------------------------------*\
| SteelSeriesRival3Controller.cpp                           |
|                                                           |
|   Driver for SteelSeries Rival 3                          |
|                                                           |
|   B Horn (bahorn)                             29 Aug 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "SteelSeriesRival3Controller.h"

SteelSeriesRival3Controller::SteelSeriesRival3Controller(hid_device* dev_handle, steelseries_type proto_type, const char* path, std::string dev_name) : SteelSeriesMouseController(dev_handle, proto_type, path, dev_name)
{

}

SteelSeriesRival3Controller::~SteelSeriesRival3Controller()
{
    hid_close(dev);
}

std::string SteelSeriesRival3Controller::GetFirmwareVersion()
{
    const uint8_t   FW_BUFFER_SIZE              = 3;
    uint8_t         usb_buf[FW_BUFFER_SIZE]     = { 0x00, 0x10, 0x00 };
    uint16_t        version;
    std::string     return_string;

    hid_write(dev, usb_buf, FW_BUFFER_SIZE);
    hid_read(dev, (unsigned char *)&version, 2);

    return_string = std::to_string(version);
    return return_string;
}

steelseries_mouse SteelSeriesRival3Controller::GetMouse()
{
    return rival_3;
}

void SteelSeriesRival3Controller::SetLightEffectAll(uint8_t effect)
{
    const uint8_t   EFFECT_BUFFER_SIZE          = 4;
    uint8_t         usb_buf[EFFECT_BUFFER_SIZE] = { 0x00, 0x06, 0x00, effect };

    hid_write(dev, usb_buf, EFFECT_BUFFER_SIZE);
}

void SteelSeriesRival3Controller::SetColor
    (
    unsigned char   zone_id,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue,
    unsigned char   brightness
    )
{
    const uint8_t   COLOR_BUFFER_SIZE           = 8;
    uint8_t         usb_buf[COLOR_BUFFER_SIZE];

    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = 0x05;
    usb_buf[0x02] = 0x00;
    usb_buf[0x03] = zone_id;

    usb_buf[0x04] = red;
    usb_buf[0x05] = green;
    usb_buf[0x06] = blue;
    usb_buf[0x07] = brightness;

    hid_write(dev, usb_buf, COLOR_BUFFER_SIZE);
}
