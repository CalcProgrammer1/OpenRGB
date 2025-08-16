/*---------------------------------------------------------*\
| RGBController_AsusAuraTUFKeyboard.cpp                     |
|                                                           |
|   RGBController for ASUS Aura TUF keyboard                |
|                                                           |
|   Mola19                                      03 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cmath>
#include <vector>
#include "RGBController_AsusAuraTUFKeyboard.h"

/**------------------------------------------------------------------*\
    @name Asus Aura TUF Keyboard
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraTUFUSBKeyboard
    @comment
\*-------------------------------------------------------------------*/

RGBController_AuraTUFKeyboard::RGBController_AuraTUFKeyboard(AuraTUFKeyboardController* controller_ptr)
{
    controller                          = controller_ptr;

    pid                                 = controller->device_pid;

    if(pid != AURA_ROG_CLAYMORE_PID)
    {
        name                            = controller->GetName();
        vendor                          = "ASUS";
        type                            = DEVICE_TYPE_KEYBOARD;
        description                     = "ASUS Aura Keyboard Device";
        version                         = controller->GetVersion();
        location                        = controller->GetDeviceLocation();
        serial                          = controller->GetSerialString();

        unsigned char AURA_KEYBOARD_SPEED_MIN     = 0;
        unsigned char AURA_KEYBOARD_SPEED_MAX     = 0;
        unsigned char AURA_KEYBOARD_SPEED_DEFAULT = 0;

        switch(pid)
        {
            case AURA_TUF_K1_GAMING_PID:
                AURA_KEYBOARD_SPEED_MIN          = 0;
                AURA_KEYBOARD_SPEED_MAX          = 2;
                AURA_KEYBOARD_SPEED_DEFAULT      = 1;
                break;

            case AURA_ROG_STRIX_FLARE_PID:
            case AURA_ROG_STRIX_FLARE_PNK_LTD_PID:
            case AURA_ROG_STRIX_FLARE_COD_BO4_PID:
            case AURA_TUF_K3_GAMING_PID:
            case AURA_TUF_K7_GAMING_PID:
                AURA_KEYBOARD_SPEED_MIN          = 15;
                AURA_KEYBOARD_SPEED_MAX          = 0;
                AURA_KEYBOARD_SPEED_DEFAULT      = 8;
                break;

            case AURA_ROG_AZOTH_USB_PID:
            case AURA_ROG_AZOTH_2_4_PID:
            case AURA_ROG_FALCHION_WIRED_PID:
            case AURA_ROG_FALCHION_WIRELESS_PID:
            case AURA_ROG_STRIX_FLARE_II_PID:
            case AURA_ROG_STRIX_FLARE_II_ANIMATE_PID:
            case AURA_ROG_STRIX_SCOPE_RX_PID:
            case AURA_ROG_STRIX_SCOPE_RX_EVA_02_PID:
            case AURA_ROG_STRIX_SCOPE_PID:
            case AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID:
            case AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID:
            case AURA_ROG_STRIX_SCOPE_II_PID:
            case AURA_ROG_STRIX_SCOPE_II_RX_PID:
            case AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID:
            case AURA_TUF_K5_GAMING_PID:
                AURA_KEYBOARD_SPEED_MIN          = 255;
                AURA_KEYBOARD_SPEED_MAX          = 0;
                AURA_KEYBOARD_SPEED_DEFAULT      = 30;
                break;

            default:
                AURA_KEYBOARD_SPEED_MIN          = 15;
                AURA_KEYBOARD_SPEED_MAX          = 0;
                AURA_KEYBOARD_SPEED_DEFAULT      = 8;
                break;
        }

        mode Direct;
        Direct.name                     = "Direct";
        Direct.value                    = AURA_KEYBOARD_MODE_DIRECT;
        Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode               = MODE_COLORS_PER_LED;
        modes.push_back(Direct);

        mode Static;
        Static.name                     = "Static";
        Static.value                    = AURA_KEYBOARD_MODE_STATIC;
        Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
        Static.brightness_min           = AURA_KEYBOARD_BRIGHTNESS_MIN;
        Static.brightness_max           = AURA_KEYBOARD_BRIGHTNESS_MAX;
        Static.brightness               = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
        Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
        Static.colors_min               = 1;
        Static.colors_max               = 1;
        Static.colors.resize(1);
        modes.push_back(Static);

        mode Breathing;
        Breathing.name                  = "Breathing";
        Breathing.value                 = AURA_KEYBOARD_MODE_BREATHING;
        Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
        if(controller->is_per_led_keyboard)
        {
            Breathing.flags            |= MODE_FLAG_HAS_RANDOM_COLOR;
        }
        Breathing.speed_min             = AURA_KEYBOARD_SPEED_MIN;
        Breathing.speed_max             = AURA_KEYBOARD_SPEED_MAX;
        Breathing.speed                 = AURA_KEYBOARD_SPEED_DEFAULT;
        Breathing.brightness_min        = AURA_KEYBOARD_BRIGHTNESS_MIN;
        Breathing.brightness_max        = AURA_KEYBOARD_BRIGHTNESS_MAX;
        Breathing.brightness            = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
        Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
        Breathing.colors_min            = 1;
        Breathing.colors_max            = 2;
        Breathing.colors.resize(1);
        modes.push_back(Breathing);

        mode Color_Cycle;
        Color_Cycle.name                = "Spectrum Cycle";
        Color_Cycle.value               = AURA_KEYBOARD_MODE_COLOR_CYCLE;
        Color_Cycle.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
        Color_Cycle.speed_min           = AURA_KEYBOARD_SPEED_MIN;
        Color_Cycle.speed_max           = AURA_KEYBOARD_SPEED_MAX;
        Color_Cycle.speed               = AURA_KEYBOARD_SPEED_DEFAULT;
        Color_Cycle.brightness_min      = AURA_KEYBOARD_BRIGHTNESS_MIN;
        Color_Cycle.brightness_max      = AURA_KEYBOARD_BRIGHTNESS_MAX;
        Color_Cycle.brightness          = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
        Color_Cycle.color_mode          = MODE_COLORS_NONE;
        modes.push_back(Color_Cycle);

        mode Wave;
        Wave.name                       = "Rainbow Wave";
        Wave.value                      = AURA_KEYBOARD_MODE_WAVE;
        Wave.flags                      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
        if(controller->is_per_led_keyboard)
        {
            Wave.flags                 |= MODE_FLAG_HAS_DIRECTION_UD;
        }
        Wave.speed_min                  = AURA_KEYBOARD_SPEED_MIN;
        Wave.speed_max                  = AURA_KEYBOARD_SPEED_MAX;
        Wave.speed                      = AURA_KEYBOARD_SPEED_DEFAULT;
        Wave.brightness_min             = AURA_KEYBOARD_BRIGHTNESS_MIN;
        Wave.brightness_max             = AURA_KEYBOARD_BRIGHTNESS_MAX;
        Wave.brightness                 = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
        Wave.direction                  = MODE_DIRECTION_LEFT;
        Wave.color_mode                 = MODE_COLORS_MODE_SPECIFIC;

        if(!controller->is_per_led_keyboard)
        {
            Wave.colors_min             = 5;
            Wave.colors_max             = 5;
        }
        else
        {
            Wave.colors_min             = 1;
            Wave.colors_max             = 7;
        }

        Wave.colors.resize(Wave.colors_max);
        modes.push_back(Wave);

        if(controller->is_per_led_keyboard)
        {
            mode Reactive;
            Reactive.name               = "Reactive";
            Reactive.value              = AURA_KEYBOARD_MODE_REACTIVE;
            Reactive.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
            Reactive.speed_min          = AURA_KEYBOARD_SPEED_MIN;
            Reactive.speed_max          = AURA_KEYBOARD_SPEED_MAX;
            Reactive.speed              = AURA_KEYBOARD_SPEED_DEFAULT;
            Reactive.brightness_min     = AURA_KEYBOARD_BRIGHTNESS_MIN;
            Reactive.brightness_max     = AURA_KEYBOARD_BRIGHTNESS_MAX;
            Reactive.brightness         = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
            Reactive.color_mode         = MODE_COLORS_MODE_SPECIFIC;
            Reactive.colors_min         = 1;
            Reactive.colors_max         = 2;
            Reactive.colors.resize(1);
            modes.push_back(Reactive);

            mode Ripple;
            Ripple.name                 = "Ripple";
            Ripple.value                = AURA_KEYBOARD_MODE_RIPPLE;
            Ripple.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
            Ripple.speed_min            = AURA_KEYBOARD_SPEED_MIN;
            Ripple.speed_max            = AURA_KEYBOARD_SPEED_MAX;
            Ripple.speed                = AURA_KEYBOARD_SPEED_DEFAULT;
            Ripple.brightness_min       = AURA_KEYBOARD_BRIGHTNESS_MIN;
            Ripple.brightness_max       = AURA_KEYBOARD_BRIGHTNESS_MAX;
            Ripple.brightness           = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
            Ripple.color_mode           = MODE_COLORS_MODE_SPECIFIC;
            Ripple.colors_min           = 1;
            Ripple.colors_max           = 8;
            Ripple.colors.resize(7);
            modes.push_back(Ripple);

            mode Starry_Night;
            Starry_Night.name           = "Starry Night";
            Starry_Night.value          = AURA_KEYBOARD_MODE_STARRY_NIGHT;
            Starry_Night.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
            Starry_Night.speed_min      = AURA_KEYBOARD_SPEED_MIN;
            Starry_Night.speed_max      = AURA_KEYBOARD_SPEED_MAX;
            Starry_Night.speed          = AURA_KEYBOARD_SPEED_DEFAULT;
            Starry_Night.brightness_min = AURA_KEYBOARD_BRIGHTNESS_MIN;
            Starry_Night.brightness_max = AURA_KEYBOARD_BRIGHTNESS_MAX;
            Starry_Night.brightness     = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
            Starry_Night.color_mode     = MODE_COLORS_MODE_SPECIFIC;
            Starry_Night.colors_min     = 1;
            Starry_Night.colors_max     = 3;
            Starry_Night.colors.resize(1);
            modes.push_back(Starry_Night);

            mode Quicksand;
            Quicksand.name              = "Quicksand";
            Quicksand.value             = AURA_KEYBOARD_MODE_QUICKSAND;
            Quicksand.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
            Quicksand.direction         = MODE_DIRECTION_DOWN;
            Quicksand.speed_min         = AURA_KEYBOARD_SPEED_MIN;
            Quicksand.speed_max         = AURA_KEYBOARD_SPEED_MAX;
            Quicksand.speed             = AURA_KEYBOARD_SPEED_DEFAULT;
            Quicksand.brightness_min    = AURA_KEYBOARD_BRIGHTNESS_MIN;
            Quicksand.brightness_max    = AURA_KEYBOARD_BRIGHTNESS_MAX;
            Quicksand.brightness        = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
            Quicksand.color_mode        = MODE_COLORS_MODE_SPECIFIC;
            Quicksand.colors_min        = 6;
            Quicksand.colors_max        = 6;
            Quicksand.colors.resize(6);
            modes.push_back(Quicksand);

            mode Current;
            Current.name                = "Current";
            Current.value               = AURA_KEYBOARD_MODE_CURRENT;
            Current.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
            Current.speed_min           = AURA_KEYBOARD_SPEED_MIN;
            Current.speed_max           = AURA_KEYBOARD_SPEED_MAX;
            Current.speed               = AURA_KEYBOARD_SPEED_DEFAULT;
            Current.brightness_min      = AURA_KEYBOARD_BRIGHTNESS_MIN;
            Current.brightness_max      = AURA_KEYBOARD_BRIGHTNESS_MAX;
            Current.brightness          = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
            Current.color_mode          = MODE_COLORS_MODE_SPECIFIC;
            Current.colors_min          = 1;
            Current.colors_max          = 3;
            Current.colors.resize(1);
            modes.push_back(Current);

            mode Rain_Drop;
            Rain_Drop.name              = "Rain Drop";
            Rain_Drop.value             = AURA_KEYBOARD_MODE_RAIN_DROP;
            Rain_Drop.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
            Rain_Drop.speed_min         = AURA_KEYBOARD_SPEED_MIN;
            Rain_Drop.speed_max         = AURA_KEYBOARD_SPEED_MAX;
            Rain_Drop.speed             = AURA_KEYBOARD_SPEED_DEFAULT;
            Rain_Drop.brightness_min    = AURA_KEYBOARD_BRIGHTNESS_MIN;
            Rain_Drop.brightness_max    = AURA_KEYBOARD_BRIGHTNESS_MAX;
            Rain_Drop.brightness        = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
            Rain_Drop.color_mode        = MODE_COLORS_MODE_SPECIFIC;
            Rain_Drop.colors_min        = 1;
            Rain_Drop.colors_max        = 3;
            Rain_Drop.colors.resize(1);
            modes.push_back(Rain_Drop);
        }
    }
    else
    {

        name                            = controller->GetName();
        vendor                          = "ASUS";
        type                            = DEVICE_TYPE_KEYBOARD;
        description                     = "ASUS Aura Keyboard Device";
        version                         = controller->GetVersion();
        location                        = controller->GetDeviceLocation();
        serial                          = controller->GetSerialString();

        mode Direct;
        Direct.name                     = "Direct";
        Direct.value                    = AURA_KEYBOARD_MODE_DIRECT;
        Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode               = MODE_COLORS_PER_LED;
        modes.push_back(Direct);

        mode Static;
        Static.name                     = "Static";
        Static.value                    = AURA_KEYBOARD_MODE_STATIC;
        Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
        Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
        Static.colors_min               = 1;
        Static.colors_max               = 1;
        Static.colors.resize(1);
        modes.push_back(Static);

        mode Breathing;
        Breathing.name                  = "Breathing";
        Breathing.value                 = AURA_KEYBOARD_MODE_BREATHING;
        Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        Breathing.speed_min             = AURA_CLAYMORE_SPEED_MIN;
        Breathing.speed_max             = AURA_CLAYMORE_SPEED_MAX;
        Breathing.speed                 = AURA_CLAYMORE_SPEED_DEFAULT_BREATHING;
        Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
        Breathing.colors_min            = 1;
        Breathing.colors_max            = 2;
        Breathing.colors.resize(2);
        modes.push_back(Breathing);

        mode Color_Cycle;
        Color_Cycle.name                = "Spectrum Cycle";
        Color_Cycle.value               = AURA_KEYBOARD_MODE_COLOR_CYCLE;
        Color_Cycle.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        Color_Cycle.speed_min           = AURA_CLAYMORE_SPEED_MIN;
        Color_Cycle.speed_max           = AURA_CLAYMORE_SPEED_MAX;
        Color_Cycle.speed               = AURA_CLAYMORE_SPEED_DEFAULT_COLOR_CYCLE;
        Color_Cycle.color_mode          = MODE_COLORS_NONE;
        modes.push_back(Color_Cycle);

        mode Reactive;
        Reactive.name                   = "Reactive";
        Reactive.value                  = AURA_KEYBOARD_MODE_REACTIVE;
        Reactive.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        Reactive.speed_min              = AURA_CLAYMORE_SPEED_MIN;
        Reactive.speed_max              = AURA_CLAYMORE_SPEED_MAX;
        Reactive.speed                  = AURA_CLAYMORE_SPEED_DEFAULT_REACTIVE;
        Reactive.color_mode             = MODE_COLORS_MODE_SPECIFIC;
        Reactive.colors_min             = 1;
        Reactive.colors_max             = 2;
        Reactive.colors.resize(2);
        modes.push_back(Reactive);

        mode Wave;
        Wave.name                       = "Rainbow Wave";
        Wave.value                      = AURA_KEYBOARD_MODE_WAVE;
        Wave.flags                      = MODE_COLORS_NONE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE;
        Wave.speed_min                  = AURA_CLAYMORE_SPEED_MIN;
        Wave.speed_max                  = AURA_CLAYMORE_SPEED_MAX;
        Wave.speed                      = AURA_CLAYMORE_SPEED_DEFAULT_WAVE;
        Wave.direction                  = MODE_DIRECTION_LEFT;
        Wave.color_mode                 = MODE_COLORS_NONE;
        modes.push_back(Wave);

        mode Color_Wave;
        Color_Wave.name                 = "Color Wave";
        Color_Wave.value                = AURA_KEYBOARD_MODE_WAVE;
        Color_Wave.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE;
        Color_Wave.speed_min            = AURA_CLAYMORE_SPEED_MIN;
        Color_Wave.speed_max            = AURA_CLAYMORE_SPEED_MAX;
        Color_Wave.speed                = AURA_CLAYMORE_SPEED_DEFAULT_WAVE;
        Color_Wave.direction            = MODE_DIRECTION_LEFT;
        Color_Wave.color_mode           = MODE_COLORS_MODE_SPECIFIC;
        Color_Wave.colors_min           = 1;
        Color_Wave.colors_max           = 2;
        Color_Wave.colors.resize(2);
        modes.push_back(Color_Wave);

        mode Ripple;
        Ripple.name                     = "Ripple";
        Ripple.value                    = AURA_KEYBOARD_MODE_RIPPLE;
        Ripple.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        Ripple.speed_min                = AURA_CLAYMORE_SPEED_MIN;
        Ripple.speed_max                = AURA_CLAYMORE_SPEED_MAX;
        Ripple.speed                    = AURA_CLAYMORE_SPEED_DEFAULT_RIPPLE;
        Ripple.color_mode               = MODE_COLORS_MODE_SPECIFIC;
        Ripple.colors_min               = 1;
        Ripple.colors_max               = 2;
        Ripple.colors.resize(2);
        modes.push_back(Ripple);

        mode Starry_Night;
        Starry_Night.name               = "Starry Night";
        Starry_Night.value              = AURA_KEYBOARD_MODE_STARRY_NIGHT;
        Starry_Night.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        Starry_Night.speed_min          = AURA_CLAYMORE_SPEED_MIN;
        Starry_Night.speed_max          = AURA_CLAYMORE_SPEED_MAX;
        Starry_Night.speed              = AURA_CLAYMORE_SPEED_DEFAULT_STARRY_NIGHT;
        Starry_Night.color_mode         = MODE_COLORS_MODE_SPECIFIC;
        Starry_Night.colors_min         = 1;
        Starry_Night.colors_max         = 2;
        Starry_Night.colors.resize(2);
        modes.push_back(Starry_Night);

        mode Quicksand;
        Quicksand.name                  = "Quicksand";
        Quicksand.value                 = AURA_KEYBOARD_MODE_QUICKSAND;
        Quicksand.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        Quicksand.direction             = MODE_DIRECTION_DOWN;
        Quicksand.speed_min             = AURA_CLAYMORE_SPEED_MIN;
        Quicksand.speed_max             = AURA_CLAYMORE_SPEED_MAX;
        Quicksand.speed                 = AURA_CLAYMORE_SPEED_DEFAULT_QUICKSAND;
        Quicksand.color_mode            = MODE_COLORS_MODE_SPECIFIC;
        Quicksand.colors_min            = 6;
        Quicksand.colors_max            = 6;
        Quicksand.colors.resize(6);
        modes.push_back(Quicksand);
    }

    SetupZones();
}

