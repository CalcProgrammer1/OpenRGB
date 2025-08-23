/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2USB.cpp                   |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 USB       |
|   motherboard                                             |
|                                                           |
|   jackun                                      08 Jan 2020 |
|   megadjc                                     31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "GigabyteFusion2USB_Devices.h"
#include "LogManager.h"
#include "RGBController_GigabyteRGBFusion2USB.h"
#include "ResourceManager.h"

/**------------------------------------------------------------------*\
    @name Gigabyte RGB Fusion 2 USB
    @category Motherboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGigabyteRGBFusion2USBControllers
    @comment The Fusion 2 USB controller applies to most AMD and
        Intel mainboards from the x570 and z390 chipsets onwards.
\*-------------------------------------------------------------------*/

RGBController_RGBFusion2USB::RGBController_RGBFusion2USB(RGBFusion2USBController* controller_ptr, std::string detector)
{
    controller                  = controller_ptr;
    name                        = controller->GetDeviceName();
    detector_name               = detector;
    vendor                      = "Gigabyte";
    type                        = DEVICE_TYPE_MOTHERBOARD;
    description                 = controller->GetDeviceDescription();
    version                     = controller->GetFWVersion();
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerial();
    device_num                  = controller->GetDeviceNum();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0xFFFF;
    Direct.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.brightness_min       = RGBFUSION2_BRIGHTNESS_MIN;
    Direct.brightness_max       = RGBFUSION2_BRIGHTNESS_MAX;
    Direct.brightness           = RGBFUSION2_BRIGHTNESS_MAX;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = EFFECT_STATIC;
    Static.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.brightness_min       = RGBFUSION2_BRIGHTNESS_MIN;
    Static.brightness_max       = RGBFUSION2_BRIGHTNESS_MAX;
    Static.brightness           = RGBFUSION2_BRIGHTNESS_MAX;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = EFFECT_PULSE;
    Breathing.flags             = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.brightness_min    = RGBFUSION2_BRIGHTNESS_MIN;
    Breathing.brightness_max    = 100;                      // Set 100 max due to controller quirks
    Breathing.brightness        = Breathing.brightness_max;
    Breathing.speed_min         = RGBFUSION2_SPEED_MIN;
    Breathing.speed_max         = RGBFUSION2_SPEED_MAX;
    Breathing.speed             = RGBFUSION2_SPEED_MID;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Blinking;
    Blinking.name               = "Flashing";
    Blinking.value              = EFFECT_BLINKING;
    Blinking.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Blinking.brightness_min     = RGBFUSION2_BRIGHTNESS_MIN;
    Blinking.brightness_max     = RGBFUSION2_BRIGHTNESS_MAX;
    Blinking.brightness         = RGBFUSION2_BRIGHTNESS_MAX;
    Blinking.speed_min          = RGBFUSION2_SPEED_MIN;
    Blinking.speed_max          = RGBFUSION2_SPEED_MAX;
    Blinking.speed              = RGBFUSION2_SPEED_MID;
    Blinking.colors_min         = 1;
    Blinking.colors_max         = 1;
    Blinking.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Blinking.colors.resize(1);
    modes.push_back(Blinking);

    mode ColorCycle;
    ColorCycle.name             = "Color Cycle";
    ColorCycle.value            = EFFECT_COLORCYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    ColorCycle.brightness_min   = RGBFUSION2_BRIGHTNESS_MIN;
    ColorCycle.brightness_max   = RGBFUSION2_BRIGHTNESS_MAX;
    ColorCycle.brightness       = RGBFUSION2_BRIGHTNESS_MAX;
    ColorCycle.speed_min        = RGBFUSION2_SPEED_MIN;
    ColorCycle.speed_max        = RGBFUSION2_SPEED_MAX;
    ColorCycle.speed            = RGBFUSION2_SPEED_MID;
    ColorCycle.color_mode       = MODE_COLORS_NONE;
    modes.push_back(ColorCycle);

    mode Flashing;
    Flashing.name               = "Double Flash";
    Flashing.value              = EFFECT_DFLASH;
    Flashing.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Flashing.brightness_min     = RGBFUSION2_BRIGHTNESS_MIN;
    Flashing.brightness_max     = RGBFUSION2_BRIGHTNESS_MAX;
    Flashing.brightness         = RGBFUSION2_BRIGHTNESS_MAX;
    Flashing.speed_min          = RGBFUSION2_SPEED_MIN;
    Flashing.speed_max          = RGBFUSION2_SPEED_MAX;
    Flashing.speed              = RGBFUSION2_SPEED_MID;
    Flashing.colors_min         = 1;
    Flashing.colors_max         = 1;
    Flashing.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Flashing.colors.resize(1);
    modes.push_back(Flashing);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = EFFECT_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Wave.brightness_min         = RGBFUSION2_BRIGHTNESS_MIN;
    Wave.brightness_max         = RGBFUSION2_BRIGHTNESS_MAX;
    Wave.brightness             = RGBFUSION2_BRIGHTNESS_MAX;
    Wave.speed_min              = RGBFUSION2_SPEED_MIN;
    Wave.speed_max              = RGBFUSION2_SPEED_MAX;
    Wave.speed                  = RGBFUSION2_SPEED_MID;
    Wave.colors_min             = 0;
    Wave.colors_max             = 0;
    Wave.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Wave);

    mode Random;
    Random.name                 = "Random";
    Random.value                = EFFECT_RANDOM;
    Random.flags                = MODE_FLAG_HAS_BRIGHTNESS;
    Random.brightness_min       = RGBFUSION2_BRIGHTNESS_MIN;
    Random.brightness_max       = RGBFUSION2_BRIGHTNESS_MAX;
    Random.brightness           = RGBFUSION2_BRIGHTNESS_MAX;
    Random.colors_min           = 0;
    Random.colors_max           = 0;
    Random.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Random);

    mode Wave1;
    Wave1.name                  = "Wave 1";
    Wave1.value                 = EFFECT_WAVE1;
    Wave1.flags                 = MODE_FLAG_HAS_BRIGHTNESS;
    Wave1.brightness_min        = RGBFUSION2_BRIGHTNESS_MIN;
    Wave1.brightness_max        = RGBFUSION2_BRIGHTNESS_MAX;
    Wave1.brightness            = RGBFUSION2_BRIGHTNESS_MAX;
    Wave1.colors_min            = 0;
    Wave1.colors_max            = 0;
    Wave1.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Wave1);

    mode Wave2;
    Wave2.name                  = "Wave 2";
    Wave2.value                 = EFFECT_WAVE2;
    Wave2.flags                 = MODE_FLAG_HAS_BRIGHTNESS;
    Wave2.brightness_min        = RGBFUSION2_BRIGHTNESS_MIN;
    Wave2.brightness_max        = RGBFUSION2_BRIGHTNESS_MAX;
    Wave2.brightness            = RGBFUSION2_BRIGHTNESS_MAX;
    Wave2.colors_min            = 0;
    Wave2.colors_max            = 0;
    Wave2.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Wave2);

    mode Wave3;
    Wave3.name                  = "Wave 3";
    Wave3.value                 = EFFECT_WAVE3;
    Wave3.flags                 = MODE_FLAG_HAS_BRIGHTNESS;
    Wave3.brightness_min        = RGBFUSION2_BRIGHTNESS_MIN;
    Wave3.brightness_max        = RGBFUSION2_BRIGHTNESS_MAX;
    Wave3.brightness            = RGBFUSION2_BRIGHTNESS_MAX;
    Wave3.colors_min            = 0;
    Wave3.colors_max            = 0;
    Wave3.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Wave3);

    mode Wave4;
    Wave4.name                  = "Wave 4";
    Wave4.value                 = EFFECT_WAVE4;
    Wave4.flags                 = MODE_FLAG_HAS_BRIGHTNESS;
    Wave4.brightness_min        = RGBFUSION2_BRIGHTNESS_MIN;
    Wave4.brightness_max        = RGBFUSION2_BRIGHTNESS_MAX;
    Wave4.brightness            = RGBFUSION2_BRIGHTNESS_MAX;
    Wave4.colors_min            = 0;
    Wave4.colors_max            = 0;
    Wave4.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Wave4);

    Init_Controller();
    SetupZones();
}

