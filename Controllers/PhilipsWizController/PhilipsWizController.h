/*---------------------------------------------------------*\
| PhilipsWizController.h                                    |
|                                                           |
|   Driver for Philips Wiz                                  |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      03 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <thread>
#include <vector>
#include "RGBController.h"
#include "net_port.h"

#define PHILIPSWIZ_BRIGHTNESS_MAX   100
#define PHILIPSWIZ_BRIGHTNESS_MIN   10

enum
{
    PHILLIPSWIZ_MODE_STATIC        = 0,
    PHILLIPSWIZ_MODE_OCEAN         = 1,
    PHILLIPSWIZ_MODE_ROMANCE       = 2,
    PHILLIPSWIZ_MODE_SUNSET        = 3,
    PHILLIPSWIZ_MODE_PARTY         = 4,
    PHILLIPSWIZ_MODE_FIREPLACE     = 5,
    PHILLIPSWIZ_MODE_COZY          = 6,
    PHILLIPSWIZ_MODE_FOREST        = 7,
    PHILLIPSWIZ_MODE_PASTEL_COLORS = 8,
    PHILLIPSWIZ_MODE_WAKE_UP       = 9,
    PHILLIPSWIZ_MODE_BEDTIME       = 10,
    PHILLIPSWIZ_MODE_WARM_WHITE    = 11,
    PHILLIPSWIZ_MODE_DAYLIGHT      = 12,
    PHILLIPSWIZ_MODE_COOL_WHITE    = 13,
    PHILLIPSWIZ_MODE_NIGHT_LIGHT   = 14,
    PHILLIPSWIZ_MODE_FOCUS         = 15,
    PHILLIPSWIZ_MODE_RELAX         = 16,
    PHILLIPSWIZ_MODE_TRUE_COLORS   = 17,
    PHILLIPSWIZ_MODE_TV_TIME       = 18,
    PHILLIPSWIZ_MODE_PLANTGROWTH   = 19,
    PHILLIPSWIZ_MODE_SPRING        = 20,
    PHILLIPSWIZ_MODE_SUMMER        = 21,
    PHILLIPSWIZ_MODE_FALL          = 22,
    PHILLIPSWIZ_MODE_DEEPDIVE      = 23,
    PHILLIPSWIZ_MODE_JUNGLE        = 24,
    PHILLIPSWIZ_MODE_MOJITO        = 25,
    PHILLIPSWIZ_MODE_CLUB          = 26,
    PHILLIPSWIZ_MODE_CHRISTMAS     = 27,
    PHILLIPSWIZ_MODE_HALLOWEEN     = 28,
    PHILLIPSWIZ_MODE_CANDLELIGHT   = 29,
    PHILLIPSWIZ_MODE_GOLDEN_WHITE  = 30,
    PHILLIPSWIZ_MODE_PULSE         = 31,
    PHILLIPSWIZ_MODE_STEAMPUNK     = 32
};

class PhilipsWizController
{
public:
    PhilipsWizController(std::string ip, bool use_cool, bool use_warm, std::string selected_white_strategy);
    ~PhilipsWizController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetModuleName();
    std::string GetManufacturer();
    std::string GetUniqueID();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char brightness);

    void SetScene(int scene, unsigned char brightness);

    void ReceiveThreadFunction();
    void RequestSystemConfig();

private:
    std::string         firmware_version;
    std::string         module_name;
    std::string         module_mac;
    std::string         location;
    net_port            port;
    std::thread*        ReceiveThread;
    std::atomic<bool>   ReceiveThreadRun;

    bool                use_cool_white;
    bool                use_warm_white;
    std::string         white_strategy;

    void SendSetPilot();
};
