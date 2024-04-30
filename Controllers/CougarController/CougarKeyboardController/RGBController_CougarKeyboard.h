/*-------------------------------------------------------------------*\
|  RGBController_CougarKeyboardController.h                           |
|                                                                     |
|  Driver for CougarKeyboard USB Controller                           |
|                                                                     |
|  Chris M (DrNo)          5 Apr 2022                                 |
\*-------------------------------------------------------------------*/

#pragma once
#include "LogManager.h"
#include "RGBController.h"
#include "CougarKeyboardController.h"
#include <vector>

class RGBController_CougarKeyboard : public RGBController
{
public:
    RGBController_CougarKeyboard(CougarKeyboardController* controller_ptr);
    ~RGBController_CougarKeyboard();

    void                SetupZones();
    void                ResizeZone(int zone, int new_size);

    void                DeviceUpdateLEDs();
    void                UpdateZoneLEDs(int zone);
    void                UpdateSingleLED(int led);

    void                DeviceUpdateMode();
    void                DeviceSaveMode();

private:
    int                 GetDeviceMode();
    int                 GetLED_Zone(int led_idx);

    CougarKeyboardController*      controller;
};
