/*---------------------------------------------------------*\
| RGBController_HyperXAlloyOrigins60and65.h                 |
|                                                           |
|   RGBController for HyperX Alloy Origins 60 and 65        |
|   keyboard                                                |
|                                                           |
|   Derek Huber                                 18 Mar 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "HyperXAlloyOrigins60and65Controller.h"

enum AlloyOrigins60and65MappingLayoutType
{
    ALLOY_ORIGINS_60_LAYOUT,
    ALLOY_ORIGINS_65_LAYOUT
};

typedef struct
{
    const char*         name;
    const zone_type     type;
    const unsigned int  size;
    matrix_map_type*    matrix;
} led_zone;

class RGBController_HyperXAlloyOrigins60and65 : public RGBController
{
public:
    RGBController_HyperXAlloyOrigins60and65(HyperXAlloyOrigins60and65Controller* controller_ptr, AlloyOrigins60and65MappingLayoutType keyboard_layout);
    ~RGBController_HyperXAlloyOrigins60and65();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    HyperXAlloyOrigins60and65Controller*                controller;
    AlloyOrigins60and65MappingLayoutType                layout;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
