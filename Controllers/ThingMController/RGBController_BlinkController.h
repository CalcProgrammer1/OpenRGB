/*-------------------------------------------------------------------*\
|  RGBController_BlinkController.h                                    |
|                                                                     |
|  Driver for ThingM Blink device                                     |
|                                                                     |
|  Eric S (edbgon)          1st Oct 2021                              |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "BlinkController.h"

class RGBController_BlinkController : public RGBController
{
public:
    RGBController_BlinkController(BlinkController* blink_ptr);
    ~RGBController_BlinkController();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
private:
    BlinkController* Blink;

    int         GetDeviceMode();
};
