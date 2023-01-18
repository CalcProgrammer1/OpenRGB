/*-----------------------------------------*\
|  RGBController_ASRockPolychromeUSB.h      |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ASRock Polychrome USB Driver             |
|                                           |
|  Ed Kambulow (Dredvard) 12/26/2020        |
|  Shady Nawara (ShadyNawara) 01/16/2023    |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ASRockPolychromeUSBController.h"

class RGBController_PolychromeUSB : public RGBController
{
public:
    RGBController_PolychromeUSB(PolychromeUSBController* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    bool                            initializedMode;
    PolychromeUSBController*        controller;
    std::vector<PolychromeZoneInfo> zones_info;

    unsigned char GetDeviceMode(unsigned char zone);
};
