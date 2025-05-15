/*---------------------------------------------------------*\
| RGBController_AsusAuraCoreLaptop.cpp                      |
|                                                           |
|   RGBController for ASUS ROG Aura Core Laptop             |
|                                                           |
|   Chris M (Dr_No)                             28 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AsusAuraCoreLaptop.h"

/**------------------------------------------------------------------*\
    @name AsusAuraCoreLaptop
    @category DEVICE_TYPE_KEYBOARD
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraCoreLaptopControllers
    @comment Power profiles for this controller are set to `On` for all power
        state and scan be adjusted in the JSON config file.

        For each zone available LEDs can be set as `On = true` or `Off = false` when
        * Booting
        * Awake (Normal Usage)
        * Sleeping
        * Shutdown / Power Off

        MatrixMaps can be found in ArmouryCrate (it needs a chance to download device data)
        Default path is:
            C:\ProgramData\ASUS\ROG Live Service\DeviceContent\<Model name>\<Model name>.csv
        (Model name is the code like G814JV - can be found in the error in logs)
\*-------------------------------------------------------------------*/

RGBController_AsusAuraCoreLaptop::RGBController_AsusAuraCoreLaptop(AsusAuraCoreLaptopController *controller_ptr)
{
    controller                              = controller_ptr;
    const aura_core_laptop_device* aura_dev = controller->GetDeviceData();

    name                                    = aura_dev->dmi_name;
    vendor                                  = "Asus";
    type                                    = DEVICE_TYPE_LAPTOP;
    description                             = controller->GetDeviceDescription();
    serial                                  = controller->GetSerial();
    location                                = controller->GetLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ASUSAURACORELAPTOP_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min       = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Direct.brightness_max       = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Direct.brightness           = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ASUSAURACORELAPTOP_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(Static.colors_max);
    Static.brightness_min       = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Static.brightness_max       = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Static.brightness           = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Static.speed_min            = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Static.speed_max            = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.speed                = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ASUSAURACORELAPTOP_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 2;
    Breathing.colors.resize(Breathing.colors_min);
    Breathing.brightness_min    = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Breathing.brightness        = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Breathing.speed_min         = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Breathing.speed_max         = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed             = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name               = "Flashing";
    Flashing.value              = ASUSAURACORELAPTOP_MODE_FLASHING;
    Flashing.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Flashing.colors_min         = 1;
    Flashing.colors_max         = 1;
    Flashing.colors.resize(Flashing.colors_max);
    Flashing.brightness_min     = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Flashing.brightness_max     = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Flashing.brightness         = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Flashing.speed_min          = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Flashing.speed_max          = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Flashing.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Flashing.speed              = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Flashing);

    mode Spectrum;
    Spectrum.name               = "Spectrum Cycle";
    Spectrum.value              = ASUSAURACORELAPTOP_MODE_SPECTRUM;
    Spectrum.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Spectrum.brightness_min     = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Spectrum.brightness_max     = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Spectrum.brightness         = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Spectrum.speed_min          = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Spectrum.speed_max          = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    Spectrum.speed              = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Spectrum);

    mode Rainbow;
    Rainbow.name                = "Rainbow Wave";
    Rainbow.value               = ASUSAURACORELAPTOP_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    Rainbow.brightness_min      = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Rainbow.brightness          = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Rainbow.speed_min           = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Rainbow.speed_max           = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.speed               = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Rainbow);

    mode Starry;
    Starry.name                 = "Starry Night";
    Starry.value                = ASUSAURACORELAPTOP_MODE_STARRY_NIGHT;
    Starry.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Starry.colors_min           = 1;
    Starry.colors_max           = 2;
    Starry.colors.resize(Starry.colors_min);
    Starry.brightness_min       = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Starry.brightness_max       = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Starry.brightness           = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Starry.speed_min            = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Starry.speed_max            = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Starry.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Starry.speed                = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Starry);

    mode Rain;
    Rain.name                   = "Rain";
    Rain.value                  = ASUSAURACORELAPTOP_MODE_RAIN;
    Rain.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rain.brightness_min         = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Rain.brightness_max         = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Rain.brightness             = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Rain.speed_min              = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Rain.speed_max              = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Rain.color_mode             = MODE_COLORS_NONE;
    Rain.speed                  = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Rain);

    mode ReactFade;
    ReactFade.name              = "Reactive - Fade";
    ReactFade.value             = ASUSAURACORELAPTOP_MODE_REACT_FADE;
    ReactFade.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ReactFade.colors_min        = 1;
    ReactFade.colors_max        = 1;
    ReactFade.colors.resize(ReactFade.colors_max);
    ReactFade.brightness_min    = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    ReactFade.brightness_max    = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactFade.brightness        = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactFade.speed_min         = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    ReactFade.speed_max         = ASUSAURACORELAPTOP_SPEED_FASTEST;
    ReactFade.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    ReactFade.speed             = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(ReactFade);

    mode ReactLaser;
    ReactLaser.name             = "Reactive - Laser";
    ReactLaser.value            = ASUSAURACORELAPTOP_MODE_REACT_LASER;
    ReactLaser.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ReactLaser.colors_min       = 1;
    ReactLaser.colors_max       = 1;
    ReactLaser.colors.resize(ReactLaser.colors_max);
    ReactLaser.brightness_min   = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    ReactLaser.brightness_max   = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactLaser.brightness       = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactLaser.speed_min        = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    ReactLaser.speed_max        = ASUSAURACORELAPTOP_SPEED_FASTEST;
    ReactLaser.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    ReactLaser.speed            = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(ReactLaser);

    mode ReactRipple;
    ReactRipple.name            = "Reactive - Ripple";
    ReactRipple.value           = ASUSAURACORELAPTOP_MODE_REACT_RIPPLE;
    ReactRipple.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ReactRipple.colors_min      = 1;
    ReactRipple.colors_max      = 1;
    ReactRipple.colors.resize(ReactRipple.colors_max);
    ReactRipple.brightness_min  = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    ReactRipple.brightness_max  = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactRipple.brightness      = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactRipple.speed_min       = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    ReactRipple.speed_max       = ASUSAURACORELAPTOP_SPEED_FASTEST;
    ReactRipple.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    ReactRipple.speed           = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(ReactRipple);

    mode Comet;
    Comet.name                  = "Comet";
    Comet.value                 = ASUSAURACORELAPTOP_MODE_COMET;
    Comet.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Comet.colors_min            = 1;
    Comet.colors_max            = 1;
    Comet.colors.resize(Comet.colors_max);
    Comet.brightness_min        = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Comet.brightness_max        = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Comet.brightness            = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Comet.speed_min             = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Comet.speed_max             = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Comet.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Comet.speed                 = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Comet);

    mode FlashNDash;
    FlashNDash.name             = "Flash N Dash";
    FlashNDash.value            = ASUSAURACORELAPTOP_MODE_FLASHNDASH;
    FlashNDash.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    FlashNDash.colors_min       = 1;
    FlashNDash.colors_max       = 1;
    FlashNDash.colors.resize(FlashNDash.colors_max);
    FlashNDash.brightness_min   = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    FlashNDash.brightness_max   = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    FlashNDash.brightness       = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    FlashNDash.speed_min        = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    FlashNDash.speed_max        = ASUSAURACORELAPTOP_SPEED_FASTEST;
    FlashNDash.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    FlashNDash.speed            = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(FlashNDash);

    mode Keystone;
    Keystone.name               = "Keystone";
    Keystone.value              = ASUSAURACORELAPTOP_MODE_KEYSTONE;
    Keystone.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Keystone.colors_min         = 1;
    Keystone.colors_max         = 1;
    Keystone.colors.resize(Keystone.colors_max);
    Keystone.brightness_min     = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Keystone.brightness_max     = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Keystone.brightness         = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Keystone.speed_min          = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Keystone.speed_max          = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Keystone.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Keystone.speed              = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Keystone);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ASUSAURACORELAPTOP_MODE_OFF;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();

    SetMode(active_mode);
}

