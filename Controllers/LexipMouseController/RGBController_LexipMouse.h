/*-----------------------------------------*\
|  RGBController_LexipMouse.h               |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Lexip mouse RGB USB Driver               |
|                                           |
|  Guimard Morgan (morg) 2/21/2022          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LexipMouseController.h"

class RGBController_LexipMouse : public RGBController
{
public:
    RGBController_LexipMouse(LexipMouseController* controller_ptr);
    ~RGBController_LexipMouse();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LexipMouseController* controller;
};
