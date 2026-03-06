/*---------------------------------------------------------*\
| RGBController_PNYARGBEpicXGPU.cpp                         |
|                                                           |
|   RGBController for PNY ARGB Epic-X GPU                   |
|                                                           |
|   Peter Berendi                               27 Apr 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "pci_ids.h"
#include "RGBController_PNYARGBEpicXGPU.h"

#define __ 0xFFFFFFFF

/**------------------------------------------------------------------*\
    @name PNY ARGB Epic-X GPU
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectPNYARGBEpicXGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PNYARGBEpicXGPU::RGBController_PNYARGBEpicXGPU(PNYARGBEpicXGPUController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "PNY";
    description                 = "PNY ARGB Epic-X GPU Device";
    location                    = controller->GetDeviceLocation();
    type                        = DEVICE_TYPE_GPU;

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = PNY_GPU_MODE_ARGB_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = PNY_GPU_MODE_ARGB_OFF;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Cycle;
    Cycle.name                  = "Spectrum Cycle";
    Cycle.value                 = PNY_GPU_MODE_ARGB_CYCLE;
    Cycle.flags                 = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Cycle.color_mode            = MODE_COLORS_RANDOM;
    Cycle.speed                 = 0x09;
    Cycle.speed_min             = 0x0F;
    Cycle.speed_max             = 0x00;
    Cycle.brightness            = 0xFF;
    Cycle.brightness_min        = 0;
    Cycle.brightness_max        = 0xFF;
    modes.push_back(Cycle);

    mode Neon;
    Neon.name                   = "Neon";
    Neon.value                  = PNY_GPU_MODE_ARGB_NEON;
    Neon.flags                  = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Neon.color_mode             = MODE_COLORS_RANDOM;
    Neon.speed                  = 0x09;
    Neon.speed_min              = 0x5F;
    Neon.speed_max              = 0x00;
    Neon.brightness             = 0xFF;
    Neon.brightness_min         = 0;
    Neon.brightness_max         = 0xFF;
    modes.push_back(Neon);

//    mode Explosion;
//    Explosion.name              = "Explosion";
//    Explosion.value             = PNY_GPU_MODE_ARGB_EXPLOSION;
//    Explosion.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
//    Explosion.color_mode        = MODE_COLORS_MODE_SPECIFIC;
//    Explosion.brightness        = 0xFF;
//    Explosion.brightness_min    = 0;
//    Explosion.brightness_max    = 0xFF;
//    Explosion.colors_min        = 1;
//    Explosion.colors_max        = 1;
//    Explosion.colors.resize(1);
//    modes.push_back(Explosion);

//    mode Supernova;
//    Supernova.name              = "Supernova";
//    Supernova.value             = PNY_GPU_MODE_ARGB_SUPERNOVA;
//    Supernova.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
//    Supernova.color_mode        = MODE_COLORS_RANDOM;
//    Supernova.brightness        = 0xFF;
//    Supernova.brightness_min    = 0;
//    Supernova.brightness_max    = 0xFF;
//    modes.push_back(Supernova);

    mode Infinity;
    Infinity.name               = "Infinity";
    Infinity.value              = PNY_GPU_MODE_ARGB_INFINITY;
    Infinity.flags              = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Infinity.color_mode         = MODE_COLORS_RANDOM;
    Infinity.speed              = 0x09;
    Infinity.speed_min          = 0x5F;
    Infinity.speed_max          = 0x00;
    Infinity.brightness         = 0xFF;
    Infinity.brightness_min     = 0;
    Infinity.brightness_max     = 0xFF;
    modes.push_back(Infinity);

    mode Streamer;
    Streamer.name               = "Streamer";
    Streamer.value              = PNY_GPU_MODE_ARGB_STREAMER;
    Streamer.flags              = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Streamer.color_mode         = MODE_COLORS_RANDOM;
    Streamer.speed              = 0x09;
    Streamer.speed_min          = 0x5F;
    Streamer.speed_max          = 0x00;
    Streamer.brightness         = 0xFF;
    Streamer.brightness_min     = 0;
    Streamer.brightness_max     = 0xFF;
    modes.push_back(Streamer);

    mode Wave;
    Wave.name                   = "Rainbow Wave";
    Wave.value                  = PNY_GPU_MODE_ARGB_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Wave.color_mode             = MODE_COLORS_RANDOM;
    Wave.speed                  = 0x09;
    Wave.speed_min              = 0x5F;
    Wave.speed_max              = 0x00;
    Wave.brightness             = 0xFF;
    Wave.brightness_min         = 0;
    Wave.brightness_max         = 0xFF;
    modes.push_back(Wave);

    SetupZones();

    active_mode = 0;
}

RGBController_PNYARGBEpicXGPU::~RGBController_PNYARGBEpicXGPU()
{
    delete controller;
}

void RGBController_PNYARGBEpicXGPU::SetupZones()
{
    /*-----------------------------------------------------*\
    | The side logo zone has 4 LEDs, but they are part of   |
    | the FRONT register zone                               |
    |                                                       |
    | This card has two variants, large and small.  Large   |
    | is used on the 5080 and 5090 and has an extra logo    |
    | zone on the rear of the card as well as additional    |
    | LEDs in the arrow zone.  The small variant is used on |
    | the 5070 Ti and lower cards.                          |
    \*-----------------------------------------------------*/
    zone side_logo;

    if(controller->IsLargeVariant())
    {
        side_logo.name          = "Side Logo";
    }
    else
    {
        side_logo.name          = "Logo";
    }

    side_logo.type              = ZONE_TYPE_LINEAR;
    side_logo.leds_min          = 4;
    side_logo.leds_max          = 4;
    side_logo.leds_count        = 4;
    side_logo.matrix_map        = NULL;
    zones.push_back(side_logo);

    for(std::size_t led_idx = 0; led_idx < side_logo.leds_count; led_idx++)
    {
        led side_logo_led;
        side_logo_led.name      = side_logo.name + " LED " + std::to_string(led_idx);
        side_logo_led.value     = PNY_GPU_REG_ZONE_FRONT;
        leds.push_back(side_logo_led);
        zone_led_idx.push_back((unsigned char)(led_idx + 20));
    }

    /*-----------------------------------------------------*\
    | The front zone has 20 LEDs in a figure 8 pattern, the |
    | FRONT register has these 20 and then the 4 log LEDs   |
    \*-----------------------------------------------------*/
    zone front;
    front.name                  = "Front";
    front.type                  = ZONE_TYPE_LINEAR;
    front.leds_min              = 20;
    front.leds_max              = 20;
    front.leds_count            = 20;
    front.matrix_map            = NULL;
    zones.push_back(front);

    for(std::size_t led_idx = 0; led_idx < front.leds_count; led_idx++)
    {
        led front_led;
        front_led.name          = "Front LED " + std::to_string(led_idx);
        front_led.value         = PNY_GPU_REG_ZONE_FRONT;
        leds.push_back(front_led);
        zone_led_idx.push_back((unsigned char)led_idx);
    }

    /*-----------------------------------------------------*\
    | The arrow zone has 17 LEDs on the small variant, 19   |
    | LEDs on the large variant                             |
    \*-----------------------------------------------------*/
    unsigned int arrow_led_count;

    if(controller->IsLargeVariant())
    {
        arrow_led_count         = 19;
    }
    else
    {
        arrow_led_count         = 17;
    }

    zone arrow;
    arrow.name                  = "Arrow";
    arrow.type                  = ZONE_TYPE_LINEAR;
    arrow.leds_min              = arrow_led_count;
    arrow.leds_max              = arrow_led_count;
    arrow.leds_count            = arrow_led_count;
    arrow.matrix_map            = NULL;
    zones.push_back(arrow);

    for(std::size_t led_idx = 0; led_idx < arrow.leds_count; led_idx++)
    {
        led arrow_led;
        arrow_led.name          = "Arrow LED " + std::to_string(led_idx);
        arrow_led.value         = PNY_GPU_REG_ZONE_ARROW;
        leds.push_back(arrow_led);
        zone_led_idx.push_back((unsigned char)led_idx);
    }

    /*-----------------------------------------------------*\
    | The rear logo zone is only present on the large       |
    | variant                                               |
    \*-----------------------------------------------------*/
    if(controller->IsLargeVariant())
    {
        zone rear_logo;
        rear_logo.name          = "Rear Logo";
        rear_logo.type          = ZONE_TYPE_SINGLE;
        rear_logo.leds_min      = 1;
        rear_logo.leds_max      = 1;
        rear_logo.leds_count    = 1;
        rear_logo.matrix_map    = NULL;
        zones.push_back(rear_logo);

        led rear_logo_led;
        rear_logo_led.name      = "Rear Logo LED";
        rear_logo_led.value     = PNY_GPU_REG_ZONE_LOGO;
        leds.push_back(rear_logo_led);
        zone_led_idx.push_back(0);
    }

    SetupColors();
}

