/*---------------------------------------------------------*\
| RGBController_ZotacV2GPU.cpp                              |
|                                                           |
|   RGBController for Zotac V2 GPU                          |
|                                                           |
|   Krzysztof Haładyn (krzys_h)                 16 Mar 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <map>
#include "RGBController_ZotacV2GPU.h"
#include "LogManager.h"

std::map<std::string, ZotacV2GPUConfig> ZOTAC_V2_GPU_CONFIG =
{
    { "N653E-1013", { 2, false } },  // ZOTAC GAMING GeForce RTX 3070 Ti Trinity OC
    { "N653A-1013", { 1, false } },  // ZOTAC GAMING GeForce RTX 3070 Ti AMP Holo
    { "N612E-1011", { 2, false } },  // ZOTAC GAMING GeForce RTX 3080 Trinity OC LHR 12GB & 3090 Trinity & 3070 Ti
    { "N612A-1012", { 2, false } },  // ZOTAC GAMING GeForce RTX 3080 Ti AMP Holo
    { "N617E-1011", { 3, false } },  // ZOTAC GAMING GeForce RTX 3070 AMP Holo LHR
    { "N618A-1015", { 4, true  } },  // ZOTAC GAMING GeForce RTX 3090 AMP Extreme Holo
    { "N696E-1040", { 1, false } },  // ZOTAC GAMING GeForce RTX 4070 Ti Trinity OC
    { "N675E-1019", { 1, true  } },  // ZOTAC GAMING GeForce RTX 4090 Trinity OC
    { "N675E-1062", { 1, true  } },  // ZOTAC GAMING GeForce RTX 4090 Trinity OC Alternate Controller Version
    { "N675A-1019", { 5, true  } },  // ZOTAC GAMING GeForce RTX 4080 16GB AMP Extreme AIRO
    { "N675A-1062", { 5, true  } },  // ZOTAC GAMING GeForce RTX 4090 AMP Extreme AIRO
};

std::vector<std::pair<RGBColor, RGBColor>> ZOTAC_V2_GPU_DUET_PRESETS =
{
    { ToRGBColor(0x32, 0xCF, 0xA7), ToRGBColor(0x93, 0x34, 0xC2) },
    { ToRGBColor(0x00, 0xC9, 0x14), ToRGBColor(0x00, 0x20, 0xF5) },
    { ToRGBColor(0xD1, 0xFC, 0x00), ToRGBColor(0xF1, 0x0C, 0x00) },
    { ToRGBColor(0xFF, 0x68, 0x7C), ToRGBColor(0xD4, 0x00, 0x4D) },
};

/**------------------------------------------------------------------*\
    @name ZOTAC 30/40 series GPU
    @category GPU
    @type I2C
    @save :robot:
    @direct :x:
    @effects :tools:
    @detectors DetectZotacV2GPUControllers
    @comment
        OpenRGB does not support per-zone effect modes, so only
        the synchronized mode is supported for now. Sound based
        effects are not supported. Idle/active config is not
        supported.

        To add new cards, in addition to entries in `pci_ids/pci_ids.h`
        and `Controllers/ZotacV2GPUController/ZotacV2GPUControllerDetect.cpp`
        an entry associating the controller version with the LED
        configuration must be added to the [ZotacV2GPUConfig map](https://gitlab.com/CalcProgrammer1/OpenRGB/-/blob/master/Controllers/ZotacV2GPUController/RGBController_ZotacV2GPU.cpp?ref_type=heads#L14) in
        `Controllers/ZotacV2GPUController/RGBController_ZotacV2GPU.cpp`.

        Controller version is identified by polling the controller on address
        0x49 (so far for all known cards) at register 0xA0 with the following
        packet `0xF1 0x00 0x00 0x00 0x00 0x00 0x00`

        The first ten bytes when read back from that address are the version,
        this must be converted into Unicode.

        The polling and reading is done by the ZOTAC Firestorm app on startup
        and can be monitored using NvAPIspy on Windows.

        For RTX 3080 Trinity OC LHR 12GB the relevant NvAPISpy log entries are:

        > ```plaintext
        > NvAPI_I2CWrite:  Dev: 0x49 RegSize: 0x00 Reg: Size: 0x08 Data: 0xA0 0xF1 0x00 0x00 0x00 0x00 0x00 0x00
        > NvAPI_I2CRead:  Dev: 0x49 RegSize: 0x01 Reg: 0xA0 Size: 0x20 Data: 0x4E 0x36 0x31 0x32 0x45 0x2D 0x31 0x30 0x31 0x31 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
        > ```

        The read data `0x4E 0x36 0x31 0x32 0x45 0x2D 0x31 0x30 0x31 0x31` converts
        to Unicode `N612E-1011`

        Cards with correct entries in `pci_ids/pci_ids.h` and `Controllers/ZotacV2GPUController/ZotacV2GPUControllerDetect.cpp`
        but not in `Controllers/ZotacV2GPUController/RGBController_ZotacV2GPU.cpp`
        should produce a log error with the controller version read from the card.

        The LED configuration is the number of independently configurable
        zones followed by a bool representing support for external led stip.
        These can be determined by looking at the Spectra tab in Firestorm.
\*-------------------------------------------------------------------*/

