/*---------------------------------------------------------*\
| RGBController_RobobloqLightStrip.cpp                      |
|                                                           |
|   Detector for Robobloq Monitor Light Strip               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_RobobloqLightStrip.h"
#include "RobobloqLightStripController.h"
#include <LogManager.h>

/**--------------------------------------------------------------------*\
    @name Robobloq Monitor Light Strip
    @category LEDStrip
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRobobloqLightStripController
    @comment
\*---------------------------------------------------------------------*/

RGBController_RobobloqLightStrip::RGBController_RobobloqLightStrip(RobobloqLightStripController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "Robobloq";
    description                     = "Robobloq Monitor Light Strip (" + std::to_string(controller->GetPhysicalSizeInInches()) + "\")";
    type                            = DEVICE_TYPE_LEDSTRIP;
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();
    version                         = controller->GetFirmwareVersion();

    mode Static;
    Static.name                     = "Static";
    Static.value                    = ROBOBLOQ_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min           = 1;
    Static.brightness_max           = 255;
    Static.brightness               = 255;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = ROBOBLOQ_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.brightness_min           = 1;
    Direct.brightness_max           = 255;
    Direct.brightness               = 255;
    modes.push_back(Direct);

    /*-----------------------------------------------------*\
    | Add dynamic modes                                     |
    \*-----------------------------------------------------*/
    struct DynamicMode
    {
        const char* name;
        int         value;
    };

    DynamicMode dynamic_modes[] =
    {
        { "Rainbow Wave",   ROBOBLOQ_MODE_DYNAMIC_RAINBOW   },
        { "Breathing",      ROBOBLOQ_MODE_DYNAMIC_BREATHING },
        { "Twist",          ROBOBLOQ_MODE_DYNAMIC_TWIST     },
        { "Beat",           ROBOBLOQ_MODE_DYNAMIC_BEAT      },
        { "Twirl",          ROBOBLOQ_MODE_DYNAMIC_TWIRL     },
        { "Lemon",          ROBOBLOQ_MODE_DYNAMIC_LEMON     },
        { "Electric",       ROBOBLOQ_MODE_DYNAMIC_ELECTRIC  },
    };

    for(unsigned int i = 0; i < (sizeof(dynamic_modes) / sizeof(DynamicMode)); i++)
    {
        mode new_mode;
        new_mode.name           = dynamic_modes[i].name;
        new_mode.value          = dynamic_modes[i].value;
        new_mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
        new_mode.color_mode     = MODE_COLORS_NONE;
        new_mode.speed_min      = 0;
        new_mode.speed_max      = 100;
        new_mode.speed          = 50;
        new_mode.brightness_min = 1;
        new_mode.brightness_max = 255;
        new_mode.brightness     = 255;
        modes.push_back(new_mode);
    }

    mode Off;
    Off.name                          = "Off";
    Off.value                         = ROBOBLOQ_MODE_OFF;
    Off.flags                         = 0;
    Off.color_mode                    = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
};

RGBController_RobobloqLightStrip::~RGBController_RobobloqLightStrip()
{
    delete controller;
}

void RGBController_RobobloqLightStrip::SetupZones()
{
    int led_count = controller->GetLEDCount();
    int leds_per_side = controller->GetLEDsPerSide();

    struct Side
    {
        const char* name;
        int         count;
    };

    std::vector<Side> sides = { { "Light Strip", led_count } };
    if(leds_per_side > 0)
    {
        sides = {
            { "Right", leds_per_side },
            { "Top",   led_count - (leds_per_side * 2) },
            { "Left",  leds_per_side },
        };
    }

    zones.clear();
    zones.resize(sides.size());
    leds.clear();
    leds.resize(led_count);

    for(unsigned int i = 0; i < sides.size(); i++)
    {
        zone& zone = zones[i];
        zone.name               = sides[i].name;
        zone.type               = ZONE_TYPE_LINEAR;
        zone.leds_count         = sides[i].count;
        zone.leds_min           = zone.leds_count;
        zone.leds_max           = zone.leds_count;
        zone.matrix_map         = NULL;
    }

    for(int i = 0; i < led_count; i++)
    {
        led& new_led = leds[i];
        new_led.name                = "LED " + std::to_string(i + 1);
        new_led.value               = i;
    }

    SetupColors();
}

void RGBController_RobobloqLightStrip::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*-----------------------------------------------------*\
    | This device does not support resizing zones           |
    \*-----------------------------------------------------*/
}

void RGBController_RobobloqLightStrip::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == ROBOBLOQ_MODE_DIRECT)
    {
        controller->SetCustom(colors);
    }
}

void RGBController_RobobloqLightStrip::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RobobloqLightStrip::UpdateSingleLED(int led)
{
    controller->SetLEDColor(led, modes[active_mode].colors[0]);
}

void RGBController_RobobloqLightStrip::DeviceUpdateMode()
{
    bool mode_changed = false;

    /*-----------------------------------------------------*\
    | Cache mode to avoid repeated SetDynamicEffect calls   |
    | when adjusting speed/brightness                       |
    \*-----------------------------------------------------*/
    if(modes[active_mode].value != cur_mode)
    {
        cur_mode = modes[active_mode].value;
        mode_changed = true;
    }

    if(cur_mode == ROBOBLOQ_MODE_OFF)
    {
        controller->TurnOff();
    }
    else if(cur_mode == ROBOBLOQ_MODE_DIRECT)
    {
        controller->SetBrightness(modes[active_mode].brightness);
    }
    else if(cur_mode == ROBOBLOQ_MODE_STATIC)
    {
        controller->SetColor(modes[active_mode].colors[0]);
        controller->SetBrightness(modes[active_mode].brightness);
    }
    else if(ROBOBLOQ_IS_DYNAMIC_EFFECT(cur_mode))
    {
        if(mode_changed)
        {
            controller->SetDynamicEffect(cur_mode);
        }

        controller->SetBrightness(modes[active_mode].brightness);
        controller->SetDynamicSpeed(modes[active_mode].speed);
    }
    else
    {
        LOG_ERROR("[Robobloq] Requested mode (%02x) is not supported", cur_mode);
    }
}
