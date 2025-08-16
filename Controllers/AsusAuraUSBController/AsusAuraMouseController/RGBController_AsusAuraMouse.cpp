/*---------------------------------------------------------*\
| RGBController_AsusAuraMouse.cpp                           |
|                                                           |
|   RGBController for ASUS Aura mouse                       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AsusAuraMouse.h"

static std::string aura_mouse_zone_names[5]
{
    "Logo",
    "Scroll Wheel",
    "Underglow",
    "All",
    "Dock"
};

/**------------------------------------------------------------------*\
    @name Asus Aura Mouse
    @category Mouse
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraUSBMice
    @comment
\*-------------------------------------------------------------------*/

RGBController_AuraMouse::RGBController_AuraMouse(AuraMouseController* controller_ptr)
{
    controller                      = controller_ptr;

    pid                             = controller->device_pid;

    name                            = controller->GetName();
    vendor                          = "ASUS";
    type                            = DEVICE_TYPE_MOUSE;
    description                     = "ASUS Aura Mouse Device";
    version                         = controller->GetVersion(aura_mouse_devices[pid].wireless, aura_mouse_devices[pid].version_protocol);
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    std::vector<uint8_t> mm         = aura_mouse_devices[pid].mouse_modes;

    if(aura_mouse_devices[pid].direct)
    {
        mode Direct;
        Direct.name                 = "Direct";
        Direct.value                = 254;
        Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode           = MODE_COLORS_PER_LED;
        modes.push_back(Direct);
    }

    int mode_value                  = 0;

    for(std::vector<uint8_t>::iterator it = mm.begin(); it != mm.end(); it++)
    {
        switch(*it)
        {
            case AURA_MOUSE_MODE_STATIC:
                /*-----------------------------------------------------------------*\
                | If there is no direct mode, this mode can be used as direct       |
                | (Asus does it the same way).                                      |
                | The acutal direct mode is only found on new devices and on        |
                | these devices static can't be used as direct anymore as it is     |
                | too slow. The Spatha X's dock can't be controlled, because        |
                | static is too slow and it can't be adressed in direct             |
                \*-----------------------------------------------------------------*/
                {
                    mode Static;
                    Static.name                 = (aura_mouse_devices[pid].direct || pid == AURA_ROG_SPATHA_X_DOCK_FAKE_PID)? "Static" : "Direct";
                    Static.value                = mode_value;
                    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    Static.brightness_min       = aura_mouse_devices[pid].brightness_min;
                    Static.brightness_max       = aura_mouse_devices[pid].brightness_max;
                    Static.brightness           = aura_mouse_devices[pid].brightness_max;
                    Static.color_mode           = MODE_COLORS_PER_LED;
                    modes.push_back(Static);
                }
                break;

            case AURA_MOUSE_MODE_BREATHING:
                {
                    mode Breathing;
                    Breathing.name              = "Breathing";
                    Breathing.value             = mode_value;
                    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    Breathing.brightness_min    = aura_mouse_devices[pid].brightness_min;
                    Breathing.brightness_max    = aura_mouse_devices[pid].brightness_max;
                    Breathing.brightness        = aura_mouse_devices[pid].brightness_max;
                    Breathing.color_mode        = MODE_COLORS_PER_LED;
                    modes.push_back(Breathing);
                }
                break;

            case AURA_MOUSE_MODE_SPECTRUM:
                {
                    mode ColorCycle;
                    ColorCycle.name             = "Spectrum Cycle";
                    ColorCycle.value            = mode_value;
                    ColorCycle.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE ;
                    ColorCycle.brightness_min   = aura_mouse_devices[pid].brightness_min;
                    ColorCycle.brightness_max   = aura_mouse_devices[pid].brightness_max;
                    ColorCycle.brightness       = aura_mouse_devices[pid].brightness_max;
                    ColorCycle.color_mode       = MODE_COLORS_NONE;
                    modes.push_back(ColorCycle);
                }
                break;

            case AURA_MOUSE_MODE_WAVE:
                {
                    mode Wave;
                    Wave.name                   = "Rainbow Wave";
                    Wave.value                  = mode_value;
                    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    Wave.direction              = 0;
                    Wave.speed_min              = aura_mouse_devices[pid].speed_min;
                    Wave.speed_max              = aura_mouse_devices[pid].speed_max;
                    Wave.speed                  = (aura_mouse_devices[pid].speed_min + aura_mouse_devices[pid].speed_max) / 2;
                    Wave.brightness_min         = aura_mouse_devices[pid].brightness_min;
                    Wave.brightness_max         = aura_mouse_devices[pid].brightness_max;
                    Wave.brightness             = aura_mouse_devices[pid].brightness_max;
                    Wave.color_mode             = MODE_COLORS_NONE;
                    modes.push_back(Wave);
                }
                break;

            case AURA_MOUSE_MODE_REACTIVE:
                {
                    mode Reactive;
                    Reactive.name               = "Reactive";
                    Reactive.value              = mode_value;
                    Reactive.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    Reactive.brightness_min     = aura_mouse_devices[pid].brightness_min;
                    Reactive.brightness_max     = aura_mouse_devices[pid].brightness_max;
                    Reactive.brightness         = aura_mouse_devices[pid].brightness_max;
                    Reactive.color_mode         = MODE_COLORS_PER_LED;
                    modes.push_back(Reactive);
                }
                break;

            case AURA_MOUSE_MODE_COMET:
                {
                    mode Comet;
                    Comet.name                  = "Comet";
                    Comet.value                 = mode_value;
                    Comet.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    Comet.brightness_min        = aura_mouse_devices[pid].brightness_min;
                    Comet.brightness_max        = aura_mouse_devices[pid].brightness_max;
                    Comet.brightness            = aura_mouse_devices[pid].brightness_max;
                    Comet.color_mode            = MODE_COLORS_MODE_SPECIFIC;
                    Comet.direction             = 0;
                    Comet.colors_min            = 1;
                    Comet.colors_max            = 1;
                    Comet.colors.resize(1);
                    modes.push_back(Comet);
                }
                break;

            case AURA_MOUSE_MODE_BATTERY:
                {
                    mode BatteryMode;
                    BatteryMode.name            = "Battery";
                    BatteryMode.value           = mode_value;
                    BatteryMode.flags           = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    BatteryMode.brightness_min  = aura_mouse_devices[pid].brightness_min;
                    BatteryMode.brightness_max  = aura_mouse_devices[pid].brightness_max;
                    BatteryMode.brightness      = aura_mouse_devices[pid].brightness_max;
                    BatteryMode.color_mode      = MODE_COLORS_NONE;
                    modes.push_back(BatteryMode);
                }
                break;
        }
        mode_value++;
    }

    SetupZones();
}

