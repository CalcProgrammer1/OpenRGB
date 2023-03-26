/*-----------------------------------------*\
|  RGBController_HyperXMicrophone.h         |
|                                           |
|  Implementation for the HyperX            |
|  Quadcast S RGB microphone                |
|                                           |
|  Matt Silva (thesilvanator) 2022          |
\*-----------------------------------------*/

#pragma once
#include <chrono>

#include "RGBController.h"
#include "HyperXMicrophoneController.h"

class RGBController_HyperXMicrophone : public RGBController
{
public:
    RGBController_HyperXMicrophone(HyperXMicrophoneController* controller_ptr);
    ~RGBController_HyperXMicrophone();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

    void        KeepaliveThread();

private:
    HyperXMicrophoneController*                         controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
