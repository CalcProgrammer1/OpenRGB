/*---------------------------------------------------------*\
| RGBController_Lenovo_Gen7_8.cpp                           |
|                                                           |
|   RGBController for Lenovo Gen7 and Gen8 devices          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "RGBController_Lenovo_Gen7_8.h"
#include "LenovoDevices.h"

using namespace std;

LenovoRGBController_Gen7_8::LenovoRGBController_Gen7_8(LenovoGen7And8USBController* controller_ptr)
{
    controller = controller_ptr;

    mode Screw;
    Screw.name = "Screw Rainbow";
    Screw.value = LENOVO_LEGION_GEN7_8_MODE_SCREW_RAINBOW;
    Screw.flags = MODE_FLAG_HAS_SPEED |
        MODE_FLAG_HAS_DIRECTION_LR |
        MODE_FLAG_HAS_BRIGHTNESS |
        MODE_FLAG_AUTOMATIC_SAVE;
    Screw.speed_min = 0x01;
    Screw.speed_max = 0x03;
    Screw.speed = 2;
    Screw.color_mode = MODE_COLORS_NONE;
    Screw.brightness_min = 0;
    Screw.brightness_max = 9;
    Screw.brightness = brightness;
    Screw.direction = MODE_DIRECTION_RIGHT;
    modes.push_back(Screw);

    mode Rainbow;
    Rainbow.name = "Rainbow Wave";
    Rainbow.value = LENOVO_LEGION_GEN7_8_MODE_RAINBOW_WAVE;
    Rainbow.flags = MODE_FLAG_HAS_SPEED |
        MODE_FLAG_HAS_DIRECTION_LR |
        MODE_FLAG_HAS_DIRECTION_UD |
        MODE_FLAG_HAS_BRIGHTNESS |
        MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.speed_min = 0x01;
    Rainbow.speed_max = 0x03;
    Rainbow.speed = 2;
    Rainbow.color_mode = MODE_COLORS_NONE;
    Rainbow.brightness_min = 0;
    Rainbow.brightness_max = 9;
    Rainbow.brightness = brightness;
    Rainbow.direction = MODE_DIRECTION_RIGHT;
    modes.push_back(Rainbow);

    mode ColorChange;
    ColorChange.name = "Color Change";
    ColorChange.value = LENOVO_LEGION_GEN7_8_MODE_COLOR_CHANGE;
    ColorChange.flags = MODE_FLAG_HAS_SPEED |
        MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
        MODE_FLAG_HAS_RANDOM_COLOR |
        MODE_FLAG_HAS_BRIGHTNESS |
        MODE_FLAG_AUTOMATIC_SAVE;
    ColorChange.speed_min = 0x01;
    ColorChange.speed_max = 0x03;
    ColorChange.speed = 2;
    ColorChange.colors_min = 1;
    ColorChange.colors_max = 4;
    ColorChange.colors.resize(4);
    ColorChange.colors[0] = 0xFFF500;
    ColorChange.color_mode = MODE_COLORS_RANDOM;
    ColorChange.brightness_min = 0;
    ColorChange.brightness_max = 9;
    ColorChange.brightness = brightness;
    modes.push_back(ColorChange);

    mode ColorPulse;
    ColorPulse.name = "Color Pulse";
    ColorPulse.value = LENOVO_LEGION_GEN7_8_MODE_COLOR_PULSE;
    ColorPulse.flags = MODE_FLAG_HAS_SPEED |
        MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
        MODE_FLAG_HAS_RANDOM_COLOR |
        MODE_FLAG_HAS_BRIGHTNESS |
        MODE_FLAG_AUTOMATIC_SAVE;
    ColorPulse.speed_min = 0x01;
    ColorPulse.speed_max = 0x03;
    ColorPulse.speed = 2;
    ColorPulse.colors_min = 1;
    ColorPulse.colors_max = 4;
    ColorPulse.colors.resize(4);
    ColorPulse.colors[0] = 0xFFF500;
    ColorPulse.color_mode = MODE_COLORS_RANDOM;
    ColorPulse.brightness_min = 0;
    ColorPulse.brightness_max = 9;
    ColorPulse.brightness = brightness;
    modes.push_back(ColorPulse);

    mode ColorWave;
    ColorWave.name = "Color Wave";
    ColorWave.value = LENOVO_LEGION_GEN7_8_MODE_COLOR_WAVE;
    ColorWave.flags = MODE_FLAG_HAS_SPEED |
        MODE_FLAG_HAS_DIRECTION_LR |
        MODE_FLAG_HAS_DIRECTION_UD |
        MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
        MODE_FLAG_HAS_RANDOM_COLOR |
        MODE_FLAG_HAS_BRIGHTNESS |
        MODE_FLAG_AUTOMATIC_SAVE;
    ColorWave.speed_min = 0x01;
    ColorWave.speed_max = 0x03;
    ColorWave.speed = 2;
    ColorWave.colors_min = 1;
    ColorWave.colors_max = 4;
    ColorWave.colors.resize(4);
    ColorWave.colors[0] = 0xFFF500;
    ColorWave.color_mode = MODE_COLORS_RANDOM;
    ColorWave.brightness_min = 0;
    ColorWave.brightness_max = 9;
    ColorWave.brightness = brightness;
    ColorWave.direction = MODE_DIRECTION_RIGHT;
    modes.push_back(ColorWave);

    mode Smooth;
    Smooth.name = "Smooth";
    Smooth.value = LENOVO_LEGION_GEN7_8_MODE_SMOOTH;
    Smooth.flags = MODE_FLAG_HAS_SPEED |
        MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
        MODE_FLAG_HAS_RANDOM_COLOR |
        MODE_FLAG_HAS_BRIGHTNESS |
        MODE_FLAG_AUTOMATIC_SAVE;
    Smooth.speed_min = 0x01;
    Smooth.speed_max = 0x03;
    Smooth.speed = 2;
    Smooth.colors_min = 1;
    Smooth.colors_max = 4;
    Smooth.colors.resize(4);
    Smooth.colors[0] = 0xFFF500;
    Smooth.color_mode = MODE_COLORS_RANDOM;
    Smooth.brightness_min = 0;
    Smooth.brightness_max = 9;
    Smooth.brightness = brightness;
    modes.push_back(Smooth);

    mode Rain;
    Rain.name = "Rain";
    Rain.value = LENOVO_LEGION_GEN7_8_MODE_RAIN;
    Rain.flags = MODE_FLAG_HAS_SPEED |
        MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
        MODE_FLAG_HAS_RANDOM_COLOR |
        MODE_FLAG_HAS_BRIGHTNESS |
        MODE_FLAG_AUTOMATIC_SAVE;
    Rain.speed_min = 0x01;
    Rain.speed_max = 0x03;
    Rain.speed = 2;
    Rain.colors_min = 1;
    Rain.colors_max = 4;
    Rain.colors.resize(4);
    Rain.colors[0] = 0xFFF500;
    Rain.color_mode = MODE_COLORS_RANDOM;
    Rain.brightness_min = 0;
    Rain.brightness_max = 9;
    Rain.brightness = brightness;
    modes.push_back(Rain);

    mode Ripple;
    Ripple.name = "Ripple";
    Ripple.value = LENOVO_LEGION_GEN7_8_MODE_RIPPLE;
    Ripple.flags = MODE_FLAG_HAS_SPEED |
        MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
        MODE_FLAG_HAS_RANDOM_COLOR |
        MODE_FLAG_HAS_BRIGHTNESS |
        MODE_FLAG_AUTOMATIC_SAVE;
    Ripple.speed_min = 0x01;
    Ripple.speed_max = 0x03;
    Ripple.speed = 2;
    Ripple.colors_min = 1;
    Ripple.colors_max = 4;
    Ripple.colors.resize(4);
    Ripple.colors[0] = 0xFFF500;
    Ripple.color_mode = MODE_COLORS_RANDOM;
    Ripple.brightness_min = 0;
    Ripple.brightness_max = 9;
    Ripple.brightness = brightness;
    modes.push_back(Ripple);

    mode AudioBounce;
    AudioBounce.name = "Audio Bounce Lighting";
    AudioBounce.value = LENOVO_LEGION_GEN7_8_MODE_AUDIO_BOUNCE;
    AudioBounce.flags = MODE_FLAG_HAS_BRIGHTNESS;
    AudioBounce.color_mode = MODE_COLORS_NONE;
    AudioBounce.brightness_min = 0;
    AudioBounce.brightness_max = 9;
    AudioBounce.brightness = brightness;
    modes.push_back(AudioBounce);

    mode AudioRipple;
    AudioRipple.name = "Audio Ripple Lighting";
    AudioRipple.value = LENOVO_LEGION_GEN7_8_MODE_AUDIO_RIPPLE;
    AudioRipple.flags = MODE_FLAG_HAS_BRIGHTNESS;
    AudioRipple.color_mode = MODE_COLORS_NONE;
    AudioRipple.brightness_min = 0;
    AudioRipple.brightness_max = 9;
    AudioRipple.brightness = brightness;
    modes.push_back(AudioRipple);

    mode Static;
    Static.name = "Static";
    Static.value = LENOVO_LEGION_GEN7_8_MODE_STATIC;
    Static.flags = MODE_FLAG_HAS_PER_LED_COLOR |
        MODE_FLAG_HAS_BRIGHTNESS |
        MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode = MODE_COLORS_PER_LED;
    Static.brightness_min = 0;
    Static.brightness_max = 9;
    Static.brightness = brightness;
    modes.push_back(Static);

    mode Type;
    Type.name = "Type Lighting";
    Type.value = LENOVO_LEGION_GEN7_8_MODE_TYPE;
    Type.flags = MODE_FLAG_HAS_SPEED |
        MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
        MODE_FLAG_HAS_RANDOM_COLOR |
        MODE_FLAG_HAS_BRIGHTNESS |
        MODE_FLAG_AUTOMATIC_SAVE;
    Type.speed_min = 0x01;
    Type.speed_max = 0x03;
    Type.speed = 2;
    Type.colors_min = 1;
    Type.colors_max = 4;
    Type.colors.resize(4);
    Type.colors[0] = 0xFFF500;
    Type.color_mode = MODE_COLORS_RANDOM;
    Type.brightness_min = 0;
    Type.brightness_max = 9;
    Type.brightness = brightness;
    modes.push_back(Type);

    mode Direct;
    Direct.name = "Direct";
    Direct.value = LENOVO_LEGION_GEN7_8_MODE_DIRECT;
    Direct.flags = MODE_FLAG_HAS_PER_LED_COLOR |
        MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode = MODE_COLORS_PER_LED;
    Direct.brightness_min = 0;
    Direct.brightness_max = 9;
    Direct.brightness = brightness;
    modes.push_back(Direct);


    name   = controller->getName();
    type   = DEVICE_TYPE_KEYBOARD;
    vendor = "Lenovo";

    switch (controller->getPid())
    {
    case LEGION_S7GEN7:
        description = "Lenovo Legion 7 Slim Generation 7";
        break;

    case LEGION_7GEN7:
        description = "Lenovo Legion 7 Generation 7";
        break;

    case LEGION_7GEN8:
        description = "Lenovo Legion 7 Generation 8";
        break;

    case LEGION_7GEN9:
    case LEGION_7GEN9_H:
        description = "Lenovo Legion 7 Generation 9";
        break;
    }

    brightness = controller->getCurrentBrightness();
    profile_id = controller->getCurrentProfileId();

    SetupZones();

    /*-----------------*\
    | Initiliaze Static |
    \*-----------------*/
    // ToDo: Commented causes a crash. (???)
    active_mode = 10;
    ReadDeviceSettings();
    last_mode = active_mode;
}

