/*---------------------------------------------------------*\
| RGBController_CMARGBGen2A1Controller.cpp                  |
|                                                           |
|   Driver for Cooler Master ARGB Gen 2 A1 controller       |
|                                                           |
|   Morgan Guimard (morg)                       26 Jun 2022 |
|   Fabian R (kderazorback)                     11 Aug 2023 |
|   Daniel Clark (megadjc)                      14 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <cstdio>
#include <cstring>
#include "RGBController_CMARGBGen2A1Controller.h"

static bool IsValidIndex(int index, std::size_t count)
{
    return(index >= 0 && (std::size_t)index < count);
}

static void AddDeviceSpecificBool(nlohmann::json& schema, nlohmann::json& config, const std::string& key, const std::string& title,
                                  const std::string& description, bool value, unsigned int order)
{
    schema[key]["title"]        = title;
    schema[key]["description"]  = description;
    schema[key]["type"]         = "bool";
    schema[key]["default"]      = value;
    schema[key]["order"]        = order;
    config[key]                 = value;
}

static void AddDeviceSpecificInteger(nlohmann::json& schema, nlohmann::json& config, const std::string& key,
                                     const std::string& title, const std::string& description, int value, int minimum, int maximum,
                                     int order)
{
    schema[key]["title"]        = title;
    schema[key]["description"]  = description;
    schema[key]["type"]         = "integer";
    schema[key]["default"]      = value;
    schema[key]["minimum"]      = minimum;
    schema[key]["maximum"]      = maximum;
    schema[key]["order"]        = order;
    config[key]                 = value;
}

static int OverlayIntervalToMilliseconds(unsigned char interval)
{
    return(interval * CM_ARGB_GEN2_A1_OVERLAY_INTERVAL_UNIT_MS);
}

static unsigned char OverlayIntervalFromMilliseconds(int milliseconds)
{
    milliseconds = std::max(0, std::min(0xFF * CM_ARGB_GEN2_A1_OVERLAY_INTERVAL_UNIT_MS, milliseconds));
    return((unsigned char)((milliseconds + (CM_ARGB_GEN2_A1_OVERLAY_INTERVAL_UNIT_MS / 2)) / CM_ARGB_GEN2_A1_OVERLAY_INTERVAL_UNIT_MS));
}

static std::string OverlaySelectorName(unsigned char selector)
{
    switch(selector)
    {
        case CM_ARGB_GEN2_A1_STATIC_MODE:
            return("Static");

        case CM_ARGB_GEN2_A1_RELOAD_MODE:
            return("Reload");

        case CM_ARGB_GEN2_A1_RECOIL_MODE:
            return("Recoil");

        case CM_ARGB_GEN2_A1_BREATHING_MODE:
            return("Breathing");

        case CM_ARGB_GEN2_A1_REFILL_MODE:
            return("Refill");

        case CM_ARGB_GEN2_A1_OFF_MODE:
        case 0x00:
            return("Off");

        default:
        {
            char unknown[16];
            snprintf(unknown, sizeof(unknown), "Unknown 0x%02X", selector);
            return(unknown);
        }
    }
}

static unsigned char OverlaySelectorValue(const std::string& selector, unsigned char fallback)
{
    if(selector == "Static")
    {
        return(CM_ARGB_GEN2_A1_STATIC_MODE);
    }

    if(selector == "Reload")
    {
        return(CM_ARGB_GEN2_A1_RELOAD_MODE);
    }

    if(selector == "Recoil")
    {
        return(CM_ARGB_GEN2_A1_RECOIL_MODE);
    }

    if(selector == "Breathing")
    {
        return(CM_ARGB_GEN2_A1_BREATHING_MODE);
    }

    if(selector == "Refill")
    {
        return(CM_ARGB_GEN2_A1_REFILL_MODE);
    }

    if(selector == "Off")
    {
        return(CM_ARGB_GEN2_A1_OFF_MODE);
    }

    return(fallback);
}

static unsigned char NormalizeOverlaySelector(unsigned char selector)
{
    return(selector == CM_ARGB_GEN2_A1_OFF_MODE ? 0x00 : selector);
}

static unsigned char EncodeOverlaySelector(unsigned char selector)
{
    if(selector == 0x00)
    {
        return(CM_ARGB_GEN2_A1_OFF_MODE);
    }

    return(selector);
}

static unsigned int OverlaySubportIndex(unsigned char subtarget)
{
    switch(subtarget)
    {
        case 0x01:
            return(0);

        case 0x02:
            return(1);

        case 0x04:
            return(2);

        case 0x08:
            return(3);

        case 0x10:
            return(4);

        default:
            return(CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT);
    }
}

static void AddOverlaySelector(nlohmann::json& schema, nlohmann::json& config, const std::string& key, const std::string& title,
                               unsigned char selector, unsigned int order)
{
    std::string value = OverlaySelectorName(selector);

    schema[key]["title"]       = title;
    schema[key]["description"] = "Controller-resident effect applied to this lighting channel during the selected Custom overlay phase.";
    schema[key]["type"]        = "string";
    schema[key]["default"]     = value;
    schema[key]["order"]       = order;
    schema[key]["enum"]        = {"Off", "Static", "Reload", "Recoil", "Breathing", "Refill"};

    if(value.compare(0, 8, "Unknown ") == 0)
    {
        schema[key]["enum"].push_back(value);
    }

    config[key] = value;
}

/*---------------------------------------------------------*\
| Encode the nominal Gen1 LED count as the two layout       |
| factors used by CMD06.                                    |
\*---------------------------------------------------------*/
static bool EncodeGen1OtherLEDCount(unsigned int led_count, unsigned char& factor_a, unsigned char& factor_b)
{
    if(led_count < CM_ARGB_GEN2_A1_GEN1_MIN_LEDS || led_count > CM_ARGB_GEN2_A1_TARGET_MAX_LEDS)
    {
        return(false);
    }

    /*-----------------------------------------------------*\
    | The two factors describe a layout within the 60-LED   |
    | transport limit and must not be interpreted as a      |
    | literal device count and LEDs-per-device pair.        |
    \*-----------------------------------------------------*/
    unsigned int a = (CM_ARGB_GEN2_A1_TARGET_MAX_LEDS + led_count - 1) / led_count;
    unsigned int b = CM_ARGB_GEN2_A1_TARGET_MAX_LEDS / a;

    if(a < 1 || a > CM_ARGB_GEN2_A1_TARGET_MAX_LEDS || b < 1 || b > CM_ARGB_GEN2_A1_TARGET_MAX_LEDS
             || a * b > CM_ARGB_GEN2_A1_TARGET_MAX_LEDS)
    {
        return(false);
    }

    factor_a = (unsigned char)a;
    factor_b = (unsigned char)b;
    return(true);
}

/*---------------------------------------------------------*\
| The CMD06 transform is not one-to-one, so the exact       |
| OpenRGB Zone Manager value remains host-owned.  When no   |
| host value exists, factor_b is used only as a             |
| conservative display fallback.                            |
\*---------------------------------------------------------*/
static unsigned int DecodeGen1NominalLEDCount(unsigned char factor_b)
{
    return(std::max<unsigned int>(CM_ARGB_GEN2_A1_GEN1_MIN_LEDS,
                                  std::min<unsigned int>(factor_b, CM_ARGB_GEN2_A1_TARGET_MAX_LEDS)));
}

/**------------------------------------------------------------------*\
    @name Coolermaster ARGB A1
    @category LEDStrip
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterARGBGen2A1
    @comment Supports read-only Gen1/Gen2 topology discovery, native
        realtime RGB streaming, controller-resident Custom colors, and
        hardware effects targeted at the discovered logical devices.
\*-------------------------------------------------------------------*/

