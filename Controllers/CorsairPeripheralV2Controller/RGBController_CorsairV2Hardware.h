/*---------------------------------------------------------------------*\
|  RGBController_CorsairV2HardwareController.h                          |
|                                                                       |
|  Driver for the newer Corsair peripherals that use the '08'           |
|    based USB protocol and support hardware lighting modes.            |
|                                                                       |
|  Chris M (Dr_No)          10 Dec 2022                                 |
\*---------------------------------------------------------------------*/
#pragma once

#include "RGBController.h"
#include "CorsairPeripheralV2Controller.h"
#include "CorsairPeripheralV2HardwareController.h"

class RGBController_CorsairV2HW : public RGBController
{
public:
    RGBController_CorsairV2HW(CorsairPeripheralV2Controller* controller_ptr);
    ~RGBController_CorsairV2HW();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();
    void KeepaliveThread();

private:
    CorsairPeripheralV2Controller*          controller;

    RGBColor                                null_color              = 0;
    std::vector<RGBColor *>                 buffer_map;

    std::thread*                            keepalive_thread;
    std::atomic<bool>                       keepalive_thread_run;
    std::chrono::time_point
        <std::chrono::steady_clock>         last_update_time;

};
