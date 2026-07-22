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
        Intel mainboards from the X570 and z390 chipsets onwards.
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
    product_id                  = controller->GetProductID();
    device_num                  = controller->GetDeviceNum();


    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0xFFFF;
    Direct.flags                = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.brightness_min       = RGBFUSION2_BRIGHTNESS_MIN;
    Direct.brightness_max       = RGBFUSION2_BRIGHTNESS_MAX;
    Direct.brightness           = RGBFUSION2_BRIGHTNESS_MAX;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = EFFECT_STATIC;
    Static.flags                = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                | MODE_FLAG_MANUAL_SAVE;
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
    Breathing.flags             = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                | MODE_FLAG_HAS_RANDOM_COLOR
                                | MODE_FLAG_MANUAL_SAVE;
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
    Blinking.flags              = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                | MODE_FLAG_HAS_RANDOM_COLOR
                                | MODE_FLAG_MANUAL_SAVE;
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
    ColorCycle.flags            = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_MANUAL_SAVE;
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
    Flashing.flags              = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                | MODE_FLAG_HAS_RANDOM_COLOR
                                | MODE_FLAG_MANUAL_SAVE;
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
    Wave.flags                  = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE
                                | MODE_FLAG_MANUAL_SAVE;
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
    Random.flags                = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_MANUAL_SAVE;
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
    Wave1.flags                 = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE
                                | MODE_FLAG_MANUAL_SAVE;
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
    Wave2.flags                 = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE
                                | MODE_FLAG_MANUAL_SAVE;
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
    Wave3.flags                 = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE
                                | MODE_FLAG_MANUAL_SAVE;
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
    Wave4.flags                 = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE
                                | MODE_FLAG_MANUAL_SAVE;
    Wave4.brightness_min        = RGBFUSION2_BRIGHTNESS_MIN;
    Wave4.brightness_max        = RGBFUSION2_BRIGHTNESS_MAX;
    Wave4.brightness            = RGBFUSION2_BRIGHTNESS_MAX;
    Wave4.colors_min            = 0;
    Wave4.colors_max            = 0;
    Wave4.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Wave4);

    if(!controller->SupportsSaveLEDState())
    {
        for(unsigned int mode_idx = 0; mode_idx < modes.size(); mode_idx++)
        {
            modes[mode_idx].flags &= ~MODE_FLAG_MANUAL_SAVE;
        }
    }

    Init_Controller();
    SetupZones();
}

RGBController_RGBFusion2USB::~RGBController_RGBFusion2USB()
{
    // Free any zones we allocated for the per-instance layout
    for(gb_fusion2_zone* z : allocated_zones)
    {
        delete z;
    }
    allocated_zones.clear();

    Shutdown();

    delete controller;
}

