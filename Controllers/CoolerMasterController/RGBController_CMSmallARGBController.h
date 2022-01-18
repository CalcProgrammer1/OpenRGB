/*-------------------------------------------------------------------*\
|  RGBController_CMSmallARGBController.h                              |
|                                                                     |
|  Driver for Coolermaster Small ARGB USB Controller                  |
|                                                                     |
|  Chris M (Dr_No)          31st Jan 2021                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CMSmallARGBController.h"
#include <vector>

#define CM_SMALL_ARGB_MIN_LEDS          4
#define CM_SMALL_ARGB_MAX_LEDS          48
#define CM_SMALL_ARGB_BRIGHTNESS_MAX    0xFF
#define CM_SMALL_ARGB_FW0012            "A202104052336"

class RGBController_CMSmallARGBController : public RGBController
{
public:
    RGBController_CMSmallARGBController(CMSmallARGBController* controller_ptr);
    ~RGBController_CMSmallARGBController();

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

    CMSmallARGBController*  controller;
};
