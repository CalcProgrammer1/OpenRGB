#pragma once

#include "RGBController.h"
#include "RtkBridgeWrapper.h"

class RGBController_RtkBridge : public RGBController
{
public:
    RGBController_RtkBridge(RtkBridgeWrapper *_wrapper);
    ~RGBController_RtkBridge();

    void        SetupStrip();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    RtkBridgeWrapper *wrapper;
};
