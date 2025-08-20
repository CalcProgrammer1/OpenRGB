/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2USB.h                     |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 USB       |
|   motherboard                                             |
|                                                           |
|   jackun                                      08 Jan 2020 |
|   megadjc                                     31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include "RGBController.h"
#include "GigabyteRGBFusion2USBController.h"
#include "RGBController_GigabyteRGBFusion2USBBoards.h"
#include "RGBController_GigabyteRGBFusion2USBLayouts.h"

#define RGBFUSION2_DIGITAL_LEDS_MIN         0
#define RGBFUSION2_DIGITAL_LEDS_MAX         1024
#define RGBFUSION2_BRIGHTNESS_MIN           0
#define RGBFUSION2_BRIGHTNESS_MAX           255
#define RGBFUSION2_SPEED_MIN                9
#define RGBFUSION2_SPEED_MID                4
#define RGBFUSION2_SPEED_MAX                0

template<typename K, typename V>
static std::map<V, K> reverse_map(const std::map<K, V>& map)
{
    std::map<V, K> reversed_map;

    for(const std::pair<K, V> entry : map)
    {
        reversed_map[entry.second] = entry.first;
    }

    return reversed_map;
}

class RGBController_RGBFusion2USB: public RGBController
{
public:
    RGBController_RGBFusion2USB(RGBFusion2USBController* controller_ptr, std::string _detector_name);
    ~RGBController_RGBFusion2USB();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MBName                      MBName2Layout;
    bool                        custom_layout;
    std::string                 detector_name;

    RGBFusion2USBController*    controller;
    int                         device_num;
    ZoneLeds                    layout;
    RGBColor                    null_color      = 0;
    uint16_t                    pid;

    void        Load_Device_Config();
    void        Init_Controller();
    int         GetLED_Zone(int led_idx);
};
