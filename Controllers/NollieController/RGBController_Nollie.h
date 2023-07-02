/*-----------------------------------------*\
|  RGBController_Nollie.h                   |
|                                           |
|  Driver for Nollie32 LED strip controller |
|                                           |
|  Name (cnn1236661)          25th Jun 2023 |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NollieController.h"

#define NOLLIE_CHANNELS_LED_NUM  256
#define NOLLIE_CHANNELS_NUM      32
#define NOLLIE_32_PID            0x4714
#define NOLLIE_32_VID            0x3061

class RGBController_Nollie : public RGBController
{
public:
    RGBController_Nollie(NollieController* controller_ptr);
    ~RGBController_Nollie();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    NollieController*           controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
