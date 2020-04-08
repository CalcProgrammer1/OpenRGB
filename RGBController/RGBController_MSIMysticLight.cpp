/*-----------------------------------------*\
|  RGBController_MSIMysticLight.cpp         |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  MSI Mystic Light USB Driver              |
|                                           |
|  T-bond 3/4/2020                          |
\*-----------------------------------------*/

#include "RGBController_MSIMysticLight.h"
#include <array>

static const std::array<ZoneDescription, 18> led_zones
{
    ZoneDescription{"JRGB1", J_RGB_1},
    ZoneDescription{"JRGB2", J_RGB_2},
    ZoneDescription{"JRAINBOW1", J_RAINBOW_1},
    ZoneDescription{"JRAINBOW2", J_RAINBOW_2},
    ZoneDescription{"JPIPE1", J_PIPE_1},
    ZoneDescription{"JPIPE2", J_PIPE_2},
    ZoneDescription{"JCORSAIR", J_CORSAIR},
    ZoneDescription{"JCORSAIR Outer", J_CORSAIR_OUTERLL120},
    ZoneDescription{"OnboardLED", ON_BOARD_LED},
    ZoneDescription{"OnboardLED1", ON_BOARD_LED_1},
    ZoneDescription{"OnboardLED2", ON_BOARD_LED_2},
    ZoneDescription{"OnboardLED3", ON_BOARD_LED_3},
    ZoneDescription{"OnboardLED4", ON_BOARD_LED_4},
    ZoneDescription{"OnboardLED5", ON_BOARD_LED_5},
    ZoneDescription{"OnboardLED6", ON_BOARD_LED_6},
    ZoneDescription{"OnboardLED7", ON_BOARD_LED_7},
    ZoneDescription{"OnboardLED8", ON_BOARD_LED_8},
    ZoneDescription{"OnboardLED9", ON_BOARD_LED_9},
};

RGBController_MSIMysticLight::RGBController_MSIMysticLight(MSIMysticLightController* controller_ptr)
{
    controller = controller_ptr;

    name        = "MSI Mystic Light Controller";
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = controller->GetDeviceName();
    version     = controller->GetFWVersion();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerial();

    SetupModes();
    SetupZones();
    SetupColors();
}

RGBController_MSIMysticLight::~RGBController_MSIMysticLight()
{
    delete controller;
}

void RGBController_MSIMysticLight::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < led_zones.size(); zone_idx++)
    {
        ZoneDescription zd = led_zones[zone_idx];
        zone new_zone;
        new_zone.name           = zd.name;
        new_zone.type           = ZONE_TYPE_LINEAR;

        new_zone.leds_min       = controller->GetZoneMinLedCount(zd.value);
        new_zone.leds_max       = controller->GetZoneMaxLedCount(zd.value);
        new_zone.leds_count     = controller->GetZoneLedCount(zd.value);
        zones.push_back(new_zone);

        /*---------------------------------------------------------*\
        | Set up LEDs                                               |
        \*---------------------------------------------------------*/
        for(std::size_t led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
        {
            led new_led;
            
            new_led.name = new_zone.name + " LED ";
            if(new_zone.leds_count > 1)
            {
                new_led.name.append(std::to_string(led_idx + 1));
            }

            new_led.value = zone_idx;

            leds.push_back(new_led);
        }
    }
}

void RGBController_MSIMysticLight::ResizeZone(int zone, int new_size)
{
    ZONE zon = ZoneFromPos(zone);
    unsigned int max_count = controller->GetZoneMaxLedCount(zon),
                min_count = controller->GetZoneMinLedCount(zon),
                new_siz = new_size;
    new_siz = std::min(std::max(new_siz, min_count), max_count); // std::clamp only from C++17
    /// TODO: Update LED count
}

