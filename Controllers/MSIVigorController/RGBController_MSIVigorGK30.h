/*-----------------------------------------*\
|  RGBController_MSIVigorGK30.h             |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  MIS Vigor GK30 RGB USB Driver            |
|                                           |
|  Guimard Morgan (morg) 6/01/2022          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIVigorGK30Controller.h"

class RGBController_MSIVigorGK30 : public RGBController
{
public:
    RGBController_MSIVigorGK30(MSIVigorGK30Controller* controller_ptr);
    ~RGBController_MSIVigorGK30();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    MSIVigorGK30Controller* controller;
};