RGBController_ZotacV2GPU::RGBController_ZotacV2GPU(ZotacV2GPUController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "ZOTAC";
    description             = "ZOTAC 30/40 series RGB GPU Device (" + controller->GetVersion() + ")";
    location                = controller->GetDeviceLocation();
    type                    = DEVICE_TYPE_GPU;

    if(ZOTAC_V2_GPU_CONFIG.count(controller->GetVersion()) > 0)
    {
        config = ZOTAC_V2_GPU_CONFIG.at(controller->GetVersion());
    }
    else
    {
        LOG_ERROR("[%s] Unrecognized controller version %s", name.c_str(), controller->GetVersion().c_str());
        config = { 0, false };
    }


    version += std::to_string(config.numberOfZones) + " zones, "
            + (config.supportsExternalLEDStrip ? "with" : "without") + " external LED strip support";

    mode STATIC;
    STATIC.name             = "Static";
    STATIC.value            = ZOTAC_V2_GPU_MODE_STATIC;
    STATIC.flags            = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    STATIC.brightness_min   = 0;
    STATIC.brightness_max   = 100;
    STATIC.brightness       = 100;
    STATIC.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    STATIC.colors_min       = 1;
    STATIC.colors_max       = 1;
    STATIC.colors.resize(1);
    STATIC.colors[0]        = ToRGBColor(0, 0, 255);
    modes.push_back(STATIC);

    mode BREATH;
    BREATH.name             = "Breath";
    BREATH.value            = ZOTAC_V2_GPU_MODE_BREATH;
    BREATH.flags            = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    BREATH.brightness_min   = 0;
    BREATH.brightness_max   = 100;
    BREATH.brightness       = 100;
    BREATH.speed_min        = 0;
    BREATH.speed_max        = 100;
    BREATH.speed            = 20;
    BREATH.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    BREATH.colors_min       = 1;
    BREATH.colors_max       = 1;
    BREATH.colors.resize(1);
    BREATH.colors[0]        = ToRGBColor(0, 0, 255);
    modes.push_back(BREATH);

    mode FADE;
    FADE.name               = "Fade";
    FADE.value              = ZOTAC_V2_GPU_MODE_FADE;
    FADE.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED;
    FADE.speed_min          = 0;
    FADE.speed_max          = 100;
    FADE.speed              = 20;
    FADE.color_mode         = MODE_COLORS_NONE;
    modes.push_back(FADE);

    mode WINK;
    WINK.name               = "Wink";
    WINK.value              = ZOTAC_V2_GPU_MODE_WINK;
    WINK.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    WINK.brightness_min     = 0;
    WINK.brightness_max     = 100;
    WINK.brightness         = 100;
    WINK.speed_min          = 0;
    WINK.speed_max          = 100;
    WINK.speed              = 20;
    WINK.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    WINK.colors_min         = 1;
    WINK.colors_max         = 1;
    WINK.colors.resize(1);
    WINK.colors[0]          = ToRGBColor(0, 0, 255);
    modes.push_back(WINK);

    if(config.numberOfZones > 1)
    {
        // This mode is only supported on GPUs with more than one zone,
        // because it spans multiple zones.

        // It's also supported in synchronized mode only (which is the only
        // thing this RGBController supports for now anyway)

        mode FLASH;
        FLASH.name          = "Flash";
        FLASH.value         = ZOTAC_V2_GPU_MODE_FLASH;
        FLASH.flags         = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED;
        FLASH.speed_min     = 0;
        FLASH.speed_max     = 100;
        FLASH.speed         = 20;
        FLASH.color_mode    = MODE_COLORS_NONE;
        modes.push_back(FLASH);
    }

    // (Sound activated - not supported)
    //mode SHINE;
    //SHINE.name            = "Shine";
    //SHINE.value           = ZOTAC_V2_GPU_MODE_SHINE;
    //SHINE.flags           = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    //SHINE.brightness_min  = 0;
    //SHINE.brightness_max  = 100;
    //SHINE.brightness      = 100;
    //SHINE.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    //SHINE.colors_min      = 1;
    //SHINE.colors_max      = 1;
    //SHINE.colors.resize(1);
    //SHINE.colors[0]       = ToRGBColor(0, 0, 255);
    //modes.push_back(SHINE);

    mode RANDOM;
    RANDOM.name             = "Random";
    RANDOM.value            = ZOTAC_V2_GPU_MODE_RANDOM;
    RANDOM.flags            = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    RANDOM.brightness_min   = 0;
    RANDOM.brightness_max   = 100;
    RANDOM.brightness       = 100;
    RANDOM.speed_min        = 0;
    RANDOM.speed_max        = 100;
    RANDOM.speed            = 20;
    RANDOM.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    RANDOM.colors_min       = 1;
    RANDOM.colors_max       = 1;
    RANDOM.colors.resize(1);
    RANDOM.colors[0]        = ToRGBColor(0, 0, 255);
    modes.push_back(RANDOM);

    mode SLIDE;
    SLIDE.name              = "Slide";
    SLIDE.value             = ZOTAC_V2_GPU_MODE_SLIDE;
    SLIDE.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    SLIDE.brightness_min    = 0;
    SLIDE.brightness_max    = 100;
    SLIDE.brightness        = 100;
    SLIDE.speed_min         = 0;
    SLIDE.speed_max         = 100;
    SLIDE.speed             = 20;
    SLIDE.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    SLIDE.colors_min        = 1;
    SLIDE.colors_max        = 1;
    SLIDE.colors.resize(1);
    SLIDE.colors[0]         = ToRGBColor(0, 0, 255);
    modes.push_back(SLIDE);

    mode RAINBOW;
    RAINBOW.name            = "Rainbow";
    RAINBOW.value           = ZOTAC_V2_GPU_MODE_RAINBOW;
    RAINBOW.flags           = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RAINBOW.speed_min       = 0;
    RAINBOW.speed_max       = 100;
    RAINBOW.speed           = 20;
    RAINBOW.color_mode      = MODE_COLORS_NONE;
    modes.push_back(RAINBOW);
    mode RAINBOW_CIRCUIT    = RAINBOW;
    RAINBOW_CIRCUIT.name    = "Rainbow (circuit)";
    modes.push_back(RAINBOW_CIRCUIT);

    mode MARQUEE;
    MARQUEE.name            = "Marquee";
    MARQUEE.value           = ZOTAC_V2_GPU_MODE_MARQUEE;
    MARQUEE.flags           = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    MARQUEE.brightness_min  = 0;
    MARQUEE.brightness_max  = 100;
    MARQUEE.brightness      = 100;
    MARQUEE.speed_min       = 0;
    MARQUEE.speed_max       = 100;
    MARQUEE.speed           = 20;
    MARQUEE.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    MARQUEE.colors_min      = 1;
    MARQUEE.colors_max      = 1;
    MARQUEE.colors.resize(1);
    MARQUEE.colors[0]       = ToRGBColor(0, 0, 255);
    modes.push_back(MARQUEE);
    mode MARQUEE_CIRCUIT    = MARQUEE;
    MARQUEE_CIRCUIT.name    = "Marquee (circuit)";
    modes.push_back(MARQUEE_CIRCUIT);

    mode DRIP;
    DRIP.name               = "Drip";
    DRIP.value              = ZOTAC_V2_GPU_MODE_DRIP;
    DRIP.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    DRIP.brightness_min     = 0;
    DRIP.brightness_max     = 100;
    DRIP.brightness         = 100;
    DRIP.speed_min          = 0;
    DRIP.speed_max          = 100;
    DRIP.speed              = 20;
    DRIP.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    DRIP.colors_min         = 1;
    DRIP.colors_max         = 1;
    DRIP.colors.resize(1);
    DRIP.colors[0]          = ToRGBColor(0, 0, 255);
    modes.push_back(DRIP);
    mode DRIP_CIRCUIT = DRIP;
    DRIP_CIRCUIT.name       = "Drip (circuit)";
    modes.push_back(DRIP_CIRCUIT);

    // (Sound activated - not supported)
    //mode DANCE;
    //DANCE.name            = "Dance (sound activated)";
    //DANCE.value           = ZOTAC_V2_GPU_MODE_DANCE;
    //DANCE.flags           = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    //DANCE.brightness_min  = 0;
    //DANCE.brightness_max  = 100;
    //DANCE.brightness      = 100;
    //DANCE.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    //DANCE.colors_min      = 1;
    //DANCE.colors_max      = 1;
    //DANCE.colors.resize(1);
    //DANCE.colors[0]       = ToRGBColor(0, 0, 255);
    //modes.push_back(DANCE);

    mode DUET;
    DUET.name               = "Duet";
    DUET.value              = ZOTAC_V2_GPU_MODE_DUET;
    DUET.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    DUET.speed_min          = 0;
    DUET.speed_max          = 100;
    DUET.speed              = 20;
    DUET.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    DUET.colors_min         = 2;
    DUET.colors_max         = 2;
    DUET.colors.resize(2);
    DUET.colors[0]          = ZOTAC_V2_GPU_DUET_PRESETS[1].first;
    DUET.colors[1]          = ZOTAC_V2_GPU_DUET_PRESETS[1].second;
    modes.push_back(DUET);
    mode DUET_CIRCUIT       = DUET;
    DUET_CIRCUIT.name       = "Duet (circuit)";
    modes.push_back(DUET_CIRCUIT);


    mode PATH;
    PATH.name               = "Path";
    PATH.value              = ZOTAC_V2_GPU_MODE_PATH;
    PATH.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    PATH.brightness_min     = 0;
    PATH.brightness_max     = 100;
    PATH.brightness         = 100;
    PATH.speed_min          = 0;
    PATH.speed_max          = 100;
    PATH.speed              = 20;
    PATH.color_mode         = MODE_COLORS_NONE;
    modes.push_back(PATH);

    SetupZones();
}

