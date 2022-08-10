/*-----------------------------------------*\
|  RGBController_DygmaRaise.h               |
|                                           |
|  RGB Interface for  DygmaRaise keyboard   |
|                                           |
|   Timo Schlegel (@eispalast) 12/12/2021   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "DygmaRaiseController.h"

class RGBController_DygmaRaise : public RGBController
{
public:
    RGBController_DygmaRaise(DygmaRaiseController* controller_ptr);
    ~RGBController_DygmaRaise();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    DygmaRaiseController*   controller;
};
