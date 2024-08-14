/*---------------------------------------------------------*\
| RGBController_Razer.cpp                                   |
|                                                           |
|   RGBController for Razer devices                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                22 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_Razer.h"
#include "RazerDevices.h"
#include "LogManager.h"

/**------------------------------------------------------------------*\
    @name Razer
    @category Keyboard,Microphone,Mouse,Mousemat,HeadsetStand,Case,GPU,Accessory
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRazerControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Razer::RGBController_Razer(RazerController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "Razer";
    type                    = controller->GetDeviceType();
    description             = "Razer Device";
    location                = controller->GetDeviceLocation();
    version                 = controller->GetFirmwareString();
    serial                  = controller->GetSerialString();
    uint8_t max_brightness  = controller->GetMaxBrightness();

    if(type == DEVICE_TYPE_KEYBOARD)
    {
        LOG_DEBUG("[%s] Checking Keyboard Layout", name.c_str());
        std::string layout = controller->GetKeyboardLayoutString();

        LOG_DEBUG("[%s] returned: %s", name.c_str(), layout.c_str());
        description.append(", ");
        description.append(layout);
    }

    LOG_DEBUG("[%s] Checking variant", name.c_str());
    std::string variant = controller->GetVariantName();

    LOG_DEBUG("[%s] returned: %s", name.c_str(), variant.c_str());
    description.append(", ");
    description.append(variant);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = RAZER_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 0;
    Direct.brightness_max   = max_brightness;
    Direct.brightness       = max_brightness;
    modes.push_back(Direct);

    mode Off;
    Off.name                = "Off";
    Off.value               = RAZER_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name             = "Static";
    Static.value            = RAZER_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min       = 1;
    Static.colors_max       = 1;
    Static.colors.resize(1);
    Static.brightness_min   = 0;
    Static.brightness_max   = max_brightness;
    Static.brightness       = max_brightness;
    modes.push_back(Static);

    if(controller->SupportsBreathing())
    {
        mode Breathing;
        Breathing.name           = "Breathing";
        Breathing.value          = RAZER_MODE_BREATHING;
        Breathing.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Breathing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        Breathing.colors_min     = 1;
        Breathing.colors_max     = 2;
        Breathing.colors.resize(1);
        Breathing.brightness_min = 0;
        Breathing.brightness_max = max_brightness;
        Breathing.brightness     = max_brightness;
        modes.push_back(Breathing);
    }

    mode SpectrumCycle;
    SpectrumCycle.name           = "Spectrum Cycle";
    SpectrumCycle.value          = RAZER_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags          = MODE_FLAG_HAS_BRIGHTNESS;
    SpectrumCycle.color_mode     = MODE_COLORS_NONE;
    SpectrumCycle.brightness_min = 0;
    SpectrumCycle.brightness_max = max_brightness;
    SpectrumCycle.brightness     = max_brightness;
    modes.push_back(SpectrumCycle);

    if(controller->SupportsWave())
    {
        mode Wave;
        Wave.name               = "Wave";
        Wave.value              = RAZER_MODE_WAVE;
        Wave.flags              = MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS;
        Wave.direction          = MODE_DIRECTION_RIGHT;
        Wave.color_mode         = MODE_COLORS_NONE;
        Wave.brightness_min     = 0;
        Wave.brightness_max     = max_brightness;
        Wave.brightness         = max_brightness;
        modes.push_back(Wave);
    }

    if(controller->SupportsReactive())
    {
        mode Reactive;
        Reactive.name           = "Reactive";
        Reactive.value          = RAZER_MODE_REACTIVE;
        Reactive.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Reactive.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        Reactive.colors_min     = 1;
        Reactive.colors_max     = 1;
        Reactive.colors.resize(1);
        Reactive.brightness_min = 0;
        Reactive.brightness_max = max_brightness;
        Reactive.brightness     = max_brightness;
        modes.push_back(Reactive);
    }

    SetupZones();
}

RGBController_Razer::~RGBController_Razer()
{
    delete controller;
}

void RGBController_Razer::SetupZones()
{
    unsigned int device_index = controller->GetDeviceIndex();
    unsigned char layout_type = controller->GetKeyboardLayoutType();

    /*---------------------------------------------------------*\
    | Fill in zone information based on device table            |
    \*---------------------------------------------------------*/
    for(unsigned int zone_id = 0; zone_id < RAZER_MAX_ZONES; zone_id++)
    {
        if(device_list[device_index]->zones[zone_id] != NULL)
        {
            zone new_zone;

            new_zone.name       = device_list[device_index]->zones[zone_id]->name;
            new_zone.type       = device_list[device_index]->zones[zone_id]->type;

            new_zone.leds_count = device_list[device_index]->zones[zone_id]->rows * device_list[device_index]->zones[zone_id]->cols;
            new_zone.leds_min   = new_zone.leds_count;
            new_zone.leds_max   = new_zone.leds_count;

            /*---------------------------------------------------------*\
            | If this is a keyboard zone, check if using Keyboard Layout|
            | Manager                                                   |
            \*---------------------------------------------------------*/
            if(new_zone.type == ZONE_TYPE_MATRIX)
            {
                if(new_zone.name == ZONE_EN_KEYBOARD && device_list[device_index]->layout != NULL)
                {
                    /*---------------------------------------------------------*\
                    | Dynamically generate a keyboard layout                    |
                    \*---------------------------------------------------------*/
                    KEYBOARD_LAYOUT new_layout;
                    switch(layout_type)
                    {
                        case RAZER_LAYOUT_TYPE_AZERTY:
                            new_layout = KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ISO_AZERTY;
                            break;

                        case RAZER_LAYOUT_TYPE_ISO:
                            new_layout = KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ISO_QWERTY;
                            break;

                        case RAZER_LAYOUT_TYPE_JIS:
                            new_layout = KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ANSI_QWERTY;
                            break;

                        case RAZER_LAYOUT_TYPE_QWERTZ:
                            new_layout = KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ISO_QWERTZ;
                            break;

                        default:
                            new_layout = KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ANSI_QWERTY;
                    }

                    KeyboardLayoutManager new_kb(new_layout, device_list[device_index]->layout->base_size,
                                                             device_list[device_index]->layout->key_values);

                    matrix_map_type * new_map   = new matrix_map_type;
                    new_zone.matrix_map         = new_map;
                    new_map->height             = device_list[device_index]->zones[zone_id]->rows;
                    new_map->width              = device_list[device_index]->zones[zone_id]->cols;
                    new_map->map                = new unsigned int[new_map->height * new_map->width];

                    if(device_list[device_index]->layout->base_size != KEYBOARD_SIZE::KEYBOARD_SIZE_EMPTY)
                    {
                        /*---------------------------------------------------------*\
                        | Minor adjustments to keyboard layout                      |
                        \*---------------------------------------------------------*/
                        keyboard_keymap_overlay_values* temp = device_list[device_index]->layout;
                        new_kb.ChangeKeys(*temp);

                        /*---------------------------------------------------------*\
                        | Matrix map still uses declared zone rows and columns      |
                        |   as the packet structure depends on the matrix map       |
                        \*---------------------------------------------------------*/
                        new_kb.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_INDEX, new_map->height, new_map->width);
                    }

                    zones.push_back(new_zone);

                    /*---------------------------------------------------------*\
                    | Check the dynamic layout                                  |
                    \*---------------------------------------------------------*/
                    if(new_kb.GetKeyCount() > 0)
                    {
                        for(std::size_t row = 0; row < zones[zone_id].matrix_map->height; row++)
                        {
                            for(std::size_t col = 0; col < zones[zone_id].matrix_map->width; col++)
                            {
                                led new_led;

                                new_led.name = new_kb.GetKeyNameAt((unsigned int)row, (unsigned int)col);

                                leds.push_back(new_led);
                            }
                        }
                    }

                    continue;
                }
                else
                {
                    /*---------------------------------------------------------*\
                    | Handle all other matrix type zones by filling in all      |
                    | entries                                                   |
                    \*---------------------------------------------------------*/
                    matrix_map_type * new_map   = new matrix_map_type;
                    new_zone.matrix_map         = new_map;
                    new_map->height             = device_list[device_index]->zones[zone_id]->rows;
                    new_map->width              = device_list[device_index]->zones[zone_id]->cols;
                    new_map->map                = new unsigned int[new_map->height * new_map->width];

                    for(unsigned int y = 0; y < new_map->height; y++)
                    {
                        for(unsigned int x = 0; x < new_map->width; x++)
                        {
                            new_map->map[(y * new_map->width) + x] = (y * new_map->width) + x;
                        }
                    }
                }
            }
            else
            {
                new_zone.matrix_map = NULL;
            }

            zones.push_back(new_zone);

            for (unsigned int row_id = 0; row_id < device_list[device_index]->zones[zone_id]->rows; row_id++)
            {
                for (unsigned int col_id = 0; col_id < device_list[device_index]->zones[zone_id]->cols; col_id++)
                {
                    led* new_led = new led();

                    new_led->name = device_list[device_index]->zones[zone_id]->name;

                    if(zones[zone_id].leds_count > 1)
                    {
                        new_led->name.append(" LED ");
                        new_led->name.append(std::to_string(col_id + 1));
                    }

                    leds.push_back(*new_led);
                }
            }
        }
    }

    SetupColors();
}

