/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2USB.h                     |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 USB       |
|   motherboard                                             |
|                                                           |
|   jackun                                      08 Jan 2020 |
|   megadjc                                     03 Sep 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteFusion2USB_Devices.h"
#include "GigabyteRGBFusion2USBController.h"

#define RGBFUSION2_BRIGHTNESS_MIN           0
#define RGBFUSION2_BRIGHTNESS_MAX           255
#define RGBFUSION2_SPEED_MIN                9
#define RGBFUSION2_SPEED_MID                4
#define RGBFUSION2_SPEED_MAX                0

class RGBController_RGBFusion2USB: public RGBController
{
public:
    RGBController_RGBFusion2USB(RGBFusion2USBController* controller_ptr, std::string _detector_name);
    ~RGBController_RGBFusion2USB();

    void                        SetupZones();

    void                        DeviceConfigureZone(int zone_idx);

    void                        DeviceUpdateLEDs();
    void                        DeviceUpdateZoneLEDs(int zone);
    void                        DeviceUpdateSingleLED(int led);

    void                        DeviceUpdateMode();
    void                        DeviceUpdateZoneMode(int zone);
    void                        DeviceSaveMode() override;
    void                        DeviceConfigureDevice() override;
    void                        DeviceUpdateDeviceSpecificConfiguration() override;

private:
    std::string                 detector_name;

    RGBFusion2USBController*    controller;
    uint8_t                     device_num;
    uint8_t                     fw_id           = 0;
    RGBColor                    null_color      = 0;
    bool                        supports_gen2   = 0;
    uint8_t                     gen2_enabled_headers = 0;
    bool                        entire_device_effect_active = false;
    bool                        persist_lighting_on_exit    = false;
    /*---------------------------------------------------------*\
    | The intial value of device_index should point to the      |
    |   layout for the generic_device                           |
    \*---------------------------------------------------------*/
    uint32_t                    device_index    = 0;
    uint16_t                    product_id      = 0;
    uint32_t                    effects_mask    = 0;
    void                        Init_Controller();
    void                        InitDeviceSpecificConfiguration();
    void                        ApplyDeviceSpecificConfiguration(bool setup_zones);
    int                         GetLED_Zone(int led_idx);

    /*---------------------------------------------------------*\
    | Per instance layout lookup tables.                        |
    \*---------------------------------------------------------*/
    gb_fusion2_device           instance_layout{};
    gb_fusion2_layout           instance_zones{};
    std::vector<gb_fusion2_zone*> allocated_zones;

};