RGBController_RGBFusion2USB::~RGBController_RGBFusion2USB()
{
    delete controller;
}

/*---------------------------------------------------------*\
| Loads JSON config data                                    |
\*---------------------------------------------------------*/
void RGBController_RGBFusion2USB::Init_Controller()
{
    const std::string SectionCustom      = "CustomLayout";
    const std::string SectionCalibration = "Calibration";
    RvrseLedHeaders ReverseLedLookup     = reverse_map(LedLookup);
    SettingsManager* settings_manager    = ResourceManager::get()->GetSettingsManager();
    nlohmann::json device_settings       = settings_manager->GetSettings(detector_name);

    /*---------------------------------------------------------*\
    | Create the custom layout from the generic_device          |
    \*---------------------------------------------------------*/
    gb_fusion2_device* layout = const_cast<gb_fusion2_device*>(gb_fusion2_device_list[device_index]);

    if(!device_settings.contains(SectionCustom))
    {
        device_settings[SectionCustom]["Enabled"]   = false;
        device_settings[SectionCustom]["Data"]      = BuildCustomLayoutJson(layout, ReverseLedLookup);
        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
    }

    bool custom_layout = device_settings[SectionCustom]["Enabled"];

    if(custom_layout)
    {
        LoadCustomLayoutFromJson(device_settings[SectionCustom]["Data"], LedLookup, layout);
    }

    EncodedCalibration hw_cal = controller->GetCalibration(false);

    if(!device_settings.contains(SectionCalibration))
    {
        device_settings[SectionCalibration]["Enabled"] = false;
        device_settings[SectionCalibration]["Data"]    = WriteCalJsonFrom(hw_cal);
        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
    }
    else
    {
        nlohmann::json& cal_sec = device_settings[SectionCalibration];
        bool cal_enable         = cal_sec.value("Enabled", false);

        if(!cal_sec.contains("Data") || !cal_sec["Data"].is_object())
        {
            cal_sec["Data"] = WriteCalJsonFrom(hw_cal);
            settings_manager->SetSettings(detector_name, device_settings);
            settings_manager->SaveSettings();
        }
        else
        {
            nlohmann::json& cdata = cal_sec["Data"];
            FillMissingWith(cdata, hw_cal);

            if(!cal_enable)
            {
                cal_sec["Data"] = WriteCalJsonFrom(hw_cal);
                settings_manager->SetSettings(detector_name, device_settings);
                settings_manager->SaveSettings();
            }
        }

        if(cal_enable)
        {
            const nlohmann::json& cdata = cal_sec["Data"];

            EncodedCalibration desired;
            desired.dled[0]   = GET_JSON_VAL_ELSE_OFF(cdata, "HDR_D_LED1");
            desired.dled[1]   = GET_JSON_VAL_ELSE_OFF(cdata, "HDR_D_LED2");
            desired.mainboard = GET_JSON_VAL_ELSE_OFF(cdata, "Mainboard");
            desired.spare[0]  = GET_JSON_VAL_ELSE_OFF(cdata, "Spare0");
            desired.spare[1]  = GET_JSON_VAL_ELSE_OFF(cdata, "Spare1");

            if(controller->GetProductID() == 0x5711)
            {
                desired.dled[2]  = GET_JSON_VAL_ELSE_OFF(cdata, "HDR_D_LED3");
                desired.dled[3]  = GET_JSON_VAL_ELSE_OFF(cdata, "HDR_D_LED4");
                desired.spare[2] = GET_JSON_VAL_ELSE_OFF(cdata, "Spare2");
                desired.spare[3] = GET_JSON_VAL_ELSE_OFF(cdata, "Spare3");
            }
            else
            {
                desired.dled[2]  = "OFF";
                desired.dled[3]  = "OFF";
                desired.spare[2] = "OFF";
                desired.spare[3] = "OFF";
            }
                controller->SetCalibration(desired, false);
        }
    }

    /*---------------------------------------------------------------------*\
    |  When no match found the first entry (generic_device) will be used    |
    |    otherwise look up channel map based on device name                 |
    \*---------------------------------------------------------------------*/
    if(!custom_layout)
    {
        /*-----------------------------------------------------------------*\
        | Loop through all known devices to look for a name match           |
        |   NB: Can be switched to device IDs lookup when acpi table        |
        |   is able to be probed accurately                                 |
        \*-----------------------------------------------------------------*/
        for(unsigned int i = 0; i < GB_FUSION2_DEVICE_COUNT; i++)
        {
            if(gb_fusion2_device_list[i]->name == name)
            {
                /*---------------------------------------------------------*\
                | Set device ID                                             |
                \*---------------------------------------------------------*/
                device_index = i;
                layout = const_cast<gb_fusion2_device*>(gb_fusion2_device_list[i]);
                break;
            }
        }
    }

    /*---------------------------------------------------------*\
    | Iterate through layout and process each zone              |
    \*---------------------------------------------------------*/
    for(uint8_t zone_idx = 0; zone_idx < GB_FUSION2_ZONES_MAX; zone_idx++)
    {
        if(!layout->zones[0][zone_idx])
        {
            continue;
        }
        const gb_fusion2_zone* zone_at_idx = layout->zones[0][zone_idx];

        zone new_zone;
        new_zone.name               = zone_at_idx->name;
        new_zone.leds_min           = zone_at_idx->leds_min;
        new_zone.leds_max           = zone_at_idx->leds_max;
        new_zone.leds_count         = new_zone.leds_min;
        new_zone.type               = (new_zone.leds_min == new_zone.leds_max) ? ZONE_TYPE_SINGLE : ZONE_TYPE_LINEAR;
        new_zone.matrix_map         = NULL;
        zones.emplace_back(new_zone);
    }
}