RGBController_ZotacV2GPU::~RGBController_ZotacV2GPU()
{
    delete controller;
}

void RGBController_ZotacV2GPU::SetupZones()
{
    led new_led;
    new_led.name           = "GPU LED";
    leds.push_back(new_led);

    zone new_zone;
    new_zone.name          = "GPU Zone";
    new_zone.type          = ZONE_TYPE_SINGLE;
    new_zone.leds_min      = 1;
    new_zone.leds_max      = 1;
    new_zone.leds_count    = 1;
    new_zone.matrix_map    = NULL;
    zones.push_back(new_zone);

    SetupColors();
    SetupInitialValues();
}

void RGBController_ZotacV2GPU::SetupInitialValues()
{
    /*---------------------------------------------------------*\
    | Retrieve current values by reading the device             |
    \*---------------------------------------------------------*/

    bool on;
    int syncMode;
    ZotacV2GPUZone zoneConfig;

    // We don't support anything other than synchronized mode, so read the last
    // config used in synchronized mode for idle settings.
    int zoneNum = FindSynchronizedZoneNum(ZOTAC_V2_GPU_SYNC_SYNCHRONIZED);
    if(!controller->GetMode(zoneNum, ZOTAC_V2_GPU_CONFIG_IDLE, syncMode, zoneConfig, on))
    {
        return;
    }

    for(unsigned int i = 0; i < modes.size(); ++i)
    {
        if(zoneConfig.mode != modes[i].value)
        {
            continue;
        }

        if(zoneConfig.mode == ZOTAC_V2_GPU_MODE_RAINBOW ||
            zoneConfig.mode == ZOTAC_V2_GPU_MODE_MARQUEE ||
            zoneConfig.mode == ZOTAC_V2_GPU_MODE_DRIP ||
            zoneConfig.mode == ZOTAC_V2_GPU_MODE_DUET)
        {
            if((zoneConfig.circuit == ZOTAC_V2_GPU_CIRCUIT_ON) != (modes[i].name.find("(circuit)") != std::string::npos))
            {
                continue;
            }
        }

        active_mode = i;
    }

    colors[0] = zoneConfig.color1;
    if(modes[active_mode].colors.size() >= 1)
    {
        modes[active_mode].colors[0] = zoneConfig.color1;
    }
    if(modes[active_mode].colors.size() >= 2)
    {
        modes[active_mode].colors[1] = zoneConfig.color2;
    }
    modes[active_mode].speed = zoneConfig.speed;
    modes[active_mode].brightness = zoneConfig.brightness;
    modes[active_mode].direction = zoneConfig.direction;

    SignalUpdate();
}

