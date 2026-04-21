#pragma once

#include "RGBController.h"
#include "RtkArgbWrapper.h"

class RGBController_RtkArgb : public RGBController
{
public:
    RGBController_RtkArgb(RtkArgbWrapper *_wrapper);
    ~RGBController_RtkArgb();

    void        SetupStrip();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int zone);
    void        DeviceUpdateMode();

private:
    RtkArgbWrapper *wrapper;
    std::vector<int> valid_grp;
};
