/*-------------------------------------------------------------------*\
|  RGBController_DarkProjectKeyboard.h                                |
|                                                                     |
|  Driver for DarkProjectKeyboard USB Controller                      |
|                                                                     |
|  Chris M (DrNo)          8 Apr 2022                                 |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "LogManager.h"
#include "RGBController.h"
#include "DarkProjectKeyboardController.h"
#include <vector>

class RGBController_DarkProjectKeyboard : public RGBController
{
public:
    RGBController_DarkProjectKeyboard(DarkProjectKeyboardController* controller_ptr);
    ~RGBController_DarkProjectKeyboard();

    void    SetupZones();
    void    ResizeZone(int zone, int new_size);

    void    DeviceUpdateLEDs();
    void    UpdateZoneLEDs(int zone);
    void    UpdateSingleLED(int led);

    void    DeviceUpdateMode();

private:
    DarkProjectKeyboardController* controller;
};
