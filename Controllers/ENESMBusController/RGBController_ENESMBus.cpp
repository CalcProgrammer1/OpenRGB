/*---------------------------------------------------------*\
| RGBController_ENESMBus.cpp                                |
|                                                           |
|   RGBController for ENE SMBus devices                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_ENESMBus.h"
#include "LogManager.h"
#include "SettingsManager.h"
#include "ResourceManager.h"

/**------------------------------------------------------------------*\
    @name ENE SMBus Device
    @category RAM,Motherboard,GPU,Storage
    @type SMBus
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectENESMBusDRAMControllers,DetectENESMBusMotherboardControllers,DetectENESMBusGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ENESMBus::RGBController_ENESMBus(ENESMBusController * controller_ptr)
{
    controller                  = controller_ptr;

    /*---------------------------------------------------------*\
    | Get ENEController settings                                |
    \*---------------------------------------------------------*/
    json ene_settings           = ResourceManager::get()->GetSettingsManager()->GetSettings("ENESMBusSettings");

    /*---------------------------------------------------------*\
    | Check if save to device is enabled                        |
    \*---------------------------------------------------------*/
    unsigned int save_flag      = 0;

    if(ene_settings.contains("enable_save"))
    {
        if(ene_settings["enable_save"] == true)
        {
            save_flag = MODE_FLAG_MANUAL_SAVE;
        }
    }

    /*---------------------------------------------------------*\
    | Determine name and type (DRAM or Motherboard) by checking |
    | the ENE controller's version string                       |
    \*---------------------------------------------------------*/
    name                        = controller->GetName();
    description                 = "ENE SMBus Device";
    version                     = controller->GetVersion();
    location                    = controller->GetLocation();
    type                        = controller->GetType();

    if((version.find("DIMM_LED") != std::string::npos) || (version.find("AUDA") != std::string::npos) )
    {
        vendor                  = "ENE";
    }
    else if(version.find("ROG STRIX ARION") != std::string::npos)
    {
        vendor                  = "ASUS";
    }
    else if(location.find("NVMe:") != std::string::npos)
    {
        vendor                  = "XPG";
    }
    else
    {
        vendor                  = "ASUS";
    }

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0xFFFF;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ENE_MODE_OFF;
    Off.flags                   = save_flag;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ENE_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | save_flag;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ENE_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | save_flag;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.speed_min         = ENE_SPEED_SLOWEST;
    Breathing.speed_max         = ENE_SPEED_FASTEST;
    Breathing.speed             = ENE_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name               = "Flashing";
    Flashing.value              = ENE_MODE_FLASHING;
    Flashing.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | save_flag;
    Flashing.color_mode         = MODE_COLORS_PER_LED;
    Flashing.speed_min          = ENE_SPEED_SLOWEST;
    Flashing.speed_max          = ENE_SPEED_FASTEST;
    Flashing.speed              = ENE_SPEED_NORMAL;
    modes.push_back(Flashing);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = ENE_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | save_flag;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    SpectrumCycle.speed_min     = ENE_SPEED_SLOWEST;
    SpectrumCycle.speed_max     = ENE_SPEED_FASTEST;
    SpectrumCycle.speed         = ENE_SPEED_NORMAL;
    modes.push_back(SpectrumCycle);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = ENE_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | save_flag;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.speed_min           = ENE_SPEED_SLOWEST;
    Rainbow.speed_max           = ENE_SPEED_FASTEST;
    Rainbow.speed               = ENE_SPEED_NORMAL;
    Rainbow.direction           = MODE_DIRECTION_LEFT;
    modes.push_back(Rainbow);

    mode ChaseFade;
    ChaseFade.name              = "Chase Fade";
    ChaseFade.value             = ENE_MODE_CHASE_FADE;
    ChaseFade.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | save_flag;
    ChaseFade.color_mode        = MODE_COLORS_PER_LED;
    ChaseFade.speed_min         = ENE_SPEED_SLOWEST;
    ChaseFade.speed_max         = ENE_SPEED_FASTEST;
    ChaseFade.speed             = ENE_SPEED_NORMAL;
    ChaseFade.direction         = MODE_DIRECTION_LEFT;
    modes.push_back(ChaseFade);

    mode Chase;
    Chase.name                  = "Chase";
    Chase.value                 = ENE_MODE_CHASE;
    Chase.flags                 = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | save_flag;
    Chase.color_mode            = MODE_COLORS_PER_LED;
    Chase.speed_min             = ENE_SPEED_SLOWEST;
    Chase.speed_max             = ENE_SPEED_FASTEST;
    Chase.speed                 = ENE_SPEED_NORMAL;
    ChaseFade.direction         = MODE_DIRECTION_LEFT;
    modes.push_back(Chase);

    mode RandomFlicker;
    RandomFlicker.name          = "Random Flicker";
    RandomFlicker.value         = ENE_MODE_RANDOM_FLICKER;
    RandomFlicker.flags         = MODE_FLAG_HAS_SPEED | save_flag;
    RandomFlicker.color_mode    = MODE_COLORS_NONE;
    RandomFlicker.speed_min     = ENE_SPEED_SLOWEST;
    RandomFlicker.speed_max     = ENE_SPEED_FASTEST;
    RandomFlicker.speed         = ENE_SPEED_NORMAL;
    modes.push_back(RandomFlicker);

    if(controller->SupportsMode14())
    {
        mode DoubleFade;
        DoubleFade.name         = "Double Fade";
        DoubleFade.value        = ENE_MODE_DOUBLE_FADE;
        DoubleFade.flags        = MODE_FLAG_HAS_SPEED;
        DoubleFade.color_mode   = MODE_COLORS_NONE;
        DoubleFade.speed_min    = ENE_SPEED_SLOWEST;
        DoubleFade.speed_max    = ENE_SPEED_FASTEST;
        DoubleFade.speed        = ENE_SPEED_NORMAL;
        modes.push_back(DoubleFade);
    }

    SetupZones();

    /*-------------------------------------------------*\
    | Initialize active mode                            |
    \*-------------------------------------------------*/
    active_mode = GetDeviceMode();
}