/*---------------------------------------------------------*\
| Loads JSON config data                                    |
\*---------------------------------------------------------*/
void RGBController_RGBFusion2USB::Init_Controller()
{
    bool settings_changed                = false;
    const gb_fusion2_device* src_layout  = gb_fusion2_device_list[device_index];
    const std::string SectionGen2        = "Gigabyte-Gen2-ARGB";
    const std::string SectionCustomBase  = "CustomLayout";
    const std::string SectionCustom      = SectionCustomBase + std::to_string(device_num);
    const std::string SectionCalibration = "Calibration";
    RvrseLedHeaders ReverseLedLookup     = reverse_map(LedLookup);
    SettingsManager* settings_manager    = ResourceManager::get()->GetSettingsManager();
    nlohmann::json device_settings       = settings_manager->GetSettings(detector_name);

    /*---------------------------------------------------------*\
    | Checks for Gen2 support and adds flag to json.            |
    \*---------------------------------------------------------*/
    if(controller->SupportsGen2())
    {
        static const char* gen2_header_keys[4] =
        {
            "D_LED1",
            "D_LED2",
            "D_LED3",
            "D_LED4"
        };

        const unsigned int header_count = controller->ExportGen2Strips().size();

        bool default_enabled = false;
        uint8_t enabled_headers = 0;

        /*-----------------------------------------------------*\
        | Preserve the old global setting when migrating        |
        \*-----------------------------------------------------*/
        if(device_settings[SectionGen2].contains("Enabled"))
        {
            default_enabled = device_settings[SectionGen2]["Enabled"];

            device_settings[SectionGen2].erase("Enabled");
            settings_changed = true;
        }

        /*-----------------------------------------------------*\
        | Create and read the per-header settings               |
        \*-----------------------------------------------------*/
        for(unsigned int header = 0; header < header_count; ++header)
        {
            if(!device_settings[SectionGen2].contains(gen2_header_keys[header]))
            {
                device_settings[SectionGen2][gen2_header_keys[header]] = default_enabled;
                settings_changed = true;
            }

            if(device_settings[SectionGen2][gen2_header_keys[header]])
            {
                enabled_headers |= 1U << header;
            }
        }

        supports_gen2 = enabled_headers != 0;

        if(supports_gen2)
        {
            controller->ScanGen2Strips(enabled_headers);
        }
    }
    else if(device_settings.contains(SectionGen2))
    {
        device_settings.erase(SectionGen2);
        settings_changed = true;
    }

    if(settings_changed)
    {
        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
    }

    /*---------------------------------------------------------*\
    | Create the custom layout from the generic layout          |
    \*---------------------------------------------------------*/
    if(device_num == 1)
    {
        switch(product_id)
        {
                break;
            case 0x5711:
                    src_layout = gb_fusion2_device_list[device_index + 4];
                break;
            default:
                    src_layout = gb_fusion2_device_list[device_index + 1];
                break;
        }
    }
    else
    {
        switch(product_id)
        {
            case 0x8950:
                    src_layout = gb_fusion2_device_list[device_index + 2];
                break;
            case 0x5711:
                    src_layout = gb_fusion2_device_list[device_index + 3];
                break;
            case 0xa100:
                    src_layout = gb_fusion2_device_list[device_index + 5];
                break;
            default:
                break;
        }
    }

    if(!device_settings.contains(SectionCustom) && (product_id != 0xA100))
    {
        device_settings[SectionCustom]["Enabled"]   = false;
        device_settings[SectionCustom]["Data"]      = BuildCustomLayoutJson(src_layout, ReverseLedLookup);
        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
    }

    bool custom_layout = device_settings[SectionCustom]["Enabled"];

    EncodedCalibration hw_cal = controller->GetCalibration(false);

    if(device_num == 0 || product_id != 0xa100)
    {
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
            if(gb_fusion2_device_list[i]->name == name &&
                gb_fusion2_device_list[i]->device_num == device_num)
            {
                /*---------------------------------------------------------*\
                | Set device ID                                             |
                \*---------------------------------------------------------*/
                device_index = i;
                src_layout = gb_fusion2_device_list[i];
                break;
            }
        }
    }
    /*---------------------------------------------------------------------*\
    |  Creates per instance copy of layouts.                                |
    \*---------------------------------------------------------------------*/
    instance_layout.zones      = &instance_zones;
    instance_layout.layout_id  = src_layout->layout_id;
    instance_layout.device_num = src_layout->device_num;
    instance_layout.name       = src_layout->name;

    for(uint8_t zi = 0; zi < GB_FUSION2_ZONES_MAX; ++zi)
    {
        (*instance_layout.zones)[zi] = (*src_layout->zones)[zi];
    }

    if(custom_layout && (product_id != 0xA100))
    {
        LoadCustomLayoutFromJson(device_settings[SectionCustom]["Data"], LedLookup, &instance_layout);
    }
    /*---------------------------------------------------------------------*\
    | Culls the mode support based on layout_id.                            |
    \*---------------------------------------------------------------------*/
    const uint32_t effect_mask = instance_layout.layout_id & GB_EFF_CORE_MASK;
    modes.erase(std::remove_if(modes.begin(), modes.end(),
        [effect_mask](const mode& m)
        {
            if(m.value == 0xFFFF /* Direct */) { return false; }
            if(m.value == EFFECT_STATIC)        { return false; }

            uint32_t bit = 0u;
            switch(m.value)
            {
                    case EFFECT_PULSE:       bit = GB_EFF_BREATH; break;
                    case EFFECT_COLORCYCLE:  bit = GB_EFF_CYCLE;  break;
                    case EFFECT_BLINKING:    bit = GB_EFF_FLASH;  break;
                    case EFFECT_RANDOM:      bit = GB_EFF_RANDOM; break;
                    case EFFECT_WAVE:        bit = GB_EFF_WAVE;   break;
                    case EFFECT_DFLASH:      bit = GB_EFF_DFLASH; break;
                    case EFFECT_WAVE1:       bit = GB_EFF_WAVE1;  break;
                    case EFFECT_WAVE2:       bit = GB_EFF_WAVE2;  break;
                    case EFFECT_WAVE3:       bit = GB_EFF_WAVE1;  break;
                    case EFFECT_WAVE4:       bit = GB_EFF_WAVE2;  break;
                default:                 bit = 0u; break;
            }
            return (bit == 0u) || ((effect_mask & bit) == 0u);
        }),
        modes.end());
}