void RGBController_RGBFusion2USB::SetupZones()
{
    /*---------------------------------------------------------*\
    | Clear any existing color/LED configuration                |
    \*---------------------------------------------------------*/
    leds.clear();
    colors.clear();

    unsigned int d1 = 0, d2 = 0, d3 = 0, d4 = 0;

    /*---------------------------------------------------------*\
    | Set up zones (Fixed so as to not spam the controller)     |
    \*---------------------------------------------------------*/

    for(uint8_t zone_idx = 0; zone_idx < GB_FUSION2_ZONES_MAX; zone_idx++)
    {
        const gb_fusion2_zone* zone_at_idx = gb_fusion2_device_list[device_index]->zones[0][zone_idx];
        if(!zone_at_idx)
        {
            continue;
        }
        bool single_zone = (zone_at_idx->leds_min == zone_at_idx->leds_max);

        if(!single_zone)
        {
            switch(zone_at_idx->idx)
            {
                case LED4:
                case HDR_D_LED2:
                    d2 = zones[zone_idx].leds_count;
                    break;
                case HDR_D_LED3:
                    d3 = zones[zone_idx].leds_count;
                    break;
                case HDR_D_LED4:
                    d4 = zones[zone_idx].leds_count;
                    break;
                default:
                    d1 = zones[zone_idx].leds_count;
                    break;
            }
        }

        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;

            new_led.name  = zone_at_idx->name;
            new_led.value = zone_at_idx->idx;

            if(!single_zone)
            {
                new_led.name.append(" LED " + std::to_string(led_idx));
            }

            leds.push_back(new_led);
        }
    }

    controller->SetLedCount(d1, d2, d3, d4);
    controller->SetStripBuiltinEffectState(-1, false);
    SetupColors();
}