RGBController_AsusAuraCoreLaptop::~RGBController_AsusAuraCoreLaptop()
{
    delete controller;
}

void RGBController_AsusAuraCoreLaptop::SetupZones()
{
    std::string physical_size;
    KEYBOARD_LAYOUT new_layout;
    unsigned int max_led_value              = 0;

    const aura_core_laptop_device* aura_dev = controller->GetDeviceData();
    unsigned int layout                     = controller->GetKeyboardLayout();

    switch(layout)
    {
        case ASUSAURACORELAPTOP_LAYOUT_ISO:
            new_layout = KEYBOARD_LAYOUT_ISO_QWERTY;
            break;

        case ASUSAURACORELAPTOP_LAYOUT_ANSI:
        default:
            new_layout = KEYBOARD_LAYOUT_ANSI_QWERTY;
            break;
    }
    LOG_DEBUG("[%s] layout set as %d", description.c_str(), new_layout);

    /*---------------------------------------------------------*\
    | Fill in zones from the device data                        |
    \*---------------------------------------------------------*/
    for(size_t i = 0; i < AURA_CORE_LAPTOP_ZONES_MAX; i++)
    {
        LOG_DEBUG("[%s] setting up zone %d", description.c_str(), i);

        if(aura_dev->zones[i] == NULL)
        {
            break;
        }
        else
        {
            zone new_zone;

            new_zone.name                   = aura_dev->zones[i]->name;

            if(aura_dev->zones[i]->layout_new != NULL)
            {
                KeyboardLayoutManager new_kb(new_layout,
                                             aura_dev->zones[i]->layout_new->base_size,
                                             aura_dev->zones[i]->layout_new->key_values);

                if(aura_dev->zones[i]->layout_new->base_size != KEYBOARD_SIZE_EMPTY ||
                   aura_dev->zones[i]->layout_new->edit_keys.size() > 0)
                {
                    /*---------------------------------------------------------*\
                    | Minor adjustments to keyboard layout                      |
                    \*---------------------------------------------------------*/
                    keyboard_keymap_overlay_values* temp    = aura_dev->zones[i]->layout_new;
                    new_kb.ChangeKeys(*temp);

                    if(new_kb.GetRowCount() == 1 || new_kb.GetColumnCount() == 1)
                    {
                        if(new_kb.GetKeyCount() == 1)
                        {
                            new_zone.type               = ZONE_TYPE_SINGLE;
                        }
                        else
                        {
                            new_zone.type               = ZONE_TYPE_LINEAR;
                        }
                    }
                    else
                    {
                        new_zone.type               = ZONE_TYPE_MATRIX;
                        matrix_map_type * new_map   = new matrix_map_type;
                        new_zone.matrix_map         = new_map;

                        /*---------------------------------------------------------*\
                        | Trusting the layout handed to the KLM is correct use the  |
                        |   row & column counts to set the matrix height & width    |
                        \*---------------------------------------------------------*/
                        new_map->height             = new_kb.GetRowCount();
                        new_map->width              = new_kb.GetColumnCount();
                        new_map->map                = new unsigned int[new_map->height * new_map->width];

                        new_kb.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT);
                    }

                    /*---------------------------------------------------------*\
                    | Create LEDs for the Matrix zone                           |
                    |   Place keys in the layout to populate the matrix         |
                    \*---------------------------------------------------------*/
                    new_zone.leds_count             = new_kb.GetKeyCount();
                    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
                    {
                        led new_led;

                        new_led.name                = new_kb.GetKeyNameAt(led_idx);
                        new_led.value               = new_kb.GetKeyValueAt(led_idx);
                        max_led_value               = std::max(max_led_value, new_led.value);
                        leds.push_back(new_led);
                    }
                }

                /*---------------------------------------------------------*\
                | Add 1 the max_led_value to account for the 0th index      |
                \*---------------------------------------------------------*/
                max_led_value++;
            }

            LOG_DEBUG("[%s] Creating a %s zone: %s with %d LEDs", name.c_str(),
                      ((new_zone.type == ZONE_TYPE_MATRIX) ? "matrix": "linear"),
                      new_zone.name.c_str(), new_zone.leds_count);
            new_zone.leds_min                   = new_zone.leds_count;
            new_zone.leds_max                   = new_zone.leds_count;
            zones.push_back(new_zone);
        }
    }

    SetupColors();

    /*---------------------------------------------------------*\
    | Create a buffer map of pointers which contains the        |
    |   layout order of colors the device expects.              |
    \*---------------------------------------------------------*/
    buffer_map.resize(max_led_value, &null_color);

    for(size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        buffer_map[leds[led_idx].value] = &colors[led_idx];
    }
}

