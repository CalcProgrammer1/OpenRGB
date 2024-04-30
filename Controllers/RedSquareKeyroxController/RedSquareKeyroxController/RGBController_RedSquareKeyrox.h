/*---------------------------------------------*\
|  RGBController_RedSquareKeyrox.h              |
|                                               |
|  Driver for Red Square Keyrox USB Controller  |
|                                               |
|  cafeed28          3 Nov 2022                 |
\*---------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RedSquareKeyroxController.h"

class RGBController_RedSquareKeyrox : public RGBController
{
public:
    RGBController_RedSquareKeyrox(RedSquareKeyroxController* controller_ptr);
    ~RGBController_RedSquareKeyrox();

    void                SetupZones();
    void                ResizeZone(int zone, int new_size);

    void                DeviceUpdateLEDs();
    void                UpdateZoneLEDs(int zone);
    void                UpdateSingleLED(int led);

    void                DeviceUpdateMode();

private:
    RedSquareKeyroxController* controller;
};