void RGBController_RGBFusion2USB::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_RGBFusion2USB::DeviceUpdateLEDs()
{
    int         mode_value  = (modes[active_mode].value);
    bool        random      = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    uint32_t*   color       = &null_color;

    /*---------------------------------------------------------*\
    | If Wave 1-4 then use special sequence.                    |
    \*---------------------------------------------------------*/
    if(mode_value == 6 || (mode_value >= 9 && mode_value <= 12))
    {
        controller->SetStripBuiltinEffectState(-1, true);
        controller->SetLEDEffect(-1, 1, 0, 0xFF, 0, color);
        controller->ApplyEffect();
        controller->SetLEDEffect( 2, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, color);
        controller->ApplyEffect();
        return;
    }

    for(int zone_idx = 0; zone_idx < (int)zones.size(); zone_idx++)
    {
        if(zones[zone_idx].type == ZONE_TYPE_SINGLE)
        {
            for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
            {
                /*---------------------------------------------------------*\
                | Motherboard LEDs always use effect mode, so use static for|
                | direct mode but get colors from zone                      |
                \*---------------------------------------------------------*/
                if(modes[active_mode].value == 0xFFFF)
                {
                    color       = &zones[zone_idx].colors[led_idx];
                    mode_value  = EFFECT_STATIC;
                }
                /*---------------------------------------------------------*\
                | If the mode uses mode-specific color, get color from mode |
                \*---------------------------------------------------------*/
                else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
                {
                    color       = &modes[active_mode].colors[0];
                }

                /*---------------------------------------------------------*\
                | Apply the mode and color to the zone                      |
                \*---------------------------------------------------------*/
                controller->SetLEDEffect(zones[zone_idx].leds[led_idx].value, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, color);
            }
        }
        /*---------------------------------------------------------*\
        | Set strip LEDs                                            |
        \*---------------------------------------------------------*/
        else
        {
            if(zones[zone_idx].leds && zones[zone_idx].leds_count)
            {
                unsigned char hdr = zones[zone_idx].leds->value;

                /*---------------------------------------------------------*\
                | Direct mode addresses a different register                |
                \*---------------------------------------------------------*/
                if(modes[active_mode].value == 0xFFFF)
                {
                    controller->SetStripBuiltinEffectState(hdr, false);
                    controller->SetStripColors(hdr, zones[zone_idx].colors, zones[zone_idx].leds_count);
                }

                /*---------------------------------------------------------*\
                | Effect mode                                               |
                \*---------------------------------------------------------*/
                else
                {
                    /*---------------------------------------------------------*\
                    | If mode has mode specific color, load color from mode     |
                    \*---------------------------------------------------------*/
                    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
                    {
                        color       = &modes[active_mode].colors[0];
                    }

                    /*---------------------------------------------------------*\
                    | Apply hardware effects to LED strips                      |
                    \*---------------------------------------------------------*/
                    controller->SetStripBuiltinEffectState(hdr, true);
                    controller->SetLEDEffect(hdr, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, color);
                }
            }
        }
    }
    controller->ApplyEffect();
}

