/*---------------------------------------------------------*\
| RGBController_PNYLovelaceGPU.cpp                          |
|                                                           |
|   RGBController for PNY Lovelace GPU                      |
|                                                           |
|   yufan                                       01 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_PNYLovelaceGPU.h"

/**------------------------------------------------------------------*\
    @name PNY GPU 40xx
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectPNYLovelaceGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PNYLovelaceGPU::RGBController_PNYLovelaceGPU(PNYLovelaceGPUController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "PNY";
    description             = "PNY RGB GPU Device";
    location                = controller->GetDeviceLocation();
    type                    = DEVICE_TYPE_GPU;

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = PNY_GPU_MODE_STATIC;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Cycle;
    Cycle.name              = "Cycle";
    Cycle.value             = PNY_GPU_MODE_CYCLE;
    Cycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Cycle.speed             = 0x89;
    Cycle.speed_min         = 0;
    Cycle.speed_max         = 0xB2;
    Cycle.brightness        = 0xFF;
    Cycle.brightness_min    = 0;
    Cycle.brightness_max    = 0xFF;
    Cycle.color_mode        = MODE_COLORS_NONE;
    modes.push_back(Cycle);

    mode Breath;
    Breath.name             = "Breath";
    Breath.value            = PNY_GPU_MODE_BREATH;
    Breath.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breath.speed            = 0x09;
    Breath.speed_min        = 0;
    Breath.speed_max        = 0x19;
    Breath.colors_min       = 1;
    Breath.colors_max       = 1;
    Breath.colors.resize(1);
    Breath.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Breath);

    mode Wave;
    Wave.name               = "Wave";
    Wave.value              = PNY_GPU_MODE_WAVE;
    Wave.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Wave.speed              = 0x60;
    Wave.speed_min          = 0;
    Wave.speed_max          = 0xBF;
    Wave.brightness         = 0xFF;
    Wave.brightness_min     = 0;
    Wave.brightness_max     = 0xFF;
    Wave.color_mode         = MODE_COLORS_NONE;
    modes.push_back(Wave);

    mode Flash;
    Flash.name              = "Flash";
    Flash.value             = PNY_GPU_MODE_FLASH;
    Flash.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Flash.speed             = 0x27;
    Flash.speed_min         = 0;
    Flash.speed_max         = 0x4D;
    Flash.brightness        = 0xFF;
    Flash.brightness_min    = 0;
    Flash.brightness_max    = 0xFF;
    Flash.colors_min        = 1;
    Flash.colors_max        = 1;
    Flash.colors.resize(1);
    Flash.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Flash);

    mode Off;
    Off.name                = "Off";
    Off.value               = PNY_GPU_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();

    // Initialize active mode
    active_mode = 0;
}

void RGBController_PNYLovelaceGPU::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only has 3 LED, so create a single zone.      |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();

    new_zone->name          = "GPU Zone";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 3;
    new_zone->leds_max      = 3;
    new_zone->leds_count    = 3;
    new_zone->matrix_map    = NULL;

    led*  new_led  = new led();
    new_led->name           = "Fan LED";
    leds.push_back(*new_led);

    new_led  = new led();
    new_led->name           = "Right LED";
    leds.push_back(*new_led);

    new_led  = new led();
    new_led->name           = "Left LED";
    leds.push_back(*new_led);

    zones.push_back(*new_zone);
    SetupColors();
}

void RGBController_PNYLovelaceGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PNYLovelaceGPU::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_PNYLovelaceGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateMode();
}

void RGBController_PNYLovelaceGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateMode();
}

void RGBController_PNYLovelaceGPU::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case PNY_GPU_MODE_OFF:
            controller->SetOff();
            break;

        case PNY_GPU_MODE_STATIC:
            for (int i = 0; i < 3; i++)
            {
                RGBColor color = GetLED(i);
                controller->SetDirect(i, RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));
            }
            break;
        case PNY_GPU_MODE_CYCLE:
            controller->SetCycle(modes[active_mode].speed, modes[active_mode].brightness);
            break;
        case PNY_GPU_MODE_BREATH:
            {
                RGBColor color = modes[active_mode].colors[0];
                controller->SetBreath(modes[active_mode].speed, RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));
                break;
            }
        case PNY_GPU_MODE_WAVE:
            controller->SetWave(modes[active_mode].speed, modes[active_mode].brightness);
            break;
        case PNY_GPU_MODE_FLASH:
            {
                RGBColor color = modes[active_mode].colors[0];
                controller->SetFlash(modes[active_mode].speed, modes[active_mode].brightness,
                                    RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));
                break;
            }
        default:
        break;
    }
}
