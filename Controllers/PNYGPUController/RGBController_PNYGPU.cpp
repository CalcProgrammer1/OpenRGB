/*---------------------------------------------------------*\
| RGBController_PNYGPU.cpp                                  |
|                                                           |
|   RGBController for PNY Turing GPU                        |
|                                                           |
|   KendallMorgan                               17 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_PNYGPU.h"

/**------------------------------------------------------------------*\
    @name PNY GPU
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectPNYGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PNYGPU::RGBController_PNYGPU(PNYGPUController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = name.substr(0, name.find(' '));
    description             = "PNY/Palit RGB GPU Device";
    location                = controller->GetDeviceLocation();

    type                    = DEVICE_TYPE_GPU;

    mode Off;
    Off.name                = "Off";
    Off.value               = PNY_GPU_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = PNY_GPU_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR| MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness       = 255;
    Direct.brightness_min   = 0;
    Direct.brightness_max   = 100;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Cycle;
    Cycle.name             = "Cycle";
    Cycle.value            = PNY_GPU_MODE_CYCLE;
    Cycle.flags            = MODE_FLAG_HAS_SPEED;
    Cycle.speed            = 3;
    Cycle.speed_max        = 0;
    Cycle.speed_max        = 100;
    Cycle.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Cycle);

    mode Strobe;
    Strobe.name             = "Strobe";
    Strobe.value            = PNY_GPU_MODE_STROBE;
    Strobe.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Strobe.speed            = 2;
    Strobe.speed_max        = 0;
    Strobe.speed_max        = 255;
    Strobe.brightness       = 255;
    Strobe.brightness_min   = 0;
    Strobe.brightness_max   = 100;
    Strobe.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Strobe);


    SetupZones();

    // Initialize active mode
    active_mode = 0;
}

void RGBController_PNYGPU::SetupZones()
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

void RGBController_PNYGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PNYGPU::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_PNYGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PNYGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PNYGPU::DeviceUpdateMode()
{
    RGBColor      color = colors[0];
    unsigned char r   = RGBGetRValue(color);
    unsigned char g   = RGBGetGValue(color);
    unsigned char b   = RGBGetBValue(color);
    unsigned char speed, brightness;
    switch(modes[active_mode].value)
    {
        case PNY_GPU_MODE_OFF:
            controller->SetOff();
            break;

        case PNY_GPU_MODE_DIRECT:
            brightness = modes[active_mode].brightness;
            controller->SetDirect(r, g, b, brightness);
            break;

        case PNY_GPU_MODE_CYCLE:
            speed = modes[active_mode].speed;
            controller->SetCycle(speed);
            break;

        case PNY_GPU_MODE_STROBE:
            speed      = modes[active_mode].speed;
            brightness = modes[active_mode].brightness;
            controller->SetStrobe(r, g, b, speed, brightness);
            break;
        default:
        break;
    }
}