RGBController_CMARGBGen2A1Controller::RGBController_CMARGBGen2A1Controller(CMARGBGen2A1controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "CoolerMaster";
    type                        = DEVICE_TYPE_LEDSTRIP;
    description                 = "CoolerMaster LED Controller A1 Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();
    version                     = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = CM_ARGB_GEN2_A1_DIRECT_MODE;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Spectrum;
    Spectrum.name               = "Spectrum";
    Spectrum.value              = CM_ARGB_GEN2_A1_SPECTRUM_MODE;
    Spectrum.flags              = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    Spectrum.brightness         = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Spectrum.brightness_min     = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Spectrum.brightness_max     = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Spectrum.speed              = CM_ARGB_GEN2_A1_SPEED_HALF;
    Spectrum.speed_min          = CM_ARGB_GEN2_A1_SPEED_MIN;
    Spectrum.speed_max          = CM_ARGB_GEN2_A1_SPEED_MAX;
    modes.push_back(Spectrum);

    mode Static;
    Static.name                 = "Static";
    Static.value                = CM_ARGB_GEN2_A1_STATIC_MODE;
    Static.flags                = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                | MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness           = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Static.brightness_min       = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Static.brightness_max       = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Reload;
    Reload.name                 = "Reload";
    Reload.value                = CM_ARGB_GEN2_A1_RELOAD_MODE;
    Reload.flags                = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                | MODE_FLAG_HAS_RANDOM_COLOR
                                | MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Reload.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Reload.brightness           = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Reload.brightness_min       = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Reload.brightness_max       = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Reload.speed                = CM_ARGB_GEN2_A1_SPEED_HALF;
    Reload.speed_min            = CM_ARGB_GEN2_A1_SPEED_MIN;
    Reload.speed_max            = CM_ARGB_GEN2_A1_SPEED_MAX;
    Reload.colors_min           = 1;
    Reload.colors_max           = 1;
    Reload.colors.resize(1);
    modes.push_back(Reload);

    mode Recoil;
    Recoil.name                 = "Recoil";
    Recoil.value                = CM_ARGB_GEN2_A1_RECOIL_MODE;
    Recoil.flags                = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                | MODE_FLAG_HAS_RANDOM_COLOR
                                | MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Recoil.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Recoil.brightness           = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Recoil.brightness_min       = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Recoil.brightness_max       = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Recoil.speed                = CM_ARGB_GEN2_A1_SPEED_HALF;
    Recoil.speed_min            = CM_ARGB_GEN2_A1_SPEED_MIN;
    Recoil.speed_max            = CM_ARGB_GEN2_A1_SPEED_MAX;
    Recoil.colors_min           = 1;
    Recoil.colors_max           = 1;
    Recoil.colors.resize(1);
    modes.push_back(Recoil);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = CM_ARGB_GEN2_A1_BREATHING_MODE;
    Breathing.flags             = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                | MODE_FLAG_HAS_RANDOM_COLOR
                                | MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.brightness        = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Breathing.brightness_min    = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Breathing.brightness_max    = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Breathing.speed             = CM_ARGB_GEN2_A1_SPEED_HALF;
    Breathing.speed_min         = CM_ARGB_GEN2_A1_SPEED_MIN;
    Breathing.speed_max         = CM_ARGB_GEN2_A1_SPEED_MAX;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Refill;
    Refill.name                 = "Refill";
    Refill.value                = CM_ARGB_GEN2_A1_REFILL_MODE;
    Refill.flags                = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                | MODE_FLAG_HAS_RANDOM_COLOR
                                | MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Refill.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Refill.brightness           = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Refill.brightness_min       = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Refill.brightness_max       = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Refill.speed                = CM_ARGB_GEN2_A1_SPEED_HALF;
    Refill.speed_min            = CM_ARGB_GEN2_A1_SPEED_MIN;
    Refill.speed_max            = CM_ARGB_GEN2_A1_SPEED_MAX;
    Refill.colors_min           = 1;
    Refill.colors_max           = 1;
    Refill.colors.resize(1);
    modes.push_back(Refill);

    mode Demo;
    Demo.name                   = "Demo";
    Demo.value                  = CM_ARGB_GEN2_A1_DEMO_MODE;
    Demo.flags                  = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Demo.color_mode             = MODE_COLORS_NONE;
    Demo.brightness             = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Demo.brightness_min         = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Demo.brightness_max         = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Demo.speed                  = CM_ARGB_GEN2_A1_SPEED_HALF;
    Demo.speed_min              = CM_ARGB_GEN2_A1_SPEED_MIN;
    Demo.speed_max              = CM_ARGB_GEN2_A1_SPEED_MAX;
    modes.push_back(Demo);

    mode FillFlow;
    FillFlow.name               = "Fill Flow";
    FillFlow.value              = CM_ARGB_GEN2_A1_FILL_FLOW_MODE;
    FillFlow.flags              = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    FillFlow.color_mode         = MODE_COLORS_NONE;
    FillFlow.brightness         = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    FillFlow.brightness_min     = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    FillFlow.brightness_max     = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    FillFlow.speed              = CM_ARGB_GEN2_A1_SPEED_HALF;
    FillFlow.speed_min          = CM_ARGB_GEN2_A1_SPEED_MIN;
    FillFlow.speed_max          = CM_ARGB_GEN2_A1_SPEED_MAX;
    modes.push_back(FillFlow);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = CM_ARGB_GEN2_A1_RAINBOW_MODE;
    Rainbow.flags               = MODE_FLAG_HAS_BRIGHTNESS
                                | MODE_FLAG_HAS_SPEED
                                | MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.brightness          = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Rainbow.brightness_min      = CM_ARGB_GEN2_A1_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    Rainbow.speed               = CM_ARGB_GEN2_A1_SPEED_HALF;
    Rainbow.speed_min           = CM_ARGB_GEN2_A1_SPEED_MIN;
    Rainbow.speed_max           = CM_ARGB_GEN2_A1_SPEED_MAX;
    modes.push_back(Rainbow);

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = CM_ARGB_GEN2_A1_CUSTOM_MODE;
    Custom.flags                = MODE_FLAG_HAS_PER_LED_COLOR
                                | MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = CM_ARGB_GEN2_A1_OFF_MODE;
    Off.flags                   = MODE_FLAG_MANUAL_SAVE
                                | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Multilayer;
    Multilayer.name             = "Multilayer";
    Multilayer.value            = CM_ARGB_GEN2_A1_MULTILAYER_MODE;
    Multilayer.flags            = MODE_FLAG_MANUAL_SAVE;
    Multilayer.color_mode       = MODE_COLORS_NONE;
    modes.push_back(Multilayer);

    /*-----------------------------------------------------*\
    | Direct is a host-owned realtime mode and cannot be    |
    | inferred from persistent controller state.  Use an    |
    | autonomous mode as the fallback until the read-only   |
    | state import below identifies the actual saved mode.  |
    \*-----------------------------------------------------*/
    active_mode = FindGlobalModeIndex(CM_ARGB_GEN2_A1_STATIC_MODE);

    InitDeviceSpecificConfiguration();
    SetupZones();
    ImportDeviceLightingState();
}

RGBController_CMARGBGen2A1Controller::~RGBController_CMARGBGen2A1Controller()
{
    Shutdown();
    delete controller;
}

void RGBController_CMARGBGen2A1Controller::InitDeviceSpecificConfiguration()
{
    bool enabled = false;
    unsigned char interval = CM_ARGB_GEN2_A1_OVERLAY_INTERVAL_DEFAULT;

    if(controller->GetOverlayMode(enabled, interval))
    {
        overlay_enabled     = enabled;
        overlay_gate_active = enabled;
        overlay_interval    = interval;
    }

    unsigned char current_multilayer_mode = CM_ARGB_GEN2_A1_MULTILAYER_GLOBAL;

    if(controller->GetMultilayerMode(current_multilayer_mode))
    {
        multilayer_mode = current_multilayer_mode;
    }
    else
    {
        multilayer_mode = 0xFF;
    }

    for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
    {
        controller->GetOverlayTable(port, overlay_tables[port]);
    }

    memset(overlay_table_dirty, 0, sizeof(overlay_table_dirty));
    RebuildDeviceSpecificConfiguration();
}

void RGBController_CMARGBGen2A1Controller::ImportEffectRecord(mode& target_mode, const CMARGBGen2A1EffectRecord& effect)
{
    if(target_mode.flags & MODE_FLAG_HAS_SPEED)
    {
        target_mode.speed = std::max<unsigned int>(target_mode.speed_min, std::min<unsigned int>(target_mode.speed_max, effect.speed));
    }

    if(target_mode.flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        target_mode.brightness = std::max<unsigned int>(target_mode.brightness_min, std::min<unsigned int>(target_mode.brightness_max, effect.brightness));
    }

    if((target_mode.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR) && !target_mode.colors.empty())
    {
        target_mode.colors[0] = ToRGBColor(effect.red, effect.green, effect.blue);

        if(target_mode.flags & MODE_FLAG_HAS_RANDOM_COLOR)
        {
            target_mode.color_mode = effect.random != 0 ? MODE_COLORS_RANDOM : MODE_COLORS_MODE_SPECIFIC;
        }
    }
}

int RGBController_CMARGBGen2A1Controller::FindGlobalModeIndex(int mode_value) const
{
    for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
    {
        if(modes[mode_idx].value == mode_value)
        {
            return((int)mode_idx);
        }
    }

    return(-1);
}

