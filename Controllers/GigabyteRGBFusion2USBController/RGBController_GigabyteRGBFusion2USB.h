/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2USB.h                     |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 USB       |
|   motherboard                                             |
|                                                           |
|   jackun                                      08 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include <vector>
#include "RGBController.h"
#include "GigabyteRGBFusion2USBController.h"

#define RGBFusion2_Digital_LEDS_Min         0;
#define RGBFusion2_Digital_LEDS_Max         1024;
#define RGBFUSION2_BRIGHTNESS_MIN           0;
#define RGBFUSION2_BRIGHTNESS_MAX           5;

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

typedef std::map< std::string, int > FwdLedHeaders;
typedef std::map< int, std::string > RvrseLedHeaders;

struct LedPort
{
    std::string name;
    int         header;
    int         count;
};

typedef std::map< std::string, std::string >            MBName;
typedef std::map< std::string, std::vector<LedPort> >   ZoneLeds;
typedef std::map< std::string, ZoneLeds>                KnownLayout;

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
    ZoneLeds                    layout;

    void        Load_Device_Config();
    void        Init_Controller();
    int         GetLED_Zone(int led_idx);
};
