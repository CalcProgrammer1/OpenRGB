#pragma once
#include "RGBController.h"
#include "LogitechG203LController.h"

class RGBController_LogitechG203L : public RGBController
{
public:
    RGBController_LogitechG203L(LogitechG203LController* logitech_ptr);
    ~RGBController_LogitechG203L();

    void        SetupZones();
    
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechG203LController*    logitech;
};
