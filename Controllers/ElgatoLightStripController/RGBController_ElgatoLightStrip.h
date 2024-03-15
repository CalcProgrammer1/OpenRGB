/*--------------------------------------------------------*\
|  RGBController_ElgatoLightStrip.h                        |
|                                                          |
|  Generic RGB Interface for Elgato LightStrip             |
|                                                          |
|  Monks (imtherealestmonkey@gmail.com)         11/01/2021 |
|  Edit by DomePlaysHD                          12/03/2024 |
\*--------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ElgatoLightStripController.h"

class RGBController_ElgatoLightStrip : public RGBController
{
    public:
        RGBController_ElgatoLightStrip(ElgatoLightStripController* controller_ptr);
        ~RGBController_ElgatoLightStrip();

        void SetupZones();

        void ResizeZone(int zone, int new_size);

        void DeviceUpdateLEDs();
        void UpdateZoneLEDs(int zone);
        void UpdateSingleLED(int led);

        void DeviceUpdateMode();

    private:
        ElgatoLightStripController* controller;
};
