/*-----------------------------------------*\
|  RGBController_ASRockPolychromeSMBus.h    |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ASRock ASR LED and Polychrome RGB Driver |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/15/2019  |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ASRockPolychromeSMBusController.h"

class RGBController_Polychrome : public RGBController
{
public:
    RGBController_Polychrome(PolychromeController* polychrome_ptr);
    ~RGBController_Polychrome();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    PolychromeController* polychrome;
};
