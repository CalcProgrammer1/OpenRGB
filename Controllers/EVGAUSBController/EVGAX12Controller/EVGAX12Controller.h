/*---------------------------------------------------------*\
| EVGAX12Controller.h                                       |
|                                                           |
|   Driver for the EVGA X12 gaming mouse                    |
|                                                           |
|   QuickMythril                               08 Aug 2026  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <array>
#include <mutex>
#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define EVGA_X12_REPORT_ID                  0x07
#define EVGA_X12_REPORT_SIZE                40
#define EVGA_X12_LED_COUNT                  3
#define EVGA_X12_FIRMWARE_SLOT_COUNT        4
#define EVGA_X12_BRIGHTNESS_MIN             0
#define EVGA_X12_BRIGHTNESS_MAX             255

enum
{
    EVGA_X12_MODE_OFF                       = 0,
    EVGA_X12_MODE_STATIC                    = 1,
};

/*---------------------------------------------------------*\
| HEAD_LEFT drives both physical side lights. HEAD_RIGHT    |
| is writable and readable, but has no physical emitter.    |
\*---------------------------------------------------------*/
enum
{
    EVGA_X12_FIRMWARE_SLOT_HEAD_LEFT        = 0,
    EVGA_X12_FIRMWARE_SLOT_HEAD_RIGHT       = 1,
    EVGA_X12_FIRMWARE_SLOT_WHEEL            = 2,
    EVGA_X12_FIRMWARE_SLOT_LOGO             = 3,
};

struct EVGAX12LightingState
{
    std::array<unsigned char, EVGA_X12_LED_COUNT> modes;
    std::array<unsigned char, EVGA_X12_LED_COUNT> brightness;
    std::array<RGBColor, EVGA_X12_LED_COUNT>      colors;
    bool                                          modes_valid;
    bool                                          static_valid;
};

class EVGAX12Controller
{
public:
    EVGAX12Controller(hid_device* dev_handle, const hid_device_info& info, const std::string& dev_name);
    ~EVGAX12Controller();

    std::string             GetDeviceLocation();
    std::string             GetNameString();
    std::string             GetSerialString();

    EVGAX12LightingState    GetLightingState();
    bool                    SetMode(unsigned char mode);
    bool                    SetStatic(const std::vector<RGBColor>& colors, unsigned char brightness);
    bool                    SetStaticAndActivate(const std::vector<RGBColor>& colors, unsigned char brightness);

private:
    bool                    GetReport(unsigned char command, unsigned char* packet);
    bool                    SetModeReport(unsigned char mode);
    bool                    SetStaticReport(const std::vector<RGBColor>& colors, unsigned char brightness);
    bool                    SendReport(const unsigned char* packet);

    hid_device*             dev;
    std::string             location;
    std::string             name;
    std::mutex              device_mutex;
};