void RGBController_RGBFusion2USB::SetupZones()
{
    /*-----------------------------------------------------*\
    | Only set LED count on the first run                   |
    \*-----------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }


    /*-----------------------------------------------------*\
    | Clear any existing color/LED configuration            |
    \*-----------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*-----------------------------------------------------*\
    | Count number of zones to resize zones vector          |
    \*-----------------------------------------------------*/
    unsigned int num_zones;

    for(num_zones = 0; num_zones < GB_FUSION2_ZONES_MAX; num_zones++)
    {
        if(!(*instance_layout.zones)[num_zones])
        {
            break;
        }
    }

    zones.resize(num_zones);

    unsigned int d1 = 0, d2 = 0, d3 = 0, d4 = 0;

    /*-----------------------------------------------------*\
    | Retrieve the latest Gen2 scan results                 |
    \*-----------------------------------------------------*/
    std::vector<Gen2StripInfo> strips;

    if(supports_gen2)
    {
        strips = controller->ExportGen2Strips();
    }

    /*-----------------------------------------------------*\
    | Set up zones                                          |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        const gb_fusion2_zone* zone_at_idx = (*instance_layout.zones)[zone_idx];

        if(!zone_at_idx)
        {
            continue;
        }

        bool fixed_zone = (zone_at_idx->leds_min == zone_at_idx->leds_max);

        const Gen2StripInfo* gen2_info = nullptr;

        /*-------------------------------------------------*\
        | Locate the Gen2 result corresponding to this zone |
        \*-------------------------------------------------*/
        if(supports_gen2 && !fixed_zone)
        {
            unsigned int slot = 0;

            switch(zone_at_idx->idx)
            {
                case LED4:
                case HDR_D_LED2:
                    slot = 1;
                    break;
                case HDR_D_LED3:
                    slot = 2;
                    break;
                case HDR_D_LED4:
                    slot = 3;
                    break;
                default:
                    slot = 0;
                    break;
            }

            if(slot < strips.size())
            {
                if(strips[slot].totalLeds > 0)
                {
                    gen2_info = &strips[slot];
                }
            }
        }

        /*-------------------------------------------------*\
        | Automatically detected Gen2 ARGB zone             |
        \*-------------------------------------------------*/
        if(gen2_info != nullptr)
        {
            bool preserve_segments = false;

            /*---------------------------------------------*\
            | A saved segment configuration may only rename |
            | or subdivide each detected physical strip.    |
            \*---------------------------------------------*/
            if(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS)
            {
                bool valid_segments     = true;
                std::size_t segment_idx = 0;

                for(std::size_t strip_idx = 0; strip_idx < gen2_info->LedsOfStrip.size(); strip_idx++)
                {
                    unsigned int strip_leds = gen2_info->LedsOfStrip[strip_idx];

                    /*-------------------------------------*\
                    | Require one group for this strip      |
                    \*-------------------------------------*/
                    if(segment_idx >= zones[zone_idx].segments.size())
                    {
                        valid_segments = false;
                        break;
                    }

                    if(!(zones[zone_idx].segments[segment_idx].flags & SEGMENT_FLAG_GROUP_START))
                    {
                        valid_segments = false;
                        break;
                    }

                    segment_idx++;

                    /*-------------------------------------*\
                    | Validate subdivisions of this strip   |
                    \*-------------------------------------*/
                    unsigned int member_leds = 0;
                    bool found_member        = false;

                    while(segment_idx < zones[zone_idx].segments.size())
                    {
                        const segment& member = zones[zone_idx].segments[segment_idx];

                        if(member.flags & SEGMENT_FLAG_GROUP_START)
                        {
                            break;
                        }

                        if(!(member.flags & SEGMENT_FLAG_GROUP_MEMBER)
                                          || member.type != ZONE_TYPE_LINEAR
                                          || member.leds_count == 0)
                        {
                            valid_segments = false;
                            break;
                        }

                        member_leds += member.leds_count;

                        if(member_leds > strip_leds)
                        {
                            valid_segments = false;
                            break;
                        }

                        found_member = true;
                        segment_idx++;
                    }

                    if(!valid_segments)
                    {
                        break;
                    }

                    if(!found_member || member_leds != strip_leds)
                    {
                        valid_segments = false;
                        break;
                    }
                }

                if(segment_idx != zones[zone_idx].segments.size())
                {
                    valid_segments = false;
                }

                preserve_segments = valid_segments;
            }

            /*---------------------------------------------*\
            | Gen2 scan controls all zone-level geometry    |
            \*---------------------------------------------*/
            zones[zone_idx].name                        = zone_at_idx->name;
            zones[zone_idx].type                        = ZONE_TYPE_SEGMENTED;
            zones[zone_idx].leds_count                  = gen2_info->totalLeds;
            zones[zone_idx].leds_min                    = zones[zone_idx].leds_count;
            zones[zone_idx].leds_max                    = zones[zone_idx].leds_count;
            zones[zone_idx].matrix_map.width            = 0;
            zones[zone_idx].matrix_map.height           = 0;
            zones[zone_idx].matrix_map.map.clear();
            zones[zone_idx].flags                       = ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;

            if(preserve_segments)
            {
                zones[zone_idx].flags                   |= ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS;
            }
            else
            {
                zones[zone_idx].segments.clear();
                zones[zone_idx].segments.reserve(gen2_info->LedsOfStrip.size() * 2);

                unsigned int start_idx = 0;

                for(std::size_t strip_idx = 0; strip_idx < gen2_info->LedsOfStrip.size(); strip_idx++)
                {
                    unsigned int strip_leds = gen2_info->LedsOfStrip[strip_idx];

                    /*-------------------------------------*\
                    | Empty marker for a physical strip     |
                    \*-------------------------------------*/
                    segment group;

                    group.name                          = "Strip " + std::to_string(strip_idx + 1);
                    group.type                          = ZONE_TYPE_LINEAR;
                    group.start_idx                     = 0;
                    group.leds_count                    = 0;
                    group.flags                         = SEGMENT_FLAG_GROUP_START;
                    group.matrix_map.width              = 0;
                    group.matrix_map.height             = 0;
                    group.matrix_map.map.clear();

                    zones[zone_idx].segments.push_back(group);

                    /*-------------------------------------*\
                    | Initial member covers the whole strip |
                    \*-------------------------------------*/
                    segment member;

                    member.name                         = "Strip " + std::to_string(strip_idx + 1);
                    member.type                         = ZONE_TYPE_LINEAR;
                    member.start_idx                    = start_idx;
                    member.leds_count                   = strip_leds;
                    member.flags                        = SEGMENT_FLAG_GROUP_MEMBER;
                    member.matrix_map.width             = 0;
                    member.matrix_map.height            = 0;
                    member.matrix_map.map.clear();
                    zones[zone_idx].segments.push_back(member);
                    start_idx                           += strip_leds;
                }
            }
        }

        /*-------------------------------------------------*\
        | Fixed motherboard zone                            |
        \*-------------------------------------------------*/
        else if(fixed_zone)
        {
            zones[zone_idx].name                        = zone_at_idx->name;
            zones[zone_idx].type                        = ZONE_TYPE_SINGLE;
            zones[zone_idx].leds_min                    = zone_at_idx->leds_min;
            zones[zone_idx].leds_max                    = zone_at_idx->leds_max;
            zones[zone_idx].leds_count                  = zone_at_idx->leds_min;
            zones[zone_idx].flags                       = 0;
            zones[zone_idx].matrix_map.width            = 0;
            zones[zone_idx].matrix_map.height           = 0;
            zones[zone_idx].matrix_map.map.clear();
            zones[zone_idx].segments.clear();
        }

        /*-------------------------------------------------*\
        | Manually configurable ARGB zone                   |
        \*-------------------------------------------------*/
        else
        {
            bool reset_manual_zone = first_run || !(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE);

            if(reset_manual_zone)
            {
                zones[zone_idx].flags                   = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP
                                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
                zones[zone_idx].name                    = zone_at_idx->name;
                zones[zone_idx].type                    = ZONE_TYPE_LINEAR;
                zones[zone_idx].leds_count              = zone_at_idx->leds_min;
                zones[zone_idx].matrix_map.width        = 0;
                zones[zone_idx].matrix_map.height       = 0;
                zones[zone_idx].matrix_map.map.clear();
                zones[zone_idx].segments.clear();
            }

            zones[zone_idx].leds_min                    = zone_at_idx->leds_min;
            zones[zone_idx].leds_max                    = zone_at_idx->leds_max;

            if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
            {
                zones[zone_idx].name                    = zone_at_idx->name;
            }

            if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
            {
                zones[zone_idx].leds_count              = zone_at_idx->leds_min;
            }

            if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
            {
                zones[zone_idx].type                    = ZONE_TYPE_LINEAR;
            }

            if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
            {
                zones[zone_idx].matrix_map.width        = 0;
                zones[zone_idx].matrix_map.height       = 0;
                zones[zone_idx].matrix_map.map.clear();
            }

            if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS))
            {
                zones[zone_idx].segments.clear();
            }
        }

        /*-------------------------------------------------*\
        | Initialize per-zone modes                         |
        \*-------------------------------------------------*/
        if(zones[zone_idx].modes.empty())
        {
            for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
            {
                if(!(modes[mode_idx].flags & MODE_FLAG_REQUIRES_ENTIRE_DEVICE))
                {
                    zones[zone_idx].modes.push_back(modes[mode_idx]);
                }
            }

            zones[zone_idx].active_mode = -1;
        }

        /*-------------------------------------------------*\
        | Initialize LEDs                                   |
        \*-------------------------------------------------*/
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name  = zones[zone_idx].name;
            new_led.value = zone_at_idx->idx;

            if(!fixed_zone)
            {
                new_led.name.append(", LED " + std::to_string(led_idx + 1));
            }

            leds.push_back(new_led);
        }

        /*-------------------------------------------------*\
        | Restore the ARGB header LED counts                |
        \*-------------------------------------------------*/
        if(!fixed_zone)
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
    }

    controller->SetLedCount(d1, d2, d3, d4);
    controller->SetStripBuiltinEffectState(-1, false);
    SetupColors();
}

