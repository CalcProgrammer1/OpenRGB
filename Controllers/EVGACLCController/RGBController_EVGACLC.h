/*---------------------------------------------------------*\
| RGBController_EVGACLC.h                                   |
|                                                           |
|   RGBController for EVGA CLC liquid coolers               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGACLCController.h"

class RGBController_EVGACLC : public RGBController
{
public:
    RGBController_EVGACLC(EVGACLCController* controller_ptr, const std::string& detector_name);
    ~RGBController_EVGACLC();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    EVGACLCController*  controller;
    std::string         det_name;
    unsigned char       cal_r;
    unsigned char       cal_g;
    unsigned char       cal_b;

    void        InitModes();
    void        LoadCalibration();
    void        SaveCalibration();
    unsigned char CalibrateChannel(unsigned char value, unsigned char scale);
};
