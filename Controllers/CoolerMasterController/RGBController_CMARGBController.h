/*-------------------------------------------------------------------*\
|  RGBController_CMARGBController.h                                   |
|                                                                     |
|  Driver for Coolermaster ARGB Controller                            |
|                                                                     |
|  Chris M (Dr_No)          14th Oct 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CMARGBcontroller.h"
#include <vector>

#define first_zone(zn)          ((zones.size() > 1) ? 1 : 0)
#define last_zone(zn)           ((zones.size() > 1) ? 4 : 1)

class RGBController_CMARGBController : public RGBController
{
public:
    RGBController_CMARGBController(CMARGBController* cmargb_ptr);
    ~RGBController_CMARGBController();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    void        Init_Controller();
    int         GetDeviceMode();
    int         GetLED_Zone(int led_idx);

    CMARGBController* cmargb;
};
