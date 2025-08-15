/*---------------------------------------------------------*\
| RGBController_SapphireNitroGlowV3.cpp                     |
|                                                           |
|   RGBController for Sapphire Nitro Glow V3                |
|                                                           |
|   K900                                        03 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_SapphireNitroGlowV3.h"

/**------------------------------------------------------------------*\
    @name Sapphire Nitro Glow v3
    @category GPU
    @type I2C
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectSapphireV3Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_SapphireNitroGlowV3::RGBController_SapphireNitroGlowV3(SapphireNitroGlowV3Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "Sapphire";
    description             = "Sapphire Nitro Glow V3 Device";
    location                = controller->GetDeviceLocation();
    type                    = DEVICE_TYPE_GPU;

    mode Static;
    Static.name             = "Static";
    Static.value            = SAPPHIRE_NITRO_GLOW_V3_MODE_CUSTOM;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name            = "Rainbow Wave";
    Rainbow.value           = SAPPHIRE_NITRO_GLOW_V3_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min       = 10;
    Rainbow.speed_max       = 250;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Runway;
    Runway.name             = "Runway";
    Runway.value            = SAPPHIRE_NITRO_GLOW_V3_MODE_RUNWAY;
    Runway.flags            = MODE_FLAG_HAS_SPEED;
    Runway.speed_min        = 5;
    Runway.speed_max        = 50;
    Runway.color_mode       = MODE_COLORS_NONE;
    modes.push_back(Runway);

    mode ColorCycle;
    ColorCycle.name         = "Spectrum Cycle";
    ColorCycle.value        = SAPPHIRE_NITRO_GLOW_V3_MODE_COLOR_CYCLE;
    ColorCycle.flags        = MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min    = 30;
    ColorCycle.speed_max    = 1;
    ColorCycle.color_mode   = MODE_COLORS_NONE;
    modes.push_back(ColorCycle);

    mode Serial;
    Serial.name             = "Serial";
    Serial.value            = SAPPHIRE_NITRO_GLOW_V3_MODE_SERIAL;
    Serial.flags            = MODE_FLAG_HAS_SPEED;
    Serial.speed_min        = 255;
    Serial.speed_max        = 5;
    Serial.color_mode       = MODE_COLORS_NONE;
    modes.push_back(Serial);

    mode External;
    External.name           = "External Control";
    External.value          = SAPPHIRE_NITRO_GLOW_V3_MODE_EXTERNAL_CONTROL;
    External.flags          = 0;
    External.color_mode     = MODE_COLORS_NONE;
    modes.push_back(External);

    mode Off;
    Off.name                = "Off";
    Off.value               = SAPPHIRE_NITRO_GLOW_V3_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();

    ReadConfiguration();
}

RGBController_SapphireNitroGlowV3::~RGBController_SapphireNitroGlowV3()
{
    delete controller;
}

void RGBController_SapphireNitroGlowV3::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only has one LED, so create a single zone and |
    | LED for it                                                |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();
    led*  new_led  = new led();

    new_zone->name          = "GPU Zone";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;

    new_led->name           = "GPU LED";

    /*---------------------------------------------------------*\
    | Push the zone and LED on to device vectors                |
    \*---------------------------------------------------------*/
    leds.push_back(*new_led);
    zones.push_back(*new_zone);

    SetupColors();
}

void RGBController_SapphireNitroGlowV3::ReadConfiguration()
{
    modes[1].speed = controller->GetRainbowAnimationSpeed();
    modes[2].speed = controller->GetRunwayAnimationSpeed();
    modes[3].speed = controller->GetColorCycleAnimationSpeed();
    modes[4].speed = controller->GetSerialAnimationSpeed();

    colors[0] = ToRGBColor(
        controller->GetRed(),
        controller->GetBlue(),
        controller->GetGreen()
    );

    if(controller->GetExternalControl())
    {
        active_mode = 5;
        return;
    }

    switch(controller->GetMode())
    {
        case SAPPHIRE_NITRO_GLOW_V3_MODE_CUSTOM:
            active_mode = 0;
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_RAINBOW:
            active_mode = 1;
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_RUNWAY:
            active_mode = 2;
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_COLOR_CYCLE:
            active_mode = 3;
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_SERIAL:
            active_mode = 4;
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_OFF:
            active_mode = 6;
            colors[0] = ToRGBColor(0, 0, 0);
            break;

        default:
            active_mode = 0;
            break;
    }
}

void RGBController_SapphireNitroGlowV3::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SapphireNitroGlowV3::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->SetColor(red, grn, blu);
}

void RGBController_SapphireNitroGlowV3::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SapphireNitroGlowV3::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SapphireNitroGlowV3::DeviceUpdateMode()
{
    auto mode = modes[active_mode];

    switch(mode.value)
    {
        case SAPPHIRE_NITRO_GLOW_V3_MODE_CUSTOM:
            controller->SetExternalControl(false);
            controller->SetMode(mode.value);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_RAINBOW:
            controller->SetExternalControl(false);
            controller->SetRainbowAnimationSpeed(mode.speed);
            controller->SetMode(mode.value);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_RUNWAY:
            controller->SetExternalControl(false);
            controller->SetRunwayAnimationSpeed(mode.speed);
            controller->SetMode(mode.value);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_COLOR_CYCLE:
            controller->SetExternalControl(false);
            controller->SetColorCycleAnimationSpeed(mode.speed);
            controller->SetMode(mode.value);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_SERIAL:
            controller->SetExternalControl(false);
            controller->SetSerialAnimationSpeed(mode.speed);
            controller->SetMode(mode.value);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_EXTERNAL_CONTROL:
            controller->SetExternalControl(true);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_OFF:
            controller->SetExternalControl(false);
            controller->SetColor(0, 0, 0);
            controller->SetMode(mode.value);
            break;
    }
}
