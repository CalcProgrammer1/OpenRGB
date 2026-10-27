/*---------------------------------------------------------*\
| RGBController_AlienwareKeyboard.cpp                       |
|                                                           |
|   RGBController for Alienware AW510K keyboard             |
|                                                           |
|   Mohamad Sallal (msallal)                    22 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <iterator>
#include "RGBControllerKeyNames.h"
#include "RGBController_AlienwareKeyboard.h"

int GetAlienwareWaveDirection(int input);

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

/**------------------------------------------------------------------*\
    @name Alienware AW510 Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAlienwareKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AlienwareKeyboard::RGBController_AlienwareKeyboard(AlienwareKeyboardController* controller_ptr, const alienware_device* device_ptr)
{
    controller                      = controller_ptr;
    device                          = device_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "Alienware";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = device->name;
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = ALIENWARE_MODE_DIRECT_PER_LED;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = ALIENWARE_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Pulse;
    Pulse.name                      = "Pulse";
    Pulse.value                     = ALIENWARE_MODE_PULSE;
    Pulse.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Pulse.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Pulse.speed_min                 = ALIENWARE_SPEED_SLOWEST;
    Pulse.speed_max                 = ALIENWARE_SPEED_NORMAL;
    Pulse.speed                     = ALIENWARE_SPEED_NORMAL;
    Pulse.colors_min                = 1;
    Pulse.colors_max                = 1;
    Pulse.colors.resize(1);
    modes.push_back(Pulse);

    mode Morph;
    Morph.name                      = "Morph";
    Morph.value                     = ALIENWARE_MODE_MORPH;
    Morph.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Morph.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Morph.speed_min                 = ALIENWARE_SPEED_SLOWEST;
    Morph.speed_max                 = ALIENWARE_SPEED_NORMAL;
    Morph.speed                     = ALIENWARE_SPEED_NORMAL;
    Morph.colors_min                = 2;
    Morph.colors_max                = 2;
    Morph.colors.resize(2);
    modes.push_back(Morph);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = ALIENWARE_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.speed_min             = ALIENWARE_SPEED_SLOWEST;
    Breathing.speed_max             = ALIENWARE_SPEED_FASTEST;
    Breathing.speed                 = ALIENWARE_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode SingleWave;
    SingleWave.name                  = "Single Wave";
    SingleWave.value                 = ALIENWARE_MODE_SINGLE_WAVE;
    SingleWave.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    SingleWave.speed_min             = ALIENWARE_SPEED_SLOWEST;
    SingleWave.speed_max             = ALIENWARE_SPEED_FASTEST;
    SingleWave.speed                 = ALIENWARE_SPEED_NORMAL;
    SingleWave.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    SingleWave.colors_min            = 1;
    SingleWave.colors_max            = 1;
    SingleWave.colors.resize(1);
    modes.push_back(SingleWave);

    mode RainbowWave;
    RainbowWave.name                  = "Rainbow Wave";
    RainbowWave.value                 = ALIENWARE_MODE_RAINBOW_WAVE;
    RainbowWave.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    RainbowWave.speed_min             = ALIENWARE_SPEED_SLOWEST;
    RainbowWave.speed_max             = ALIENWARE_SPEED_FASTEST;
    RainbowWave.speed                 = ALIENWARE_SPEED_NORMAL;
    RainbowWave.colors_min            = 1;
    RainbowWave.colors_max            = 1;
    RainbowWave.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    RainbowWave.colors.resize(1);
    modes.push_back(RainbowWave);

    mode Off;
    Off.name                          = "Off";
    Off.value                         = ALIENWARE_MODE_OFF;
    Off.flags                         = 0;
    Off.color_mode                    = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
    std::copy(colors.begin(), colors.end(),std::back_inserter(current_colors));
}

RGBController_AlienwareKeyboard::~RGBController_AlienwareKeyboard()
{
    Shutdown();

    delete controller;
}

void RGBController_AlienwareKeyboard::SetupZones()
{
    /*---------------------------------------------------------*    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name                   = device->zone_name;
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.leds_min               = device->zone_size;
    new_zone.leds_max               = device->zone_size;
    new_zone.leds_count             = device->zone_size;

    new_zone.matrix_map.Set(device->rows, device->cols, (unsigned int *)device->matrix_map);

    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < device->led_count; led_idx++)
    {
        led new_led;
        new_led.name  = device->leds[led_idx].name;
        new_led.value = device->leds[led_idx].idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_AlienwareKeyboard::DeviceUpdateLEDs()
{
    std::vector<SelectedKeys>   frame_buf_keys;
    std::vector<RGBColor>       new_colors;

    std::copy(colors.begin(), colors.end(),std::back_inserter(new_colors));

    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        if (current_colors[led_idx]==new_colors[led_idx])
        {
            /*---------------------------------------------*\
            | Don't send if key color is not changed        |
            \*---------------------------------------------*/
            continue;
        }

        if(RGBGetRValue(colors[led_idx]) != 0x00 || RGBGetGValue(colors[led_idx]) != 0x00 || RGBGetBValue(colors[led_idx]) != 0x00)
        {
            SelectedKeys key;

            key.idx     = (unsigned char)leds[led_idx].value;
            key.red     = RGBGetRValue(colors[led_idx]);
            key.green   = RGBGetGValue(colors[led_idx]);
            key.blue    = RGBGetBValue(colors[led_idx]);

            frame_buf_keys.push_back(key);
        }
        else
        {
            SelectedKeys key;

            key.idx     = (unsigned char)leds[led_idx].value;
            key.red     = RGBGetRValue(colors[led_idx]);
            key.green   = RGBGetGValue(colors[led_idx]);
            key.blue    = RGBGetBValue(colors[led_idx]);

            frame_buf_keys.push_back(key);
        }
    }

    controller->SendInitialize();
    controller->SendfeatureReport(0x05, 0x01, 0x51, 0x00);
    controller->SendCommit();

    if(frame_buf_keys.size() > 0)
    {
        controller->SendDirectOn(frame_buf_keys);
    }

    std::copy(new_colors.begin(), new_colors.end(),current_colors.begin());
}

