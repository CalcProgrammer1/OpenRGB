/*---------------------------------------------------------*\
| RGBController_Network.h                                   |
|                                                           |
|   RGBController implementation that represents a remote   |
|   RGBController instance from a connected OpenRGB server  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NetworkClient.h"

class RGBController_Network : public RGBController
{
public:
    RGBController_Network(NetworkClient * client_ptr, unsigned int dev_idx_val);

    void        SetupZones();

    void        ClearSegments(int zone);
    void        AddSegment(int zone, segment new_segment);
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
    void        DeviceSaveMode();

    void        UpdateLEDs();

private:
    NetworkClient *     client;
    unsigned int        dev_idx;
};