LenovoRGBController_Gen7_8::~LenovoRGBController_Gen7_8()
{
    delete controller;
}


void LenovoRGBController_Gen7_8::SetupZones()
{
    vector<lenovo_zone> lenovo_zones;
    lenovo_zones.push_back(legion7_gen7and8_kbd_ansi);
    lenovo_zones.push_back(legion7_gen7and8_neon);

    if (controller->getPid() == LEGION_7GEN7)
    {
        lenovo_zones.push_back(lenovo_legion_7gen7_logo);
        lenovo_zones.push_back(lenovo_legion_7gen7_vents);
    }

    for(unsigned int i = 0; i < lenovo_zones.size(); i++)
    {
        zone new_zone;
        new_zone.name       = lenovo_zones[i].name;
        new_zone.type       = lenovo_zones[i].type;
        new_zone.leds_count = lenovo_zones[i].end - lenovo_zones[i].start + 1;
        new_zone.leds_max   = new_zone.leds_count;
        new_zone.leds_min   = new_zone.leds_count;

        LOG_DEBUG("[Lenovo Gen7/8 Controller] adding zone: %s with %u LEDs", new_zone.name.c_str(), new_zone.leds_count);

        if(lenovo_zones[i].type == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = new matrix_map_type;
            new_zone.matrix_map->height = lenovo_zones[i].height;
            new_zone.matrix_map->width  = lenovo_zones[i].width;
            new_zone.matrix_map->map    = new unsigned int[new_zone.matrix_map->height * new_zone.matrix_map->width];

            if(lenovo_zones[i].matrix_map != NULL)
            {
                new_zone.matrix_map->map = (unsigned int *) lenovo_zones[i].matrix_map;
            }
        }
        else
        {
            new_zone.matrix_map = NULL;
        }

        zones.push_back(new_zone);

        for(unsigned int led_idx = lenovo_zones[i].start; led_idx <= lenovo_zones[i].end; led_idx++ )
        {
            led new_led;
            new_led.name = lenovo_zones[i].leds[led_idx].name;
            new_led.value = lenovo_zones[i].id << 8 | lenovo_zones[i].leds[led_idx].led_num ;
            leds.push_back(new_led);

            /*---------------------------------------------------------*\
            | create led id to index map for fast look up               |
            \*---------------------------------------------------------*/
            led_id_to_index[new_led.value]=leds.size() - 1;
        }
    }

    SetupColors();
}