void RGBController_AlienwareKeyboard::DeviceUpdateZoneLEDs(int zone)
{
    controller->SetDirect((unsigned char) zone, RGBGetRValue(zones[zone].colors[0]), RGBGetGValue(zones[zone].colors[0]), RGBGetBValue(zones[zone].colors[0]));
}

void RGBController_AlienwareKeyboard::DeviceUpdateSingleLED(int led)
{
    controller->DeviceUpdateSingleLED(leds[led].value, RGBGetRValue(colors[led]), RGBGetGValue(colors[led]), RGBGetBValue(colors[led]));
}

void RGBController_AlienwareKeyboard::DeviceUpdateMode()
{
    if(active_mode == 0xFFFF)
    {
        UpdateLEDsInternal();
        return;
    }

    controller->SendfeatureReport(0x05, 0x01, 0x51, 0x00);
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    switch(modes[active_mode].value)
    {
        case ALIENWARE_MODE_MORPH:
            /*---------------------------------------------*\
            | In case of morph it requires two colors       |
            | (color1 and color2)                           |
            \*---------------------------------------------*/
            {
                unsigned char red2 = RGBGetRValue(modes[active_mode].colors[1]);
                unsigned char grn2 = RGBGetGValue(modes[active_mode].colors[1]);
                unsigned char blu2 = RGBGetBValue(modes[active_mode].colors[1]);

                controller->SetMorphMode(modes[active_mode].value, modes[active_mode].speed, red, grn, blu, red2, grn2, blu2);
            }
            break;

        case ALIENWARE_MODE_SPECTRUM:
            /*---------------------------------------------*\
            | Spectrum only set mode, speed and colorMode   |
            \*---------------------------------------------*/
            controller->SetMode(modes[active_mode].value, modes[active_mode].speed, 0x00, ALIENWARE_RAINBOW_COLOR_MODE, 0x00, 0x00, 0x00);
            break;

        case ALIENWARE_MODE_SINGLE_WAVE:
            /*---------------------------------------------*\
            | Wave only set mode, speed, direction and      |
            | colorMode                                     |
            \*---------------------------------------------*/
            {
                int waveDirection = GetAlienwareWaveDirection(modes[active_mode].direction);
                controller->SetMode(modes[active_mode].value, modes[active_mode].speed, waveDirection, ALIENWARE_SINGLE_COLOR_MODE, red, grn, blu);
            }
            break;

        case ALIENWARE_MODE_RAINBOW_WAVE:
            /*---------------------------------------------*\
            | Wave only set mode, speed, direction and      |
            | colorMode                                     |
            \*---------------------------------------------*/
            {
            int waveDirection = GetAlienwareWaveDirection(modes[active_mode].direction);
            controller->SetMode(modes[active_mode].value, modes[active_mode].speed, waveDirection, ALIENWARE_RAINBOW_COLOR_MODE, 0x00, 0x00, 0x00);
            }
            break;


        default:
            controller->SetMode(modes[active_mode].value, modes[active_mode].speed, 0x00, ALIENWARE_SINGLE_COLOR_MODE, red, grn, blu);
            break;
    }
}

int GetAlienwareWaveDirection(int input)
{
    switch(input)
    {
        case MODE_DIRECTION_LEFT:
            return(ALIENWARE_DIRECTION_RIGHT_TO_LEFT);

        case MODE_DIRECTION_RIGHT:
            return(ALIENWARE_DIRECTION_LEFT_TO_RIGHT);

        case MODE_DIRECTION_UP:
            return(ALIENWARE_DIRECTION_BOTTOM_TO_TOP);

        case MODE_DIRECTION_DOWN:
            return(ALIENWARE_DIRECTION_TOP_TO_BOTTOM);

        default:
            return(ALIENWARE_DIRECTION_RIGHT_TO_LEFT);
    }
}