void RGBController_CMARGBGen2A1Controller::ImportDeviceLightingState()
{
    /*-----------------------------------------------------*\
    | Import the controller-resident state without writing  |
    | anything back.  CMD0C supplies the saved per-effect   |
    | parameters while CMD03 supplies the effect that is    |
    | currently executing.                                  |
    \*-----------------------------------------------------*/
    CMARGBGen2A1EffectRecord effect_bank[CM_ARGB_GEN2_A1_EFFECT_COUNT] = {};

    if(controller->GetEffectBank(CM_ARGB_GEN2_A1_CHANNEL_ALL, CM_ARGB_GEN2_A1_SUBCHANNEL_ALL, effect_bank))
    {
        for(unsigned int record_idx = 0; record_idx < CM_ARGB_GEN2_A1_EFFECT_COUNT; record_idx++)
        {
            CMARGBGen2A1EffectRecord& effect = effect_bank[record_idx];
            int mode_idx = FindGlobalModeIndex(effect.mode);

            if(mode_idx >= 0 && effect.mode <= CM_ARGB_GEN2_A1_OFF_MODE)
            {
                ImportEffectRecord(modes[mode_idx], effect);
            }
        }
    }

    for(std::size_t zone_idx = 0; zone_idx < zones.size() && zone_idx < zone_targets.size(); zone_idx++)
    {
        ZoneTarget& target = zone_targets[zone_idx];
        const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(target.port);

        if(!info.valid)
        {
            continue;
        }

        for(unsigned int record_idx = 0; record_idx < CM_ARGB_GEN2_A1_EFFECT_COUNT; record_idx++)
        {
            effect_bank[record_idx] = CMARGBGen2A1EffectRecord{};
        }

        if(controller->GetEffectBank(info.outer_mask, target.subtarget, effect_bank))
        {
            for(unsigned int record_idx = 0; record_idx < CM_ARGB_GEN2_A1_EFFECT_COUNT; record_idx++)
            {
                CMARGBGen2A1EffectRecord& effect = effect_bank[record_idx];
                int mode_idx = FindZoneModeIndex(zones[zone_idx], effect.mode);

                if(mode_idx >= 0 && effect.mode <= CM_ARGB_GEN2_A1_OFF_MODE)
                {
                    ImportEffectRecord(zones[zone_idx].modes[mode_idx], effect);
                }
            }
        }

        std::vector<RGBColor> custom_colors;

        if(controller->GetCustomColors(info.outer_mask, target.subtarget, zones[zone_idx].leds_count, custom_colors))
        {
            std::size_t color_offset = zones[zone_idx].start_idx;
            std::size_t color_count = std::min<std::size_t>(zones[zone_idx].leds_count, custom_colors.size());

            if(color_offset <= colors.size() && color_count <= colors.size() - color_offset)
            {
                std::copy_n(custom_colors.begin(), color_count, colors.begin() + color_offset);
            }
        }
    }

    if(multilayer_mode == CM_ARGB_GEN2_A1_MULTILAYER_INDEPENDENT)
    {
        int multilayer_idx = FindGlobalModeIndex(CM_ARGB_GEN2_A1_MULTILAYER_MODE);

        if(multilayer_idx >= 0)
        {
            active_mode = multilayer_idx;
        }

        if(multilayer_zone_mode_values.size() != zones.size())
        {
            multilayer_zone_mode_values.resize(zones.size(), -1);
        }

        for(std::size_t zone_idx = 0; zone_idx < zones.size() && zone_idx < zone_targets.size(); zone_idx++)
        {
            ZoneTarget& target = zone_targets[zone_idx];
            const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(target.port);
            CMARGBGen2A1EffectRecord current{};

            if(!info.valid || !controller->GetEffect(info.outer_mask, target.subtarget, current))
            {
                continue;
            }

            int mode_idx = FindZoneModeIndex(zones[zone_idx], current.mode);

            if(mode_idx < 0)
            {
                continue;
            }

            zones[zone_idx].active_mode = mode_idx;
            multilayer_zone_mode_values[zone_idx] = current.mode;
        }

        return;
    }

    CMARGBGen2A1EffectRecord current_global{};
    unsigned char current_outer = 0;
    unsigned char current_subtarget = 0;
    bool have_current_global = false;

    /*------------------------------------------------------*\
    | Firmware rejects CMD03 GET FF/FF even though           |
    | global CMD03 SET FF/FF is valid. In global ownership   |
    | each active real target reports the visible effect, so |
    | use the first target that returns a current record.    |
    \*------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size() && zone_idx < zone_targets.size(); zone_idx++)
    {
        ZoneTarget& target = zone_targets[zone_idx];
        const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(target.port);

        if(!info.valid)
        {
            continue;
        }

        if(controller->GetEffect(info.outer_mask, target.subtarget, current_global))
        {
            current_outer = info.outer_mask;
            current_subtarget = target.subtarget;
            have_current_global = true;
            break;
        }
    }

    if(!have_current_global)
    {
        return;
    }

    int current_mode_idx = FindGlobalModeIndex(current_global.mode);

    if(current_mode_idx < 0)
    {
        return;
    }

    if(current_global.mode <= CM_ARGB_GEN2_A1_OFF_MODE)
    {
        last_global_effect_value = current_global.mode;

        /*-------------------------------------------------*\
        | CMD03 identifies the selected effect, while CMD04 |
        | provides its stored editable parameters without   |
        | runtime normalization such as speed clamping.     |
        \*-------------------------------------------------*/
        CMARGBGen2A1EffectRecord stored_effect{};

        if(controller->GetStoredEffect(current_outer, current_subtarget, current_global.mode, stored_effect))
        {
            ImportEffectRecord(modes[current_mode_idx], stored_effect);
        }
    }

    active_mode = current_mode_idx;
}

void RGBController_CMARGBGen2A1Controller::RebuildDeviceSpecificConfiguration()
{
    nlohmann::json configuration_json;
    configuration_json["schema"]        = nlohmann::json::object();
    configuration_json["configuration"] = nlohmann::json::object();

    nlohmann::json& schema = configuration_json["schema"];
    nlohmann::json& config = configuration_json["configuration"];

    /*-----------------------------------------------------*\
    | Physical-port generation controls                     |
    \*-----------------------------------------------------*/
    for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
    {
        const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(port);
        std::string key = "port_" + std::to_string(port + 1) + "_gen2";
        std::string port_name = "Port " + std::to_string(port + 1) + " - Gen2";

        AddDeviceSpecificBool(schema, config, key, port_name,
                              "Enable Gen2 device detection on this port. When disabled, the port operates as a manually sized Gen1 zone in OpenRGB's Zone Manager. Connected Gen2 devices may require a manual reset after switching the port to Gen1.",
                              info.generation == CM_ARGB_GEN2_A1_GENERATION_GEN2, 10 + port);
    }

    /*-----------------------------------------------------*\
    | Controller-wide Custom overlay controls               |
    \*-----------------------------------------------------*/
    AddDeviceSpecificBool(schema, config, "overlay_enabled", "Custom Overlay",
                          "Enable the controller-resident six-phase overlay when Custom mode is active.", overlay_enabled, 20);
    AddDeviceSpecificInteger(schema, config, "overlay_interval_ms", "Overlay Phase Interval (ms)",
                             "Overlay phase interval in milliseconds. Hardware resolution is 100 ms; MasterPlus uses 5000 ms.",
                             OverlayIntervalToMilliseconds(overlay_interval), 0, 0xFF * CM_ARGB_GEN2_A1_OVERLAY_INTERVAL_UNIT_MS,
                             21);

    /*-----------------------------------------------------*\
    | Build overlay controls from the active port topology. |
    | Gen1 uses one logical target on the first CMD10 row;  |
    | Gen2 exposes each active CMD07 logical subport.       |
    \*-----------------------------------------------------*/
    unsigned int overlay_order = 30;

    for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
    {
        const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(port);

        if(!info.valid)
        {
            continue;
        }

        if(info.generation == CM_ARGB_GEN2_A1_GENERATION_GEN1)
        {
            for(unsigned int phase = 0; phase < CM_ARGB_GEN2_A1_OVERLAY_PHASE_COUNT; ++phase)
            {
                std::string key = "overlay_port_" + std::to_string(port + 1) + "_gen1_phase_" + std::to_string(phase + 1);
                std::string title = "Port " + std::to_string(port + 1) + ", Gen1 - Overlay Phase " + std::to_string(phase + 1);

                AddOverlaySelector(schema, config, key, title, overlay_tables[port][phase], overlay_order++);
            }

            continue;
        }

        if(info.generation != CM_ARGB_GEN2_A1_GENERATION_GEN2)
        {
            continue;
        }

        for(std::size_t device_idx = 0; device_idx < info.gen2_devices.size(); ++device_idx)
        {
            unsigned int subport_idx = OverlaySubportIndex(info.gen2_devices[device_idx].mask);

            if(subport_idx >= CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT)
            {
                continue;
            }

            unsigned int table_offset = subport_idx * CM_ARGB_GEN2_A1_OVERLAY_PHASE_COUNT;

            for(unsigned int phase = 0; phase < CM_ARGB_GEN2_A1_OVERLAY_PHASE_COUNT; ++phase)
            {
                std::string key = "overlay_port_" + std::to_string(port + 1) + "_subport_" + std::to_string(subport_idx + 1)
                                                  + "_phase_" + std::to_string(phase + 1);
                std::string title = "Port " + std::to_string(port + 1) + ", Gen2 Subport " + std::to_string(subport_idx + 1)
                                            + " - Overlay Phase " + std::to_string(phase + 1);

                AddOverlaySelector(schema, config, key, title, overlay_tables[port][table_offset + phase], overlay_order++);
            }
        }
    }

    if(!schema.empty())
    {
        flags |= CONTROLLER_FLAG_MANUALLY_CONFIGURABLE_DEVICE_SPECIFIC;
    }

    configuration = configuration_json.dump();
}