void RGBController_RGBFusion2USB::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        SetupZones();
    }
}

void RGBController_RGBFusion2USB::DeviceUpdateLEDs()
{
    int         mode_value  = modes[active_mode].value;
    bool        random      = modes[active_mode].color_mode == MODE_COLORS_RANDOM;
    uint32_t*   color       = &null_color;

    /*---------------------------------------------------------*\
    | Entire-device modes use the special wave sequence         |
    \*---------------------------------------------------------*/
    if(modes[active_mode].flags & MODE_FLAG_REQUIRES_ENTIRE_DEVICE)
    {
        controller->SetStripBuiltinEffectState(-1, true);
        controller->SetLEDEffect(-1, EFFECT_STATIC, 0, 0xFF, false, color);
        controller->ApplyEffect();
        controller->SetLEDEffect(2, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, color);
        controller->ApplyEffect();
        return;
    }

    for(int zone_idx = 0; zone_idx < (int)zones.size(); zone_idx++)
    {
        mode* selected_mode = &modes[active_mode];

        /*---------------------------------------------------------*\
        | Use the zone-specific mode when one is selected           |
        \*---------------------------------------------------------*/
        if(zones[zone_idx].active_mode >= 0)
        {
            selected_mode = &zones[zone_idx].modes[zones[zone_idx].active_mode];
        }

        mode_value = selected_mode->value;
        random     = selected_mode->color_mode == MODE_COLORS_RANDOM;
        color      = &null_color;

        if(zones[zone_idx].type == ZONE_TYPE_SINGLE)
        {
            for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
            {
                int         led_mode_value  = mode_value;
                uint32_t*   led_color       = color;

                /*---------------------------------------------------------*\
                | Motherboard LEDs always use effect mode, so use static for|
                | direct mode but get colors from zone                      |
                \*---------------------------------------------------------*/
                if(selected_mode->value == 0xFFFF)
                {
                    led_color       = &zones[zone_idx].colors[led_idx];
                    led_mode_value  = EFFECT_STATIC;
                }
                /*---------------------------------------------------------*\
                | If the mode uses mode-specific color, get color from mode |
                \*---------------------------------------------------------*/
                else if(selected_mode->color_mode == MODE_COLORS_MODE_SPECIFIC)
                {
                    led_color = &selected_mode->colors[0];
                }

                /*---------------------------------------------------------*\
                | Apply the mode and color to the zone                      |
                \*---------------------------------------------------------*/
                controller->SetLEDEffect(zones[zone_idx].leds[led_idx].value, led_mode_value, selected_mode->speed, selected_mode->brightness, random, led_color);
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
                if(selected_mode->value == 0xFFFF)
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
                    if(selected_mode->color_mode == MODE_COLORS_MODE_SPECIFIC)
                    {
                        color = &selected_mode->colors[0];
                    }

                    /*---------------------------------------------------------*\
                    | Apply hardware effects to LED strips                      |
                    \*---------------------------------------------------------*/
                    controller->SetStripBuiltinEffectState(hdr, true);
                    controller->SetLEDEffect(hdr, mode_value, selected_mode->speed, selected_mode->brightness, random, color);
                }
            }
        }
    }

    controller->ApplyEffect();
}