void RGBController_Razer::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Razer::DeviceUpdateLEDs()
{
    controller->SetLEDs(&colors[0]);
}

void RGBController_Razer::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Razer::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Razer::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case RAZER_MODE_OFF:
            controller->SetModeOff();
            break;

        case RAZER_MODE_STATIC:
            if(modes[active_mode].colors.size() == 1)
            {
                unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
                unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
                unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);

                controller->SetModeStatic(red, grn, blu);
            }
            break;

        case RAZER_MODE_BREATHING:
            if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
            {
                controller->SetModeBreathingRandom();
            }
            else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                if(modes[active_mode].colors.size() == 1)
                {
                    unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
                    unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
                    unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);

                    controller->SetModeBreathingOneColor(red, grn, blu);
                }
                else if(modes[active_mode].colors.size() == 2)
                {
                    unsigned char red1 = RGBGetRValue(modes[active_mode].colors[0]);
                    unsigned char grn1 = RGBGetGValue(modes[active_mode].colors[0]);
                    unsigned char blu1 = RGBGetBValue(modes[active_mode].colors[0]);
                    unsigned char red2 = RGBGetRValue(modes[active_mode].colors[1]);
                    unsigned char grn2 = RGBGetGValue(modes[active_mode].colors[1]);
                    unsigned char blu2 = RGBGetBValue(modes[active_mode].colors[1]);

                    controller->SetModeBreathingTwoColors(red1, grn1, blu1, red2, grn2, blu2);
                }
            }
            break;

        case RAZER_MODE_SPECTRUM_CYCLE:
            controller->SetModeSpectrumCycle();
            break;

        case RAZER_MODE_WAVE:
            switch(modes[active_mode].direction)
            {
                case MODE_DIRECTION_LEFT:
                    controller->SetModeWave(2);
                    break;

                default:
                    controller->SetModeWave(1);
                    break;
            }
            break;
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        controller->SetBrightness(modes[active_mode].brightness);
    }
    else
    {
        controller->SetBrightness(255);
    }
}