void RGBController_CMARGBGen2A1Controller::ApplyDeviceSpecificConfiguration(bool setup_zones)
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

    nlohmann::json& config = configuration_json["configuration"];
    bool requested_overlay_enabled = config.value("overlay_enabled", overlay_enabled);
    int requested_overlay_interval_ms = config.value("overlay_interval_ms", OverlayIntervalToMilliseconds(overlay_interval));
    unsigned char requested_overlay_interval = OverlayIntervalFromMilliseconds(requested_overlay_interval_ms);
    bool overlay_setting_changed = requested_overlay_enabled != overlay_enabled || requested_overlay_interval != overlay_interval;

    requested_overlay_interval_ms = OverlayIntervalToMilliseconds(requested_overlay_interval);
    config["overlay_enabled"]     = requested_overlay_enabled;
    config["overlay_interval_ms"] = requested_overlay_interval_ms;

    unsigned char desired_generation[CM_ARGB_GEN2_A1_CHANNEL_COUNT] = {};
    bool candidate_change[CM_ARGB_GEN2_A1_CHANNEL_COUNT] = {};
    bool rebuild_port[CM_ARGB_GEN2_A1_CHANNEL_COUNT] = {};
    bool overlay_table_changed[CM_ARGB_GEN2_A1_CHANNEL_COUNT] = {};
    unsigned char desired_overlay_tables[CM_ARGB_GEN2_A1_CHANNEL_COUNT][CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH];
    memcpy(desired_overlay_tables, overlay_tables, sizeof(desired_overlay_tables));

    bool have_candidate = false;
    bool have_overlay_table_change = false;

    /*-----------------------------------------------------*\
    | Compare the requested generation with the cached      |
    | physical-port topology. A Save that changes nothing   |
    | must not disturb realtime or rebuild OpenRGB zones.   |
    \*-----------------------------------------------------*/
    for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
    {
        const CMARGBGen2A1PortInfo& cached = controller->GetPortInfo(port);
        std::string key = "port_" + std::to_string(port + 1) + "_gen2";
        bool cached_gen2 = cached.valid && cached.generation == CM_ARGB_GEN2_A1_GENERATION_GEN2;
        bool desired_gen2 = config.value(key, cached_gen2);

        desired_generation[port] = desired_gen2 ? CM_ARGB_GEN2_A1_GENERATION_GEN2 : CM_ARGB_GEN2_A1_GENERATION_GEN1;

        candidate_change[port] = !cached.valid || cached.generation != desired_generation[port];
        have_candidate |= candidate_change[port];

        if(cached.valid)
        {
            config[key] = cached_gen2;
        }
    }

    /*-----------------------------------------------------*\
    | Build CMD10 updates from the active topology. Ports   |
    | changing generation ignore stale overlay keys until   |
    | SetupZones rebuilds the device-specific schema.       |
    \*-----------------------------------------------------*/
    for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
    {
        if(candidate_change[port])
        {
            continue;
        }

        const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(port);

        if(!info.valid)
        {
            continue;
        }

        for(unsigned int table_idx = 0; table_idx < CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH; ++table_idx)
        {
            desired_overlay_tables[port][table_idx] = EncodeOverlaySelector(desired_overlay_tables[port][table_idx]);
        }

        if(info.generation == CM_ARGB_GEN2_A1_GENERATION_GEN1)
        {
            for(unsigned int phase = 0; phase < CM_ARGB_GEN2_A1_OVERLAY_PHASE_COUNT; ++phase)
            {
                std::string key = "overlay_port_" + std::to_string(port + 1) + "_gen1_phase_" + std::to_string(phase + 1);
                std::string requested = config.value(key, OverlaySelectorName(desired_overlay_tables[port][phase]));
                unsigned char requested_selector = OverlaySelectorValue(requested, desired_overlay_tables[port][phase]);

                config[key] = OverlaySelectorName(requested_selector);

                if(NormalizeOverlaySelector(requested_selector) != NormalizeOverlaySelector(desired_overlay_tables[port][phase]))
                {
                    desired_overlay_tables[port][phase] = requested_selector;
                    overlay_table_changed[port] = true;
                    have_overlay_table_change = true;
                }
            }

            continue;
        }

        if(info.generation != CM_ARGB_GEN2_A1_GENERATION_GEN2)
        {
            continue;
        }

        for(std::size_t device_idx = 0; device_idx < info.gen2_devices.size(); ++device_idx)
        {
            unsigned int subport_idx = OverlaySubportIndex(info.gen2_devices[device_idx].mask);

            if(subport_idx >= CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT)
            {
                continue;
            }

            unsigned int table_offset = subport_idx * CM_ARGB_GEN2_A1_OVERLAY_PHASE_COUNT;

            for(unsigned int phase = 0; phase < CM_ARGB_GEN2_A1_OVERLAY_PHASE_COUNT; ++phase)
            {
                unsigned int table_idx = table_offset + phase;
                std::string key = "overlay_port_" + std::to_string(port + 1) + "_subport_" + std::to_string(subport_idx + 1)
                                                  + "_phase_" + std::to_string(phase + 1);
                std::string requested = config.value(key, OverlaySelectorName(desired_overlay_tables[port][table_idx]));
                unsigned char requested_selector = OverlaySelectorValue(requested, desired_overlay_tables[port][table_idx]);

                config[key] = OverlaySelectorName(requested_selector);

                if(NormalizeOverlaySelector(requested_selector) != NormalizeOverlaySelector(desired_overlay_tables[port][table_idx]))
                {
                    desired_overlay_tables[port][table_idx] = requested_selector;
                    overlay_table_changed[port] = true;
                    have_overlay_table_change = true;
                }
            }
        }
    }

    if(!have_candidate && !overlay_setting_changed && !have_overlay_table_change)
    {
        configuration = configuration_json.dump();
        return;
    }

    bool direct_active = IsValidIndex(active_mode, modes.size()) && modes[active_mode].value == CM_ARGB_GEN2_A1_DIRECT_MODE;

    /*-----------------------------------------------------*\
    | Overlay configuration is not part of realtime output. |
    | While Direct owns the controller, retain requested    |
    | CMD10/CMD11 state in OpenRGB and apply it next time   |
    | Custom becomes active. Generation changes still have  |
    | to leave realtime because they change live topology.  |
    \*-----------------------------------------------------*/
    if(direct_active)
    {
        overlay_enabled  = requested_overlay_enabled;
        overlay_interval = requested_overlay_interval;

        for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
        {
            if(overlay_table_changed[port])
            {
                memcpy(overlay_tables[port], desired_overlay_tables[port], CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH);
                overlay_table_dirty[port] = true;
            }
        }

        if(!have_candidate)
        {
            RebuildDeviceSpecificConfiguration();
            return;
        }
    }

    /*-----------------------------------------------------*\
    | Preserve host-owned Gen1 zone sizes before topology   |
    | changes. Gen2 geometry is always owned by CMD07.      |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size() && zone_idx < zone_targets.size(); ++zone_idx)
    {
        ZoneTarget& target = zone_targets[zone_idx];

        if(target.gen2 || target.port >= CM_ARGB_GEN2_A1_CHANNEL_COUNT)
        {
            continue;
        }

        gen1_zone_led_counts[target.port] = zones[zone_idx].leds_count;
        gen1_zone_size_configured[target.port] = (zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE) != 0;
    }

    bool restart_direct = controller->IsRealtimeMode();

    if(restart_direct && !controller->ExitRealtimeMode())
    {
        RebuildDeviceSpecificConfiguration();
        return;
    }

    for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
    {
        if(!candidate_change[port])
        {
            continue;
        }

        unsigned char live_generation = CM_ARGB_GEN2_A1_GENERATION_UNKNOWN;
        bool have_live_generation = controller->QueryPortGeneration(port, live_generation);

        if(have_live_generation && live_generation == desired_generation[port])
        {
            rebuild_port[port] = controller->RefreshPortTopology(port);
        }
        else
        {
            rebuild_port[port] = controller->SetPortGeneration(port, desired_generation[port]);

            if(!rebuild_port[port])
            {
                rebuild_port[port] = controller->RefreshPortTopology(port);
            }
        }

        /*-------------------------------------------------*\
        | Generation changes do not rewrite retained Gen1   |
        | CMD06 geometry. Import only when OpenRGB has no   |
        | host-owned size for this physical port.           |
        \*-------------------------------------------------*/
        const CMARGBGen2A1PortInfo& final_info = controller->GetPortInfo(port);

        if(final_info.valid && final_info.generation == CM_ARGB_GEN2_A1_GENERATION_GEN1 && !gen1_zone_size_configured[port] && gen1_zone_led_counts[port] == 0)
        {
            gen1_zone_led_counts[port] = DecodeGen1NominalLEDCount(final_info.gen1_leds_per_device);
        }
    }

    bool topology_changed = false;

    for(unsigned int port = 0; port < CM_ARGB_GEN2_A1_CHANNEL_COUNT; port++)
    {
        topology_changed |= rebuild_port[port];
    }

    if(setup_zones && topology_changed)
    {
        /*-------------------------------------------------*\
        | A Gen2 port can change the number of OpenRGB      |
        | zones. SetupZones also rebuilds the dynamic       |
        | overlay schema from the new CMD07 topology.       |
        \*-------------------------------------------------*/
        SetupZones();
    }

    overlay_enabled  = requested_overlay_enabled;
    overlay_interval = requested_overlay_interval;

    if(direct_active)
    {
        RebuildDeviceSpecificConfiguration();
        DeviceUpdateLEDs();
        return;
    }

    bool custom_active = IsValidIndex(active_mode, modes.size()) && modes[active_mode].value == CM_ARGB_GEN2_A1_CUSTOM_MODE;
    bool desired_overlay_gate = custom_active && overlay_enabled;

    if(overlay_setting_changed && !desired_overlay_gate)
    {
        if(!ApplyOverlayGate(false, overlay_interval))
        {
            RebuildDeviceSpecificConfiguration();

            if(restart_direct)
            {
                DeviceUpdateLEDs();
            }
            return;
        }
    }

    for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
    {
        if(!overlay_table_changed[port])
        {
            continue;
        }

        /*-------------------------------------------------*\
        | MasterPlus reasserts CMD11 before each CMD10 SET. |
        | Do the same when the overlay is currently active. |
        \*-------------------------------------------------*/
        if(desired_overlay_gate && !ApplyOverlayGate(true, overlay_interval))
        {
            RebuildDeviceSpecificConfiguration();

            if(restart_direct)
            {
                DeviceUpdateLEDs();
            }
            return;
        }

        if(controller->SetOverlayTable(port, desired_overlay_tables[port]))
        {
            memcpy(overlay_tables[port], desired_overlay_tables[port], CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH);
            overlay_table_dirty[port] = false;
        }
    }

    if(overlay_setting_changed && desired_overlay_gate && !have_overlay_table_change)
    {
        if(!ApplyOverlayGate(true, overlay_interval))
        {
            RebuildDeviceSpecificConfiguration();

            if(restart_direct)
            {
                DeviceUpdateLEDs();
            }
            return;
        }
    }

    RebuildDeviceSpecificConfiguration();

    if(!IsValidIndex(active_mode, modes.size()))
    {
        return;
    }

    if(modes[active_mode].value == CM_ARGB_GEN2_A1_DIRECT_MODE)
    {
        DeviceUpdateLEDs();
        return;
    }

    DeviceUpdateMode();
}


