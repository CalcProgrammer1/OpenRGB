/*---------------------------------------------------------*\
| RGBController_LightSalt.h                                 |
|                                                           |
|   RGBController for LightSalt Peripherals                 |
|                                                           |
|   James Buren (braewoods)                     23 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LightSaltController.h"

class RGBController_LightSalt : public RGBController
{
public:
    ~RGBController_LightSalt();

    void SetupData(LightSaltController* controller_ptr);
    void SetupModes();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateColors(bool save);
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();
    void DeviceSaveMode();

protected:
    struct
    {
        struct
        {
            std::string name;
            int type;
        } device;
        struct
        {
            int sets;
            int rows;
            int columns;
            int count;
            char const * const * names;
            const int* indices;
            const int* deflections;
        } led;
        struct
        {
            unsigned int height;
            unsigned int width;
            unsigned int* matrix;
        } map;
    } table;
    LightSaltController* controller;
};
