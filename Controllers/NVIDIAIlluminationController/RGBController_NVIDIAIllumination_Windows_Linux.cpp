/*---------------------------------------------------------*\
| RGBController_NVIDIAIllumination_Windows_Linux.cpp        |
|                                                           |
|   RGBController for NVIDIA Illumination GPU               |
|                                                           |
|   Carter Miller (GingerRunner)                04 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <array>
#include "RGBController_NVIDIAIllumination_Windows_Linux.h"

/**------------------------------------------------------------------*\
    @name NVIDIA Illumination
    @category GPU
    @type PCI
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectNVIDIAIllumGPUs
    @comment Tested on various 30 series GPUs and a Founders Edition 2070 Super
        If you want to see if your card should also use this controller, download the DLLs from the release of [this](https://gitlab.com/OpenRGBDevelopers/NvAPISpy).

        Perform the global replacement technique, which is specified in the README of NvAPI spy.  Once this is complete, use an RGB program of your choice and make some basic
        lighting changes.

        Check the C:\NvAPISpy\ folder and see if the logs created are filled with calls like this:
        ```
        NvAPI_GPU_ClientIllumZonesGetControl:   version: 72012  numIllumZones: 2  bDefault: 0  rsvdField: 0
                                                ZoneIdx: 0 ----------------------------------------
                                                **ZoneType: RGBW  ControlMode: MANUAL
                                                **DATA_RGBW::  Red: 255  Green: 0  Blue: 0  White: 0  Brightness%: 36
                                                ZoneIdx: 1 ----------------------------------------
                                                **ZoneType: SINGLE_COLOR  ControlMode: MANUAL
                                                **DATA_SINGLE_COLOR::  Brightness% 100
        NvAPI_GPU_ClientIllumZonesSetControl:   version: 72012  numIllumZones: 2  bDefault: 0  rsvdField: 0
                                                ZoneIdx: 0 ----------------------------------------
                                                **ZoneType: RGBW  ControlMode: MANUAL
                                                **DATA_RGBW::  Red: 255  Green: 0  Blue: 0  White: 0  Brightness%: 36
                                                ZoneIdx: 1 ----------------------------------------
                                                **ZoneType: SINGLE_COLOR  ControlMode: MANUAL
                                                **DATA_SINGLE_COLOR::  Brightness% 44
        ```
        If you see Get/Set Calls above for zone control, please create a [new device issue](https://gitlab.com/CalcProgrammer1/OpenRGB/-/issues/new?issuable_template=New%20Device#)
        and attach the relevant details to request support for your device (try various modes in each color, especially white and shades around it, since some cards treat RGBW as
        standard RGB).
\*-------------------------------------------------------------------*/

RGBController_NVIDIAIlluminationV1::RGBController_NVIDIAIlluminationV1(NVIDIAIlluminationV1Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "NVIDIA";
    description             = "NVIDIA Illumination RGB GPU Device";
    type                    = DEVICE_TYPE_GPU;

    mode Off;
    Off.name                = "Off";
    Off.value               = NVIDIA_ILLUMINATION_OFF;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name             = "Direct";
    Static.value            = NVIDIA_ILLUMINATION_DIRECT;
    Static.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode       = MODE_COLORS_PER_LED;
    Static.colors_min       = 1;
    Static.colors_max       = 1;
    Static.brightness_min   = 0;
    Static.brightness       = 100;
    Static.brightness_max   = 100;
    modes.push_back(Static);

    SetupZones();

    for(unsigned int i = 0; i < zones.size(); i++)
    {
        zones[i].colors[0]  = controller->getZoneColor(i);
    }
}

RGBController_NVIDIAIlluminationV1::~RGBController_NVIDIAIlluminationV1()
{
    delete controller;
}

void RGBController_NVIDIAIlluminationV1::UpdateSingleLED(int)
{
    DeviceUpdateLEDs();
}

void RGBController_NVIDIAIlluminationV1::SetupZones()
{
    /*--------------------------------------------------------------------------*\
    | Use the NvAPI to gather existing zones on the card and their capabilities, |
    | populate available zones accordingly.                                      |
    \*--------------------------------------------------------------------------*/
    zoneTypes = controller->getInfo();
    nvidia_illum_zone_names[NV_GPU_CLIENT_ILLUM_ZONE_TYPE_RGB]          = "RGB";
    nvidia_illum_zone_names[NV_GPU_CLIENT_ILLUM_ZONE_TYPE_RGBW]         = "RGBW";
    nvidia_illum_zone_names[NV_GPU_CLIENT_ILLUM_ZONE_TYPE_COLOR_FIXED]  = "FIXED COLOR";
    nvidia_illum_zone_names[NV_GPU_CLIENT_ILLUM_ZONE_TYPE_SINGLE_COLOR] = "SINGLE COLOR";
    for(uint8_t zone_idx = 0; zone_idx < zoneTypes.size(); zone_idx++)
    {
        zone* new_zone = new zone();
        led*  new_led  = new led();

        new_zone->name          = std::to_string(zone_idx) + " - " + (std::string)nvidia_illum_zone_names[zoneTypes[zone_idx]];
        new_zone->type          = ZONE_TYPE_SINGLE;
        new_zone->leds_min      = 1;
        new_zone->leds_max      = 1;
        new_zone->leds_count    = 1;
        new_zone->matrix_map    = NULL;
        new_led->name           = "Entire Zone";
        leds.push_back(*new_led);
        zones.push_back(*new_zone);
        zoneIndexMap.push_back(zone_idx);
    }
    SetupColors();

}

void RGBController_NVIDIAIlluminationV1::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_NVIDIAIlluminationV1::DeviceUpdateLEDs()
{
    NVIDIAIllumination_Config nv_zone_config;
    for(uint8_t zone_idx = 0; zone_idx < zoneIndexMap.size(); zone_idx++)
    {
        nv_zone_config.colors[0]  = colors[zone_idx];
        nv_zone_config.brightness = modes[active_mode].brightness;
        controller->setZone(zone_idx, modes[active_mode].value, nv_zone_config);
    }
}

void RGBController_NVIDIAIlluminationV1::UpdateZoneLEDs(int zone)
{
    NVIDIAIllumination_Config nv_zone_config;
    nv_zone_config.colors[0]  = colors[zone];
    nv_zone_config.brightness = modes[active_mode].brightness;
    controller->setZone(zone, modes[active_mode].value, nv_zone_config);
}

uint8_t RGBController_NVIDIAIlluminationV1::getModeIndex(uint8_t mode_value)
{
    for(uint8_t mode_index = 0; mode_index < modes.size(); mode_index++)
    {
        if(modes[mode_index].value == mode_value)
        {
            return mode_index;
        }
    }
    return 0;
}

void RGBController_NVIDIAIlluminationV1::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
