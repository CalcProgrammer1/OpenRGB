/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2AorusMasterGPU.cpp        |
|                                                           |
|   RGBController for Gigabyte AORUS MASTER GPU             |
|                                                           |
|   Each fan has 8 individually addressable LEDs            |
|   Plus 1 logo LED = 25 total LEDs                         |
|                                                           |
|   Note: Fan LEDs only illuminate when fans are spinning   |
|   Use nvidia-settings to force fans on for testing        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteRGBFusion2AorusMasterGPU.h"
#include "LogManager.h"

/**------------------------------------------------------------------*\
    @name Gigabyte AORUS MASTER GPU
    @category GPU
    @type I2C
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectGigabyteRGBFusion2AorusMasterGPUControllers
    @comment Fan LEDs only work when fans are spinning. Each fan has 8
             individually addressable LEDs. Protocol uses zone-based
             mode commands before color commands.
\*-------------------------------------------------------------------*/

RGBController_RGBFusion2AorusMasterGPU::RGBController_RGBFusion2AorusMasterGPU(RGBFusion2AorusMasterGPUController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "Gigabyte";
    description                     = "Gigabyte AORUS MASTER GPU with Fan Ring LEDs";
    location                        = controller->GetDeviceLocation();
    type                            = DEVICE_TYPE_GPU;

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = RGB_FUSION2_AORUS_MASTER_MODE_STATIC;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.brightness_min           = RGB_FUSION2_AORUS_MASTER_BRIGHTNESS_MIN;
    Direct.brightness_max           = RGB_FUSION2_AORUS_MASTER_BRIGHTNESS_MAX;
    Direct.brightness               = RGB_FUSION2_AORUS_MASTER_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_RGBFusion2AorusMasterGPU::~RGBController_RGBFusion2AorusMasterGPU()
{
    delete controller;
}

void RGBController_RGBFusion2AorusMasterGPU::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create 3 fan zones with 8 LEDs each (linear zones)        |
    \*---------------------------------------------------------*/
    const char* fan_names[] = { "Left Fan", "Middle Fan", "Right Fan" };

    for(int fan_idx = 0; fan_idx < 3; fan_idx++)
    {
        zone fan_zone;
        fan_zone.name       = fan_names[fan_idx];
        fan_zone.type       = ZONE_TYPE_LINEAR;
        fan_zone.leds_min   = 8;
        fan_zone.leds_max   = 8;
        fan_zone.leds_count = 8;
        fan_zone.matrix_map = NULL;

        zones.push_back(fan_zone);

        /*-----------------------------------------------------*\
        | Create 8 LEDs for this fan zone                       |
        \*-----------------------------------------------------*/
        for(int led_idx = 0; led_idx < 8; led_idx++)
        {
            led new_led;
            new_led.name = fan_names[fan_idx];
            new_led.name.append(" LED ");
            new_led.name.append(std::to_string(led_idx + 1));
            leds.push_back(new_led);
        }
    }

    /*---------------------------------------------------------*\
    | Create logo zone with 1 LED                               |
    \*---------------------------------------------------------*/
    zone logo_zone;
    logo_zone.name       = "Logo";
    logo_zone.type       = ZONE_TYPE_SINGLE;
    logo_zone.leds_min   = 1;
    logo_zone.leds_max   = 1;
    logo_zone.leds_count = 1;
    logo_zone.matrix_map = NULL;

    zones.push_back(logo_zone);

    led logo_led;
    logo_led.name = "Logo";
    leds.push_back(logo_led);

    SetupColors();
}

void RGBController_RGBFusion2AorusMasterGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RGBFusion2AorusMasterGPU::DeviceUpdateLEDs()
{
    uint8_t brightness = modes[active_mode].brightness;

    /*---------------------------------------------------------*\
    | Update each fan zone                                      |
    | LED indices: 0-7 (left), 8-15 (middle), 16-23 (right)     |
    \*---------------------------------------------------------*/
    for(int fan_idx = 0; fan_idx < 3; fan_idx++)
    {
        RGBColor fan_colors[8];
        int base_led_idx = fan_idx * 8;

        for(int led_idx = 0; led_idx < 8; led_idx++)
        {
            fan_colors[led_idx] = colors[base_led_idx + led_idx];
        }

        controller->SetFanColors(fan_idx, fan_colors, brightness);
    }

    /*---------------------------------------------------------*\
    | Update logo (LED index 24)                                |
    \*---------------------------------------------------------*/
    controller->SetLogoColor(colors[24]);

    /*---------------------------------------------------------*\
    | Apply all changes                                         |
    \*---------------------------------------------------------*/
    controller->ApplyChanges();
}

void RGBController_RGBFusion2AorusMasterGPU::UpdateZoneLEDs(int /*zone*/)
{
    /*---------------------------------------------------------*\
    | For simplicity, update all LEDs when any zone changes     |
    | The protocol requires sequential updates anyway           |
    \*---------------------------------------------------------*/
    DeviceUpdateLEDs();
}

void RGBController_RGBFusion2AorusMasterGPU::UpdateSingleLED(int /*led*/)
{
    /*---------------------------------------------------------*\
    | For simplicity, update all LEDs when any LED changes      |
    | The protocol requires mode command + color sequence       |
    \*---------------------------------------------------------*/
    DeviceUpdateLEDs();
}

void RGBController_RGBFusion2AorusMasterGPU::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusion2AorusMasterGPU::DeviceSaveMode()
{
    /*---------------------------------------------------------*\
    | Apply command also saves to device memory                 |
    \*---------------------------------------------------------*/
    controller->ApplyChanges();
}