void RGBController_RGBFusion2USB::UpdateZoneLEDs(int zone)
{
    /*---------------------------------------------------------*\
    | Get mode parameters                                       |
    \*---------------------------------------------------------*/
    int         mode_value  = (modes[active_mode].value);
    bool        random      = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    uint32_t*   color       = &null_color;

    /*---------------------------------------------------------*\
    | If Wave 1-4 then use special sequence.                    |
    \*---------------------------------------------------------*/
    if(mode_value == 6 || (mode_value >= 9 && mode_value <= 12))
    {
        controller->SetStripBuiltinEffectState(-1, true);
        controller->SetLEDEffect(-1, 1, 0, 0xFF, 0, color);
        controller->ApplyEffect();
        controller->SetLEDEffect( 2, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, color);
        controller->ApplyEffect();
        return;
    }

    /*---------------------------------------------------------*\
    | Set motherboard LEDs                                      |
    \*---------------------------------------------------------*/
    if(zones[zone].type == ZONE_TYPE_SINGLE)
    {
        for(std::size_t led_idx = 0; led_idx < zones[zone].leds_count; led_idx++)
        {
            /*------------------------------------------------------------*\
            | Motherboard LEDs always use effect mode, so use static for   |
            | direct mode but get colors from zone                         |
            \*------------------------------------------------------------*/
            if(mode_value == 0xFFFF)
            {
                color       = &zones[zone].colors[led_idx];
                mode_value  = EFFECT_STATIC;
            }

            /*---------------------------------------------------------*\
            | If the mode uses mode-specific color, get color from mode |
            \*---------------------------------------------------------*/
            else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                color       = &modes[active_mode].colors[0];
            }

            /*---------------------------------------------------------*\
            | Apply the mode and color to the zone                      |
            \*---------------------------------------------------------*/
            controller->SetLEDEffect(zones[zone].leds[led_idx].value, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, color);
            controller->ApplyEffect();
        }
    }

    /*---------------------------------------------------------*\
    | Set strip LEDs                                            |
    \*---------------------------------------------------------*/
    else
    {
        if(zones[zone].leds && zones[zone].leds_count)
        {
            unsigned char hdr = zones[zone].leds->value;

            /*---------------------------------------------------------*\
            | Direct mode addresses a different register                |
            \*---------------------------------------------------------*/
            if(mode_value == 0xFFFF)
            {
                controller->SetStripBuiltinEffectState(hdr, false);
                controller->SetStripColors(hdr, zones[zone].colors, zones[zone].leds_count);
            }

            /*---------------------------------------------------------*\
            | Effect mode                                               |
            \*---------------------------------------------------------*/
            else
            {
                /*---------------------------------------------------------*\
                | If mode has mode specific color, load color from mode     |
                \*---------------------------------------------------------*/
                if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
                {
                    color       = &modes[active_mode].colors[0];
                }

                /*---------------------------------------------------------*\
                | Apply built-in effects to LED strips                      |
                \*---------------------------------------------------------*/
                controller->SetStripBuiltinEffectState(hdr, true);
                controller->SetLEDEffect(hdr, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, color);
                controller->ApplyEffect();
            }
        }
    }
}

