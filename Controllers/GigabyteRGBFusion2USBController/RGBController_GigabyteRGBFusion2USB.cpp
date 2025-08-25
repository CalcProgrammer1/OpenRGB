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
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteRGBFusion2USB.h"
#include "RGBController_GigabyteRGBFusion2USBBoards.h"
#include "RGBController_GigabyteRGBFusion2USBLayouts.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

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

/*---------------------------------------------------------*\
| Convert calibration data to JSON                          |
\*---------------------------------------------------------*/
static nlohmann::json WriteCalJsonFrom(const EncodedCalibration& src, uint16_t pid)
{
    nlohmann::json calib_json;
    calib_json["HDR_D_LED1"] = src.dled[0];
    calib_json["HDR_D_LED2"] = src.dled[1];

    if(pid == 0x5711)
    {
        calib_json["HDR_D_LED3"] = src.dled[2];
        calib_json["HDR_D_LED4"] = src.dled[3];
    }
    calib_json["Mainboard"] = src.mainboard;
    calib_json["Spare0"]    = src.spare[0];
    calib_json["Spare1"]    = src.spare[1];

    if(pid == 0x5711)
    {
        calib_json["Spare2"] = src.spare[2];
        calib_json["Spare3"] = src.spare[3];
    }
    return calib_json;
}

/*---------------------------------------------------------*\
| Fill missing JSON calibration keys                        |
\*---------------------------------------------------------*/
static void FillMissingWith(nlohmann::json& dst, const EncodedCalibration& fb, uint16_t pid)
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
    if(pid==0x5711)
    {
        set_if_missing("HDR_D_LED3", fb.dled[2]);
        set_if_missing("HDR_D_LED4", fb.dled[3]);
    }
    set_if_missing("Mainboard", fb.mainboard);
    set_if_missing("Spare0",    fb.spare[0]);
    set_if_missing("Spare1",    fb.spare[1]);
    if(pid==0x5711)
    {
        set_if_missing("Spare2", fb.spare[2]);
        set_if_missing("Spare3", fb.spare[3]);
    }
}

/*---------------------------------------------------------*\
| JSON safe string fetch (calibration)                      |
\*---------------------------------------------------------*/
static std::string GetOrOff(const nlohmann::json& obj, const char* key)
{
    return obj.contains(key) ? obj.at(key).get<std::string>() : std::string("OFF");
}

/*---------------------------------------------------------*\
| Build custom layout in JSON                               |
\*---------------------------------------------------------*/
static nlohmann::json BuildCustomLayoutJson(const ZoneLeds& src_layout, const RvrseLedHeaders& reverseLookup)
{
    nlohmann::json json_HCL;
    for(ZoneLeds::const_iterator zl = src_layout.begin(); zl != src_layout.end(); ++zl)
    {
        const std::string& zone_name = zl->first;
        const std::vector<LedPort>& v_lp = zl->second;

        nlohmann::json json_zl;
        for(std::vector<LedPort>::const_iterator it = v_lp.begin(); it != v_lp.end(); ++it)
        {
            const LedPort& lp = *it;
            nlohmann::json json_lp;
            json_lp["name"]   = lp.name;
            json_lp["header"] = reverseLookup.at(lp.header);
            json_lp["count"]  = lp.count;
            json_zl.push_back(json_lp);
        }
        json_HCL.emplace(zone_name, json_zl);
    }
    return json_HCL;
}

