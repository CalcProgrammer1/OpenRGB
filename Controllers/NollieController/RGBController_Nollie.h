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

#define NOLLIE_12_CH_LED_NUM       42
#define NOLLIE_8_CH_LED_NUM        126
#define NOLLIE_HS_CH_LED_NUM       256
#define NOLLIE_FS_CH_LED_NUM       525

#define NOLLIE32_CHANNELS_NUM      32
#define NOLLIE32_PID               0x4714
#define NOLLIE32_VID               0x3061

#define NOLLIE16_CHANNELS_NUM      16
#define NOLLIE16_PID               0x4716
#define NOLLIE16_VID               0x3061

#define NOLLIE8_CHANNELS_NUM       8
#define NOLLIE8_PID                0x1F01
#define NOLLIE8_VID                0x16D2

#define NOLLIE28_12_CHANNELS_NUM   12
#define NOLLIE28_12_VID            0x16D2
#define NOLLIE28_12_PID            0x1616
#define NOLLIE28_L1_PID            0x1617
#define NOLLIE28_L2_PID            0x1618

#define NOLLIE32_MOS_TRIGGER_CH    26
#define NOLLIE32_MOS_TRIGGER_LED   20
#define NOLLIE32_FLAG1_CHANNEL     15
#define NOLLIE32_FLAG2_CHANNEL     31


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

    int* channel_index;

    int ch32[32] = {5, 4, 3, 2, 1, 0, 15, 14, 26, 27, 28, 29, 30, 31, 8, 9, 19, 18, 17, 16, 7, 6, 25, 24, 23, 22, 21, 20, 13, 12, 11, 10};
    int ch16[32] = {19, 18, 17, 16, 24, 25, 26, 27, 20, 21, 22, 23, 31, 30, 29, 28, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
};