void RGBController_RGBFusion2USB::UpdateSingleLED(int led)
{
    /*---------------------------------------------------------*\
    | Get mode parameters                                       |
    \*---------------------------------------------------------*/
    int         mode_value  = (modes[active_mode].value);
    bool        random      = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    uint32_t*   color       = &null_color;

    /*---------------------------------------------------------*\
    | If Wave 1-4 then use special sequence.                    |
    \*---------------------------------------------------------*/
    if(mode_value == 6 || (mode_value >= 9 && mode_value <= 12))
    {
        controller->SetStripBuiltinEffectState(-1, true);
        controller->SetLEDEffect(-1, 1, 0, 0xFF, 0, color);
        controller->ApplyEffect();
        controller->SetLEDEffect( 2, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, color);
        controller->ApplyEffect();
        return;
    }
    unsigned int    zone_idx    = GetLED_Zone(led);

    /*---------------------------------------------------------*\
    | Set motherboard LEDs                                      |
    \*---------------------------------------------------------*/
    if(zones[zone_idx].type == ZONE_TYPE_SINGLE)
    {
        /*---------------------------------------------------------*\
        | Motherboard LEDs always use effect mode, so use static for|
        | direct mode but get colors from zone                      |
        \*---------------------------------------------------------*/
        if(mode_value == 0xFFFF)
        {
            color       = &colors[led];
            mode_value  = EFFECT_STATIC;
        }

        /*---------------------------------------------------------*\
        | If the mode uses mode-specific color, get color from mode |
        \*---------------------------------------------------------*/
        else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            color       = &modes[active_mode].colors[0];
        }

        controller->SetLEDEffect(leds[led].value, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, color);
        controller->ApplyEffect();
    }

    /*---------------------------------------------------------*\
    | Set strip LEDs                                            |
    \*---------------------------------------------------------*/
    else
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_RGBFusion2USB::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

int RGBController_RGBFusion2USB::GetLED_Zone(int led_idx)
{
    for(int zone_idx = 0; zone_idx < (int)zones.size(); zone_idx++)
    {
        int zone_start  = zones[zone_idx].start_idx;
        int zone_end    = zone_start + zones[zone_idx].leds_count - 1;

        if((zone_start <= led_idx) && (zone_end >= led_idx))
        {
            return(zone_idx);
        }
    }

    /*---------------------------------------------------------*\
    | If zone is not found, return -1                           |
    \*---------------------------------------------------------*/
    return(-1);
}

/*---------------------------------------------------------*\
| Convert calibration data to JSON                          |
\*---------------------------------------------------------*/
nlohmann::json RGBController_RGBFusion2USB::WriteCalJsonFrom(const EncodedCalibration& src)
{
    nlohmann::json calib_json;
    calib_json["HDR_D_LED1"]    = src.dled[0];
    calib_json["HDR_D_LED2"]    = src.dled[1];
    calib_json["HDR_D_LED3"]    = src.dled[2];
    calib_json["HDR_D_LED4"]    = src.dled[3];
    calib_json["Mainboard"]     = src.mainboard;
    calib_json["Spare0"]        = src.spare[0];
    calib_json["Spare1"]        = src.spare[1];
    calib_json["Spare2"]        = src.spare[2];
    calib_json["Spare3"]        = src.spare[3];

    return calib_json;
}

