/*-----------------------------------------*\
|  HyperXPulsefireSurgeController.h         |
|                                           |
|  Definitions and types for HyperX         |
|  Pulsefire Surge lighting controller      |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/25/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class HyperXPulsefireSurgeController
{
public:
    HyperXPulsefireSurgeController(hid_device* dev_handle);
    ~HyperXPulsefireSurgeController();

    void SendData
            (
            RGBColor*   color_data
            );

private:
    hid_device*             dev;

    void SendWakeup();
};
