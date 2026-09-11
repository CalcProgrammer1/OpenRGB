/*---------------------------------------------------------*\
| RGBController_PalitGPUv2.cpp                              |
|                                                           |
|   RGBController for Palit v2 GPU                          |
|                                                           |
|   KundaPanda                                  04 Jan 2021 |
|   KendallMorgan                               17 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_PalitGPUv2.h"

/**------------------------------------------------------------------*\
    @name Palit GPU v2
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectPalitGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PalitGPUv2::RGBController_PalitGPUv2(PalitGPUv2Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "Palit";
    type                        = DEVICE_TYPE_GPU;
    description                 = "Palit GPU V2 Device";
    location                    = controller->GetDeviceLocation();

    /*-----------------------------------------------------*\
    | Off mode                                              |
    \*-----------------------------------------------------*/
    mode Off;
    Off.name                    = "Off";
    Off.value                   = PALIT_V2_MODE_ID_OFF;
    Off.flags                   = 0;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    /*-----------------------------------------------------*\
    | Direct mode                                           |
    \*-----------------------------------------------------*/
    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = PALIT_V2_MODE_ID_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness           = 255;
    Direct.brightness_min       = 0;
    Direct.brightness_max       = 100;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    /*-----------------------------------------------------*\
    | Breathing mode                                        |
    \*-----------------------------------------------------*/
    mode Breathe;
    Breathe.name                = "Breathing";
    Breathe.value               = PALIT_V2_MODE_ID_BREATHING;
    Breathe.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathe.speed_max           = 0x000a;
    Breathe.speed_min           = 0x1324;
    Breathe.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Breathe.colors_min          = 2;
    Breathe.colors_max          = 2;
    Breathe.colors.resize(2);
    modes.push_back(Breathe);

    /*-----------------------------------------------------*\
    | Spectrum Cycle mode                                   |
    \*-----------------------------------------------------*/
    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = PALIT_V2_MODE_ID_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    SpectrumCycle.speed         = 3;
    SpectrumCycle.speed_max     = 0;
    SpectrumCycle.speed_min     = 100;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    /*-----------------------------------------------------*\
    | Strobe mode                                           |
    \*-----------------------------------------------------*/
    mode Strobe;
    Strobe.name                 = "Strobe";
    Strobe.value                = PALIT_V2_MODE_ID_STROBE;
    Strobe.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Strobe.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Strobe.colors_min           = 1;
    Strobe.colors_max           = 1;
    Strobe.colors.resize(1);
    Strobe.speed_max            = 0x0;
    Strobe.speed_min            = 0xF;
    modes.push_back(Strobe);

    SetupZones();

    active_mode = 0;
}

RGBController_PalitGPUv2::~RGBController_PalitGPUv2()
{
    Shutdown();

    delete controller;
}

void RGBController_PalitGPUv2::SetupZones()
{
    zone* new_zone = new zone();
    led*  new_led  = new led();

    new_zone->name          = "GPU Zone";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;

    new_led->name           = "GPU LED";

    leds.push_back(*new_led);
    zones.push_back(*new_zone);
    SetupColors();

    unsigned char red = controller->GetLEDRed();
    unsigned char grn = controller->GetLEDGreen();
    unsigned char blu = controller->GetLEDBlue();

    colors[0] =  ToRGBColor(red, grn, blu);
}

void RGBController_PalitGPUv2::DeviceUpdateLEDs()
{
    for(unsigned int color : colors)
    {
        unsigned char red = RGBGetRValue(color);
        unsigned char grn = RGBGetGValue(color);
        unsigned char blu = RGBGetBValue(color);

        controller->SetLEDColors(red, grn, blu);
        controller->SetMode(PALIT_V2_MODE_STATIC, 0x2);
    }
}

void RGBController_PalitGPUv2::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PalitGPUv2::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PalitGPUv2::DeviceUpdateMode()
{
    RGBColor      color = colors[0];
    unsigned char r     = RGBGetRValue(color);
    unsigned char g     = RGBGetGValue(color);
    unsigned char b     = RGBGetBValue(color);

    switch(modes[active_mode].value)
    {
        case PALIT_V2_MODE_ID_OFF:
            controller->SetOff();
            break;

        case PALIT_V2_MODE_ID_DIRECT:
            controller->SetDirect(r, g, b, modes[active_mode].brightness);
            break;

        case PALIT_V2_MODE_ID_BREATHING:
            {
                controller->SetBreathingSpeed(modes[active_mode].speed);

                unsigned char r1 = RGBGetRValue(modes[active_mode].colors[0]);
                unsigned char g1 = RGBGetGValue(modes[active_mode].colors[0]);
                unsigned char b1 = RGBGetBValue(modes[active_mode].colors[0]);
                controller->SetLEDColors(r1, g1, b1);

                unsigned char r2 = RGBGetRValue(modes[active_mode].colors[1]);
                unsigned char g2 = RGBGetGValue(modes[active_mode].colors[1]);
                unsigned char b2 = RGBGetBValue(modes[active_mode].colors[1]);
                controller->SetLEDColors(r2, g2, b2, PALIT_V2_COLOR_REGISTER_SECONDARY);
                controller->SetMode(PALIT_V2_MODE_STATIC, 0x2, PALIT_V2_STATIC_BREATHING);
            }
            break;

        case PALIT_V2_MODE_ID_CYCLE:
            controller->SetMode(PALIT_V2_MODE_CYCLE, (unsigned char)(modes[active_mode].speed));
            controller->SetDirection(modes[active_mode].direction);
            break;

        case PALIT_V2_MODE_ID_STROBE:
            {
                mode current_mode = modes[(unsigned int)active_mode];

                unsigned char r_strobe = RGBGetRValue(current_mode.colors[0]);
                unsigned char g_strobe = RGBGetGValue(current_mode.colors[0]);
                unsigned char b_strobe = RGBGetBValue(current_mode.colors[0]);
                controller->SetLEDColors(r_strobe, g_strobe, b_strobe, PALIT_V2_COLOR_REGISTER_TERTIARY);

                controller->SetMode(PALIT_V2_MODE_STROBE, (unsigned char)(current_mode.speed));
                controller->SetDirection(current_mode.direction);
            }
            break;

        default:
            break;
    }
}
