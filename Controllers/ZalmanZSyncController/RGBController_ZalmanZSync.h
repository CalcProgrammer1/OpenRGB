/*-----------------------------------------*\
|  RGBController_ZalmanZSync.h              |
|                                           |
|  Generic RGB Interface for Zalman Z Sync  |
|  lighting devices                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/30/2021   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "ZalmanZSyncController.h"

class RGBController_ZalmanZSync : public RGBController
{
public:
    RGBController_ZalmanZSync(ZalmanZSyncController* controller_ptr);
    ~RGBController_ZalmanZSync();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    ZalmanZSyncController*      controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
