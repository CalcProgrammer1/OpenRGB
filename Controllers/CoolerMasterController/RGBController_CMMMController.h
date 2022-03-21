/*-------------------------------------------------------------------*\
|  RGBController_CMMMController.h                                     |
|                                                                     |
|  Driver for Coolermaster MaterMouse Controller                      |
|                                                                     |
|  Chris M (Dr_No)          14th Feb 2021   -  MM711 Base             |
|  Dracrius                 12th Mar 2022                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CMMMController.h"
#include <vector>

#define CM_MM_ARGB_BRIGHTNESS_MIN           0x00
#define CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT   0xFF
#define CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM  0x7F

class RGBController_CMMMController : public RGBController
{
public:
    RGBController_CMMMController(CMMMController* controller_ptr);
    ~RGBController_CMMMController();

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

    int         leds_count;

    CMMMController*  controller;
};