void LenovoRGBController_Gen7_8::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void LenovoRGBController_Gen7_8::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void LenovoRGBController_Gen7_8::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void LenovoRGBController_Gen7_8::DeviceUpdateMode()
{
    uint8_t hw_profile_id = controller->getCurrentProfileId();
    if(hw_profile_id != profile_id)
    {
        profile_id = hw_profile_id;
        ReadDeviceSettings();
        last_mode = active_mode;
    }

    if(brightness != modes[active_mode].brightness)
    {
        brightness = modes[active_mode].brightness;
        controller->setBrightness(brightness);
        for(mode &m : modes)
        {
            m.brightness = brightness;
        }
    }

    if(last_mode != active_mode)
    {

        if(modes[last_mode].value == LENOVO_LEGION_GEN7_8_MODE_DIRECT)
        {
                controller->setLedsDirectOff(profile_id);
        }


        if(modes[active_mode].value == LENOVO_LEGION_GEN7_8_MODE_DIRECT)
        {
                controller->setLedsDirectOn(profile_id);
                controller->setLedsByGroup(profile_id, GetLedGroups());
        }

        last_mode = active_mode;
    }

    if(modes[active_mode].value != LENOVO_LEGION_GEN7_8_MODE_DIRECT)
    {
        DeviceUpdateLEDs();
    }
}

