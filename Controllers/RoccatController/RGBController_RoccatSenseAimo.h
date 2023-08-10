/*-----------------------------------------*\
|  RGBController_RoccatSenseAimo.h          |
|                                           |
|  Generic RGB Interface for Roccat Sense   |
|  Aimo controller                          |
|                                           |
|  Mola19 08/09/2023                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RoccatSenseAimoController.h"

class RGBController_RoccatSenseAimo : public RGBController
{
public:
    RGBController_RoccatSenseAimo(RoccatSenseAimoController* controller_ptr);
    ~RGBController_RoccatSenseAimo();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatSenseAimoController* controller;
};