RGBController_ENESMBus::~RGBController_ENESMBus()
{
    delete controller;
}

int RGBController_ENESMBus::GetDeviceMode()
{
    /*-----------------------------------------------------------------*\
    | Determine starting mode by reading the mode and direct registers  |
    \*-----------------------------------------------------------------*/
    int dev_mode    = controller->ENERegisterRead(ENE_REG_MODE);
    int color_mode  = MODE_COLORS_PER_LED;
    int speed       = controller->ENERegisterRead(ENE_REG_SPEED);
    int direction   = controller->ENERegisterRead(ENE_REG_DIRECTION);

    LOG_TRACE("[%s] Retrieved ENE mode from module: %02d", name.c_str(), dev_mode);

    if(controller->ENERegisterRead(ENE_REG_DIRECT))
    {
        dev_mode = 0xFFFF;
    }

    switch(dev_mode)
    {
        case ENE_MODE_OFF:
        case ENE_MODE_RAINBOW:
        case ENE_MODE_SPECTRUM_CYCLE:
        case ENE_MODE_RANDOM_FLICKER:
        case ENE_MODE_DOUBLE_FADE:
            color_mode = MODE_COLORS_NONE;
            break;

        case ENE_MODE_SPECTRUM_CYCLE_CHASE:
            dev_mode   = ENE_MODE_CHASE;
            color_mode = MODE_COLORS_RANDOM;
            break;

        case ENE_MODE_SPECTRUM_CYCLE_BREATHING:
            dev_mode   = ENE_MODE_BREATHING;
            color_mode = MODE_COLORS_RANDOM;
            break;

        case ENE_MODE_SPECTRUM_CYCLE_CHASE_FADE:
            dev_mode   = ENE_MODE_CHASE_FADE;
            color_mode = MODE_COLORS_RANDOM;
            break;
    }

    for(int mode = 0; mode < (int)modes.size(); mode++)
    {
        if(modes[mode].value == dev_mode)
        {
            active_mode                 = mode;
            modes[mode].color_mode      = color_mode;

            if(modes[mode].flags & MODE_FLAG_HAS_SPEED)
            {
                modes[mode].speed       = speed;
            }

            if(modes[mode].flags & MODE_FLAG_HAS_DIRECTION_LR)
            {
                modes[mode].direction   = direction;
            }

            break;
        }
    }

    /*---------------------------------------------------------*\
    | Initialize colors for each LED                            |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned int  led = leds[led_idx].value;
        unsigned char red;
        unsigned char grn;
        unsigned char blu;

        if(active_mode == 0)
        {
            red = controller->GetLEDRed(led);
            grn = controller->GetLEDGreen(led);
            blu = controller->GetLEDBlue(led);
        }
        else
        {
            red = controller->GetLEDRedEffect(led);
            grn = controller->GetLEDGreenEffect(led);
            blu = controller->GetLEDBlueEffect(led);
        }

        colors[led_idx] = ToRGBColor(red, grn, blu);
    }

    return(active_mode);
}

void RGBController_ENESMBus::DeviceUpdateLEDs()
{
    if(GetMode() == 0)
    {
        controller->SetAllColorsDirect(&colors[0]);
    }
    else
    {
        controller->SetAllColorsEffect(&colors[0]);
    }

}

void RGBController_ENESMBus::UpdateZoneLEDs(int zone)
{
    for(std::size_t led_idx = 0; led_idx < zones[zone].leds_count; led_idx++)
    {
        int           led   = zones[zone].leds[led_idx].value;
        RGBColor      color = colors[led];
        unsigned char red   = RGBGetRValue(color);
        unsigned char grn   = RGBGetGValue(color);
        unsigned char blu   = RGBGetBValue(color);

        if(GetMode() == 0)
        {
            controller->SetLEDColorDirect(led, red, grn, blu);
        }
        else
        {
            controller->SetLEDColorEffect(led, red, grn, blu);
        }
    }
}

void RGBController_ENESMBus::UpdateSingleLED(int led)
{
    RGBColor color    = colors[led];
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if(GetMode() == 0)
    {
        controller->SetLEDColorDirect(led, red, grn, blu);
    }
    else
    {
        controller->SetLEDColorEffect(led, red, grn, blu);
    }
}

void RGBController_ENESMBus::SetupZones()
{
    /*---------------------------------------------------------*\
    | Search through all LEDs and create zones for each channel |
    | type                                                      |
    \*---------------------------------------------------------*/
    for(unsigned int cfg_zone_idx = 0; cfg_zone_idx < ENE_NUM_ZONES; cfg_zone_idx++)
    {
        /*---------------------------------------------------------*\
        | Get the number of LEDs in the zone                        |
        \*---------------------------------------------------------*/
        unsigned int leds_in_zone = controller->GetLEDCount(cfg_zone_idx);

        if(leds_in_zone > 0)
        {
            /*---------------------------------------------------------*\
            | Search through existing zones to make sure we don't       |
            | create a duplicate zone                                   |
            \*---------------------------------------------------------*/
            bool matched                    = false;
            std::size_t existing_zone_idx   = 0;

            for(existing_zone_idx = 0; existing_zone_idx < zones.size(); existing_zone_idx++)
            {
                if(controller->GetChannelName(cfg_zone_idx) == zones[existing_zone_idx].name)
                {
                    matched = true;
                    break;
                }
            }

            /*---------------------------------------------------------*\
            | If zone does not already exist, create it                 |
            \*---------------------------------------------------------*/
            if(matched == false)
            {
                zone* new_zone = new zone();

                /*---------------------------------------------------------*\
                | Set zone name to channel name                             |
                \*---------------------------------------------------------*/
                new_zone->name = controller->GetChannelName(cfg_zone_idx);

                /*---------------------------------------------------------*\
                | Set zone LED count to LEDs in zone                        |
                \*---------------------------------------------------------*/
                new_zone->leds_count = leds_in_zone;

                /*---------------------------------------------------------*\
                | Push new zone to zones vector                             |
                \*---------------------------------------------------------*/
                zones.push_back(*new_zone);
            }
            /*---------------------------------------------------------*\
            | Otherwise, add the number of LEDs from this zone to the   |
            | existing one.                                             |
            \*---------------------------------------------------------*/
            else
            {
                zones[existing_zone_idx].leds_count += leds_in_zone;
            }
        }
    }

    /*---------------------------------------------------------*\
    | Finish setting up the zones                               |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].leds_min    = zones[zone_idx].leds_count;
        zones[zone_idx].leds_max    = zones[zone_idx].leds_count;

        if(zones[zone_idx].leds_count > 1)
        {
            zones[zone_idx].type    = ZONE_TYPE_LINEAR;
        }
        else
        {
            zones[zone_idx].type    = ZONE_TYPE_SINGLE;
        }

        zones[zone_idx].matrix_map  = NULL;
    }

    /*---------------------------------------------------------*\
    | Create LED entries for each zone                          |
    \*---------------------------------------------------------*/
    unsigned int led_idx = 0;
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(std::size_t zone_led_idx = 0; zone_led_idx < zones[zone_idx].leds_count; zone_led_idx++)
        {
            led* new_led = new led();

            new_led->name = zones[zone_idx].name + " LED ";
            new_led->name.append(std::to_string(zone_led_idx + 1));

            new_led->value = led_idx;
            led_idx++;

            leds.push_back(*new_led);
        }
    }

    SetupColors();
}

