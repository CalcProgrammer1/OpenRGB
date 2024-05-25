/*---------------------------------------------------------*\
| RGBController_LianLiStrimerLConnect.h                     |
|                                                           |
|   RGBController for Lian Li Strimer L Connect             |
|                                                           |
|   Chris M (Dr_No)                             03 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "LogManager.h"
#include "RGBController.h"
#include "LianLiStrimerLConnectController.h"

class RGBController_LianLiStrimerLConnect : public RGBController
{
public:
    RGBController_LianLiStrimerLConnect(LianLiStrimerLConnectController* controller_ptr);
    ~RGBController_LianLiStrimerLConnect();

    void                SetupZones();
    void                ResizeZone(int zone, int new_size);

    void                DeviceUpdateLEDs();
    void                UpdateZoneLEDs(int zone);
    void                UpdateSingleLED(int led);

    void                DeviceUpdateMode();

private:
    void                Init_Controller();
    int                 GetDeviceMode();
    int                 GetLED_Zone(int led_idx);

    mode                CreateMode(std::string name, int value, uint8_t colour_count, uint8_t colour_mode);
    bool                TimeToSend();

    LianLiStrimerLConnectController*      controller;
    std::chrono::time_point<std::chrono::steady_clock> last_commit_time;
};
