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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SinowealthKeyboardController* controller;
};
