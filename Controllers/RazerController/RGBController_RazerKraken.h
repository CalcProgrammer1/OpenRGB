/*-----------------------------------------*\
|  RGBController_RazerKraken.h              |
|                                           |
|  Generic RGB Interface for Razer Kraken   |
|  devices                                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/28/2021   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RazerKrakenController.h"

enum
{
    RAZER_KRAKEN_MODE_DIRECT,
    RAZER_KRAKEN_MODE_OFF,
    RAZER_KRAKEN_MODE_STATIC,
    RAZER_KRAKEN_MODE_BREATHING,
    RAZER_KRAKEN_MODE_SPECTRUM_CYCLE,
};

class RGBController_RazerKraken : public RGBController
{
public:
    RGBController_RazerKraken(RazerKrakenController* controller_ptr);
    ~RGBController_RazerKraken();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    RazerKrakenController*  controller;
};
