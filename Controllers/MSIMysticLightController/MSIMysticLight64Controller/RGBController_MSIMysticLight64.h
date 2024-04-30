#pragma once
#include "RGBController.h"
#include "MSIMysticLight64Controller.h"

class RGBController_MSIMysticLight64 : public RGBController
{
public:
    RGBController_MSIMysticLight64(MSIMysticLight64Controller* controller_ptr);
    ~RGBController_MSIMysticLight64();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size); 

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MSIMysticLight64Controller*       controller;
    void        SetupModes();
    void        SetupMode
                (
                const char      *name,
                MSI_64_MODE      mode,
                unsigned int     flags
                );
};