void LenovoRGBController_Gen7_8::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == LENOVO_LEGION_GEN7_8_MODE_DIRECT)
    {
        controller->setLedsDirect(leds, colors);
    }
    else
    {
        controller->setLedsByGroup(profile_id, GetLedGroups());
    }

}

void LenovoRGBController_Gen7_8::ReadDeviceSettings()
{
    vector<led_group> current_settings = controller->getProfileSettings(profile_id);
    if(current_settings.size() > 0)
    {
        for(unsigned int i = 0; i < modes.size(); i++)
        {
            if(modes[i].value == current_settings[0].mode)
            {
                switch(current_settings[0].color_mode)
                {
                    case 0x02:
                        if(modes[i].flags & MODE_FLAG_HAS_PER_LED_COLOR)
                        {
                            modes[i].color_mode = MODE_COLORS_PER_LED;
                        }
                        else
                        {
                            modes[i].color_mode = MODE_COLORS_MODE_SPECIFIC;
                        }
                        break;

                    case 0x01:
                        modes[i].color_mode = MODE_COLORS_RANDOM;
                        break;

                    default:
                        modes[i].color_mode = MODE_COLORS_NONE;
                }

                switch(modes[i].color_mode)
                {
                    case MODE_COLORS_PER_LED:
                        for(size_t j=0; j < colors.size(); j++)
                        {
                            colors[j]=0x00;
                        }
                        for(const led_group &lg : current_settings)
                        {
                            if(lg.colors.size() > 0)
                            {
                                for(uint16_t led_id : lg.leds)
                                {
                                    if(auto search = led_id_to_index.find(led_id); search != led_id_to_index.end())
                                    {
                                        colors[search->second] = lg.colors[0];
                                    }
                                }
                            }
                        }
                        break;

                    case MODE_COLORS_MODE_SPECIFIC:
                        for(size_t j=0; j < modes[i].colors.size(); j++)
                        {
                            if(j < current_settings[0].colors.size())
                            {
                                modes[i].colors[j] = current_settings[0].colors[j];
                            }
                            else
                            {
                                modes[i].colors[j] = 0x00;
                            }
                        }
                }

                switch(current_settings[0].direction)
                {
                    case 0x01:
                        modes[i].direction = MODE_DIRECTION_UP;
                        break;
                    case 0x02:
                        modes[i].direction = MODE_DIRECTION_DOWN;
                        break;
                    case 0x03:
                        modes[i].direction = MODE_DIRECTION_LEFT;
                        break;
                    case 0x04:
                        modes[i].direction = MODE_DIRECTION_RIGHT;
                        break;
                }

                switch(current_settings[0].spin)
                {
                    case 0x01:
                        modes[i].direction = MODE_DIRECTION_RIGHT;
                        break;
                    case 0x02:
                        modes[i].direction = MODE_DIRECTION_LEFT;
                        break;
                }

                active_mode = i;
                break; //stop for loop
            }
        }
    }
}

