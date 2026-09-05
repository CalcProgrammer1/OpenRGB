/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2USB.cpp                   |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 USB       |
|   motherboard                                             |
|                                                           |
|   jackun                                      08 Jan 2020 |
|   megadjc                                     03 Sep 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <mutex>
#include "GigabyteFusion2USB_Devices.h"
#include "RGBController_GigabyteRGBFusion2USB.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
struct IT5711ZoneCounts
{
    uint16_t led10 = 0;
    uint16_t led11 = 0;
};

/*---------------------------------------------------------*\
| Sets LED10/11 counts based on firmware series             |
\*---------------------------------------------------------*/
static IT5711ZoneCounts GetIT5711ZoneCounts(uint8_t fw_id, uint32_t lid)
{
    switch(fw_id)
    {
        case 0x00:
            return {27, 2};

        case 0x02:
            switch(lid)
            {
                case 0x013001DF:
                    return {4, 6};

                case 0x015001DF:
                    return {6, 6};

                default:
                    return {14, 6};
            }

        case 0x03:
        case 0x08:
            return {13, 0};

        case 0x05:
        case 0x0C:
            switch(lid)
            {
                case 0x016001DF:
                    return {8, 4};

                case 0x028001DF:
                    return {26, 18};

                default:
                    return {16, 4};
            }

        case 0x06:
        case 0x09:
            return {12, 0};

        default:
            return {};
    }
}

static void ApplyIT5711ZoneCounts(gb_fusion2_device* layout, uint8_t fw_id, std::vector<gb_fusion2_zone*>& allocated_zones)
{
    IT5711ZoneCounts counts = GetIT5711ZoneCounts(fw_id, layout->layout_id);

    for(uint8_t zone_idx = 0; zone_idx < GB_FUSION2_ZONES_MAX; ++zone_idx)
    {
        const gb_fusion2_zone* zone = (*layout->zones)[zone_idx];

        if(!zone)
        {
            continue;
        }

        uint16_t led_count = 0;

        switch(zone->idx)
        {
            case LED10:
                led_count = counts.led10;
                break;

            case LED11:
                led_count = counts.led11;
                break;

            default:
                continue;
        }

        /*-----------------------------------------------------*\
        | A zero count means that no firmware mapping is known  |
        \*-----------------------------------------------------*/
        if(led_count == 0)
        {
            continue;
        }

        /*-----------------------------------------------------*\
        | Static layouts contain const zone objects, so create  |
        | a mutable per-instance copy before changing the count |
        \*-----------------------------------------------------*/
        gb_fusion2_zone* new_zone = new gb_fusion2_zone(*zone);

        new_zone->leds_min = led_count;
        new_zone->leds_max = led_count;

        (*layout->zones)[zone_idx] = new_zone;
        allocated_zones.push_back(new_zone);
    }
}


/*---------------------------------------------------------*\
| Return the Gen2 scan slot used by a layout header zone    |
\*---------------------------------------------------------*/
static int GetGen2HeaderSlot(const gb_fusion2_zone* zone)
{
    if(zone == nullptr)
    {
        return -1;
    }

    /*-----------------------------------------------------*\
    | External ARGB headers are the variable-size zones in  |
    | the selected controller layout.                       |
    \*-----------------------------------------------------*/
    if(zone->leds_min >= zone->leds_max)
    {
        return -1;
    }

    switch(zone->idx)
    {
        case LED4:
        case HDR_D_LED2:
            return 1;

        case HDR_D_LED3:
            return 2;

        case HDR_D_LED4:
            return 3;

        default:
            return 0;
    }
}

/*---------------------------------------------------------*\
| Return the layout zone exposed for a Gen2 scan slot       |
\*---------------------------------------------------------*/
static const gb_fusion2_zone* GetGen2HeaderZone(const gb_fusion2_device& layout, int slot, std::size_t available_slots)
{
    if(layout.zones == nullptr || slot < 0 || slot >= 4 || static_cast<std::size_t>(slot) >= available_slots)
    {
        return nullptr;
    }

    for(uint8_t zone_idx = 0; zone_idx < GB_FUSION2_ZONES_MAX; ++zone_idx)
    {
        const gb_fusion2_zone* zone = (*layout.zones)[zone_idx];

        if(GetGen2HeaderSlot(zone) == slot)
        {
            return zone;
        }
    }

    return nullptr;
}

