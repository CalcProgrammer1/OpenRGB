/*---------------------------------------------------------*\
| QMKOpenRGBBaseController.h                                |
|                                                           |
|   Common Driver for OpenRGB QMK Keyboard Protocol         |
|                                                           |
|   ChrisM                                      20 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "LogManager.h"
#include "RGBController.h"
#include "RGBControllerKeyNames.h"
#include "SettingsManager.h"
#include "QMKOpenRGBController.h"

typedef std::map<uint8_t, std::string>  qmk_keycodes;
extern qmk_keycodes                     qmk_keycode_keyname_map;

class QMKOpenRGBBaseController
{
public:
    QMKOpenRGBBaseController(hid_device *dev_handle, const char *path, unsigned char max_led_count);
    virtual ~QMKOpenRGBBaseController();

    std::string                 GetLocation();
    std::string                 GetDeviceName();
    std::string                 GetDeviceVendor();

    unsigned int                GetTotalNumberOfLEDs();
    unsigned int                GetTotalNumberOfLEDsWithEmptySpace();
    unsigned int                GetMode();
    unsigned int                GetModeSpeed();
    unsigned int                GetModeColor();

    std::vector<point_t>        GetLEDPoints();
    std::vector<unsigned int>   GetLEDFlags();
    std::vector<std::string>    GetLEDNames();
    std::vector<RGBColor>       GetLEDColors();

    unsigned int                GetProtocolVersion();
    std::string                 GetQMKVersion();
    void                        GetDeviceInfo();
    void                        GetModeInfo();

    void                        SetMode(hsv_t hsv_color, unsigned char mode, unsigned char speed);
    void                        SetMode(hsv_t hsv_color, unsigned char mode, unsigned char speed, bool save);

    virtual void                GetLEDInfo(unsigned int leds_count)                                                                     = 0;
    virtual void                DirectModeSetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)    = 0;
    virtual void                DirectModeSetLEDs(std::vector<RGBColor> colors, unsigned int leds_count)                                = 0;

protected:
    hid_device                  *dev;

    unsigned int                leds_per_update;

    std::string                 location;

    std::string                 device_name;
    std::string                 device_vendor;

    std::chrono::milliseconds   delay;

    unsigned int                total_number_of_leds;
    unsigned int                total_number_of_leds_with_empty_space;
    unsigned int                mode;
    unsigned int                mode_speed;

    RGBColor                    mode_color;

    std::vector<point_t>        led_points;
    std::vector<unsigned int>   led_flags;
    std::vector<std::string>    led_names;
    std::vector<RGBColor>       led_colors;

private:
};