void RGBController_RGBFusion2USB::DeviceUpdateZoneLEDs(int zone)
{
    int         mode_value  = modes[active_mode].value;
    bool        random      = modes[active_mode].color_mode == MODE_COLORS_RANDOM;
    uint32_t*   color       = &null_color;

    /*---------------------------------------------------------*\
    | Entire-device modes cannot be updated one zone at a time  |
    \*---------------------------------------------------------*/
    if(modes[active_mode].flags & MODE_FLAG_REQUIRES_ENTIRE_DEVICE)
    {
        DeviceUpdateLEDs();
        return;
    }

    mode* selected_mode = &modes[active_mode];

    /*---------------------------------------------------------*\
    | Use the zone-specific mode when one is selected           |
    \*---------------------------------------------------------*/
    if(zones[zone].active_mode >= 0)
    {
        selected_mode = &zones[zone].modes[zones[zone].active_mode];
    }

    mode_value = selected_mode->value;
    random     = selected_mode->color_mode == MODE_COLORS_RANDOM;

    /*---------------------------------------------------------*\
    | Set motherboard LEDs                                      |
    \*---------------------------------------------------------*/
    if(zones[zone].type == ZONE_TYPE_SINGLE)
    {
        for(std::size_t led_idx = 0; led_idx < zones[zone].leds_count; led_idx++)
        {
            int         led_mode_value  = mode_value;
            uint32_t*   led_color       = color;

            /*------------------------------------------------------------*\
            | Motherboard LEDs always use effect mode, so use static for   |
            | direct mode but get colors from zone                         |
            \*------------------------------------------------------------*/
            if(selected_mode->value == 0xFFFF)
            {
                led_color       = &zones[zone].colors[led_idx];
                led_mode_value  = EFFECT_STATIC;
            }

            /*---------------------------------------------------------*\
            | If the mode uses mode-specific color, get color from mode |
            \*---------------------------------------------------------*/
            else if(selected_mode->color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                led_color = &selected_mode->colors[0];
            }

            /*---------------------------------------------------------*\
            | Apply the mode and color to the zone                      |
            \*---------------------------------------------------------*/
            controller->SetLEDEffect(zones[zone].leds[led_idx].value, led_mode_value, selected_mode->speed, selected_mode->brightness, random, led_color);
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
            if(selected_mode->value == 0xFFFF)
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
                if(selected_mode->color_mode == MODE_COLORS_MODE_SPECIFIC)
                {
                    color = &selected_mode->colors[0];
                }

                /*---------------------------------------------------------*\
                | Apply built-in effects to LED strips                      |
                \*---------------------------------------------------------*/
                controller->SetStripBuiltinEffectState(hdr, true);
                controller->SetLEDEffect(hdr, mode_value, selected_mode->speed, selected_mode->brightness, random, color);
                controller->ApplyEffect();
            }
        }
    }
}