RGBController_AuraTUFKeyboard::~RGBController_AuraTUFKeyboard()
{
    delete controller;
}

void RGBController_AuraTUFKeyboard::SetupZones()
{
    std::map<int,layout_info> * keyboard_ptr;

    switch(pid)
    {
        case AURA_ROG_STRIX_FLARE_PID:
        case AURA_ROG_STRIX_FLARE_PNK_LTD_PID:
        case AURA_ROG_STRIX_FLARE_COD_BO4_PID:
            keyboard_ptr = &AsusROGStrixFlareLayouts;
            break;
        case AURA_TUF_K3_GAMING_PID:
        case AURA_TUF_K7_GAMING_PID:
            keyboard_ptr = &AsusTUFK7Layouts;
            break;
        case AURA_ROG_STRIX_SCOPE_PID:
        case AURA_ROG_STRIX_SCOPE_RX_PID:
        case AURA_ROG_STRIX_SCOPE_RX_EVA_02_PID:
        case AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID:
        case AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID:
            keyboard_ptr = &AsusROGStrixScopeLayouts;
            break;
        case AURA_ROG_STRIX_SCOPE_II_PID:
        case AURA_ROG_STRIX_SCOPE_II_RX_PID:
            keyboard_ptr = &AsusROGStrixScopeIILayouts;
            break;
        case AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID:
            keyboard_ptr = &AsusROGStrixScopeII96WirelessLayouts;
            break;
        case AURA_ROG_STRIX_FLARE_II_PID:
            keyboard_ptr = &AsusROGStrixFlareIILayouts;
			break;
        case AURA_ROG_STRIX_FLARE_II_ANIMATE_PID:
            keyboard_ptr = &AsusROGStrixFlareIIAnimateLayouts;
            break;
        case AURA_ROG_AZOTH_USB_PID:
        case AURA_ROG_AZOTH_2_4_PID:
            keyboard_ptr = &AsusROGAzothLayouts;
            break;
        case AURA_ROG_FALCHION_WIRED_PID:
        case AURA_ROG_FALCHION_WIRELESS_PID:
            keyboard_ptr = &AsusFalchionLayouts;
            break;
        case AURA_ROG_CLAYMORE_PID:
            unsigned char numpad;
            numpad = controller->GetNumpadLocation();
            switch(numpad)
            {
                case 0:
                    keyboard_ptr = &AsusClaymoreNoNumpadLayouts;
                    break;
                case 2:
                    keyboard_ptr = &AsusClaymoreNumpadRightLayouts;
                    break;
                case 3:
                    keyboard_ptr = &AsusClaymoreNumpadLeftLayouts;
                    break;
                default:
                    keyboard_ptr = &AsusClaymoreNoNumpadLayouts;
            }
            break;
        case AURA_TUF_K1_GAMING_PID:
        case AURA_TUF_K5_GAMING_PID:
            keyboard_ptr = &AsusTufK1Layouts;
            break;
        default:
            keyboard_ptr = &AsusTUFK7Layouts;
    }

    std::map<int,layout_info> & keyboard = *keyboard_ptr;

    unsigned char layout = controller->GetLayout();

    if(keyboard.find(layout % 100) == keyboard.end())
    {
        /*---------------------------------------------------------*\
        | If Layout not found, take uk or us                        |
        \*---------------------------------------------------------*/
        layout = std::floor(layout/100) == 2 ? ASUS_TUF_K7_LAYOUT_UK : ASUS_TUF_K7_LAYOUT_US;
    }
    else
    {
        layout = layout % 100;
    }

    zone keyboard_zone;
    keyboard_zone.name               = "Keyboard";
    keyboard_zone.type               = ZONE_TYPE_MATRIX;
    keyboard_zone.leds_min           = keyboard[layout].size;
    keyboard_zone.leds_max           = keyboard[layout].size;
    keyboard_zone.leds_count         = keyboard[layout].size;
    keyboard_zone.matrix_map         = new matrix_map_type;
    keyboard_zone.matrix_map->height = keyboard[layout].rows;
    keyboard_zone.matrix_map->width  = keyboard[layout].cols;
    keyboard_zone.matrix_map->map    = keyboard[layout].matrix_map;
    zones.push_back(keyboard_zone);

    for(int led_id = 0; led_id < keyboard[layout].size; led_id++)
    {
        led new_led;
        new_led.name  = keyboard[layout].led_names[led_id].name;
        new_led.value = keyboard[layout].led_names[led_id].id;
        leds.push_back(new_led);
    }

    SetupColors();

    /*---------------------------------------------------------*\
    | sends the init packet for the default mode (direct)       |
    \*---------------------------------------------------------*/
    DeviceUpdateMode();
}

