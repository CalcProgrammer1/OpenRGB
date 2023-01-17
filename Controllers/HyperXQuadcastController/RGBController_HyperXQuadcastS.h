/*-----------------------------------------*\
|  RGBController_HyperXQuadcastS.h          |
|                                           |
|  Implementation for the HyperX            |
|  Quadcast S RGB microphone                |
|                                           |
|  Matt Silva (thesilvanator) 2022          |
\*-----------------------------------------*/

#pragma once
#include <chrono>

#include "RGBController.h"
#include "HyperXQuadcastSController.h"

enum
{
    HXQS_MODE_DIRECT = 0,
    HXQS_MODE_OFF    = 1,
};

class RGBController_HyperXQuadcastS : public RGBController
{
public:
    RGBController_HyperXQuadcastS(HyperXQuadcastSController* controller_ptr);
    ~RGBController_HyperXQuadcastS();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

    void        KeepaliveThread();

private:
    HyperXQuadcastSController*                          controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
