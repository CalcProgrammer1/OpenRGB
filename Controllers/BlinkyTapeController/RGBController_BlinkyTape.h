/*---------------------------------------------------------*\
|  RGBController_BlinkyTape.h                               |
|                                                           |
|  Generic RGB Interface for BlinkyTape Led controller      |
|                                                           |
|  Matt Mets (matt@blinkinlabs.com), 07/01/2021             |
\*---------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "serial_port.h"
#include "BlinkyTapeController.h"

class RGBController_BlinkyTape : public RGBController
{
public:
    RGBController_BlinkyTape(BlinkyTapeController* controller_ptr);
    ~RGBController_BlinkyTape();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    BlinkyTapeController*   controller;
};