std::vector<RGBController_CMARGBGen2A1Controller::ZoneState> RGBController_CMARGBGen2A1Controller::SnapshotZoneStates() const
{
    std::vector<ZoneState> saved;

    if(zone_targets.size() != zones.size())
    {
        return(saved);
    }

    saved.reserve(zones.size());

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); ++zone_idx)
    {
        ZoneState state;
        state.target = zone_targets[zone_idx];

        if(zone_idx < multilayer_zone_mode_values.size() && multilayer_zone_mode_values[zone_idx] >= 0)
        {
            state.mode_value = multilayer_zone_mode_values[zone_idx];
        }
        else if(IsValidIndex(zones[zone_idx].active_mode, zones[zone_idx].modes.size()))
        {
            state.mode_value = zones[zone_idx].modes[zones[zone_idx].active_mode].value;
        }

        std::size_t start = std::min<std::size_t>(zones[zone_idx].start_idx, colors.size());
        std::size_t end = colors.size();

        if(zone_idx + 1 < zones.size())
        {
            end = std::min<std::size_t>(zones[zone_idx + 1].start_idx, colors.size());
        }

        if(end >= start)
        {
            state.colors.assign(colors.begin() + start, colors.begin() + end);
        }

        saved.push_back(std::move(state));
    }

    return(saved);
}

