#pragma once
#include "RGBController.h"
#include "CreativeSoundBlasterXG6Controller.h"

class RGBController_CreativeSoundBlasterXG6: public RGBController
{
public:
    RGBController_CreativeSoundBlasterXG6(CreativeSoundBlasterXG6Controller* creative_device);
    ~RGBController_CreativeSoundBlasterXG6();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    CreativeSoundBlasterXG6Controller*   creative;
};
