/*---------------------------------------------------------*\
| RGBController_HoltekA09F.cpp                              |
|                                                           |
|   RGBController for Holtek A09F RGB gaming mouse          |
|   (E-Signal LUOM G10, VID 04D9 PID A09F)                  |
|                                                           |
|   Protocol documented in reverse-engineering notes.       |
|   Supported features:                                     |
|     - 12 lighting modes (Off, Standard, Breathing, etc.)  |
|     - DPI stage colours (8 stages × RGB)                  |
|                                                           |
|   Juampe                                         2026     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_HoltekA09F.h"

/**------------------------------------------------------------------*\
    @name Holtek A09F
    @category Mouse
    @type USB
    @save :white_check_mark:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectHoltekA09FControllers
    @comment Phoenix Void RGB gaming mouse (Holtek A09F chip, VID 04D9 PID A09F).
             Supports 12 lighting modes and 8 DPI stage colours.
             Lighting changes are saved automatically by the device.
\*-------------------------------------------------------------------*/

RGBController_HoltekA09F::RGBController_HoltekA09F(HoltekA09FController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "Holtek";
    type                    = DEVICE_TYPE_MOUSE;
    description             = "Phoenix Void RGB Gaming Mouse";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    /*-----------------------------------------------------------------*\
    | Off mode                                                          |
    \*-----------------------------------------------------------------*/
    mode off;
    off.name                = "Off";
    off.value               = HOLTEK_A09F_MODE_VALUE_OFF;
    off.flags               = MODE_FLAG_AUTOMATIC_SAVE;
    off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(off);

    /*-----------------------------------------------------------------*\
    | Standard (static colour — per DPI stage)                         |
    \*-----------------------------------------------------------------*/
    mode static_mode;
    static_mode.name        = "Static";
    static_mode.value       = HOLTEK_A09F_MODE_VALUE_STANDARD;
    static_mode.flags       = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    static_mode.color_mode  = MODE_COLORS_PER_LED;
    modes.push_back(static_mode);

    /*-----------------------------------------------------------------*\
    | Breathing                                                         |
    \*-----------------------------------------------------------------*/
    mode breathing;
    breathing.name          = "Breathing";
    breathing.value         = HOLTEK_A09F_MODE_VALUE_BREATHING;
    breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    breathing.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(breathing);

    /*-----------------------------------------------------------------*\
    | Neon                                                              |
    \*-----------------------------------------------------------------*/
    mode neon;
    neon.name               = "Neon";
    neon.value              = HOLTEK_A09F_MODE_VALUE_NEON;
    neon.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    neon.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(neon);

    /*-----------------------------------------------------------------*\
    | Wave                                                              |
    \*-----------------------------------------------------------------*/
    mode wave;
    wave.name               = "Wave";
    wave.value              = HOLTEK_A09F_MODE_VALUE_WAVE;
    wave.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    wave.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(wave);

    /*-----------------------------------------------------------------*\
    | Key Reaction                                                      |
    \*-----------------------------------------------------------------*/
    mode key_reaction;
    key_reaction.name       = "Key Reaction";
    key_reaction.value      = HOLTEK_A09F_MODE_VALUE_KEY_REACTION;
    key_reaction.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    key_reaction.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(key_reaction);

    /*-----------------------------------------------------------------*\
    | Trailing                                                          |
    \*-----------------------------------------------------------------*/
    mode trailing;
    trailing.name           = "Trailing";
    trailing.value          = HOLTEK_A09F_MODE_VALUE_TRAILING;
    trailing.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    trailing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(trailing);

    /*-----------------------------------------------------------------*\
    | Drag                                                              |
    \*-----------------------------------------------------------------*/
    mode drag;
    drag.name               = "Drag";
    drag.value              = HOLTEK_A09F_MODE_VALUE_DRAG;
    drag.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    drag.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(drag);

    /*-----------------------------------------------------------------*\
    | Slide                                                             |
    \*-----------------------------------------------------------------*/
    mode slide;
    slide.name              = "Slide";
    slide.value             = HOLTEK_A09F_MODE_VALUE_SLIDE;
    slide.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    slide.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(slide);

    /*-----------------------------------------------------------------*\
    | Yo-Yo                                                             |
    \*-----------------------------------------------------------------*/
    mode yoyo;
    yoyo.name               = "Yo-Yo";
    yoyo.value              = HOLTEK_A09F_MODE_VALUE_YO_YO;
    yoyo.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    yoyo.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(yoyo);

    /*-----------------------------------------------------------------*\
    | Marbles                                                           |
    \*-----------------------------------------------------------------*/
    mode marbles;
    marbles.name            = "Marbles";
    marbles.value           = HOLTEK_A09F_MODE_VALUE_MARBLES;
    marbles.flags           = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    marbles.color_mode      = MODE_COLORS_PER_LED;
    modes.push_back(marbles);

    /*-----------------------------------------------------------------*\
    | Flying Star                                                       |
    \*-----------------------------------------------------------------*/
    mode flying_star;
    flying_star.name        = "Flying Star";
    flying_star.value       = HOLTEK_A09F_MODE_VALUE_FLYING_STAR;
    flying_star.flags       = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    flying_star.color_mode  = MODE_COLORS_PER_LED;
    modes.push_back(flying_star);

    SetupZones();
}

RGBController_HoltekA09F::~RGBController_HoltekA09F()
{
    delete controller;
}

void RGBController_HoltekA09F::SetupZones()
{
    /*-----------------------------------------------------------------*\
    | One zone with 8 LEDs — one per DPI stage                          |
    \*-----------------------------------------------------------------*/
    zone dpi_zone;
    dpi_zone.name           = "DPI Stages";
    dpi_zone.type           = ZONE_TYPE_LINEAR;
    dpi_zone.leds_min       = HOLTEK_A09F_DPI_STAGES;
    dpi_zone.leds_max       = HOLTEK_A09F_DPI_STAGES;
    dpi_zone.leds_count     = HOLTEK_A09F_DPI_STAGES;
    dpi_zone.matrix_map     = NULL;
    zones.push_back(dpi_zone);

    for(unsigned int i = 0; i < HOLTEK_A09F_DPI_STAGES; i++)
    {
        led stage_led;
        stage_led.name = "DPI Stage " + std::to_string(i + 1);
        leds.push_back(stage_led);
    }

    SetupColors();
}

void RGBController_HoltekA09F::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HoltekA09F::DeviceUpdateLEDs()
{
    controller->SetDPIColors(colors.data(), (unsigned int)colors.size());
}

void RGBController_HoltekA09F::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HoltekA09F::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HoltekA09F::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].value);
}
