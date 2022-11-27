/*-----------------------------------------*\
|  RGBController_RoccatVulcanAimo.cpp       |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|                                           |
|                                           |
|  Mola19 17/12/2021                        |
\*-----------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_RoccatVulcanAimo.h"
#include <vector>

#define NA  0xFFFFFFFF

/**------------------------------------------------------------------*\
    @name Roccat Vulcan Aimo
    @category Keyboard
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRoccatVulcanAimoControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RoccatVulcanAimo::RGBController_RoccatVulcanAimo(RoccatVulcanAimoController* controller_ptr)
{
    controller  = controller_ptr;

    controller->InitDeviceInfo();

    name        = "Roccat Vulcan 120-Series Aimo";
    vendor      = "Roccat";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Roccat Vulcan Aimo Keyboard";
    version     = controller->GetDeviceInfo().version;
    location    = controller->GetLocation();
    serial      = controller->GetSerial();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = ROCCAT_VULCAN_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name             = "Static";
    Static.value            = ROCCAT_VULCAN_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.brightness_min   = ROCCAT_VULCAN_BRIGHTNESS_MIN;
    Static.brightness_max   = ROCCAT_VULCAN_BRIGHTNESS_MAX;
    Static.brightness       = ROCCAT_VULCAN_BRIGHTNESS_DEFAULT;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Wave;
    Wave.name           = "Rainbow Wave";
    Wave.value          = ROCCAT_VULCAN_MODE_WAVE;
    Wave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min      = ROCCAT_VULCAN_SPEED_MIN;
    Wave.speed_max      = ROCCAT_VULCAN_SPEED_MAX;
    Wave.speed          = ROCCAT_VULCAN_SPEED_DEFAULT;
    Wave.brightness_min = ROCCAT_VULCAN_BRIGHTNESS_MIN;
    Wave.brightness_max = ROCCAT_VULCAN_BRIGHTNESS_MAX;
    Wave.brightness     = ROCCAT_VULCAN_BRIGHTNESS_DEFAULT;
    Wave.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Wave);

    SetupZones();
}

RGBController_RoccatVulcanAimo::~RGBController_RoccatVulcanAimo()
{
    delete controller;
}

void RGBController_RoccatVulcanAimo::SetupZones()
{
    unsigned char layout = controller->GetDeviceInfo().layout_variant;

    if(RoccatVulcanLayouts.find(layout) == RoccatVulcanLayouts.end())
    {
        /*---------------------------------------------------------*\
        | If Layout not found, take uk or us                        |
        \*---------------------------------------------------------*/
        layout = controller->GetDeviceInfo().layout_type == 1 ? ROCCAT_VULCAN_LAYOUT_UK : ROCCAT_VULCAN_LAYOUT_US;
    }

    zone keyboard_zone;
    keyboard_zone.name               = "Keyboard";
    keyboard_zone.type               = ZONE_TYPE_MATRIX;
    keyboard_zone.leds_min           = RoccatVulcanLayouts[layout].size;
    keyboard_zone.leds_max           = RoccatVulcanLayouts[layout].size;
    keyboard_zone.leds_count         = RoccatVulcanLayouts[layout].size;
    keyboard_zone.matrix_map         = new matrix_map_type;
    keyboard_zone.matrix_map->height = RoccatVulcanLayouts[layout].rows;
    keyboard_zone.matrix_map->width  = RoccatVulcanLayouts[layout].cols;
    keyboard_zone.matrix_map->map    = RoccatVulcanLayouts[layout].matrix_map;
    zones.push_back(keyboard_zone);

    for(int led_id = 0; led_id < RoccatVulcanLayouts[layout].size; led_id++)
    {
        led new_led;
        new_led.name  = RoccatVulcanLayouts[layout].led_names[led_id].name;
        new_led.value = RoccatVulcanLayouts[layout].led_names[led_id].id;
        leds.push_back(new_led);
    }

    SetupColors();

    /*---------------------------------------------------------*\
    | sends the init packet for the default mode (direct)       |
    \*---------------------------------------------------------*/
    DeviceUpdateMode();
}

void RGBController_RoccatVulcanAimo::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatVulcanAimo::DeviceUpdateLEDs()
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

void RGBController_RoccatVulcanAimo::UpdateZoneLEDs(int /*zone_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatVulcanAimo::UpdateSingleLED(int /*led_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatVulcanAimo::DeviceUpdateMode()
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
}
