/*-----------------------------------------*\
|  RGBController_ZETBladeOptical.cpp        |
|                                           |
|  Generic RGB Interface for ZET Blade      |
|  Optical Keyboard                         |
|                                           |
|  Based on HyperX Alloy Elite2 impl by     |
|                            KundaPanda     |
|                                           |
|  Moon_darker (Vaker) 23/01/2022           |
\*-----------------------------------------*/

#pragma once
#include <atomic>
#include <chrono>
#include <thread>

#include "RGBController.h"
#include "ZETBladeOpticalController.h"

#define ZET_BLADE_OPTICAL_ROWS           6
#define ZET_BLADE_OPTICAL_COLUMNS       22

class RGBController_ZETBladeOptical : public RGBController
{
public:
    RGBController_ZETBladeOptical(ZETBladeOpticalController* controller_ptr);
    ~RGBController_ZETBladeOptical();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ZETBladeOpticalController*                          controller;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
