/*------------------------------------------*\
|  RGBController_SinowealthKeyboard.h        |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard, Hopefully generic, this was     |
|  made spefically for FL eSports F11 KB     |
|                                            |
|  Dmitri Kalinichenko (Dima-Kal) 23/06/2021 |
\*-----------------------------------------=*/

#pragma once

#include "RGBController.h"
#include "SinowealthKeyboardController.h"

class RGBController_SinowealthKeyboard : public RGBController
{
public:
    RGBController_SinowealthKeyboard(SinowealthKeyboardController* controller_ptr);
    ~RGBController_SinowealthKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SinowealthKeyboardController* controller;
};