/*---------------------------------------------------------*\
| Return the layout zone backed by an ARGB calibration slot |
\*---------------------------------------------------------*/
static const gb_fusion2_zone* GetCalibrationARGBZone(const gb_fusion2_device& layout, int slot)
{
    if(layout.zones == nullptr || slot < 0 || slot >= 6)
    {
        return nullptr;
    }

    for(uint8_t zone_idx = 0; zone_idx < GB_FUSION2_ZONES_MAX; ++zone_idx)
    {
        const gb_fusion2_zone* zone = (*layout.zones)[zone_idx];

        if(zone == nullptr)
        {
            continue;
        }

        if(slot < 4)
        {
            if(GetGen2HeaderSlot(zone) == slot)
            {
                return zone;
            }

            continue;
        }

        if((slot == 4 && zone->idx == LED10)
        || (slot == 5 && zone->idx == LED11))
        {
            return zone;
        }
    }

    return nullptr;
}

/*---------------------------------------------------------*\
| Add a boolean device-specific configuration entry         |
\*---------------------------------------------------------*/
static void AddDeviceSpecificBool(nlohmann::json& schema, nlohmann::json& config, const char* key, const char* title, const char* description, bool value, int order)
{
    schema[key]["title"]       = title;
    schema[key]["description"] = description;
    schema[key]["type"]        = "bool";
    schema[key]["default"]     = false;
    schema[key]["order"]       = order;
    config[key]                 = value;
}

/*---------------------------------------------------------*\
| Normalize a controller calibration value for the UI       |
\*---------------------------------------------------------*/
static std::string NormalizeCalibrationValue(const std::string& value)
{
    if(value == "OFF"
    || value == "RGB" || value == "RBG"
    || value == "GRB" || value == "GBR"
    || value == "BRG" || value == "BGR")
    {
        return value;
    }

    /*-----------------------------------------------------*\
    | Malformed or unavailable controller calibration must  |
    | never be silently converted to OFF.                   |
    \*-----------------------------------------------------*/
    return "INVALID";
}

/*---------------------------------------------------------*\
| Add a calibration device-specific configuration entry     |
\*---------------------------------------------------------*/
static void AddCalibrationSetting(nlohmann::json& schema, nlohmann::json& config, const char* key, const char* title, const std::string& value, const std::string& default_value, int order)
{
    schema[key]["title"]        = title;
    schema[key]["description"]  = "RGB channel order used by this output. INVALID indicates malformed or unavailable controller calibration and cannot be selected by the user.";
    schema[key]["type"]         = "string";
    schema[key]["default"]      = NormalizeCalibrationValue(default_value);
    schema[key]["order"]        = order;
    schema[key]["enum"]         = {"OFF", "RGB", "RBG", "GRB", "GBR", "BRG", "BGR", "INVALID"};
    config[key]                  = NormalizeCalibrationValue(value);
}

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
    fw_id                       = controller->GetFWID();


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

    if(!controller->SupportsSetPersistentLighting())
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

    if(persist_lighting_on_exit && active_mode >= 0
                                && active_mode < static_cast<int>(modes.size())
                                && (modes[active_mode].flags & MODE_FLAG_MANUAL_SAVE))
    {
        DeviceSaveMode();
    }

    delete controller;
}

