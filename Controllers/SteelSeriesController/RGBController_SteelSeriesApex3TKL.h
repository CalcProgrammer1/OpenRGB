/*-----------------------------------------*\
|  RGBController_SteelSeriesApex3TKL.h      |
|                                           |
|  Paul K. Gerke - 27.10.2022               |
|  Dr_no (Chris M)                          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesApex3TKLController.h"

enum class APEX3_TKL_MODES
{
    DIRECT = 0,
    RAINBOW = 1
};

class RGBController_SteelSeriesApex3TKL : public RGBController
{
public:
    RGBController_SteelSeriesApex3TKL(SteelSeriesApex3TKLController* apex_tzone_ptr);
    ~RGBController_SteelSeriesApex3TKL();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
private:
    SteelSeriesApex3TKLController*     controller;
    unsigned int                       current_brightness;

    void        UpdateBrightness();
};
