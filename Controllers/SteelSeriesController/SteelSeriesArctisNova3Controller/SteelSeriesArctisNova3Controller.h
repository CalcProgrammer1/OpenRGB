/*---------------------------------------------------------*\
| SteelSeriesArctisNova3Controller.h                        |
|                                                           |
|   Driver for SteelSeries Arctis Nova 3                    |
|                                                           |
|   Tomasz Gorczyca (eldiablo123)               18 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/
#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define ARCTIS_NOVA3_REPORT_SIZE           521
#define ARCTIS_NOVA3_APPLY_REPORT_SIZE     64
#define ARCTIS_NOVA3_REPORT_ID             0x06

enum
{
    ARCTIS_NOVA3_MODE_OFF                  = 0x00,
    ARCTIS_NOVA3_MODE_DIRECT               = 0x01,
    ARCTIS_NOVA3_MODE_BREATHE              = 0x02,
    ARCTIS_NOVA3_MODE_COLORSHIFT_RAINBOW   = 0x03,
    ARCTIS_NOVA3_MODE_COLORSHIFT_ORANGE    = 0x04,
    ARCTIS_NOVA3_MODE_COLORSHIFT_BLUE      = 0x05,
};

class SteelSeriesArctisNova3Controller
{
public:
    SteelSeriesArctisNova3Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~SteelSeriesArctisNova3Controller();

    std::string         GetDeviceLocation();
    std::string         GetNameString();
    std::string         GetSerialString();

    void                SetOff(unsigned char zone_id);
    void                Save(unsigned char zone_id);
    void                SetDirect(unsigned char zone_id, RGBColor color);
    void                SetBreathe(unsigned char zone_id, RGBColor color, unsigned char speed);
    void                SetColorShiftRainbow(unsigned char zone_id, unsigned char speed);
    void                SetColorShiftOrange(unsigned char zone_id, unsigned char speed);
    void                SetColorShiftBlue(unsigned char zone_id, unsigned char speed);

private:
    hid_device*         dev;
    std::string         location;
    std::string         name;

    void                SetSpeedBytes(unsigned char* usb_buf, unsigned char speed);
    void                SelectZone(unsigned char zone_id, unsigned char state);
    void                ApplyZone(unsigned char zone_id);
    void                SendEffectPacket(unsigned char* usb_buf);
};
