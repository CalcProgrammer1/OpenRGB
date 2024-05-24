/*---------------------------------------------------------*\
| RGBController_HYTEMousemat.h                              |
|                                                           |
|   RGBController for HYTE mousemat                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      18 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#ifdef _WIN32
#include "HYTEMousematController_Windows.h"
#else
#include "HYTEMousematController_Linux.h"
#endif

enum
{
    HYTE_CNVS_MODE_DIRECT           = 0,    /* Direct (streaming) mode                  */
    HYTE_CNVS_MODE_RAINBOW          = 1,    /* Rainbow wave (firmware animation) mode   */
};

class RGBController_HYTEMousemat : public RGBController
{
public:
    RGBController_HYTEMousemat(HYTEMousematController* controller_ptr);
    ~RGBController_HYTEMousemat();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HYTEMousematController*     controller;
};