void RGBController_AuraTUFKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AuraTUFKeyboard::DeviceUpdateLEDs()
{
    std::vector<led_color> led_color_list = {};

    for(size_t i = 0; i < colors.size(); i++)
    {
        led_color_list.push_back({ leds[i].value, colors[i] });
    }

    controller->UpdateLeds(led_color_list);
}

void RGBController_AuraTUFKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraTUFKeyboard::UpdateSingleLED(int led)
{
    if(!controller->is_per_led_keyboard)
    {
        return DeviceUpdateLEDs();
    }

    unsigned char red   = RGBGetRValue(colors[led]);
    unsigned char green = RGBGetGValue(colors[led]);
    unsigned char blue  = RGBGetBValue(colors[led]);

    controller->UpdateSingleLed(leds[led].value, red, green, blue);
}

static const uint8_t direction_map[2][6] =
{
    { 4,  0,  6,  2,  8,  1 },          // Default directions Left, Right, Up, Down, Horizontal, Vertical
    { 0,  4,  6,  2,  0xFF,  0xFF },    // AURA_ROG_CLAYMORE directions Left, Right, Up, Down
};

void RGBController_AuraTUFKeyboard::DeviceUpdateMode()
{
    if(pid == AURA_ROG_CLAYMORE_PID)
    {
        controller->AllowRemoteControl(1);
    }

    unsigned char color_mode    = 0;
    unsigned char direction     = 0;
    unsigned char brightness    = 0;

    if(modes[active_mode].value == AURA_KEYBOARD_MODE_DIRECT)
    {
        if(pid == AURA_ROG_CLAYMORE_PID) controller->AllowRemoteControl(3);
        return;
    };

    if(pid != AURA_ROG_CLAYMORE_PID)
    {
        brightness = modes[active_mode].brightness * 25;

        switch(modes[active_mode].value)
        {
            case AURA_KEYBOARD_MODE_BREATHING:
            case AURA_KEYBOARD_MODE_REACTIVE:
            case AURA_KEYBOARD_MODE_STARRY_NIGHT:
            case AURA_KEYBOARD_MODE_CURRENT:
            case AURA_KEYBOARD_MODE_RAIN_DROP:
                if(!controller->is_per_led_keyboard && modes[active_mode].colors.size() > 1)
                {
                    color_mode = 1;
                    break;
                }

                bool color_is_black = (modes[active_mode].colors.size() > 1 && modes[active_mode].colors[1] == 000);

                if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC && !color_is_black)
                {
                    color_mode = 16;
                }
                break;
        }

        if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
        {
            color_mode = 1;
        }

        if(modes[active_mode].value == AURA_KEYBOARD_MODE_WAVE || modes[active_mode].value == AURA_KEYBOARD_MODE_QUICKSAND)
        {
            /*----------------------------------------------------------*\
            | converting openrgb direction value to keyboard directions  |
            \*----------------------------------------------------------*/
            direction  = direction_map[0][modes[active_mode].direction];
        }
    }
    else
    {
        if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
        {
            color_mode = 1;
        }

        if(modes[active_mode].value == AURA_KEYBOARD_MODE_WAVE) {
            if (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC) color_mode = 2;

            /*----------------------------------------------------------*\
            | converting openrgb direction value to keyboard directions  |
            \*----------------------------------------------------------*/
            direction = direction_map[1][modes[active_mode].direction];
        }
    }


    controller->UpdateDevice(modes[active_mode].value, std::vector<RGBColor>(modes[active_mode].colors), direction, color_mode, modes[active_mode].speed, brightness);

    if(pid == AURA_ROG_CLAYMORE_PID)
    {
        controller->UpdateMode(modes[active_mode].value);
        controller->SaveMode();
        controller->AllowRemoteControl(0);
    }
}

void RGBController_AuraTUFKeyboard::DeviceSaveMode()
{
    /*----------------------------------------------------------*\
    | not available for Claymore                                 |
    \*----------------------------------------------------------*/
    if(pid != AURA_ROG_CLAYMORE_PID)
    {
        DeviceUpdateMode();
        controller->SaveMode();
    }
}