/*---------------------------------------------------------*\
| Build custom layout from JSON                             |
\*---------------------------------------------------------*/
static void LoadCustomLayoutFromJson(const nlohmann::json& json_HCL, const FwdLedHeaders& forwardLookup, ZoneLeds& out_layout)
{
    out_layout.clear();

    for(nlohmann::json::const_iterator json_layout_it = json_HCL.begin();
        json_layout_it != json_HCL.end();
        ++json_layout_it)
    {
        const std::string&    zone_name = json_layout_it.key();
        const nlohmann::json& json_zl   = json_layout_it.value();

        std::vector<LedPort> v_lp;

        for(nlohmann::json::const_iterator zl_it = json_zl.begin();
            zl_it != json_zl.end();
            ++zl_it)
        {
            const nlohmann::json& zl = *zl_it;
            nlohmann::json jv        = zl;
            LedPort        lp;

            lp.name   = jv["name"].get<std::string>();
            lp.header = forwardLookup.at(jv["header"].get<std::string>());
            lp.count  = jv.contains("count") ? jv["count"].get<int>() : 1;

            v_lp.push_back(lp);
        }

        out_layout.insert(std::pair<std::string, std::vector<LedPort>>(zone_name, v_lp));
    }
}

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
    pid                         = controller->GetProductID();
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

    Load_Device_Config();
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
void RGBController_RGBFusion2USB::Load_Device_Config()
{
    const std::string SectionCustom      = "CustomLayout";
    const std::string SectionCalibration = "Calibration";
    RvrseLedHeaders ReverseLedLookup     = reverse_map(LedLookup);
    SettingsManager* settings_manager    = ResourceManager::get()->GetSettingsManager();
    nlohmann::json device_settings       = settings_manager->GetSettings(detector_name);

    if(pid == 0x5711)
    {
        layout = HardcodedCustom_Gen2.find("Custom")->second;
    }
    else
    {
        layout = HardcodedCustom_Gen1.find("Custom")->second;
    }

    /*---------------------------------------------------------*\
    | Remove legacy top-level "MotherboardLayouts"              |
    \*---------------------------------------------------------*/
    if(device_settings.contains("MotherboardLayouts"))
    {
        device_settings.erase("MotherboardLayouts");
        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
    }

    if(!device_settings.contains(SectionCustom))
    {
        nlohmann::json json_HCL = BuildCustomLayoutJson(layout, ReverseLedLookup);
        device_settings[SectionCustom]["Enabled"] = false;
        device_settings[SectionCustom]["Data"] = BuildCustomLayoutJson(layout, ReverseLedLookup);
        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
    }

    custom_layout = device_settings[SectionCustom]["Enabled"];

    if(custom_layout)
    {
        const nlohmann::json json_HCL = device_settings[SectionCustom]["Data"];
        LoadCustomLayoutFromJson(json_HCL, LedLookup, layout);
    }

    EncodedCalibration hw_cal = controller->GetCalibration(false);

    if(!device_settings.contains(SectionCalibration))
    {
        device_settings[SectionCalibration]["Enabled"] = false;
        device_settings[SectionCalibration]["Data"]    = WriteCalJsonFrom(hw_cal, pid);
        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
    }
    else
    {
        nlohmann::json& cal_sec = device_settings[SectionCalibration];
        bool cal_enable         = cal_sec.value("Enabled", false);

        if(!cal_sec.contains("Data") || !cal_sec["Data"].is_object())
        {
            cal_sec["Data"] = WriteCalJsonFrom(hw_cal, pid);
            settings_manager->SetSettings(detector_name, device_settings);
            settings_manager->SaveSettings();
        }
        else
        {
            nlohmann::json& cdata = cal_sec["Data"];
            FillMissingWith(cdata, hw_cal, pid);

            if(!cal_enable)
            {
                cal_sec["Data"] = WriteCalJsonFrom(hw_cal, pid);
                settings_manager->SetSettings(detector_name, device_settings);
                settings_manager->SaveSettings();
            }
        }

        if(cal_enable)
        {
            const nlohmann::json& cdata = cal_sec["Data"];

            EncodedCalibration desired;
            desired.dled[0]   = GetOrOff(cdata, "HDR_D_LED1");
            desired.dled[1]   = GetOrOff(cdata, "HDR_D_LED2");
            desired.mainboard = GetOrOff(cdata, "Mainboard");
            desired.spare[0]  = GetOrOff(cdata, "Spare0");
            desired.spare[1]  = GetOrOff(cdata, "Spare1");

            if(pid == 0x5711)
            {
                desired.dled[2]  = GetOrOff(cdata, "HDR_D_LED3");
                desired.dled[3]  = GetOrOff(cdata, "HDR_D_LED4");
                desired.spare[2] = GetOrOff(cdata, "Spare2");
                desired.spare[3] = GetOrOff(cdata, "Spare3");
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
}

/*---------------------------------------------------------*\
| Loads layout and zone data for controller                 |
\*---------------------------------------------------------*/
void RGBController_RGBFusion2USB::Init_Controller()
{
    /*---------------------------------------------------------*\
    | Look up channel map based on device name                  |
    \*---------------------------------------------------------*/
    if(!custom_layout)
    {
        std::string layout_key;

        bool found = false;

        switch(pid)
        {
            case 0x5711:
                if(MBName2LayoutLookup5711.count(name))
                {
                    layout_key = MBName2LayoutLookup5711.at(name);
                    found = true;
                }
                break;

            case 0x5702:
                if(MBName2LayoutLookup5702.count(name))
                {
                    layout_key = MBName2LayoutLookup5702.at(name);
                    found = true;
                }
                break;

            case 0x8950:
                if(MBName2LayoutLookup8950.count(name))
                {
                    layout_key = MBName2LayoutLookup8950.at(name);
                    found = true;
                }
                break;

            case 0x8297:
                if(MBName2LayoutLookup8297.count(name))
                {
                    layout_key = MBName2LayoutLookup8297.at(name);
                    found = true;
                }
                break;

            default:
                break;
        }

        if(found)
        {
            layout = knownLayoutsLookup.at(layout_key);
        }
        else
        {
            switch(pid)
            {
                case 0x8297:
                case 0x8950:
                case 0x5702:
                    layout = knownLayoutsLookup.at("STD_ATX");
                    break;

                default:
                    layout = knownLayoutsLookup.at("IT5711-Generic");
                    break;
            }
        }
    }

    /*---------------------------------------------------------*\
    | Initialize the number of zones from the layout            |
    \*---------------------------------------------------------*/
    zones.resize(layout.size());

    /*---------------------------------------------------------*\
    | Iterate through layout and process each zone              |
    \*---------------------------------------------------------*/
    int zone_idx = 0;
    for(ZoneLeds::iterator zl = layout.begin(); zl != layout.end(); zl++)
    {
        std::vector<LedPort> lp     = zl->second;
        int LED_count               = 0;
        bool single_zone            = true;

        for(std::size_t lp_idx = 0; lp_idx < lp.size(); lp_idx++)
        {
            int lp_count            = lp[lp_idx].count;
            single_zone             = single_zone && (lp_count == 1);
            LED_count              += lp_count;
        }

        zones[zone_idx].name        = zl->first;
        zones[zone_idx].leds_min    = (single_zone) ? LED_count : RGBFUSION2_DIGITAL_LEDS_MIN;
        zones[zone_idx].leds_max    = (single_zone) ? LED_count : RGBFUSION2_DIGITAL_LEDS_MAX;
        zones[zone_idx].leds_count  = (single_zone) ? LED_count : 0;
        zones[zone_idx].type        = (single_zone) ? ZONE_TYPE_SINGLE : ZONE_TYPE_LINEAR;
        zones[zone_idx].matrix_map  = NULL;
        zone_idx++;
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
    int zone_idx = 0;
    for (ZoneLeds::iterator zl = layout.begin(); zl != layout.end(); zl++)
    {
        bool single_zone = (zones[zone_idx].type == ZONE_TYPE_SINGLE);

        if (!single_zone)
        {
            unsigned char hdr = zl->second.at(0).header;

            switch (hdr)
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

        for (unsigned int lp_idx = 0; lp_idx < zones[zone_idx].leds_count; lp_idx++)
        {
            led new_led;

            if (single_zone)
            {
                new_led.name  = zl->second.at(lp_idx).name;
                new_led.value = zl->second.at(lp_idx).header;
            }
            else
            {
                new_led.name  = zl->second.at(0).name;
                new_led.name.append(" LED " + std::to_string(lp_idx));
                new_led.value = zl->second.at(0).header;
            }

            leds.push_back(new_led);
        }

        zone_idx++;
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