/*---------------------------------------------------------*\
| Fill missing JSON calibration keys                        |
\*---------------------------------------------------------*/
void RGBController_RGBFusion2USB::FillMissingWith(nlohmann::json& dst, const EncodedCalibration& fb)
{
    struct SetIfMissing
    {
        nlohmann::json& dst;

        void operator()(const char* key, const std::string& val) const
        {
            if(!dst.contains(key))
            {
                dst[key] = val;
            }
        }
    };

    SetIfMissing set_if_missing{dst};

    set_if_missing("HDR_D_LED1", fb.dled[0]);
    set_if_missing("HDR_D_LED2", fb.dled[1]);
    set_if_missing("Mainboard", fb.mainboard);
    set_if_missing("Spare0",    fb.spare[0]);
    set_if_missing("Spare1",    fb.spare[1]);

    if(controller->GetProductID() == 0x5711)
    {
        set_if_missing("HDR_D_LED3", fb.dled[2]);
        set_if_missing("HDR_D_LED4", fb.dled[3]);
        set_if_missing("Spare2", fb.spare[2]);
        set_if_missing("Spare3", fb.spare[3]);
    }
}

/*---------------------------------------------------------*\
| Build custom layout in JSON                               |
\*---------------------------------------------------------*/
nlohmann::json RGBController_RGBFusion2USB::BuildCustomLayoutJson(
        const gb_fusion2_device* layout,
        const RvrseLedHeaders& reverseLookup)
{
    nlohmann::json json_custom;
    for(uint8_t zone_idx = 0; zone_idx < GB_FUSION2_ZONES_MAX; zone_idx++)
    {
        if(!layout->zones[0][zone_idx])
        {
            continue;
        }

        nlohmann::json json_zone;
        json_zone["name"]           = layout->zones[0][zone_idx]->name;
        json_zone["header"]         = reverseLookup.at(layout->zones[0][zone_idx]->idx);
        json_zone["leds_min"]       = layout->zones[0][zone_idx]->leds_min;
        json_zone["leds_max"]       = layout->zones[0][zone_idx]->leds_max;

        json_custom[layout->name].push_back(json_zone);
    }
    return json_custom;
}

/*---------------------------------------------------------*\
| Build custom layout from JSON                             |
\*---------------------------------------------------------*/
void RGBController_RGBFusion2USB::LoadCustomLayoutFromJson(
        const nlohmann::json& json_custom,
        const FwdLedHeaders& forwardLookup,
        gb_fusion2_device* layout)
{
    for(uint8_t zone_idx = 0; zone_idx < GB_FUSION2_ZONES_MAX; zone_idx++)
    {
        /*---------------------------------------------------------*\
        | Check if there are more JSON objects to parse             |
        \*---------------------------------------------------------*/
        if(json_custom[layout->name].size() <= zone_idx)
        {
            layout->zones[0][zone_idx] = nullptr;
            continue;
        }
        nlohmann::json json_zone    = json_custom[layout->name].at(zone_idx);
        gb_fusion2_zone* new_zone   = new gb_fusion2_zone();

        new_zone->name              = json_zone["name"].get<std::string>();
        std::string header          = json_zone["header"].get<std::string>();
        new_zone->idx               = forwardLookup.at(header);
        if(    header == "HDR_D_LED1"
            || header == "HDR_D_LED2"
            || header == "HDR_D_LED3"
            || header == "HDR_D_LED4")
        {
            new_zone->leds_min      = std::max(json_zone["leds_min"].get<int>(), 1);
            new_zone->leds_max      = std::min(json_zone["leds_max"].get<int>(), 1024);
        }
        else
        {
            new_zone->leds_min      = 1;
            new_zone->leds_max      = 1;
        }

        /*---------------------------------------------------------*\
        | Check for valid values from JSON                          |
        \*---------------------------------------------------------*/
        if(new_zone->name != ""
           && new_zone->leds_min <= new_zone->leds_max
           && new_zone->idx >= GB_FUSION2_LED_IDX::LED1
           && new_zone->idx <= GB_FUSION2_LED_IDX::LED11)
        {
            layout->zones[0][zone_idx]  = new_zone;
        }
        else
        {
            LOG_ERROR("[%s] Error creating zone %d: Validation failed for %s @ index %d (LEDs min %d to %d max)",
                      controller->GetDeviceName().c_str(),
                      zone_idx,
                      new_zone->name.c_str(),
                      new_zone->idx,
                      new_zone->leds_min,
                      new_zone->leds_max);
        }
    }
}

