/*---------------------------------------------------------*\
| RGBController_FrameworkDesktop_Linux.cpp                  |
|                                                           |
|   RGBController for the Framework Desktop ARGB header     |
|                                                           |
|   Francis De Brabandere (francisdb@gmail.com) 16 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_FrameworkDesktop_Linux.h"

/*---------------------------------------------------------*\
| Layout of the 8 LED Cooler Master ARGB fan sold with the  |
| Framework Desktop: a ring with index 0 at the top and the |
| others following counter-clockwise when facing the fan.   |
| Other fans on the ARGB header may be wired differently.   |
\*---------------------------------------------------------*/
#define FRAMEWORK_DESKTOP_FAN_LEDS  8

#define NA 0xFFFFFFFF
static unsigned int matrix_map[3][3] =
{
    { 1,   0,   7  },
    { 2,   NA,  6  },
    { 3,   4,   5  }
};

/**------------------------------------------------------------------*\
    @name Framework Desktop
    @category Motherboard
    @type cros_ec
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectFrameworkDesktopControllers
    @comment The ARGB header of the Framework Desktop, controlled
        through the embedded controller (/dev/cros_ec).  The header
        defaults to the 8 LED Cooler Master ARGB fan sold with the
        Framework Desktop; its size (up to 128 LEDs) and layout can
        be changed for other devices.  Linux only.
\*-------------------------------------------------------------------*/

RGBController_FrameworkDesktop::RGBController_FrameworkDesktop(FrameworkDesktopController* controller_ptr)
{
    controller          = controller_ptr;

    name                = "Framework Desktop";
    vendor              = "Framework";
    type                = DEVICE_TYPE_MOTHERBOARD;
    description         = "Framework Desktop ARGB Header (cros_ec)";
    location            = controller->GetLocation();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_FrameworkDesktop::~RGBController_FrameworkDesktop()
{
    Shutdown();

    delete controller;
}

void RGBController_FrameworkDesktop::SetupZones()
{
    /*-----------------------------------------------------*\
    | Only set the zone flags on the first run              |
    \*-----------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    leds.clear();
    colors.clear();
    zones.resize(1);

    zones[0].leds_min                   = 0;
    zones[0].leds_max                   = FRAMEWORK_DESKTOP_MAX_LEDS;

    if(first_run)
    {
        zones[0].flags                  = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
    {
        zones[0].name                   = "ARGB Header";
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
    {
        zones[0].leds_count             = FRAMEWORK_DESKTOP_FAN_LEDS;
    }

    /*-----------------------------------------------------*\
    | Use the Cooler Master fan ring for 8 LEDs, a linear   |
    | strip for any other size                              |
    \*-----------------------------------------------------*/
    bool fan_layout = (zones[0].leds_count == FRAMEWORK_DESKTOP_FAN_LEDS);

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
    {
        zones[0].type                   = fan_layout ? ZONE_TYPE_MATRIX : ZONE_TYPE_LINEAR;
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
    {
        if(fan_layout)
        {
            zones[0].matrix_map.Set(3, 3, (unsigned int *)&matrix_map);
        }
        else
        {
            zones[0].matrix_map.Reset();
        }
    }

    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name    = zones[0].name + ", LED " + std::to_string(led_idx + 1);
        new_led.value   = led_idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_FrameworkDesktop::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        SetupZones();
    }
}

void RGBController_FrameworkDesktop::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_FrameworkDesktop::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_FrameworkDesktop::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_FrameworkDesktop::DeviceUpdateMode()
{

}