void RGBController_RGBFusion2USB::DeviceUpdateSingleLED(int led)
{
    int         mode_value  = modes[active_mode].value;
    bool        random      = modes[active_mode].color_mode == MODE_COLORS_RANDOM;
    uint32_t*   color       = &null_color;

    /*---------------------------------------------------------*\
    | Entire-device modes cannot update an individual LED       |
    \*---------------------------------------------------------*/
    if(modes[active_mode].flags & MODE_FLAG_REQUIRES_ENTIRE_DEVICE)
    {
        DeviceUpdateLEDs();
        return;
    }

    int zone_idx = GetLED_Zone(led);

    if(zone_idx < 0)
    {
        return;
    }

    mode* selected_mode = &modes[active_mode];

    /*---------------------------------------------------------*\
    | Use the zone-specific mode when one is selected           |
    \*---------------------------------------------------------*/
    if(zones[zone_idx].active_mode >= 0)
    {
        selected_mode = &zones[zone_idx].modes[zones[zone_idx].active_mode];
    }

    mode_value = selected_mode->value;
    random     = selected_mode->color_mode == MODE_COLORS_RANDOM;

    /*---------------------------------------------------------*\
    | Set motherboard LEDs                                      |
    \*---------------------------------------------------------*/
    if(zones[zone_idx].type == ZONE_TYPE_SINGLE)
    {
        /*---------------------------------------------------------*\
        | Motherboard LEDs always use effect mode, so use static for|
        | direct mode but get colors from zone                      |
        \*---------------------------------------------------------*/
        if(selected_mode->value == 0xFFFF)
        {
            color       = &colors[led];
            mode_value  = EFFECT_STATIC;
        }

        /*---------------------------------------------------------*\
        | If the mode uses mode-specific color, get color from mode |
        \*---------------------------------------------------------*/
        else if(selected_mode->color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            color = &selected_mode->colors[0];
        }

        controller->SetLEDEffect(leds[led].value, mode_value, selected_mode->speed, selected_mode->brightness, random, color);
        controller->ApplyEffect();
    }

    /*---------------------------------------------------------*\
    | Set strip LEDs                                            |
    \*---------------------------------------------------------*/
    else
    {
        DeviceUpdateZoneLEDs(zone_idx);
    }
}

void RGBController_RGBFusion2USB::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusion2USB::DeviceUpdateZoneMode(int zone)
{
    DeviceUpdateZoneLEDs(zone);
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

void RGBController_RGBFusion2USB::DeviceSaveMode()
{
    controller->SaveLEDState(true);
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
            allocated_zones.push_back(new_zone);
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

