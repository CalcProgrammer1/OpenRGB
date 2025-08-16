/*---------------------------------------------------------*\
| RGBController_RoccatSenseAimo.cpp                         |
|                                                           |
|   RGBController for Roccat Sense Aimo                     |
|                                                           |
|   Mola19                                      09 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_RoccatSenseAimo.h"

/**------------------------------------------------------------------*\
    @name Roccat Sense Aimo Mousepad
    @category Mousemat
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRoccatSenseAimoControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RoccatSenseAimo::RGBController_RoccatSenseAimo(RoccatSenseAimoController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "Roccat";
    type                        = DEVICE_TYPE_MOUSEMAT;
    description                 = "Roccat Sense Aimo Mousepad Device";
    version                     = controller->GetVersion();
    location                    = controller->GetLocation();
    serial                      = controller->GetSerial();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ROCCAT_SENSE_AIMO_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ROCCAT_SENSE_AIMO_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode           = MODE_COLORS_PER_LED;
    Static.brightness           = ROCCAT_SENSE_AIMO_BRIGHTNESS_DEFAULT;
    Static.brightness_min       = ROCCAT_SENSE_AIMO_BRIGHTNESS_MIN;
    Static.brightness_max       = ROCCAT_SENSE_AIMO_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = ROCCAT_SENSE_AIMO_MODE_WAVE;
    Rainbow.flags               = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.brightness          = ROCCAT_SENSE_AIMO_BRIGHTNESS_DEFAULT;
    Rainbow.brightness_min      = ROCCAT_SENSE_AIMO_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = ROCCAT_SENSE_AIMO_BRIGHTNESS_MAX;
    Rainbow.speed               = ROCCAT_SENSE_AIMO_SPEED_DEFAULT;
    Rainbow.speed_min           = ROCCAT_SENSE_AIMO_SPEED_MIN;
    Rainbow.speed_max           = ROCCAT_SENSE_AIMO_SPEED_MAX;
    modes.push_back(Rainbow);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ROCCAT_SENSE_AIMO_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.brightness        = ROCCAT_SENSE_AIMO_BRIGHTNESS_DEFAULT;
    Breathing.brightness_min    = ROCCAT_SENSE_AIMO_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ROCCAT_SENSE_AIMO_BRIGHTNESS_MAX;
    Breathing.speed             = ROCCAT_SENSE_AIMO_SPEED_DEFAULT;
    Breathing.speed_min         = ROCCAT_SENSE_AIMO_SPEED_MIN;
    Breathing.speed_max         = ROCCAT_SENSE_AIMO_SPEED_MAX;
    modes.push_back(Breathing);

    mode Heartbeat;
    Heartbeat.name              = "Heartbeat";
    Heartbeat.value             = ROCCAT_SENSE_AIMO_MODE_HEARTBEAT;
    Heartbeat.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Heartbeat.color_mode        = MODE_COLORS_PER_LED;
    Heartbeat.brightness        = ROCCAT_SENSE_AIMO_BRIGHTNESS_DEFAULT;
    Heartbeat.brightness_min    = ROCCAT_SENSE_AIMO_BRIGHTNESS_MIN;
    Heartbeat.brightness_max    = ROCCAT_SENSE_AIMO_BRIGHTNESS_MAX;
    Heartbeat.speed             = ROCCAT_SENSE_AIMO_SPEED_DEFAULT;
    Heartbeat.speed_min         = ROCCAT_SENSE_AIMO_SPEED_MIN;
    Heartbeat.speed_max         = ROCCAT_SENSE_AIMO_SPEED_MAX;
    modes.push_back(Heartbeat);

    /*---------------------------------------------------------------------*\
    | This is the default mode for software modes, while swarm isn't active |
    \*---------------------------------------------------------------------*/
    mode Default;
    Default.name               = "Default";
    Default.value              = ROCCAT_SENSE_AIMO_MODE_DEFAULT;
    Default.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Default.color_mode         = MODE_COLORS_NONE;
    Default.brightness         = ROCCAT_SENSE_AIMO_BRIGHTNESS_DEFAULT;
    Default.brightness_min     = ROCCAT_SENSE_AIMO_BRIGHTNESS_MIN;
    Default.brightness_max     = ROCCAT_SENSE_AIMO_BRIGHTNESS_MAX;
    modes.push_back(Default);

    SetupZones();

    mode_struct active = controller->GetMode();

    for(uint32_t i = 0; i < modes.size(); i++)
    {
        if(modes[i].value == active.mode)
        {
            active_mode = i;
            break;
        }

        /*----------------------------------------------*\
        | If no mode was found, select 0th mode (direct) |
        \*----------------------------------------------*/
        if(i == modes.size() - 1)
        {
            active_mode = 0;
        }
    }

    modes[active_mode].speed      = active.speed;
    modes[active_mode].brightness = active.brightness;

    colors[0] = active.left;
    colors[1] = active.right;
}

RGBController_RoccatSenseAimo::~RGBController_RoccatSenseAimo()
{
    delete controller;
}

void RGBController_RoccatSenseAimo::SetupZones()
{
    zone pad;
    pad.name          = "Mousepad";
    pad.type          = ZONE_TYPE_LINEAR;
    pad.leds_min      = 2;
    pad.leds_max      = 2;
    pad.leds_count    = 2;
    pad.matrix_map    = NULL;
    zones.push_back(pad);

    led left_led;
    left_led.name = "Mousepad left led";
    leds.push_back(left_led);

    led right_led;
    right_led.name = "Mousepad right led";
    leds.push_back(right_led);

    SetupColors();
}

void RGBController_RoccatSenseAimo::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatSenseAimo::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == ROCCAT_SENSE_AIMO_MODE_DIRECT)
    {
        controller->SendDirect(colors);
    }
    else
    {
        DeviceUpdateMode();
    }
}

void RGBController_RoccatSenseAimo::UpdateZoneLEDs(int /*zone_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatSenseAimo::UpdateSingleLED(int /*led_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatSenseAimo::DeviceUpdateMode()
{
    mode selected = modes[active_mode];

    mode_struct active = controller->GetMode();
    controller->SetMode(active.profile, selected.value, selected.speed, selected.brightness, colors);
}