/*---------------------------------------------------------*\
| Initialize controller layout and device-specific settings |
\*---------------------------------------------------------*/
void RGBController_RGBFusion2USB::Init_Controller()
{
    const gb_fusion2_device* src_layout = gb_fusion2_device_list[device_index];

    /*---------------------------------------------------------*\
    | Select controller-specific generic fallback layout        |
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

    /*---------------------------------------------------------------------*\
    |  When no match found the first entry (generic_device) will be used    |
    |    otherwise look up channel map based on device name                 |
    \*---------------------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | Apply physical onboard ARGB LED counts for IT5711         |
    \*---------------------------------------------------------*/
    if(product_id == 0x5711)
    {
        ApplyIT5711ZoneCounts(&instance_layout, fw_id, allocated_zones);
    }

    /*---------------------------------------------------------*\
    | Device-specific settings depend on the resolved layout.   |
    \*---------------------------------------------------------*/
    InitDeviceSpecificConfiguration();
    ApplyDeviceSpecificConfiguration(false);

    /*---------------------------------------------------------------------*\
    | Culls the mode support based on layout_id.                            |
    \*---------------------------------------------------------------------*/
    uint32_t effect_mask = instance_layout.layout_id & GB_EFF_CORE_MASK;
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

/*---------------------------------------------------------*\
| Build device-specific configuration schema and defaults   |
\*---------------------------------------------------------*/
void RGBController_RGBFusion2USB::InitDeviceSpecificConfiguration()
{
    nlohmann::json configuration_json;
    configuration_json["schema"]        = nlohmann::json::object();
    configuration_json["configuration"] = nlohmann::json::object();

    nlohmann::json& schema = configuration_json["schema"];
    nlohmann::json& config = configuration_json["configuration"];

    /*---------------------------------------------------------*\
    | Remove obsolete detector-wide settings. Device-specific   |
    | settings now live in Configuration.json.                  |
    \*---------------------------------------------------------*/
    static std::mutex legacy_cleanup_mutex;
    static bool legacy_cleanup_done = false;

    {
        std::lock_guard<std::mutex> lock(legacy_cleanup_mutex);

        if(!legacy_cleanup_done)
        {
            SettingsManager* settings_manager = ResourceManager::get()->GetSettingsManager();
            nlohmann::json stored_settings = settings_manager->GetSettings(detector_name);

            if(stored_settings.is_object())
            {
                bool settings_changed = false;

                for(const char* section :
                    {"Gigabyte-Gen2-ARGB", "PersistLightingOnExit", "Calibration",
                     "MotherboardLayouts", "CustomLayout", "CustomLayout0", "CustomLayout1"})
                {
                    if(stored_settings.contains(section))
                    {
                        stored_settings.erase(section);
                        settings_changed = true;
                    }
                }

                if(settings_changed)
                {
                    settings_manager->SetSettings(detector_name, stored_settings);
                    settings_manager->SaveSettings();
                }
            }

            legacy_cleanup_done = true;
        }
    }

    /*---------------------------------------------------------*\
    | Gen2 ARGB header settings                                 |
    \*---------------------------------------------------------*/
    if(controller->SupportsGen2())
    {
        static const char* config_keys[4] =
        {
            "gen2_d_led1",
            "gen2_d_led2",
            "gen2_d_led3",
            "gen2_d_led4"
        };

        std::size_t available_slots = controller->ExportGen2Strips().size();

        for(int slot = 0; slot < 4; ++slot)
        {
            const gb_fusion2_zone* header_zone =
                GetGen2HeaderZone(instance_layout, slot, available_slots);

            if(header_zone == nullptr)
            {
                continue;
            }

            std::string title = "Gen2 ARGB - " + header_zone->name;

            AddDeviceSpecificBool(schema, config, config_keys[slot], title.c_str(),
                                  "Automatically detect Gen2 ARGB devices on this header",
                                  false, 10 + slot);
        }
    }

    /*---------------------------------------------------------*\
    | Persistent lighting on controller teardown                |
    \*---------------------------------------------------------*/
    if(controller->SupportsSetPersistentLighting())
    {
        AddDeviceSpecificBool(schema, config, "persist_lighting_on_exit", "Persist Lighting on Exit",
                              "Save the current hardware lighting state to flash when OpenRGB closes",
                              false, 20);
    }

    /*---------------------------------------------------------*\
    | RGB calibration                                           |
    \*---------------------------------------------------------*/
    if(device_num == 0 || product_id != 0xa100)
    {
        EncodedCalibration hw_cal = controller->GetCalibration(false);

        AddDeviceSpecificBool(schema, config, "calibration_enabled", "Override RGB Calibration",
                              "Apply the selected RGB channel orders to the controller calibration. Disabling this setting does not restore an earlier calibration.",
                              false, 30);

        static const char* calibration_keys[6] =
        {
            "calibration_d_led1",
            "calibration_d_led2",
            "calibration_d_led3",
            "calibration_d_led4",
            "calibration_onboard1_argb",
            "calibration_onboard2_argb"
        };

        int calibration_slots = product_id == 0x5711 ? 6 : 2;

        for(int slot = 0; slot < calibration_slots; ++slot)
        {
            const gb_fusion2_zone* calibration_zone =
                GetCalibrationARGBZone(instance_layout, slot);

            if(calibration_zone == nullptr)
            {
                continue;
            }

            std::string title = calibration_zone->name + " Color Order";

            AddCalibrationSetting(schema, config, calibration_keys[slot], title.c_str(),
                                  hw_cal.dled[slot], hw_cal.dled[slot],
                                  31 + slot);
        }

        AddCalibrationSetting(schema, config, "calibration_mainboard", "LED_C(x) Color Order",
                              hw_cal.mainboard, hw_cal.mainboard, 37);

    }

    if(!schema.empty())
    {
        flags |= CONTROLLER_FLAG_MANUALLY_CONFIGURABLE_DEVICE_SPECIFIC;
    }

    configuration = configuration_json.dump();
}

/*---------------------------------------------------------*\
| Apply device-specific configuration                       |
\*---------------------------------------------------------*/
void RGBController_RGBFusion2USB::ApplyDeviceSpecificConfiguration(bool setup_zones)
{
    nlohmann::json configuration_json;

    try
    {
        configuration_json = nlohmann::json::parse(configuration);
    }
    catch(...)
    {
        return;
    }

    if(!configuration_json.contains("configuration") || !configuration_json["configuration"].is_object())
    {
        return;
    }

    const nlohmann::json& config = configuration_json["configuration"];
    bool gen2_changed = false;

    /*---------------------------------------------------------*\
    | Gen2 ARGB header settings                                 |
    \*---------------------------------------------------------*/
    if(controller->SupportsGen2())
    {
        static const char* config_keys[4] =
        {
            "gen2_d_led1",
            "gen2_d_led2",
            "gen2_d_led3",
            "gen2_d_led4"
        };

        std::size_t available_slots = controller->ExportGen2Strips().size();
        uint8_t enabled_headers = 0;

        for(int slot = 0; slot < 4; ++slot)
        {
            if(GetGen2HeaderZone(instance_layout, slot, available_slots) == nullptr)
            {
                continue;
            }

            if(config.value(config_keys[slot], false))
            {
                enabled_headers |= 1U << slot;
            }
        }

        if(enabled_headers != gen2_enabled_headers)
        {
            controller->ScanGen2Strips(enabled_headers);
            gen2_enabled_headers = enabled_headers;
            gen2_changed = true;
        }

        supports_gen2 = enabled_headers != 0;
    }
    else
    {
        supports_gen2 = false;
    }

    /*---------------------------------------------------------*\
    | Persistent lighting on controller teardown                |
    \*---------------------------------------------------------*/
    persist_lighting_on_exit = controller->SupportsSetPersistentLighting()
                             && config.value("persist_lighting_on_exit", false);

    /*---------------------------------------------------------*\
    | RGB calibration                                           |
    \*---------------------------------------------------------*/
    if(config.value("calibration_enabled", false))
    {
        /*-----------------------------------------------------*\
        | Preserve calibration values for channels that are not |
        | exposed by the selected motherboard layout.           |
        \*-----------------------------------------------------*/
        EncodedCalibration desired = controller->GetCalibration(false);

        static const char* calibration_keys[6] =
        {
            "calibration_d_led1",
            "calibration_d_led2",
            "calibration_d_led3",
            "calibration_d_led4",
            "calibration_onboard1_argb",
            "calibration_onboard2_argb"
        };

        int calibration_slots = product_id == 0x5711 ? 6 : 2;

        for(int slot = 0; slot < calibration_slots; ++slot)
        {
            if(GetCalibrationARGBZone(instance_layout, slot) == nullptr)
            {
                continue;
            }

            if(config.contains(calibration_keys[slot])
            && config[calibration_keys[slot]].is_string())
            {
                desired.dled[slot] = config[calibration_keys[slot]].get<std::string>();
            }
        }

        if(config.contains("calibration_mainboard")
        && config["calibration_mainboard"].is_string())
        {
            desired.mainboard = config["calibration_mainboard"].get<std::string>();
        }

        controller->SetCalibration(desired, false);
    }

    if(setup_zones && gen2_changed)
    {
        SetupZones();
    }
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

    unsigned int d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0, d6 =0;

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

        bool fixed_zone = (zone_at_idx->leds_min == 1 && zone_at_idx->leds_max == 1);

        const Gen2StripInfo* gen2_info = nullptr;

        int gen2_slot = GetGen2HeaderSlot(zone_at_idx);

        /*-------------------------------------------------*\
        | Locate the Gen2 result corresponding to this zone |
        \*-------------------------------------------------*/
        if(supports_gen2 && !fixed_zone && gen2_slot >= 0)
        {
            unsigned int slot = static_cast<unsigned int>(gen2_slot);

            if(slot < strips.size() && strips[slot].totalLeds > 0)
            {
                gen2_info = &strips[slot];
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
                case LED10:
                    d5 = zones[zone_idx].leds_count;
                    break;
                case LED11:
                    d6 = zones[zone_idx].leds_count;
                    break;
                default:
                    d1 = zones[zone_idx].leds_count;
                    break;
            }
        }
    }

    controller->SetLedCount(d1, d2, d3, d4, d5, d6);
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
        entire_device_effect_active = true;
        return;
    }

    /*---------------------------------------------------------*\
    | Tear down the special wave slot before returning to       |
    | normal per-zone/direct operation                          |
    \*---------------------------------------------------------*/
    if(entire_device_effect_active)
    {
        controller->SetLEDEffect(2, EFFECT_STATIC, 0, 0xFF, false, color);
        controller->ApplyEffect();
        entire_device_effect_active = false;
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
    controller->SetPersistentLightingEnabled(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    controller->SaveLightingStateToFlash();
}

void RGBController_RGBFusion2USB::DeviceConfigureDevice()
{
    if((flags & CONTROLLER_FLAG_MANUALLY_CONFIGURED_DEVICE_SPECIFIC) == 0)
    {
        InitDeviceSpecificConfiguration();
        ApplyDeviceSpecificConfiguration(true);
    }
}

void RGBController_RGBFusion2USB::DeviceUpdateDeviceSpecificConfiguration()
{
    /*---------------------------------------------------------*\
    | SetDeviceSpecificConfiguration() already holds AccessMutex|
    | here, so parse the protected configuration string directly|
    \*---------------------------------------------------------*/
    ApplyDeviceSpecificConfiguration(true);
}
