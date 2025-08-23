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
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include "RGBController.h"
#include "GigabyteFusion2USB_Devices.h"
#include "GigabyteRGBFusion2USBController.h"
#include "SettingsManager.h"

#define RGBFUSION2_DIGITAL_LEDS_MIN         0
#define RGBFUSION2_DIGITAL_LEDS_MAX         1024
#define RGBFUSION2_BRIGHTNESS_MIN           0
#define RGBFUSION2_BRIGHTNESS_MAX           255
#define RGBFUSION2_SPEED_MIN                9
#define RGBFUSION2_SPEED_MID                4
#define RGBFUSION2_SPEED_MAX                0

#define GET_JSON_VAL_ELSE_OFF(obj, key) obj.contains(key) ? obj.at(key).get<std::string>() : std::string("OFF")

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

    void                        SetupZones();

    void                        ResizeZone(int zone, int new_size);

    void                        DeviceUpdateLEDs();
    void                        UpdateZoneLEDs(int zone);
    void                        UpdateSingleLED(int led);

    void                        DeviceUpdateMode();

private:
    std::string                 detector_name;

    RGBFusion2USBController*    controller;
    int                         device_num;
    RGBColor                    null_color      = 0;
    /*---------------------------------------------------------*\
    | The intial value of device_index should point to the      |
    |   layout for the generic_device                           |
    \*---------------------------------------------------------*/
    uint32_t                    device_index    = 0;

    void                        Init_Controller();
    int                         GetLED_Zone(int led_idx);

    nlohmann::json              WriteCalJsonFrom(
                                    const EncodedCalibration& src);
    void                        FillMissingWith(
                                    nlohmann::json& dst,
                                    const EncodedCalibration& fb);
    nlohmann::json              BuildCustomLayoutJson(
                                    const gb_fusion2_device* layout,
                                    const RvrseLedHeaders& reverseLookup);
    void                        LoadCustomLayoutFromJson(
                                    const nlohmann::json& json_custom,
                                    const FwdLedHeaders& forwardLookup,
                                    gb_fusion2_device* layout);
};
