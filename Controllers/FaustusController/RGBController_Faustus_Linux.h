/*---------------------------------------------------------*\
| RGBController_Faustus_Linux.h                             |
|                                                           |
|   RGBController for Faustus devices                       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <fstream>
#include "RGBController.h"

enum
{
    FAUSTUS_MODE_STATIC      = 0,
    FAUSTUS_MODE_BREATHING   = 1,
    FAUSTUS_MODE_COLOR_CYCLE = 2,
    FAUSTUS_MODE_STROBE      = 3
};
enum
{
    FAUSTUS_SPEED_SLOWEST = 0,
    FAUSTUS_SPEED_NORMAL  = 1,
    FAUSTUS_SPEED_FASTEST = 2,
};

class RGBController_Faustus : public RGBController
{
    private:
        std::string r_path;
        std::string g_path;
        std::string b_path;
        std::string mode_path;
        std::string flags_path;
        std::string set_path;

    public:
        RGBController_Faustus(const std::string& dev_path);

        void        SetupZones();

        void        DeviceUpdateLEDs();
        void        DeviceUpdateZoneLEDs(int zone);
        void        DeviceUpdateSingleLED(int led);

        void        DeviceUpdateMode();
};
