/*---------------------------------------------------------*\
| RGBController_HYTEMousemat.h                              |
|                                                           |
|   RGBController for HYTE mousemat                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      18 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"

#if defined(_WIN32) || defined(__APPLE__)
#include "HYTEMousematController_Windows_MacOS.h"
#endif

#if defined(__FreeBSD__) || defined(__linux__)
#include "HYTEMousematController_FreeBSD_Linux.h"
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HYTEMousematController*     controller;
};
