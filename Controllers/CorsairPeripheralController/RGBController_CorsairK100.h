/*-----------------------------------------*\
|  RGBController_CorsairK100.cpp            |
|                                           |
|  Driver for Corsair K100 Keyboard         |
\*-----------------------------------------*/

#ifndef RGBCONTROLLER_CORSAIRK100_H
#define RGBCONTROLLER_CORSAIRK100_H

#include "RGBController.h"
#include "CorsairK100Controller.h"

class RGBController_CorsairK100 : public RGBController
{
public:
    RGBController_CorsairK100(CorsairK100Controller* controller_ptr);
    ~RGBController_CorsairK100();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    void KeepaliveThread();

private:
    CorsairK100Controller*                              controller;
    CorsairKeyboardType                                 logical_layout;

    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;

};

#endif // RGBCONTROLLER_CORSAIRK100_H