void RGBController_CMARGBGen2A1Controller::RebuildLEDConfiguration()
{
    leds.clear();
    colors.clear();

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); ++zone_idx)
    {
        unsigned int port = zone_idx < zone_targets.size() ? zone_targets[zone_idx].port : (unsigned int)zone_idx;

        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; ++led_idx)
        {
            led new_led;
            new_led.name  = zones[zone_idx].name + ", LED " + std::to_string(led_idx + 1);
            new_led.value = port;
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

std::vector<std::vector<RGBColor>> RGBController_CMARGBGen2A1Controller::SnapshotZoneColors() const
{
    std::vector<std::vector<RGBColor>> saved(zones.size());

    /*-----------------------------------------------------*\
    | ConfigureZone() updates zones[zone].leds_count before |
    | DeviceConfigureZone() is called.  start_idx still     |
    | describes the old flattened color buffer at that      |
    | point, so derive each old slice from consecutive      |
    | start_idx values rather than from the already-mutated |
    | leds_count.                                           |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); ++zone_idx)
    {
        std::size_t start = std::min<std::size_t>(zones[zone_idx].start_idx, colors.size());
        std::size_t end = colors.size();

        if(zone_idx + 1 < zones.size())
        {
            end = std::min<std::size_t>(zones[zone_idx + 1].start_idx, colors.size());
        }

        if(end >= start)
        {
            saved[zone_idx].assign(colors.begin() + start, colors.begin() + end);
        }
    }

    return(saved);
}

void RGBController_CMARGBGen2A1Controller::RestoreZoneColors(const std::vector<std::vector<RGBColor>>& saved_colors)
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size() && zone_idx < saved_colors.size(); ++zone_idx)
    {
        std::size_t start = zones[zone_idx].start_idx;
        std::size_t count = std::min<std::size_t>(zones[zone_idx].leds_count, saved_colors[zone_idx].size());

        if(start <= colors.size() && count <= colors.size() - start)
        {
            std::copy_n(saved_colors[zone_idx].begin(), count, colors.begin() + start);
        }
    }
}

const RGBController_CMARGBGen2A1Controller::ZoneState* RGBController_CMARGBGen2A1Controller::FindPreviousZoneState(
    const std::vector<ZoneState>& states, const ZoneTarget& target) const
{
    for(std::size_t state_idx = 0; state_idx < states.size(); state_idx++)
    {
        const ZoneState& previous = states[state_idx];

        if(previous.target.port == target.port && previous.target.subtarget == target.subtarget
                                               && previous.target.gen2 == target.gen2)
        {
            return(&previous);
        }
    }

    if(target.gen2)
    {
        for(std::size_t state_idx = 0; state_idx < states.size(); state_idx++)
        {
            const ZoneState& previous = states[state_idx];

            if(previous.target.port == target.port && !previous.target.gen2)
            {
                return(&previous);
            }
        }
    }
    else
    {
        const ZoneState* source_state = nullptr;

        for(std::size_t state_idx = 0; state_idx < states.size(); state_idx++)
        {
            const ZoneState& previous = states[state_idx];

            if(previous.target.port != target.port || !previous.target.gen2)
            {
                continue;
            }

            if(source_state == nullptr || previous.colors.size() > source_state->colors.size())
            {
                source_state = &previous;
            }
        }

        return(source_state);
    }

    return(nullptr);
}

int RGBController_CMARGBGen2A1Controller::FindZoneModeIndex(const zone& current_zone, int mode_value) const
{
    for(std::size_t mode_idx = 0; mode_idx < current_zone.modes.size(); mode_idx++)
    {
        if(current_zone.modes[mode_idx].value == mode_value)
        {
            return((int)mode_idx);
        }
    }

    return(-1);
}

void RGBController_CMARGBGen2A1Controller::SetupZones()
{

    std::vector<ZoneState> previous_states = SnapshotZoneStates();

    zones.clear();
    zone_targets.clear();
    multilayer_zone_mode_values.clear();

    for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
    {
        const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(port);

        if(info.valid && info.generation == CM_ARGB_GEN2_A1_GENERATION_GEN2)
        {
            for(std::size_t device_idx = 0; device_idx < info.gen2_devices.size(); device_idx++)
            {
                const CMARGBGen2A1Subdevice& device = info.gen2_devices[device_idx];
                ZoneTarget target;
                target.port      = port;
                target.subtarget = device.mask;
                target.gen2      = true;

                unsigned int subport_number = 0;

                for(unsigned char mask = device.mask; mask != 0; mask >>= 1)
                {
                    subport_number++;
                }

                zone new_zone;
                new_zone.name              = "Port " + std::to_string(port + 1) + ", Gen2 Subport " + std::to_string(subport_number);
                new_zone.type              = ZONE_TYPE_LINEAR;
                new_zone.leds_count        = std::min<unsigned int>(device.led_count, CM_ARGB_GEN2_A1_TARGET_MAX_LEDS);
                new_zone.leds_min          = new_zone.leds_count;
                new_zone.leds_max          = new_zone.leds_count;
                new_zone.matrix_map.width  = 0;
                new_zone.matrix_map.height = 0;
                new_zone.matrix_map.map.clear();
                new_zone.segments.clear();

                for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
                {
                    if(modes[mode_idx].value <= CM_ARGB_GEN2_A1_OFF_MODE)
                    {
                        mode zone_mode = modes[mode_idx];
                        zone_mode.flags &= ~MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
                        new_zone.modes.push_back(zone_mode);
                    }
                }

                const ZoneState* previous = FindPreviousZoneState(previous_states, target);
                int restore_mode_value = previous != nullptr ? previous->mode_value : -1;

                if(IsValidIndex(active_mode, modes.size()) && modes[active_mode].value == CM_ARGB_GEN2_A1_MULTILAYER_MODE)
                {
                    new_zone.active_mode = FindZoneModeIndex(new_zone, restore_mode_value);
                }

                zones.push_back(std::move(new_zone));
                zone_targets.push_back(target);
                multilayer_zone_mode_values.push_back(restore_mode_value);
            }

            continue;
        }

        if(!gen1_zone_size_configured[port] && gen1_zone_led_counts[port] == 0 && info.gen1_leds_per_device != 0)
        {
            gen1_zone_led_counts[port] = DecodeGen1NominalLEDCount(info.gen1_leds_per_device);
        }

        ZoneTarget target;
        target.port      = port;
        target.subtarget = CM_ARGB_GEN2_A1_GEN1_SUBTARGET;
        target.gen2      = false;

        zone new_zone;
        new_zone.name       = "Port " + std::to_string(port + 1) + " (Gen1)";
        new_zone.type       = ZONE_TYPE_LINEAR;
        new_zone.flags      = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE;
        new_zone.leds_min   = CM_ARGB_GEN2_A1_GEN1_MIN_LEDS;
        new_zone.leds_max   = CM_ARGB_GEN2_A1_TARGET_MAX_LEDS;
        new_zone.leds_count = std::max<unsigned int>(CM_ARGB_GEN2_A1_GEN1_MIN_LEDS,
                                                     std::min<unsigned int>(gen1_zone_led_counts[port], CM_ARGB_GEN2_A1_TARGET_MAX_LEDS));

        if(gen1_zone_size_configured[port])
        {
            new_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SIZE;
        }

        new_zone.matrix_map.width  = 0;
        new_zone.matrix_map.height = 0;
        new_zone.matrix_map.map.clear();
        new_zone.segments.clear();

        for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
        {
            if(modes[mode_idx].value <= CM_ARGB_GEN2_A1_OFF_MODE)
            {
                mode zone_mode = modes[mode_idx];
                zone_mode.flags &= ~MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
                new_zone.modes.push_back(zone_mode);
            }
        }

        const ZoneState* previous = FindPreviousZoneState(previous_states, target);
        int restore_mode_value = previous != nullptr ? previous->mode_value : -1;

        if(IsValidIndex(active_mode, modes.size()) && modes[active_mode].value == CM_ARGB_GEN2_A1_MULTILAYER_MODE)
        {
            new_zone.active_mode = FindZoneModeIndex(new_zone, restore_mode_value);
        }

        zones.push_back(std::move(new_zone));
        zone_targets.push_back(target);
        multilayer_zone_mode_values.push_back(restore_mode_value);
    }

    RebuildLEDConfiguration();

    /*-----------------------------------------------------*\
    | Preserve colors by physical target across topology    |
    | rebuilds. Gen1 patterns seed new Gen2 targets; Gen2   |
    | collapse uses the longest previous logical target.    |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); ++zone_idx)
    {
        ZoneTarget& target = zone_targets[zone_idx];
        const ZoneState* source_state = nullptr;

        /*-------------------------------------------------*\
        | Preserve an exact logical target when possible.   |
        \*-------------------------------------------------*/
        for(std::size_t state_idx = 0; state_idx < previous_states.size(); state_idx++)
        {
            ZoneState& previous = previous_states[state_idx];
            if(previous.target.port == target.port && previous.target.subtarget == target.subtarget
                                                   && previous.target.gen2 == target.gen2)
            {
                source_state = &previous;
                break;
            }
        }

        if(source_state == nullptr && target.gen2)
        {
            /*---------------------------------------------*\
            | Seed each Gen2 target from the Gen1 pattern.  |
            \*---------------------------------------------*/
            for(std::size_t state_idx = 0; state_idx < previous_states.size(); state_idx++)
            {
                ZoneState& previous = previous_states[state_idx];
                if(previous.target.port == target.port && !previous.target.gen2)
                {
                    source_state = &previous;
                    break;
                }
            }
        }
        else if(source_state == nullptr && !target.gen2)
        {
            /*---------------------------------------------*\
            | Keep the longest prior Gen2 target as Gen1.   |
            \*---------------------------------------------*/
            for(std::size_t state_idx = 0; state_idx < previous_states.size(); state_idx++)
            {
                ZoneState& previous = previous_states[state_idx];
                if(previous.target.port != target.port || !previous.target.gen2)
                {
                    continue;
                }

                if(source_state == nullptr || previous.colors.size() > source_state->colors.size())
                {
                    source_state = &previous;
                }
            }
        }

        if(source_state == nullptr || source_state->colors.empty())
        {
            continue;
        }

        std::size_t copy_count = std::min<std::size_t>(zones[zone_idx].leds_count, source_state->colors.size());
        std::size_t start_idx = zones[zone_idx].start_idx;

        if(start_idx <= colors.size() && copy_count <= colors.size() - start_idx)
        {
            std::copy_n(source_state->colors.begin(), copy_count, colors.begin() + start_idx);
        }
    }

    RebuildDeviceSpecificConfiguration();
}

bool RGBController_CMARGBGen2A1Controller::SetGen1ZoneLEDCount(unsigned int port, unsigned int led_count)
{
    if(port >= controller->GetPortCount())
    {
        return(false);
    }

    unsigned char factor_a = 0;
    unsigned char factor_b = 0;

    if(!EncodeGen1OtherLEDCount(led_count, factor_a, factor_b))
    {
        return(false);
    }

    if(!controller->SetGen1Geometry(port, factor_a, factor_b))
    {
        return(false);
    }

    gen1_zone_led_counts[port] = led_count;
    return(true);
}

