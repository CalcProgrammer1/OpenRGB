/*---------------------------------------------------------*\
| Lenovo4ZoneUSBController.cpp                              |
|                                                           |
|   Driver for Lenovo 4-Zone devices                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iostream>
#include "Lenovo4ZoneUSBController.h"
#include "LogManager.h"
#include "StringUtils.h"

Lenovo4ZoneUSBController::Lenovo4ZoneUSBController(hid_device* dev_handle, const char* path, uint16_t in_pid, std::string dev_name)
{
    dev                     = dev_handle;
    location                = path;
    pid                     = in_pid;
    name                    = dev_name;
}

Lenovo4ZoneUSBController::~Lenovo4ZoneUSBController()
{
    hid_close(dev);
}

void Lenovo4ZoneUSBController::setMode(const KeyboardState &in_mode)
{
    uint8_t buffer[LENOVO_4_ZONE_HID_PACKET_SIZE] =
    {
        in_mode.header[0], in_mode.header[1],
        in_mode.effect,
        in_mode.speed,
        in_mode.brightness,
        in_mode.zone0_rgb[0], in_mode.zone0_rgb[1], in_mode.zone0_rgb[2],
        in_mode.zone1_rgb[0], in_mode.zone1_rgb[1], in_mode.zone1_rgb[2],
        in_mode.zone2_rgb[0], in_mode.zone2_rgb[1], in_mode.zone2_rgb[2],
        in_mode.zone3_rgb[0], in_mode.zone3_rgb[1], in_mode.zone3_rgb[2],
        0x00,
        in_mode.wave_ltr, in_mode.wave_rtl
    };
    hid_send_feature_report(dev, buffer, LENOVO_4_ZONE_HID_PACKET_SIZE);
}

uint16_t Lenovo4ZoneUSBController::getPid()
{
    return pid;
}

std::string Lenovo4ZoneUSBController::getName()
{
    return name;
}

std::string Lenovo4ZoneUSBController::getLocation()
{
    return location;
}
