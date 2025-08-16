/*---------------------------------------------------------*\
| RGBController_RoccatVulcanKeyboard.cpp                    |
|                                                           |
|   RGBController for Roccat Vulcan keyboard                |
|                                                           |
|   Mola19                                      17 Dec 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "RGBControllerKeyNames.h"
#include "RGBController_RoccatVulcanKeyboard.h"

#define NA  0xFFFFFFFF

/**------------------------------------------------------------------*\
    @name Roccat Vulcan Keyboard
    @category Keyboard
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRoccatVulcanKeyboardControllers
    @comment The mode "Default" differs from device to device and
    and sometimes also based on which profile you are on.
    Often it is very close to the rainbow mode.
\*-------------------------------------------------------------------*/

RGBController_RoccatVulcanKeyboard::RGBController_RoccatVulcanKeyboard(RoccatVulcanKeyboardController* controller_ptr)
{
    controller                  = controller_ptr;

    pid                         = controller->device_pid;

    controller->InitDeviceInfo();

    name                        = controller->GetName();
    vendor                      = "Roccat";
    type                        = DEVICE_TYPE_KEYBOARD;
    description                 = "Roccat Vulcan Keyboard Device";
    version                     = controller->GetDeviceInfo().version;
    location                    = controller->GetLocation();
    serial                      = controller->GetSerial();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ROCCAT_VULCAN_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;

    if(pid != ROCCAT_VULCAN_120_AIMO_PID && pid != ROCCAT_VULCAN_100_AIMO_PID)
    {
        Direct.flags           |= MODE_FLAG_HAS_BRIGHTNESS;
        Direct.brightness_min   = ROCCAT_VULCAN_BRIGHTNESS_MIN;
        Direct.brightness_max   = ROCCAT_VULCAN_BRIGHTNESS_MAX;
        Direct.brightness       = ROCCAT_VULCAN_BRIGHTNESS_DEFAULT;
    }

    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ROCCAT_VULCAN_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.brightness_min       = ROCCAT_VULCAN_BRIGHTNESS_MIN;
    Static.brightness_max       = ROCCAT_VULCAN_BRIGHTNESS_MAX;
    Static.brightness           = ROCCAT_VULCAN_BRIGHTNESS_DEFAULT;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Wave;
    Wave.name                   = "Rainbow Wave";
    Wave.value                  = ROCCAT_VULCAN_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min              = ROCCAT_VULCAN_SPEED_MIN;
    Wave.speed_max              = ROCCAT_VULCAN_SPEED_MAX;
    Wave.speed                  = ROCCAT_VULCAN_SPEED_DEFAULT;
    Wave.brightness_min         = ROCCAT_VULCAN_BRIGHTNESS_MIN;
    Wave.brightness_max         = ROCCAT_VULCAN_BRIGHTNESS_MAX;
    Wave.brightness             = ROCCAT_VULCAN_BRIGHTNESS_DEFAULT;
    Wave.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Wave);

    mode Default;
    Default.name                = "Default";
    Default.value               = ROCCAT_VULCAN_MODE_DEFAULT;
    Default.flags               = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Default.brightness_min      = ROCCAT_VULCAN_BRIGHTNESS_MIN;
    Default.brightness_max      = ROCCAT_VULCAN_BRIGHTNESS_MAX;
    Default.brightness          = ROCCAT_VULCAN_BRIGHTNESS_DEFAULT;
    Default.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Default);

    SetupZones();
}

RGBController_RoccatVulcanKeyboard::~RGBController_RoccatVulcanKeyboard()
{
    delete controller;
}

void RGBController_RoccatVulcanKeyboard::SetupZones()
{
    std::map<int,layout_info> * keyboard_ptr;

    switch(pid)
    {
        case ROCCAT_VULCAN_100_AIMO_PID:
        case ROCCAT_VULCAN_120_AIMO_PID:
            keyboard_ptr = &RoccatVulcan120AimoLayouts;
            break;
        case ROCCAT_VULCAN_TKL_PID:
        case TURTLE_BEACH_VULCAN_II_TKL_PID:
            keyboard_ptr = &RoccatVulcanTKLLayouts;
            break;
        case ROCCAT_VULCAN_PRO_PID:
        case ROCCAT_PYRO_PID:
            keyboard_ptr = &RoccatPyroLayouts;
            break;
        case ROCCAT_VULCAN_II_PID:
        case TURTLE_BEACH_VULCAN_II_PID:
            keyboard_ptr = &RoccatVulcanIILayouts;
            break;
        case ROCCAT_MAGMA_PID:
        case ROCCAT_MAGMA_MINI_PID:
            keyboard_ptr = &RoccatMagmaLayouts;
            break;
        default:
            keyboard_ptr = &RoccatVulcan120AimoLayouts;
    }

    std::map<int,layout_info> & keyboard = *keyboard_ptr;

    unsigned char layout;

    switch(controller->GetDeviceInfo().layout_type)
    {
        case ROCCAT_VULCAN_LAYOUT_DE:
        case ROCCAT_VULCAN_LAYOUT_UK:
        case ROCCAT_VULCAN_LAYOUT_FR:
            layout = ROCCAT_VULCAN_LAYOUT_UK;
            break;
        case ROCCAT_VULCAN_LAYOUT_US:
        default:
            layout = ROCCAT_VULCAN_LAYOUT_US;
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
    DeviceUpdateLEDs();
}

void RGBController_RoccatVulcanKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatVulcanKeyboard::DeviceUpdateLEDs()
{
    if (modes[active_mode].value == ROCCAT_VULCAN_MODE_DIRECT)
    {
        std::vector<led_color> led_color_list = {};

        for(unsigned int i = 0; i < colors.size(); i++)
        {
            led_color_list.push_back({ leds[i].value, colors[i] });
        }

        controller->SendColors(led_color_list);
    }
    else
    {
        DeviceUpdateMode();
    }
}

void RGBController_RoccatVulcanKeyboard::UpdateZoneLEDs(int /*zone_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatVulcanKeyboard::UpdateSingleLED(int /*led_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatVulcanKeyboard::DeviceUpdateMode()
{
    std::vector<led_color> led_color_list = {};

    if(modes[active_mode].value == ROCCAT_VULCAN_MODE_STATIC)
    {
        for(unsigned int i = 0; i < colors.size(); i++)
        {
            led_color_list.push_back({ leds[i].value, colors[i] });
        }
    }

    controller->SendMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, led_color_list);
    controller->WaitUntilReady();

    controller->EnableDirect(modes[active_mode].value != ROCCAT_VULCAN_MODE_DEFAULT);
    controller->WaitUntilReady();
}