void RGBController_CMARGBGen2A1Controller::DeviceConfigureZone(int zone_idx)
{
    if(!IsValidIndex(zone_idx, zones.size()) || !IsValidIndex(zone_idx, zone_targets.size()))
    {
        return;
    }

    ZoneTarget target = zone_targets[zone_idx];
    unsigned int port = target.port;
    const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(port);
    std::vector<std::vector<RGBColor>> saved_colors = SnapshotZoneColors();

    /*-----------------------------------------------------*\
    | Gen2 zone size is hardware-owned by CMD07. Profiles   |
    | can contain an old saved size, so restore the current |
    | logical target's fixed geometry instead of accepting  |
    | a host resize.                                        |
    \*-----------------------------------------------------*/
    if(target.gen2)
    {
        unsigned int hardware_count = 0;

        for(std::size_t device_idx = 0; device_idx < info.gen2_devices.size(); device_idx++)
        {
            const CMARGBGen2A1Subdevice& device = info.gen2_devices[device_idx];

            if(device.mask == target.subtarget)
            {
                hardware_count = std::min<unsigned int>(device.led_count, CM_ARGB_GEN2_A1_TARGET_MAX_LEDS);
                break;
            }
        }

        zones[zone_idx].type       = ZONE_TYPE_LINEAR;
        zones[zone_idx].flags      = 0;
        zones[zone_idx].leds_count = hardware_count;
        zones[zone_idx].leds_min   = hardware_count;
        zones[zone_idx].leds_max   = hardware_count;
        zones[zone_idx].segments.clear();
        RebuildLEDConfiguration();
        RestoreZoneColors(saved_colors);

        if(IsValidIndex(active_mode, modes.size()) && (modes[active_mode].value == CM_ARGB_GEN2_A1_DIRECT_MODE
                                                   || modes[active_mode].value == CM_ARGB_GEN2_A1_CUSTOM_MODE))
        {
            DeviceUpdateLEDs();
        }

        return;
    }

    unsigned int requested_count = zones[zone_idx].leds_count;
    unsigned int previous_count = gen1_zone_led_counts[port];
    bool previous_configured = gen1_zone_size_configured[port];
    bool was_realtime = controller->IsRealtimeMode();

    if(!previous_configured && previous_count == 0 && info.gen1_leds_per_device != 0)
    {
        previous_count = DecodeGen1NominalLEDCount(info.gen1_leds_per_device);
    }

    previous_count = std::max<unsigned int>(CM_ARGB_GEN2_A1_GEN1_MIN_LEDS,
                                            std::min<unsigned int>(previous_count, CM_ARGB_GEN2_A1_TARGET_MAX_LEDS));

    if(was_realtime && !controller->ExitRealtimeMode())
    {
        zones[zone_idx].leds_count = previous_count;
        if(previous_configured)
        {
            zones[zone_idx].flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SIZE;
        }
        else
        {
            zones[zone_idx].flags &= ~ZONE_FLAG_MANUALLY_CONFIGURED_SIZE;
        }
        RebuildLEDConfiguration();
        RestoreZoneColors(saved_colors);
        return;
    }

    bool applied = SetGen1ZoneLEDCount(port, requested_count);

    if(applied)
    {
        gen1_zone_led_counts[port]      = requested_count;
        gen1_zone_size_configured[port] = true;
        zones[zone_idx].leds_count      = requested_count;
        zones[zone_idx].flags          |= ZONE_FLAG_MANUALLY_CONFIGURED_SIZE;
    }
    else
    {
        zones[zone_idx].leds_count = previous_count;
        gen1_zone_led_counts[port] = previous_count;
        gen1_zone_size_configured[port] = previous_configured;

        if(previous_configured)
        {
            zones[zone_idx].flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SIZE;
        }
        else
        {
            zones[zone_idx].flags &= ~ZONE_FLAG_MANUALLY_CONFIGURED_SIZE;
        }
    }

    RebuildLEDConfiguration();
    RestoreZoneColors(saved_colors);

    if(IsValidIndex(active_mode, modes.size()) && (modes[active_mode].value == CM_ARGB_GEN2_A1_DIRECT_MODE
                                               || modes[active_mode].value == CM_ARGB_GEN2_A1_CUSTOM_MODE))
    {
        DeviceUpdateLEDs();
    }
}

void RGBController_CMARGBGen2A1Controller::UpdateZoneCustomColors(unsigned int zone_idx)
{
    if(zone_idx >= zones.size() || zone_idx >= zone_targets.size())
    {
        return;
    }

    ZoneTarget& target = zone_targets[zone_idx];
    const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(target.port);

    if(!info.valid || zones[zone_idx].leds_count == 0)
    {
        return;
    }

    unsigned int color_offset = zones[zone_idx].start_idx;
    unsigned int count = std::min<unsigned int>(zones[zone_idx].leds_count, CM_ARGB_GEN2_A1_TARGET_MAX_LEDS);

    if(color_offset + count > colors.size())
    {
        return;
    }

    std::vector<RGBColor> target_colors(colors.begin() + color_offset, colors.begin() + color_offset + count);

    controller->SendCustomColors(info.outer_mask, target.subtarget, target_colors);
}

std::vector<CMARGBGen2A1RealtimeTarget> RGBController_CMARGBGen2A1Controller::BuildRealtimeTargets() const
{
    std::vector<CMARGBGen2A1RealtimeTarget> targets;

    for(std::size_t zone_idx = 0; zone_idx < zones.size() && zone_idx < zone_targets.size(); ++zone_idx)
    {
        const ZoneTarget& zone_target = zone_targets[zone_idx];
        const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(zone_target.port);

        if(!info.valid || zones[zone_idx].leds_count == 0)
        {
            continue;
        }

        unsigned int color_offset = zones[zone_idx].start_idx;
        unsigned int count = std::min<unsigned int>(zones[zone_idx].leds_count, CM_ARGB_GEN2_A1_TARGET_MAX_LEDS);

        if(color_offset + count > colors.size())
        {
            continue;
        }

        CMARGBGen2A1RealtimeTarget target;
        target.outer_mask = info.outer_mask;
        target.subtarget  = zone_target.subtarget;
        target.colors.assign(colors.begin() + color_offset, colors.begin() + color_offset + count);
        targets.push_back(std::move(target));
    }

    return(targets);
}

void RGBController_CMARGBGen2A1Controller::DeviceUpdateLEDs()
{
    if(!IsValidIndex(active_mode, modes.size()))
    {
        return;
    }

    mode& global_mode = modes[active_mode];

    if(global_mode.value == CM_ARGB_GEN2_A1_DIRECT_MODE)
    {
        controller->SetRealtimeColors(BuildRealtimeTargets());
        return;
    }

    if(global_mode.value == CM_ARGB_GEN2_A1_CUSTOM_MODE)
    {
        controller->ExitRealtimeMode();

        for(unsigned int zone_idx = 0; zone_idx < zones.size(); ++zone_idx)
        {
            UpdateZoneCustomColors(zone_idx);
        }
    }
}

void RGBController_CMARGBGen2A1Controller::DeviceUpdateZoneLEDs(int zone)
{
    if(!IsValidIndex(zone, zones.size()) || !IsValidIndex(active_mode, modes.size()))
    {
        return;
    }

    mode& global_mode = modes[active_mode];

    if(global_mode.value == CM_ARGB_GEN2_A1_DIRECT_MODE)
    {
        DeviceUpdateLEDs();
        return;
    }

    if(global_mode.value == CM_ARGB_GEN2_A1_CUSTOM_MODE)
    {
        controller->ExitRealtimeMode();
        UpdateZoneCustomColors((unsigned int)zone);
    }
}

int RGBController_CMARGBGen2A1Controller::GetLEDZone(int led_idx) const
{
    if(led_idx < 0)
    {
        return(-1);
    }

    unsigned int index = (unsigned int)led_idx;

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); ++zone_idx)
    {
        unsigned int start = zones[zone_idx].start_idx;
        unsigned int end = start + zones[zone_idx].leds_count;

        if(index >= start && index < end)
        {
            return((int)zone_idx);
        }
    }

    return(-1);
}

void RGBController_CMARGBGen2A1Controller::DeviceUpdateSingleLED(int led)
{
    int zone = GetLEDZone(led);

    if(zone >= 0)
    {
        /*-------------------------------------------------*\
        | The hardware streams/stores a complete target RGB |
        | frame, so update only the containing logical      |
        | zone.                                             |
        \*-------------------------------------------------*/
        DeviceUpdateZoneLEDs(zone);
    }
}

bool RGBController_CMARGBGen2A1Controller::EnsureMultilayerMode(unsigned char requested_mode)
{
    if(multilayer_mode == requested_mode)
    {
        return(true);
    }

    if(!controller->SetMultilayerMode(requested_mode))
    {
        return(false);
    }

    multilayer_mode = requested_mode;
    return(true);
}

bool RGBController_CMARGBGen2A1Controller::ApplyOverlayGate(bool enabled, unsigned char interval)
{
    if(!controller->SetOverlayMode(enabled, interval))
    {
        return(false);
    }

    overlay_gate_active = enabled;
    return(true);
}

bool RGBController_CMARGBGen2A1Controller::ApplyPendingOverlayTables()
{
    bool have_pending_table = false;

    for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
    {
        have_pending_table |= overlay_table_dirty[port];
    }

    if(!have_pending_table)
    {
        return(true);
    }

    if(!overlay_enabled && !ApplyOverlayGate(false, overlay_interval))
    {
        return(false);
    }

    for(unsigned int port = 0; port < controller->GetPortCount(); ++port)
    {
        if(!overlay_table_dirty[port])
        {
            continue;
        }

        if(overlay_enabled && !ApplyOverlayGate(true, overlay_interval))
        {
            return(false);
        }

        if(!controller->SetOverlayTable(port, overlay_tables[port]))
        {
            return(false);
        }

        overlay_table_dirty[port] = false;
    }

    return(true);
}

bool RGBController_CMARGBGen2A1Controller::ApplyGlobalMode(const mode& selected_mode)
{
    RGBColor color = selected_mode.color_mode == MODE_COLORS_MODE_SPECIFIC && !selected_mode.colors.empty()
                   ? selected_mode.colors[0] : ToRGBColor(0xFF, 0xFF, 0xFF);
    bool random = selected_mode.color_mode == MODE_COLORS_RANDOM;

    return(controller->SetEffect(CM_ARGB_GEN2_A1_CHANNEL_ALL, CM_ARGB_GEN2_A1_SUBCHANNEL_ALL, selected_mode.value,
                                 selected_mode.speed, selected_mode.brightness, color, random));
}

