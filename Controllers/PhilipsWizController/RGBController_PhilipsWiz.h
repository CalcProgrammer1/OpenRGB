/*-----------------------------------------*\
|  RGBController_PhilipsWiz.h               |
|                                           |
|  Generic RGB Interface for Philips Wiz    |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/3/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "PhilipsWizController.h"

class RGBController_PhilipsWiz : public RGBController
{
public:
    RGBController_PhilipsWiz(PhilipsWizController* light_ptr);
    ~RGBController_PhilipsWiz();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    PhilipsWizController* light;
};
