/*-----------------------------------------*\
|  RGBController_AsusAuraSMBus.h            |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Asus Aura SMBus driver                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/13/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraSMBusController.h"

class RGBController_AuraSMBus : public RGBController
{
public:
    RGBController_AuraSMBus(AuraSMBusController* aura_ptr);
    ~RGBController_AuraSMBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    AuraSMBusController* aura;

    int         GetDeviceMode();
};