void RGBController_CMARGBGen2A1Controller::ApplyModeToZone(unsigned int zone_idx, const mode& selected_mode)
{
    if(zone_idx >= zones.size() || zone_idx >= zone_targets.size() || (selected_mode.flags & MODE_FLAG_REQUIRES_ENTIRE_DEVICE))
    {
        return;
    }

    ZoneTarget& target = zone_targets[zone_idx];
    const CMARGBGen2A1PortInfo& info = controller->GetPortInfo(target.port);

    if(!info.valid)
    {
        return;
    }

    RGBColor color = selected_mode.color_mode == MODE_COLORS_MODE_SPECIFIC && !selected_mode.colors.empty() ? selected_mode.colors[0] : ToRGBColor(0xFF, 0xFF, 0xFF);
    bool random = selected_mode.color_mode == MODE_COLORS_RANDOM;

    controller->SetEffect(info.outer_mask, target.subtarget, selected_mode.value, selected_mode.speed, selected_mode.brightness, color, random);
}

void RGBController_CMARGBGen2A1Controller::PrepareMultilayerModes()
{
    if(multilayer_zone_mode_values.size() != zones.size())
    {
        multilayer_zone_mode_values.resize(zones.size(), -1);
    }

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        int mode_value = multilayer_zone_mode_values[zone_idx];

        if(mode_value < 0 && IsValidIndex(zones[zone_idx].active_mode, zones[zone_idx].modes.size()))
        {
            mode_value = zones[zone_idx].modes[zones[zone_idx].active_mode].value;
        }

        int mode_idx = FindZoneModeIndex(zones[zone_idx], mode_value);

        if(mode_idx < 0)
        {
            mode_idx = FindZoneModeIndex(zones[zone_idx], last_global_effect_value);
        }

        if(mode_idx < 0)
        {
            mode_idx = FindZoneModeIndex(zones[zone_idx], CM_ARGB_GEN2_A1_STATIC_MODE);
        }

        if(mode_idx < 0)
        {
            continue;
        }

        zones[zone_idx].active_mode = mode_idx;
        multilayer_zone_mode_values[zone_idx] = zones[zone_idx].modes[mode_idx].value;
    }
}

void RGBController_CMARGBGen2A1Controller::ApplyMultilayerModes()
{
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(!IsValidIndex(zones[zone_idx].active_mode, zones[zone_idx].modes.size()))
        {
            continue;
        }

        ApplyModeToZone(zone_idx, zones[zone_idx].modes[zones[zone_idx].active_mode]);
    }
}

void RGBController_CMARGBGen2A1Controller::DeviceUpdateMode()
{
    if(!IsValidIndex(active_mode, modes.size()))
    {
        return;
    }

    mode& global_mode = modes[active_mode];

    if(global_mode.value == CM_ARGB_GEN2_A1_DIRECT_MODE)
    {
        DeviceUpdateLEDs();
        return;
    }

    if(!controller->ExitRealtimeMode())
    {
        return;
    }

    if(global_mode.value == CM_ARGB_GEN2_A1_CUSTOM_MODE)
    {
        if(!EnsureMultilayerMode(CM_ARGB_GEN2_A1_MULTILAYER_GLOBAL))
        {
            return;
        }

        if(!controller->SetEffect(CM_ARGB_GEN2_A1_CHANNEL_ALL, CM_ARGB_GEN2_A1_SUBCHANNEL_ALL, CM_ARGB_GEN2_A1_CUSTOM_MODE,
                                  CM_ARGB_GEN2_A1_SPEED_HALF, CM_ARGB_GEN2_A1_BRIGHTNESS_MAX, ToRGBColor(0xFF, 0xFF, 0xFF), false))
        {
            return;
        }

        for(unsigned int zone_idx = 0; zone_idx < zones.size(); ++zone_idx)
        {
            UpdateZoneCustomColors(zone_idx);
        }

        if(!ApplyPendingOverlayTables())
        {
            return;
        }

        if(!ApplyOverlayGate(overlay_enabled, overlay_interval))
        {
            return;
        }

        return;
    }

    if(!ApplyOverlayGate(false, overlay_interval))
    {
        return;
    }

    if(global_mode.value == CM_ARGB_GEN2_A1_MULTILAYER_MODE)
    {
        if(!EnsureMultilayerMode(CM_ARGB_GEN2_A1_MULTILAYER_INDEPENDENT))
        {
            return;
        }

        PrepareMultilayerModes();
        ApplyMultilayerModes();
        return;
    }

    if(!EnsureMultilayerMode(CM_ARGB_GEN2_A1_MULTILAYER_GLOBAL))
    {
        return;
    }

    if(ApplyGlobalMode(global_mode) && global_mode.value <= CM_ARGB_GEN2_A1_OFF_MODE)
    {
        last_global_effect_value = global_mode.value;
    }
}

void RGBController_CMARGBGen2A1Controller::DeviceUpdateZoneMode(int zone)
{
    if(!IsValidIndex(zone, zones.size()) || !IsValidIndex(active_mode, modes.size())
                                         || modes[active_mode].value != CM_ARGB_GEN2_A1_MULTILAYER_MODE)
    {
        return;
    }

    if(!IsValidIndex(zones[zone].active_mode, zones[zone].modes.size()))
    {
        return;
    }

    if(!EnsureMultilayerMode(CM_ARGB_GEN2_A1_MULTILAYER_INDEPENDENT))
    {
        return;
    }

    if(multilayer_zone_mode_values.size() != zones.size())
    {
        multilayer_zone_mode_values.resize(zones.size(), -1);
    }

    mode& selected_mode = zones[zone].modes[zones[zone].active_mode];
    multilayer_zone_mode_values[zone] = selected_mode.value;
    ApplyModeToZone((unsigned int)zone, selected_mode);
}

void RGBController_CMARGBGen2A1Controller::DeviceSaveMode()
{
    if(IsValidIndex(active_mode, modes.size()) && modes[active_mode].value != CM_ARGB_GEN2_A1_DIRECT_MODE)
    {
        controller->SaveToFlash();
    }
}

void RGBController_CMARGBGen2A1Controller::SetDeviceSpecificConfiguration(nlohmann::json configuration_json)
{
    std::vector<ZoneTarget> previous_targets = zone_targets;
    std::vector<unsigned int> previous_led_counts;
    previous_led_counts.reserve(zones.size());

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        previous_led_counts.push_back(zones[zone_idx].leds_count);
    }

    /*-----------------------------------------------------*\
    | Let the normal OpenRGB configuration path validate    |
    | and apply the JSON first. It releases AccessMutex     |
    | before returning here.                                |
    \*-----------------------------------------------------*/
    RGBController::SetDeviceSpecificConfiguration(configuration_json);

    bool topology_changed = previous_targets.size() != zone_targets.size() || previous_led_counts.size() != zones.size();

    if(!topology_changed)
    {
        for(std::size_t zone_idx = 0; zone_idx < zone_targets.size(); ++zone_idx)
        {
            ZoneTarget& previous = previous_targets[zone_idx];
            ZoneTarget& current  = zone_targets[zone_idx];

            if(previous.port != current.port || previous.subtarget != current.subtarget || previous.gen2 != current.gen2
                                             || previous_led_counts[zone_idx] != zones[zone_idx].leds_count)
            {
                topology_changed = true;
                break;
            }
        }
    }

    /*-----------------------------------------------------*\
    | A generation change can alter the number or identity  |
    | of logical zones. Request a normal device-page        |
    | rebuild only when that public topology changed.       |
    \*-----------------------------------------------------*/
    if(topology_changed)
    {
        SignalUpdate(RGBCONTROLLER_UPDATE_REASON_CONFIGUREDEVICE);
    }
}

void RGBController_CMARGBGen2A1Controller::DeviceConfigureDevice()
{
    /*-----------------------------------------------------*\
    | ConfigureDevice() is called before the                |
    | device-specific JSON is applied.  Resetting device    |
    | configuration must therefore be read-only: import the |
    | controller's current generation state rather than     |
    | writing CMD0B here.                                   |
    \*-----------------------------------------------------*/
    if((flags & CONTROLLER_FLAG_MANUALLY_CONFIGURED_DEVICE_SPECIFIC) == 0)
    {
        controller->RefreshTopology();
        InitDeviceSpecificConfiguration();
        SetupZones();
        ImportDeviceLightingState();
    }
}

void RGBController_CMARGBGen2A1Controller::DeviceUpdateDeviceSpecificConfiguration()
{
    ApplyDeviceSpecificConfiguration(true);
}
