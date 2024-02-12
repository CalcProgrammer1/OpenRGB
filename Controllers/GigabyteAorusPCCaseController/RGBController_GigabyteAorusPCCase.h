/*------------------------------------------*\
|  RGBController_GigabyteAorusPCCase.h       |
|                                            |
|  Generic RGB Interface for for Gigabyte    |
|  Aorus PC Case driver                      |
|                                            |
|  Denis Nazarov (nenderus) 2/10/2024        |
\*------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteAorusPCCaseController.h"

class RGBController_GigabyteAorusPCCase : public RGBController
{
public:
    RGBController_GigabyteAorusPCCase(GigabyteAorusPCCaseController* controller_ptr);
    ~RGBController_GigabyteAorusPCCase();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GigabyteAorusPCCaseController* controller;
};
