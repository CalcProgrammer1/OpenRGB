/*-------------------------------------------------------------------*\
|  RGBController_CMMM711Controller.h                                  |
|                                                                     |
|  Driver for Coolermaster MM711 Controller                           |
|                                                                     |
|  Chris M (Dr_No)          14th Feb 2021                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CMMM711Controller.h"
#include <vector>

#define CM_MM_ARGB_BRIGHTNESS_MIN           0x00
#define CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT   0xFF
#define CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM  0x7F

class RGBController_CMMM711Controller : public RGBController
{
public:
    RGBController_CMMM711Controller(CMMM711Controller* cmargb_ptr);
    ~RGBController_CMMM711Controller();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
    void        DeviceSaveMode();
private:
    void        Init_Controller();
    int         GetDeviceMode();

    CMMM711Controller* cmmm711;
};
