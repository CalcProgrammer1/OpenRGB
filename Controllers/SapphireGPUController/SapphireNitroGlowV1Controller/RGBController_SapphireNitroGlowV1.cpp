/*---------------------------------------------------------*\
| RGBController_SapphireNitroGlowV1.cpp                     |
|                                                           |
|   RGBController for Sapphire Nitro Glow V1                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_SapphireNitroGlowV1.h"

/**------------------------------------------------------------------*\
    @name Sapphire Nitro Glow v1
    @category GPU
    @type I2C
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectSapphireV1Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_SapphireNitroGlowV1::RGBController_SapphireNitroGlowV1(SapphireNitroGlowV1Controller* controller_ptr)
{
    controller                 = controller_ptr;

    name                       = controller->GetDeviceName();
    vendor                     = "Sapphire";
    description                = "Sapphire Nitro Glow V1 Device";
    location                   = controller->GetDeviceLocation();
    type                       = DEVICE_TYPE_GPU;

    mode Sapphire;
    Sapphire.name              = "Sapphire Blue";
    Sapphire.value             = SAPPHIRE_NITRO_GLOW_V1_MODE_SAPPHIRE_BLUE;
    Sapphire.flags             = MODE_FLAG_HAS_BRIGHTNESS;
    Sapphire.color_mode        = MODE_COLORS_NONE;
    Sapphire.brightness_min    = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MIN;
    Sapphire.brightness_max    = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MAX;
    Sapphire.brightness        = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MAX;
    modes.push_back(Sapphire);

    mode Rainbow;
    Rainbow.name               = "Rainbow Wave";
    Rainbow.value              = SAPPHIRE_NITRO_GLOW_V1_MODE_RAINBOW;
    Rainbow.flags              = MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.color_mode         = MODE_COLORS_NONE;
    Rainbow.brightness_min     = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MIN;
    Rainbow.brightness_max     = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MAX;
    Rainbow.brightness         = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MAX;
    modes.push_back(Rainbow);

    mode Temperature;
    Temperature.name           = "PCB Temperature";
    Temperature.value          = SAPPHIRE_NITRO_GLOW_V1_MODE_BOARD_TEMPERATURE;
    Temperature.flags          = MODE_FLAG_HAS_BRIGHTNESS;
    Temperature.color_mode     = MODE_COLORS_NONE;
    Temperature.brightness_min = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MIN;
    Temperature.brightness_max = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MAX;
    Temperature.brightness     = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MAX;
    modes.push_back(Temperature);

    mode FanSpeed;
    FanSpeed.name              = "Fan Speed";
    FanSpeed.value             = SAPPHIRE_NITRO_GLOW_V1_MODE_FAN_SPEED;
    FanSpeed.flags             = MODE_FLAG_HAS_BRIGHTNESS;
    FanSpeed.color_mode        = MODE_COLORS_NONE;
    FanSpeed.brightness_min    = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MIN;
    FanSpeed.brightness_max    = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MAX;
    FanSpeed.brightness        = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MAX;
    modes.push_back(FanSpeed);

    mode Static;
    Static.name                = "Static";
    Static.value               = SAPPHIRE_NITRO_GLOW_V1_MODE_CUSTOM;
    Static.flags               = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode          = MODE_COLORS_PER_LED;
    Static.brightness_min      = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MIN;
    Static.brightness_max      = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MAX;
    Static.brightness          = SAPPHITE_NITRO_GLOW_V1_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode Off;
    Off.name                   = "Off";
    Off.value                  = SAPPHIRE_NITRO_GLOW_V1_MODE_OFF;
    Off.flags                  = 0;
    Off.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();

    ReadConfiguration();
}

RGBController_SapphireNitroGlowV1::~RGBController_SapphireNitroGlowV1()
{
    delete controller;
}

void RGBController_SapphireNitroGlowV1::SetupZones()
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

void RGBController_SapphireNitroGlowV1::ReadConfiguration()
{
    colors[0] = ToRGBColor(
        controller->GetRed(),
        controller->GetGreen(),
        controller->GetBlue()
    );

    switch(controller->GetMode())
    {
        case SAPPHIRE_NITRO_GLOW_V1_MODE_SAPPHIRE_BLUE:
            active_mode = 0;
            break;

        case SAPPHIRE_NITRO_GLOW_V1_MODE_RAINBOW:
            active_mode = 1;
            break;

        case SAPPHIRE_NITRO_GLOW_V1_MODE_BOARD_TEMPERATURE:
            active_mode = 2;
            break;

        case SAPPHIRE_NITRO_GLOW_V1_MODE_FAN_SPEED:
            active_mode = 3;
            break;

        case SAPPHIRE_NITRO_GLOW_V1_MODE_CUSTOM:
            active_mode = 4;
            break;

        case SAPPHIRE_NITRO_GLOW_V1_MODE_OFF:
            active_mode = 5;
            break;

        default:
            active_mode = 0;
            break;
    }

    modes[(unsigned int)active_mode].brightness = controller->GetBrightness();
}

void RGBController_SapphireNitroGlowV1::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SapphireNitroGlowV1::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->SetColor(red, grn, blu);
}

void RGBController_SapphireNitroGlowV1::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SapphireNitroGlowV1::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SapphireNitroGlowV1::DeviceUpdateMode()
{
    controller->SetMode((unsigned char)modes[(unsigned int)active_mode].value);
    controller->SetBrightness((unsigned char)modes[(unsigned int)active_mode].brightness);
}
