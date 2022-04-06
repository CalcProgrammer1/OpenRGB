/*-----------------------------------------*\
|  RGBController_RoccatVulcanKeyboard.h     |
|                                           |
|  Generic RGB Interface for Roccat Vulcan  |
|  Keyboard controller                      |
|                                           |
|  Mola19 17/12/2021                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RoccatVulcanKeyboardController.h"

class RGBController_RoccatVulcanKeyboard : public RGBController
{
public:
    RGBController_RoccatVulcanKeyboard(RoccatVulcanKeyboardController* controller_ptr);
    ~RGBController_RoccatVulcanKeyboard();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatVulcanKeyboardController* controller;
    uint16_t                    pid;
};
