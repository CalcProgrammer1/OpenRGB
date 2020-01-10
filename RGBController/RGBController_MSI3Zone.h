/*-----------------------------------------*\
|  RGBController_MSI3Zone.h                 |
|                                           |
|  Generic RGB Interface for MSI/Steelseries|
|  3-Zone Keyboard                          |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "MSI3ZoneController.h"

class RGBController_MSI3Zone : public RGBController
{
public:
    RGBController_MSI3Zone(MSI3ZoneController* msi_ptr);
    ~RGBController_MSI3Zone();
    int         GetMode();
    void        SetMode(int mode);
    void        SetCustomMode();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

private:
    MSI3ZoneController*   msi;
};