/*-----------------------------------------*\
|  RGBController_LogitechG915.h             |
|                                           |
|  Generic RGB Interface for Logitech G915  |
|  RGB Mechanical Gaming Keyboard           |
|                                           |
|  Cheerpipe      3/20/2021                 |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechG915Controller.h"

class RGBController_LogitechG915 : public RGBController
{
public:
    RGBController_LogitechG915(LogitechG915Controller* logitech_ptr);
    ~RGBController_LogitechG915();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechG915Controller* logitech;
    std::vector<RGBColor>   current_colors;
};
