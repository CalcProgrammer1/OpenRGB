/*-----------------------------------------*\
|  RGBController_MSIMysticLight162.h        |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  MSI Mystic Light (162-byte) USB Driver   |
|                                           |
|  T-bond 3/4/2020                          |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "MSIMysticLight162Controller.h"
#include <vector>

class RGBController_MSIMysticLight162: public RGBController
{
public:
    RGBController_MSIMysticLight162(MSIMysticLight162Controller* controller_ptr);
    ~RGBController_MSIMysticLight162();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    void        SetupModes();
    void        UpdateLed(int zone, int led);
    void        SetupMode(const char *name, MSI_MODE mode, unsigned int flags);

    MSIMysticLight162Controller*    controller;
};
