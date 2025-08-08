/*---------------------------------------------------------*\
| RGBController_EVisionV2Keyboard.h                         |
|                                                           |
|   RGBController for EVision V2 keyboard                   |
|                                                           |
|   Le Philousophe                              25 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "LogManager.h"
#include "RGBController.h"
#include "EVisionV2KeyboardController.h"

class RGBController_EVisionV2Keyboard : public RGBController
{
public:
    RGBController_EVisionV2Keyboard(EVisionV2KeyboardController* controller_ptr, EVisionV2KeyboardPart kb_part);
    ~RGBController_EVisionV2Keyboard();

    void SetupZones() override;
    void ResizeZone(int zone, int new_size) override;

    void DeviceUpdateLEDs() override;
    void UpdateZoneLEDs(int zone) override;
    void UpdateSingleLED(int led) override;

    void DeviceUpdateMode() override;
    void DeviceSaveMode() override;

private:
    void SetupKeyboardModes();
    void SetupLogoEdgeModes();
    void SetupEdgeModes();
    void LoadConfig();

    void KeepaliveThread();

    EVisionV2KeyboardController*                       controller;
    EVisionV2KeyboardPart                              part;
    EVisionV2KeyboardLayout                            layout;

    std::thread*                                       keepalive_thread;
    std::atomic<bool>                                  keepalive_thread_run;
    std::atomic<bool>                                  has_color_set;
    std::chrono::time_point<std::chrono::steady_clock> last_update_time;
};