void RGBController_ZotacV2GPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ZotacV2GPU::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_ZotacV2GPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateMode();
}

void RGBController_ZotacV2GPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateMode();
}

void RGBController_ZotacV2GPU::DeviceUpdateMode()
{
    ZotacV2GPUZone zoneConfig;
    zoneConfig.mode = modes[active_mode].value;

    zoneConfig.color1 = modes[active_mode].colors.size() >= 1 ? modes[active_mode].colors[0] : ToRGBColor(0, 0, 0);
    zoneConfig.color2 = modes[active_mode].colors.size() >= 2 ? modes[active_mode].colors[1] : ToRGBColor(0, 0, 0);

    // This is probably not strictly neccessary
    zoneConfig.colorPreset = 0;
    if(zoneConfig.mode == ZOTAC_V2_GPU_MODE_DUET)
    {
        zoneConfig.colorPreset = (unsigned int)ZOTAC_V2_GPU_DUET_PRESETS.size();  // custom
        for(size_t i = 0; i < ZOTAC_V2_GPU_DUET_PRESETS.size(); ++i)
        {
            if(zoneConfig.color1 == ZOTAC_V2_GPU_DUET_PRESETS[i].first &&
                zoneConfig.color2 == ZOTAC_V2_GPU_DUET_PRESETS[i].second)
            {
                zoneConfig.colorPreset = (unsigned int)i;
            }
        }
    }

    zoneConfig.speed = modes[active_mode].speed;
    zoneConfig.brightness = modes[active_mode].brightness;
    zoneConfig.direction = modes[active_mode].direction == MODE_DIRECTION_RIGHT ? ZOTAC_V2_GPU_DIR_RIGHT : ZOTAC_V2_GPU_DIR_LEFT;

    if(zoneConfig.mode == ZOTAC_V2_GPU_MODE_RAINBOW ||
        zoneConfig.mode == ZOTAC_V2_GPU_MODE_MARQUEE ||
        zoneConfig.mode == ZOTAC_V2_GPU_MODE_DRIP ||
        zoneConfig.mode == ZOTAC_V2_GPU_MODE_DUET)
    {
        zoneConfig.circuit = modes[active_mode].name.find("(circuit)") != std::string::npos ? ZOTAC_V2_GPU_CIRCUIT_ON : ZOTAC_V2_GPU_CIRCUIT_OFF;
    }
    else
    {
        zoneConfig.circuit = 0;
    }

    int zoneNum = FindSynchronizedZoneNum(ZOTAC_V2_GPU_SYNC_SYNCHRONIZED);
    controller->TurnOnOff(true);
    controller->SetMode(zoneNum, ZOTAC_V2_GPU_CONFIG_IDLE, ZOTAC_V2_GPU_SYNC_SYNCHRONIZED, zoneConfig);
    controller->SetMode(zoneNum, ZOTAC_V2_GPU_CONFIG_ACTIVE, ZOTAC_V2_GPU_SYNC_SYNCHRONIZED, zoneConfig);
}

int RGBController_ZotacV2GPU::FindSynchronizedZoneNum(int syncMode)
{
    // Figure out the index of the zone used for ZOTAC_V2_GPU_SYNC_SYNCHRONIZED
    // or ZOTAC_V2_GPU_SYNC_SYNCHRONIZED_WITH_EXTERNAL settings based on the GPU
    // zone config

    int lastRealZone = config.numberOfZones - 1;
    if(config.supportsExternalLEDStrip)
    {
        lastRealZone += 1;
    }

    if(syncMode == ZOTAC_V2_GPU_SYNC_SYNCHRONIZED)
    {
        return lastRealZone + 1;
    }
    else if(syncMode == ZOTAC_V2_GPU_SYNC_SYNCHRONIZED_WITH_EXTERNAL)
    {
        assert(config.supportsExternalLEDStrip);
        return lastRealZone + 2;
    }
    else
    {
        assert(false);
        return 0;
    }
}
