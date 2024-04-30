/*-------------------------------------------------------------------*\
|  RGBController_RoccatKonePro.h                                      |
|                                                                     |
|  Driver for Roccat Kone Pro Mouse                                   |
|                                                                     |
|  Garrett Denham (GardenOfWyers)  01/12/2024                         |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RoccatKoneProController.h"

class RGBController_RoccatKonePro : public RGBController
{
public:
    RGBController_RoccatKonePro(RoccatKoneProController* controller_ptr);
    ~RGBController_RoccatKonePro();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatKoneProController* controller;
};
