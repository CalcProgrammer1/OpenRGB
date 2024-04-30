/*-----------------------------------------*\
|  RGBController_MSIMysticLight112.h        |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  MSI Mystic Light (112-byte) USB Driver   |
|                                           |
|  thombo 12/17/2022                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "MSIMysticLight112Controller.h"
#include <vector>

class RGBController_MSIMysticLight112: public RGBController
{
public:
    RGBController_MSIMysticLight112(MSIMysticLight112Controller* controller_ptr);
    ~RGBController_MSIMysticLight112();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    void        SetupModes();
    void        UpdateLed
                    (
                    int zone,
                    int led
                    );
    void        SetupMode
                    (
                    const char      *name,
                    MSI_MODE        mode,
                    unsigned int    flags
                    );
    int         GetDeviceMode();
    void        GetDeviceConfig();

    MSIMysticLight112Controller*    controller;
};