std::vector<led_group> LenovoRGBController_Gen7_8::GetLedGroups()
{
    std::unordered_map<RGBColor, vector<uint16_t>> led_map;

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED &&
            modes[active_mode].value != LENOVO_LEGION_GEN7_8_MODE_DIRECT)
    {
        for(size_t i = 0; i < leds.size(); i++)
        {
            led_map[colors[i]].push_back(leds[i].value);
        }
    }
    else
    {
        size_t start = 0;
        size_t end = leds.size();

        /*---------------------------------------------------------*\
        | Riplle and Type only apply to keyboard                    |
        \*---------------------------------------------------------*/
        if(modes[active_mode].value == LENOVO_LEGION_GEN7_8_MODE_RIPPLE ||
                modes[active_mode].value == LENOVO_LEGION_GEN7_8_MODE_TYPE)
        {
            for(const zone &z : zones)
            {
                if(z.name == "Keyboard")
                {
                    start = z.start_idx;
                    end = start + z.leds_count;
                }
            }

        }

        for(size_t i = start; i < end; i++)
        {
            led_map[0x00].push_back(leds[i].value);
        }
    }

    vector<led_group> led_groups;
    for(const auto &pair : led_map)
    {
        if(pair.first == 0x00 && led_map.size() != 1)
        {
            continue;
        }

        led_group group;
        group.mode = modes[active_mode].value;
        group.speed = modes[active_mode].speed;
        group.spin = 0x00;
        group.direction = 0x00;

        switch(modes[active_mode].direction)
        {
            case MODE_DIRECTION_UP:
                group.direction = 0x01;
                break;

            case MODE_DIRECTION_DOWN:
                group.direction = 0x02;
                break;

            case MODE_DIRECTION_LEFT:
                if(modes[active_mode].value == LENOVO_LEGION_GEN7_8_MODE_SCREW_RAINBOW)
                {
                    group.spin      = 0x02;
                }
                else
                {
                    group.direction = 0x03;
                }
                break;

            case MODE_DIRECTION_RIGHT:
                if(modes[active_mode].value == LENOVO_LEGION_GEN7_8_MODE_SCREW_RAINBOW)
                {
                    group.spin      = 0x01;
                }
                else
                {
                    group.direction = 0x04;
                }
                break;
        }

        switch(modes[active_mode].color_mode)
        {
            default:
            case MODE_COLORS_NONE:
                group.color_mode = 0x00;
                break;

            case MODE_COLORS_RANDOM:
                group.color_mode = 0x01;
                break;

            case MODE_COLORS_MODE_SPECIFIC:
                group.color_mode = 0x02;
                for(RGBColor c : modes[active_mode].colors)
                {
                    if(c)
                    {
                        group.colors.push_back(c);
                    }
                }

                if(group.colors.size() == 0)
                {
                    group.colors.push_back(0xFFF500);
                }
                break;

            case MODE_COLORS_PER_LED:
                group.color_mode = 0x02;
                group.colors.push_back(pair.first);
                break;
        }

        group.leds = pair.second;

        led_groups.push_back(group);
    }

    return led_groups;
}