void RGBController_AsusAuraCoreLaptop::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AsusAuraCoreLaptop::DeviceUpdateLEDs()
{
    for(size_t i = 85; i < leds.size(); i++)
    {
        LOG_DEBUG("[%s] Setting %s @ LED index %d and buffer index %d to R: %02X G: %02X B: %02X",
                  name.c_str(),
                  leds[i].name.c_str(),
                  i,
                  leds[i].value,
                  RGBGetRValue(colors[i]),
                  RGBGetGValue(colors[i]),
                  RGBGetBValue(colors[i]));
    }

    controller->SetLedsDirect(buffer_map);
}

void RGBController_AsusAuraCoreLaptop::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLedsDirect(buffer_map);
}

void RGBController_AsusAuraCoreLaptop::UpdateSingleLED(int /*led*/)
{
    controller->SetLedsDirect(buffer_map);
}

void RGBController_AsusAuraCoreLaptop::DeviceUpdateMode()
{
    mode set_mode       = modes[active_mode];

    uint8_t random      = (set_mode.color_mode == MODE_COLORS_RANDOM) ? 0xFF : 0;
    RGBColor color1     = (set_mode.colors.size() > 0) ? set_mode.colors[0] : 0;
    RGBColor color2     = (set_mode.colors.size() > 1) ? set_mode.colors[1] : 0;

    controller->SetMode(set_mode.value, set_mode.speed, set_mode.brightness, color1, color2, random, set_mode.direction );
}