RGBController_AuraMouse::~RGBController_AuraMouse()
{
    delete controller;
}

void RGBController_AuraMouse::SetupZones()
{
    for(std::vector<uint8_t>::iterator zone_it = aura_mouse_devices[pid].mouse_zones.begin(); zone_it != aura_mouse_devices[pid].mouse_zones.end(); zone_it++)
    {
        zone mouse_zone;

        mouse_zone.name         = aura_mouse_zone_names[*zone_it];
        mouse_zone.type         = ZONE_TYPE_SINGLE;
        mouse_zone.leds_min     = 1;
        mouse_zone.leds_max     = 1;
        mouse_zone.leds_count   = 1;
        mouse_zone.matrix_map   = NULL;

        zones.push_back(mouse_zone);

        led mouse_led;

        mouse_led.name          = mouse_zone.name + " LED";
        mouse_led.value         = *zone_it;

        leds.push_back(mouse_led);
    }

    SetupColors();
}

void RGBController_AuraMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AuraMouse::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == AURA_MOUSE_MODE_DIRECT)
    {
        controller->SendDirect(colors);
    }
    else
    {
        for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
        {
            UpdateSingleLED(zone_index);
        }
    }
}

void RGBController_AuraMouse::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraMouse::UpdateSingleLED(int led)
{
    if(modes[active_mode].value == AURA_MOUSE_MODE_DIRECT)
    {
        DeviceUpdateLEDs();
        return;
    }

    uint8_t red = RGBGetRValue(colors[led]);
    uint8_t grn = RGBGetGValue(colors[led]);
    uint8_t blu = RGBGetBValue(colors[led]);

    controller->SendUpdate(leds[led].value, modes[active_mode].value, red, grn, blu, 0, false, 0, modes[active_mode].brightness);
}

void RGBController_AuraMouse::DeviceUpdateMode()
{
    if(modes[active_mode].value == AURA_MOUSE_MODE_DIRECT)
    {
        return;
    }

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        unsigned char red = 0;
        unsigned char grn = 0;
        unsigned char blu = 0;

        if (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            red = RGBGetRValue(modes[active_mode].colors[0]);
            grn = RGBGetGValue(modes[active_mode].colors[0]);
            blu = RGBGetBValue(modes[active_mode].colors[0]);
        }


        if(pid == AURA_ROG_SPATHA_X_DOCK_FAKE_PID)
        {
            controller->SendUpdate(AURA_MOUSE_ZONE_DOCK, modes[active_mode].value, red, grn, blu, modes[active_mode].direction, modes[active_mode].color_mode == MODE_COLORS_RANDOM, modes[active_mode].speed, modes[active_mode].brightness);
        }
        else if(pid == AURA_ROG_STRIX_IMPACT_PID)
        {
            /*-----------------------------------------------------------------*\
            | The ROG Impact doesn't accept AURA_MOUSE_ZONE_ALL                 |
            \*-----------------------------------------------------------------*/
            controller->SendUpdate(AURA_MOUSE_ZONE_LOGO, modes[active_mode].value, red, grn, blu, modes[active_mode].direction, modes[active_mode].color_mode == MODE_COLORS_RANDOM, modes[active_mode].speed, modes[active_mode].brightness);
        }
        else
        {
            controller->SendUpdate(AURA_MOUSE_ZONE_ALL, modes[active_mode].value, red, grn, blu, modes[active_mode].direction, modes[active_mode].color_mode == MODE_COLORS_RANDOM, modes[active_mode].speed, modes[active_mode].brightness);
        }
    }
}

void RGBController_AuraMouse::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveMode();
}