void RGBController_PNYARGBEpicXGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PNYARGBEpicXGPU::DeviceUpdateLEDs()
{
    for(std::size_t i = 0; i < leds.size(); i++)
    {
        UpdateSingleLED((int)i);
    }
}

void RGBController_PNYARGBEpicXGPU::UpdateZoneLEDs(int zone)
{
    for(unsigned int i = 0; i < zones[zone].leds_count; i++)
    {
        UpdateSingleLED(zones[zone].start_idx + i);
    }
}

void RGBController_PNYARGBEpicXGPU::UpdateSingleLED(int led)
{
    controller->SetLEDDirect(leds[led].value, zone_led_idx[led], PNY_GPU_MODE_ARGB_DIRECT, colors[led]);
}

void RGBController_PNYARGBEpicXGPU::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_NONE || modes[active_mode].color_mode == MODE_COLORS_RANDOM)
    {
        controller->SetZoneMode(PNY_GPU_REG_ZONE_FRONT, modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, 0, 0);
        controller->SetZoneMode(PNY_GPU_REG_ZONE_ARROW, modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, 0, 0);

        if(controller->IsLargeVariant())
        {
            controller->SetZoneMode(PNY_GPU_REG_ZONE_LOGO,  modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, 0, 0);
        }
    }
    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        controller->SetZoneMode(PNY_GPU_REG_ZONE_FRONT, modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, 0, modes[active_mode].colors[0]);
        controller->SetZoneMode(PNY_GPU_REG_ZONE_ARROW, modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, 0, modes[active_mode].colors[0]);

        if(controller->IsLargeVariant())
        {
            controller->SetZoneMode(PNY_GPU_REG_ZONE_LOGO,  modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, 0, modes[active_mode].colors[0]);
        }
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        controller->SetZoneMode(PNY_GPU_REG_ZONE_FRONT, modes[active_mode].value, 0, 0xFF, 0, 0);
        controller->SetZoneMode(PNY_GPU_REG_ZONE_ARROW, modes[active_mode].value, 0, 0xFF, 0, 0);

        if(controller->IsLargeVariant())
        {
            controller->SetZoneMode(PNY_GPU_REG_ZONE_LOGO,  modes[active_mode].value, 0, 0xFF, 0, 0);
        }

        DeviceUpdateLEDs();
    }
}
