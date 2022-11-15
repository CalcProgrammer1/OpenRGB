/*--------------------------------------------------*\
|  RGBController_ElgatoKeyLight.h                   |
|                                                   |
|  Generic RGB Interface for Elgato KeyLight        |
|                                                   |
|  Monks (imtherealestmonkey@gmail.com) 11/1/2021   |
\*--------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "ElgatoKeyLightController.h"

class RGBController_ElgatoKeyLight : public RGBController
{
public:
    RGBController_ElgatoKeyLight(ElgatoKeyLightController* controller_ptr);
    ~RGBController_ElgatoKeyLight();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ElgatoKeyLightController* controller;
};
