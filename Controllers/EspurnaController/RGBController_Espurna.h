/*-----------------------------------------*\
|  RGBController_Espurna.h                  |
|                                           |
|  Generic RGB Interface for Espurna        |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/11/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "EspurnaController.h"

class RGBController_Espurna : public RGBController
{
public:
    RGBController_Espurna(EspurnaController* espurna_ptr);
    ~RGBController_Espurna();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    EspurnaController* espurna;
};
