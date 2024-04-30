/*-------------------------------------------------------------------*\
|  RGBController_CMRGBController.h                                    |
|                                                                     |
|  Driver for Coolermaster RGB USB Controller                         |
|                                                                     |
|  Nic W (midgetspy)        13th Apr 2021                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CMRGBController.h"
#include <vector>

class RGBController_CMRGBController : public RGBController
{
public:
    RGBController_CMRGBController(CMRGBController* controller_ptr);
    ~RGBController_CMRGBController();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    CMRGBController*    controller;
    void                LoadConfigFromDeviceController(int device_mode);
    void                ReadAllModeConfigsFromDevice();
};
