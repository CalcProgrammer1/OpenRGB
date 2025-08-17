/*---------------------------------------------------------*\
|  RGBController_KingstonFuryDRAM.cpp                       |
|                                                           |
|   Driver for Kingston Fury DDR4/5 RAM modules             |
|                                                           |
|   Geofrey Mon (geofbot)                       14 Jul 2024 |
|   Milan Cermak (krysmanta)                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_KingstonFuryDRAM.h"
#include "KingstonFuryDRAMController.h"
#include "LogManager.h"

const RGBColor default_colors[] =
{
    ToRGBColor(0xFF, 0x00, 0x00),
    ToRGBColor(0x00, 0xFF, 0x00),
    ToRGBColor(0xFF, 0x64, 0x00),
    ToRGBColor(0x00, 0x00, 0xFF),
    ToRGBColor(0xEF, 0xEF, 0x00),
    ToRGBColor(0x80, 0x00, 0x80),
    ToRGBColor(0x00, 0x6D, 0x77),
    ToRGBColor(0xFF, 0xC8, 0x00),
    ToRGBColor(0xFF, 0x55, 0xFF),
    ToRGBColor(0x3C, 0x7D, 0xFF),
};


/**------------------------------------------------------------------*\
    @name Kingston Fury DDR4/5 DRAM
    @category RAM
    @type SMBus
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectKingstonFuryDRAMControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_KingstonFuryDRAM::RGBController_KingstonFuryDRAM(KingstonFuryDRAMController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "Kingston";
    type                    = DEVICE_TYPE_DRAM;
    description             = "Kingston Fury Beast/Renegade DDR4/5 DRAM Device";
    location                = controller->GetDeviceLocation();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = FURY_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 0;
    Direct.brightness_max   = 100;
    Direct.brightness       = 80;
    modes.push_back(Direct);

    mode Static;
    Static.name             = "Static";
    Static.value            = FURY_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min       = 1;
    Static.colors_max       = 1;
    Static.colors.assign(default_colors, default_colors + 1);
    Static.brightness_min   = 0;
    Static.brightness_max   = 100;
    Static.brightness       = 80;
    modes.push_back(Static);

    // All speed values are inverted
    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = FURY_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Rainbow.speed_min       = 60;
    Rainbow.speed_max       = 0;
    Rainbow.speed           = 25;
    Rainbow.direction       = MODE_DIRECTION_UP;
    Rainbow.brightness_min  = 0;
    Rainbow.brightness_max  = 100;
    Rainbow.brightness      = 80;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Spectrum;
    Spectrum.name           = "Spectrum";
    Spectrum.value          = FURY_MODE_SPECTRUM;
    Spectrum.flags          = MODE_FLAG_HAS_SPEED |
        MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Spectrum.speed_min      = 60;
    Spectrum.speed_max      = 0;
    Spectrum.speed          = 25;
    Spectrum.direction      = MODE_DIRECTION_UP;
    Spectrum.brightness_min = 0;
    Spectrum.brightness_max = 100;
    Spectrum.brightness     = 80;
    Spectrum.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Spectrum);

    mode Rhythm;
    Rhythm.name             = "Rhythm";
    Rhythm.value            = FURY_MODE_RHYTHM;
    Rhythm.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rhythm.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Rhythm.colors_min       = 2;
    Rhythm.colors_max       = 11;
    Rhythm.colors.assign(default_colors, default_colors + 10);
    Rhythm.colors.push_back(FURY_DEFAULT_BG_COLOR);
    Rhythm.speed_min        = 10;
    Rhythm.speed_max        = 0;
    Rhythm.speed            = 0;
    Rhythm.brightness_min   = 0;
    Rhythm.brightness_max   = 100;
    Rhythm.brightness       = 80;
    modes.push_back(Rhythm);

    mode Breath;
    Breath.name             = "Breath";
    Breath.value            = FURY_MODE_BREATH;
    Breath.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Breath.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Breath.colors_min       = 1;
    Breath.colors_max       = 10;
    Breath.colors.assign(default_colors, default_colors + 10);
    Breath.speed_min        = 10;
    Breath.speed_max        = 1;
    Breath.speed            = 5;
    Breath.brightness_min   = 0;
    Breath.brightness_max   = 100;
    Breath.brightness       = 80;
    modes.push_back(Breath);

    mode Dynamic;
    Dynamic.name            = "Dynamic";
    Dynamic.value           = FURY_MODE_DYNAMIC;
    Dynamic.flags           = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Dynamic.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Dynamic.colors_min      = 1;
    Dynamic.colors_max      = 10;
    Dynamic.colors.assign(default_colors, default_colors + 10);
    Dynamic.speed_min       = 1000;
    Dynamic.speed_max       = 100;
    Dynamic.speed           = 300;
    Dynamic.brightness_min  = 0;
    Dynamic.brightness_max  = 100;
    Dynamic.brightness      = 80;
    modes.push_back(Dynamic);

    mode Slide;
    Slide.name              = "Slide";
    Slide.value             = FURY_MODE_SLIDE;
    Slide.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Slide.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Slide.colors_min        = 2;
    Slide.colors_max        = 11;
    Slide.colors.assign(default_colors, default_colors + 10);
    Slide.colors.push_back(FURY_DEFAULT_BG_COLOR);
    Slide.speed_min         = 255;
    Slide.speed_max         = 0;
    Slide.speed             = 8;
    Slide.direction         = MODE_DIRECTION_UP;
    Slide.brightness_min    = 0;
    Slide.brightness_max    = 100;
    Slide.brightness        = 80;
    modes.push_back(Slide);

    mode Slither;
    Slither.name            = "Slither";
    Slither.value           = FURY_MODE_SLITHER;
    Slither.flags           = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Slither.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Slither.colors_min      = 2;
    Slither.colors_max      = 11;
    Slither.colors.assign(default_colors, default_colors + 10);
    Slither.colors.push_back(FURY_DEFAULT_BG_COLOR);
    Slither.speed_min       = 255;
    Slither.speed_max       = 0;
    Slither.speed           = 40;
    Slither.brightness_min  = 0;
    Slither.brightness_max  = 100;
    Slither.brightness      = 80;
    modes.push_back(Slither);

    mode Teleport;
    Teleport.name           = "Teleport";
    Teleport.value          = FURY_MODE_TELEPORT;
    Teleport.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Teleport.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Teleport.colors_min     = 2;
    Teleport.colors_max     = 11;
    Teleport.colors.assign(default_colors, default_colors + 10);
    Teleport.colors.push_back(FURY_DEFAULT_BG_COLOR);
    Teleport.speed_min      = 255;
    Teleport.speed_max      = 0;
    Teleport.speed          = 8;
    Teleport.brightness_min = 0;
    Teleport.brightness_max = 100;
    Teleport.brightness = 80;
    modes.push_back(Teleport);

    mode Wind;
    Wind.name               = "Wind";
    Wind.value              = FURY_MODE_WIND;
    Wind.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Wind.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Wind.colors_min         = 2;
    Wind.colors_max         = 11;
    Wind.colors.assign(default_colors, default_colors + 10);
    Wind.colors.push_back(FURY_DEFAULT_BG_COLOR);
    Wind.speed_min          = 255;
    Wind.speed_max          = 0;
    Wind.speed              = 8;
    Wind.direction          = MODE_DIRECTION_UP;
    Wind.brightness_min     = 0;
    Wind.brightness_max     = 100;
    Wind.brightness         = 80;
    modes.push_back(Wind);

    mode Comet;
    Comet.name              = "Comet";
    Comet.value             = FURY_MODE_COMET;
    Comet.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Comet.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Comet.colors_min        = 1;
    Comet.colors_max        = 10;
    Comet.colors.assign(default_colors, default_colors + 10);
    Comet.speed_min         = 255;
    Comet.speed_max         = 0;
    Comet.speed             = 25;
    Comet.direction         = MODE_DIRECTION_UP;
    Comet.brightness_min    = 0;
    Comet.brightness_max    = 100;
    Comet.brightness        = 80;
    modes.push_back(Comet);

    mode Rain;
    Rain.name               = "Rain";
    Rain.value              = FURY_MODE_RAIN;
    Rain.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Rain.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Rain.colors_min         = 1;
    Rain.colors_max         = 10;
    Rain.colors.assign(default_colors, default_colors + 10);
    Rain.speed_min          = 28;
    Rain.speed_max          = 8;
    Rain.speed              = 25;
    Rain.direction          = MODE_DIRECTION_DOWN;
    Rain.brightness_min     = 0;
    Rain.brightness_max     = 100;
    Rain.brightness         = 80;
    modes.push_back(Rain);

    mode Firework;
    Firework.name           = "Firework";
    Firework.value          = FURY_MODE_FIREWORK;
    Firework.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Firework.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Firework.colors_min     = 1;
    Firework.colors_max     = 10;
    Firework.colors.assign(default_colors, default_colors + 10);
    Firework.speed_min      = 83;
    Firework.speed_max      = 33;
    Firework.speed          = 33;
    Firework.direction      = MODE_DIRECTION_UP;
    Firework.brightness_min = 0;
    Firework.brightness_max = 100;
    Firework.brightness     = 80;
    modes.push_back(Firework);

    mode Voltage;
    Voltage.name            = "Voltage";
    Voltage.value           = FURY_MODE_VOLTAGE;
    Voltage.flags           = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Voltage.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Voltage.colors_min      = 2;
    Voltage.colors_max      = 11;
    Voltage.colors.assign(default_colors, default_colors + 10);
    Voltage.colors.push_back(FURY_DEFAULT_BG_COLOR);
    Voltage.speed_min       = 18;
    Voltage.speed_max       = 5;
    Voltage.speed           = 16;
    Voltage.direction       = MODE_DIRECTION_UP;
    Voltage.brightness_min  = 0;
    Voltage.brightness_max  = 100;
    Voltage.brightness      = 80;
    modes.push_back(Voltage);

#ifdef FURY_SYNC
    mode Countdown;
    Countdown.name           = "Countdown";
    Countdown.value          = FURY_MODE_COUNTDOWN;
    Countdown.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Countdown.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Countdown.colors_min     = 2;
    Countdown.colors_max     = 11;
    Countdown.colors.assign(default_colors, default_colors + 10);
    Countdown.colors.push_back(FURY_DEFAULT_BG_COLOR);
    Countdown.speed_min      = 76;
    Countdown.speed_max      = 20;
    Countdown.speed          = 76;
    Countdown.direction      = MODE_DIRECTION_UP;
    Countdown.brightness_min = 0;
    Countdown.brightness_max = 100;
    Countdown.brightness     = 80;
    modes.push_back(Countdown);
#endif

    mode Flame;
    Flame.name              = "Flame";
    Flame.value             = FURY_MODE_FLAME;
    Flame.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Flame.speed_min         = 64;
    Flame.speed_max         = 40;
    Flame.speed             = 64;
    Flame.direction         = MODE_DIRECTION_UP;
    Flame.brightness_min    = 0;
    Flame.brightness_max    = 100;
    Flame.brightness        = 80;
    Flame.color_mode        = MODE_COLORS_NONE;
    modes.push_back(Flame);

    mode Twilight;
    Twilight.name           = "Twilight";
    Twilight.value          = FURY_MODE_TWILIGHT;
    Twilight.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Twilight.speed_min      = 255;
    Twilight.speed_max      = 0;
    Twilight.speed          = 64;
    Twilight.brightness_min = 0;
    Twilight.brightness_max = 100;
    Twilight.brightness     = 80;
    Twilight.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Twilight);

    mode Fury;
    Fury.name               = "Fury";
    Fury.value              = FURY_MODE_FURY;
    Fury.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD;
    Fury.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Fury.colors_min         = 2;
    Fury.colors_max         = 11;
    Fury.colors.assign(default_colors, default_colors + 10);
    Fury.colors.push_back(FURY_DEFAULT_BG_COLOR);
    Fury.speed_min          = 255;
    Fury.speed_max          = 0;
    Fury.speed              = 76;
    Fury.direction          = MODE_DIRECTION_UP;
    Fury.brightness_min     = 0;
    Fury.brightness_max     = 100;
    Fury.brightness         = 80;
    modes.push_back(Fury);

    mode Prism;
    Prism.name              = "Prism";
    Prism.value             = FURY_MODE_PRISM;
    Prism.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Prism.speed_min         = 60;
    Prism.speed_max         = 0;
    Prism.speed             = 40;
    Prism.brightness_min    = 0;
    Prism.brightness_max    = 100;
    Prism.brightness        = 80;
    Prism.color_mode        = MODE_COLORS_NONE;
    modes.push_back(Prism);

    SetupZones();

    // default per-LED color is red
    colors.assign(colors.size(), default_colors[0]);
}

RGBController_KingstonFuryDRAM::~RGBController_KingstonFuryDRAM()
{
    delete controller;
}

void RGBController_KingstonFuryDRAM::SetupZones()
{
    for(unsigned int slot = 0; slot < controller->GetSlotCount(); slot++)
    {
        zone* new_zone          = new zone;

        new_zone->name          = "Fury Slot ";
        new_zone->name.append(std::to_string(slot + 1));
        new_zone->type          = ZONE_TYPE_LINEAR;
        new_zone->leds_min      = controller->GetLEDPerDIMM();
        new_zone->leds_max      = new_zone->leds_min;
        new_zone->leds_count    = new_zone->leds_min;
        new_zone->matrix_map    = NULL;

        zones.push_back(*new_zone);
    }

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led* new_led        = new led();

            new_led->name       = "Fury Slot ";
            new_led->name.append(std::to_string(zone_idx + 1));
            new_led->name.append(", LED ");
            new_led->name.append(std::to_string(led_idx + 1));

            new_led->value      = (unsigned int)leds.size();

            leds.push_back(*new_led);
        }
    }

    SetupColors();
}

void RGBController_KingstonFuryDRAM::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
    LOG_DEBUG("[%s] resize zone",
              FURY_CONTROLLER_NAME);
}

// some modes have different actual values to be written, depending on the color mode
unsigned char RGBController_KingstonFuryDRAM::GetRealModeValue()
{
    int mode_value = modes[active_mode].value;
    switch(mode_value)
    {
    case FURY_MODE_BREATH:
        if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            return FURY_MODE_BREATH;
        }
        else
        {
            return FURY_MODE_BREATH_DIRECT;
        }
    }
    return mode_value;
}

void RGBController_KingstonFuryDRAM::DeviceUpdateLEDs()
{
    controller->SendBegin();
    controller->SetMode(GetRealModeValue());

    // Fixed mode specific parameters
    switch(modes[active_mode].value)
    {
    case FURY_MODE_STATIC:
        controller->SetRegister(FURY_REG_DIRECTION, FURY_DIR_BOTTOM_TO_TOP);
        controller->SetRegister(FURY_REG_DELAY, 0);
        controller->SetRegister(FURY_REG_SPEED, 0);
        break;

    case FURY_MODE_RAINBOW:
    case FURY_MODE_VOLTAGE:
    case FURY_MODE_COUNTDOWN:
    case FURY_MODE_FLAME:
    case FURY_MODE_TWILIGHT:
    case FURY_MODE_FURY:
        controller->SetRegister(FURY_REG_DELAY, 0);
        break;

    case FURY_MODE_RHYTHM:
        controller->SetRegister(FURY_REG_DIRECTION,
                                FURY_DIR_BOTTOM_TO_TOP);
        break;

    case FURY_MODE_BREATH:
        controller->SetRegister(FURY_REG_DIRECTION,
                                FURY_DIR_BOTTOM_TO_TOP);
        controller->SetRegister(FURY_REG_DELAY, 0);
        break;

    case FURY_MODE_DYNAMIC:
        controller->SetRegister(FURY_REG_DIRECTION,
                                FURY_DIR_BOTTOM_TO_TOP);
        controller->SetRegister(FURY_REG_DELAY, 0);
        break;

    case FURY_MODE_SLITHER:
        controller->SetRegister(FURY_REG_DELAY, 12);
        controller->SetRegister(FURY_REG_DIRECTION,
                                FURY_ALT_DIRECTIONS);
        break;

    case FURY_MODE_TELEPORT:
        controller->SetRegister(FURY_REG_DELAY, 0);
        controller->SetRegister(FURY_REG_DIRECTION,
                                FURY_ALT_DIRECTIONS);
        break;

    case FURY_MODE_RAIN:
        controller->SetRegister(FURY_REG_DELAY, 0);
        controller->SetRegister(FURY_REG_LENGTH, 3);
        break;

    case FURY_MODE_FIREWORK:
        controller->SetRegister(FURY_REG_DELAY, 0);
        controller->SetRegister(FURY_REG_LENGTH, 7);
        break;
    }

    // Mode-specific parameters that are customizable in Kingston's software
    // but which are not yet available in the OpenRGB interface.
    // Default values are used here and the parameter ranges are annotated
    switch(modes[active_mode].value)
    {
    case FURY_MODE_RHYTHM:
        // between 2 and 5
        controller->SetRegister(FURY_REG_DELAY, 3);
        break;


    case FURY_MODE_SLIDE:
        // between 1 and 4
        controller->SetRegister(FURY_REG_DELAY, 3);
        // between 1 and 12
        controller->SetRegister(FURY_REG_LENGTH, 4);
        break;

    case FURY_MODE_SLITHER:
        // between 1 and 32
        controller->SetRegister(FURY_REG_LENGTH, 12);
        break;

    case FURY_MODE_TELEPORT:
        // between 1 and 12
        controller->SetRegister(FURY_REG_LENGTH, 3);
        break;

    case FURY_MODE_WIND:
        // between 0 and 32
        controller->SetRegister(FURY_REG_DELAY, 0);
        // between 1 and 32
        controller->SetRegister(FURY_REG_LENGTH, 12);
        break;

    case FURY_MODE_COMET:
        // between 0 and 20
        controller->SetRegister(FURY_REG_DELAY, 0);
        // between 1 and 18
        controller->SetRegister(FURY_REG_LENGTH, 7);
        break;

    case FURY_MODE_PRISM:
        // between 2 and 4
        controller->SetRegister(FURY_REG_DELAY, 2);
        break;

    case FURY_MODE_SPECTRUM:
        // between 2 and 6
        controller->SetRegister(FURY_REG_DELAY, 4);
        break;
    }

    switch(modes[active_mode].color_mode)
    {
    case MODE_COLORS_PER_LED:
        controller->SetLEDColors(colors);
        break;

    case MODE_COLORS_MODE_SPECIFIC:
        switch(modes[active_mode].value)
        {
        case FURY_MODE_RHYTHM:
        case FURY_MODE_SLIDE:
        case FURY_MODE_SLITHER:
        case FURY_MODE_TELEPORT:
        case FURY_MODE_WIND:
        case FURY_MODE_VOLTAGE:
        case FURY_MODE_COUNTDOWN:
        case FURY_MODE_FURY:
            {
                std::vector<RGBColor> mode_colors(modes[active_mode].colors.begin(),
                                                  modes[active_mode].colors.end() - 1);
                controller->SetModeColors(mode_colors);
                // handle background color
                RGBColor color = modes[active_mode].colors[mode_colors.size()];
                unsigned char red = RGBGetRValue(color);
                unsigned char green = RGBGetGValue(color);
                unsigned char blue = RGBGetBValue(color);

                controller->SetRegister(FURY_REG_BG_RED, red);
                controller->SetRegister(FURY_REG_BG_GREEN, green);
                controller->SetRegister(FURY_REG_BG_BLUE, blue);
                break;
            }
        default:
            controller->SetModeColors(modes[active_mode].colors);
            break;
        }
        break;
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_UD)
    {
        if(modes[active_mode].direction == MODE_DIRECTION_UP)
        {
            controller->SetRegister(FURY_REG_DIRECTION,
                                    FURY_DIR_BOTTOM_TO_TOP);
        }
        else
        {
            controller->SetRegister(FURY_REG_DIRECTION,
                                    FURY_DIR_TOP_TO_BOTTOM);
        }
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        switch(modes[active_mode].value)
        {
        case FURY_MODE_DYNAMIC:
            controller->SetRegister(FURY_REG_SPEED, 0);

            // time spent holding a color
            controller->SetRegister(FURY_REG_DYNAMIC_HOLD_A,
                                    modes[active_mode].speed >> 8);
            // set to 1 as long as the time above is nonzero
            controller->SetRegister(FURY_REG_DYNAMIC_HOLD_B, 1);

            // time spent fading to next color
            controller->SetRegister(FURY_REG_DYNAMIC_FADE_A,
                                    (modes[active_mode].speed * 5) >> 8);
            // set to 1 as long as the time above is nonzero
            controller->SetRegister(FURY_REG_DYNAMIC_FADE_B, 1);
            break;

        case FURY_MODE_BREATH:
            controller->SetRegister(FURY_REG_SPEED, 0);

            // These are the speed values used by Kingston's software,
            // representing the time spent fading between two brightness levels
            controller->SetRegister(FURY_REG_BREATH_MIN_TO_MID,
                                    modes[active_mode].speed * 3);
            controller->SetRegister(FURY_REG_BREATH_MID_TO_MAX,
                                    modes[active_mode].speed);
            controller->SetRegister(FURY_REG_BREATH_MAX_TO_MID,
                                    modes[active_mode].speed);
            controller->SetRegister(FURY_REG_BREATH_MID_TO_MIN,
                                    modes[active_mode].speed * 3);

            // Time spent holding min brightness
            controller->SetRegister(FURY_REG_BREATH_MIN_HOLD, 1);

            // Brightness values (relative to overall brightness)
            controller->SetRegister(FURY_REG_BREATH_MAX_BRIGHTNESS, 100);
            controller->SetRegister(FURY_REG_BREATH_MID_BRIGHTNESS, 64);
            controller->SetRegister(FURY_REG_BREATH_MIN_BRIGHTNESS, 0);
            // Kingston software uses 1 for min brightness,
            // but 0 seems to look better.

            break;

        case FURY_MODE_RAIN:
            {
                // speed offsets taken from Kingston software
                unsigned char offsets[4] = {11, 0, 15, 9};
                std::vector<unsigned char> speeds;
                for (std::size_t idx = 0; idx < controller->GetSlotCount(); idx++)
                {
                    speeds.push_back(modes[active_mode].speed + offsets[idx % 4]);
                }
                controller->SetRegister(FURY_REG_SPEED, speeds);
                break;
            }

        case FURY_MODE_FIREWORK:
            {
                // speed offsets taken from Kingston software
                unsigned char offsets[4] = {15, 0, 19, 4};
                std::vector<unsigned char> speeds;
                for (std::size_t idx = 0; idx < controller->GetSlotCount(); idx++)
                {
                    speeds.push_back(modes[active_mode].speed + offsets[idx % 4]);
                }
                controller->SetRegister(FURY_REG_SPEED, speeds);
                break;
            }

        default:
            controller->SetRegister(FURY_REG_SPEED, modes[active_mode].speed);
            break;
        }
    }

    controller->SetRegister(FURY_REG_BRIGHTNESS,
                            modes[active_mode].brightness);
    controller->SetNumSlots();
    controller->SendApply();
}

void RGBController_KingstonFuryDRAM::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_KingstonFuryDRAM::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_KingstonFuryDRAM::DeviceUpdateMode()
{
    LOG_DEBUG("[%s] device update mode",
              FURY_CONTROLLER_NAME);
    // Preamble only necessary when changing modes.
    if(GetRealModeValue() != controller->GetMode())
    {
        controller->SendPreamble(modes[active_mode].value != FURY_MODE_RAIN &&
                                 modes[active_mode].value != FURY_MODE_FIREWORK &&
                                 modes[active_mode].value != FURY_MODE_DIRECT);
    }
    DeviceUpdateLEDs();
}