void RGBController_MSIMysticLight::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_MSIMysticLight::UpdateLEDs()
{
    for(size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_MSIMysticLight::UpdateZoneLEDs(int zone)
{
    for(int led_idx = zones[zone].leds_count - 1; led_idx >= 0; led_idx--)
    {
        UpdateLed(zone, led_idx);
    }
    controller->Update();
}

void RGBController_MSIMysticLight::UpdateSingleLED(int led)
{
    UpdateLed(leds[led].value, led);
    controller->Update();
}

void RGBController_MSIMysticLight::UpdateMode()
{
}

void RGBController_MSIMysticLight::SetupModes()
{
    constexpr unsigned int RANDOM_ONLY  = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR;
    constexpr unsigned int COMMON       = RANDOM_ONLY | MODE_FLAG_HAS_PER_LED_COLOR;

    SetupMode("Off", DISABLE, 0);
    SetupMode("Static", STATIC, MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR);
    SetupMode("Breathing", BREATHING, COMMON);
    SetupMode("Flashing", FLASHING, COMMON);
    SetupMode("Double flashing", DOUBLE_FLASHING, COMMON);
    SetupMode("Lightning", LIGHTNING, COMMON);
    SetupMode("MSI Marquee", MSI_MARQUEE, COMMON);
    SetupMode("Meteor", METEOR, COMMON);
    SetupMode("Water drop", WATER_DROP, COMMON);
    SetupMode("MSI Rainbow", MSI_RAINBOW, RANDOM_ONLY);
    SetupMode("Pop", POP, COMMON);
    SetupMode("Rap", RAP, COMMON);
    SetupMode("Jazz", JAZZ, COMMON);
    SetupMode("Play", PLAY, COMMON);
    SetupMode("Movie", MOVIE, COMMON);
    SetupMode("Color ring", COLOR_RING, COMMON);
    SetupMode("Planetary", PLANETARY, COMMON);
    SetupMode("Double meteor", DOUBLE_METEOR, COMMON);
    SetupMode("Energy", ENERGY, COMMON);
    SetupMode("Blink", BLINK, COMMON);
    SetupMode("Clock", CLOCK, COMMON);
    SetupMode("Color pulse", COLOR_PULSE, COMMON);
    SetupMode("Color shift", COLOR_SHIFT, COMMON);
    SetupMode("Color wave", COLOR_WAVE, COMMON);
    SetupMode("Marquee", MARQUEE, COMMON);
    SetupMode("Rainbow", RAINBOW, COMMON);
    SetupMode("Rainbow wave", RAINBOW_WAVE, COMMON);
    SetupMode("Visor", VISOR, COMMON);
    SetupMode("JRainbow", JRAINBOW, COMMON);
    SetupMode("Rainbow flashing", RAINBOW_FLASHING, COMMON);
    SetupMode("Rainbow double flashing", RAINBOW_DOUBLE_FLASHING, COMMON);
    SetupMode("Random", RANDOM, COMMON);
    SetupMode("Fan control", FAN_CONTROL, COMMON);
    SetupMode("Off 2", DISABLE_2, COMMON);
    SetupMode("Color ring flashing", COLOR_RING_FLASHING, COMMON);
    SetupMode("Color ring double flashing", COLOR_RING_DOUBLE_FLASHING, COMMON);
    SetupMode("Stack", STACK, COMMON);
    SetupMode("Corsair Que", CORSAIR_QUE, COMMON);
    SetupMode("Fire", FIRE, COMMON);
    SetupMode("Lava", LAVA, COMMON);
}

void RGBController_MSIMysticLight::UpdateLed(int zone, int led)
{
    bool random = modes[active_mode].color_mode == MODE_COLORS_RANDOM;
    unsigned char red = RGBGetRValue(zones[zone].colors[led]);
    unsigned char grn = RGBGetGValue(zones[zone].colors[led]);
    unsigned char blu = RGBGetBValue(zones[zone].colors[led]);
    EFFECT mode = static_cast<EFFECT>(modes[active_mode].value);
    SPEED speed = static_cast<SPEED>(modes[active_mode].speed);
    ZONE zon = ZoneFromPos(zone);
    
    controller->SetMode(zon, mode, speed, BRIGHTNESS::LEVEL_100, random);
    controller->SetZoneColor(zon, red, grn, blu, red, grn, blu);
}

ZONE RGBController_MSIMysticLight::ZoneFromPos(int zone)
{
    return led_zones[zone].value;
}

void RGBController_MSIMysticLight::SetupMode(const char *name, EFFECT mod, unsigned int flags)
{
    mode Mode;
    Mode.name       = name;
    Mode.value      = mod;
    Mode.flags      = flags;
    if(flags & MODE_FLAG_HAS_PER_LED_COLOR)
    {
        Mode.color_mode = MODE_COLORS_PER_LED;
    }
    else
    {
        Mode.color_mode = MODE_COLORS_RANDOM;
    }

    if(flags & MODE_FLAG_HAS_SPEED)
    {
        Mode.speed      = SPEED::MEDIUM;
        Mode.speed_max  = SPEED::HIGH;
        Mode.speed_min  = SPEED::LOW;
    }

    modes.push_back(Mode);
}