void RGBController_ENESMBus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ENESMBus::DeviceUpdateMode()
{
    if (modes[active_mode].value == 0xFFFF)
    {
        controller->SetDirect(true);
    }
    else
    {
        int new_mode        = modes[active_mode].value;
        int new_speed       = 0;
        int new_direction   = 0;

        if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
        {
            switch(new_mode)
            {
            case ENE_MODE_CHASE:
                new_mode = ENE_MODE_SPECTRUM_CYCLE_CHASE;
                break;
            case ENE_MODE_BREATHING:
                new_mode = ENE_MODE_SPECTRUM_CYCLE_BREATHING;
                break;
            case ENE_MODE_CHASE_FADE:
                new_mode = ENE_MODE_SPECTRUM_CYCLE_CHASE_FADE;
                break;
            }
        }

        if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
        {
            new_speed = modes[active_mode].speed;
        }

        if(modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR)
        {
            switch(modes[active_mode].direction)
            {
                case MODE_DIRECTION_LEFT:
                    new_direction = ENE_DIRECTION_FORWARD;
                    break;

                case MODE_DIRECTION_RIGHT:
                    new_direction = ENE_DIRECTION_REVERSE;
                    break;
            }
        }

        controller->SetMode(new_mode, new_speed, new_direction);
        controller->SetDirect(false);
    }
}

void RGBController_ENESMBus::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveMode();
}
