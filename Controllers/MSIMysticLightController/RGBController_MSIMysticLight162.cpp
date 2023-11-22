/*-----------------------------------------*\
|  RGBController_MSIMysticLight162.cpp      |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  MSI Mystic Light (162-byte) USB Driver   |
|                                           |
|  T-bond 3/4/2020                          |
\*-----------------------------------------*/

#include "RGBController_MSIMysticLight162.h"

struct ZoneDescription
{
    std::string     name;
    const MSI_ZONE* leds;
};

static const MSI_ZONE ZoneList_JRGB1[] =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_NONE
};

static const MSI_ZONE ZoneList_JRGB2[] =
{
    MSI_ZONE_J_RGB_2,
    MSI_ZONE_NONE
};

static const MSI_ZONE ZoneList_JRainbow1[] =
{
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_NONE
};

static const MSI_ZONE ZoneList_JRainbow2[] =
{
    MSI_ZONE_J_RAINBOW_2,
    MSI_ZONE_NONE
};

static const MSI_ZONE ZoneList_JCorsair[] =
{
    MSI_ZONE_J_CORSAIR,
    MSI_ZONE_NONE
};

static const MSI_ZONE ZoneList_Onboard[] =
{
    MSI_ZONE_ON_BOARD_LED_0,
    MSI_ZONE_ON_BOARD_LED_1,
    MSI_ZONE_ON_BOARD_LED_2,
    MSI_ZONE_ON_BOARD_LED_3,
    MSI_ZONE_ON_BOARD_LED_4,
    MSI_ZONE_ON_BOARD_LED_5,
    MSI_ZONE_ON_BOARD_LED_6,
    MSI_ZONE_ON_BOARD_LED_7,
    MSI_ZONE_ON_BOARD_LED_8,
    MSI_ZONE_ON_BOARD_LED_9,
    MSI_ZONE_ON_BOARD_LED_10,
    MSI_ZONE_NONE
};

#define NUM_ZONES (sizeof(led_zones) / sizeof(led_zones[0]))

static const ZoneDescription led_zones[] =
{
    ZoneDescription{"JRGB1",            ZoneList_JRGB1      },
    ZoneDescription{"JRGB2",            ZoneList_JRGB2      },
    ZoneDescription{"JRAINBOW1",        ZoneList_JRainbow1  },
    ZoneDescription{"JRAINBOW2",        ZoneList_JRainbow2  },
    ZoneDescription{"JCORSAIR",         ZoneList_JCorsair   },
    ZoneDescription{"Onboard LEDs",     ZoneList_Onboard    },
};

RGBController_MSIMysticLight162::RGBController_MSIMysticLight162(MSIMysticLight162Controller* controller_ptr)
{
    controller = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "MSI";
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = "MSI Mystic Light Device (162-byte)";
    version     = controller->GetFWVersion();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerial();

    SetupModes();
    SetupZones();
    SetupColors();
}

RGBController_MSIMysticLight162::~RGBController_MSIMysticLight162()
{
    delete controller;
}

void RGBController_MSIMysticLight162::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < NUM_ZONES; zone_idx++)
    {
        ZoneDescription zd      = led_zones[zone_idx];
        
        zone new_zone;

        unsigned int led_count = 0;

        while(zd.leds[led_count] != MSI_ZONE_NONE)
        {
            led_count++;
        }

        new_zone.name           = zd.name;
        new_zone.type           = ZONE_TYPE_LINEAR;

        new_zone.leds_min       = led_count;
        new_zone.leds_max       = led_count;
        new_zone.leds_count     = led_count;
        new_zone.matrix_map     = NULL;
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

            new_led.value = zd.leds[led_idx];

            leds.push_back(new_led);
        }
    }
}

void RGBController_MSIMysticLight162::ResizeZone(int zone, int new_size)
{
}

void RGBController_MSIMysticLight162::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_MSIMysticLight162::DeviceUpdateLEDs()
{
    for(size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(int led_idx = zones[zone_idx].leds_count - 1; led_idx >= 0; led_idx--)
        {
            UpdateLed(zone_idx, led_idx);
        }
    }
    controller->Update();
}

void RGBController_MSIMysticLight162::UpdateZoneLEDs(int zone)
{
    for(int led_idx = zones[zone].leds_count - 1; led_idx >= 0; led_idx--)
    {
        UpdateLed(zone, led_idx);
    }
    controller->Update();
}

void RGBController_MSIMysticLight162::UpdateSingleLED(int led)
{
    UpdateLed(leds[led].value, led);
    controller->Update();
}

void RGBController_MSIMysticLight162::DeviceUpdateMode()
{
}

void RGBController_MSIMysticLight162::SetupModes()
{
    constexpr unsigned int RANDOM_ONLY  = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR;
    constexpr unsigned int COMMON       = RANDOM_ONLY | MODE_FLAG_HAS_PER_LED_COLOR;

    SetupMode("Direct",                     MSI_MODE_STATIC,                        MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR);
    SetupMode("Off",                        MSI_MODE_DISABLE,                       0);
    SetupMode("Breathing",                  MSI_MODE_BREATHING,                     COMMON);
    SetupMode("Flashing",                   MSI_MODE_FLASHING,                      COMMON);
    SetupMode("Double flashing",            MSI_MODE_DOUBLE_FLASHING,               COMMON);
    SetupMode("Lightning",                  MSI_MODE_LIGHTNING,                     COMMON);
    // SetupMode("MSI Marquee",                MSI_MODE_MSI_MARQUEE,                   COMMON);
    // SetupMode("Meteor",                     MSI_MODE_METEOR,                        COMMON);
    // SetupMode("Water drop",                 MSI_MODE_WATER_DROP,                    COMMON);
    // SetupMode("MSI Rainbow",                MSI_MODE_MSI_RAINBOW,                   RANDOM_ONLY);
    // SetupMode("Pop",                        MSI_MODE_POP,                           COMMON);
    // SetupMode("Rap",                        MSI_MODE_RAP,                           COMMON);
    // SetupMode("Jazz",                       MSI_MODE_JAZZ,                          COMMON);
    // SetupMode("Play",                       MSI_MODE_PLAY,                          COMMON);
    // SetupMode("Movie",                      MSI_MODE_MOVIE,                         COMMON);
    // SetupMode("Color ring",                 MSI_MODE_COLOR_RING,                    COMMON);
    // SetupMode("Planetary",                  MSI_MODE_PLANETARY,                     COMMON);
    // SetupMode("Double meteor",              MSI_MODE_DOUBLE_METEOR,                 COMMON);
    // SetupMode("Energy",                     MSI_MODE_ENERGY,                        COMMON);
    // SetupMode("Blink",                      MSI_MODE_BLINK,                         COMMON);
    // SetupMode("Clock",                      MSI_MODE_CLOCK,                         COMMON);
    // SetupMode("Color pulse",                MSI_MODE_COLOR_PULSE,                   COMMON);
    // SetupMode("Color shift",                MSI_MODE_COLOR_SHIFT,                   COMMON);
    // SetupMode("Color wave",                 MSI_MODE_COLOR_WAVE,                    COMMON);
    // SetupMode("Marquee",                    MSI_MODE_MARQUEE,                       COMMON);
    // SetupMode("Rainbow",                    MSI_MODE_RAINBOW,                       COMMON);
    // SetupMode("Rainbow wave",               MSI_MODE_RAINBOW_WAVE,                  COMMON);
    // SetupMode("Visor",                      MSI_MODE_VISOR,                         COMMON);
    // SetupMode("JRainbow",                   MSI_MODE_JRAINBOW,                      COMMON);
    // SetupMode("Rainbow flashing",           MSI_MODE_RAINBOW_FLASHING,              COMMON);
    // SetupMode("Rainbow double flashing",    MSI_MODE_RAINBOW_DOUBLE_FLASHING,       COMMON);
    // SetupMode("Random",                     MSI_MODE_RANDOM,                        COMMON);
    // SetupMode("Fan control",                MSI_MODE_FAN_CONTROL,                   COMMON);
    // SetupMode("Off 2",                      MSI_MODE_DISABLE_2,                     COMMON);
    // SetupMode("Color ring flashing",        MSI_MODE_COLOR_RING_FLASHING,           COMMON);
    // SetupMode("Color ring double flashing", MSI_MODE_COLOR_RING_DOUBLE_FLASHING,    COMMON);
    // SetupMode("Stack",                      MSI_MODE_STACK,                         COMMON);
    // SetupMode("Corsair Que",                MSI_MODE_CORSAIR_QUE,                   COMMON);
    // SetupMode("Fire",                       MSI_MODE_FIRE,                          COMMON);
    // SetupMode("Lava",                       MSI_MODE_LAVA,                          COMMON);
}

void RGBController_MSIMysticLight162::UpdateLed(int zone, int led)
{
    bool random         = modes[active_mode].color_mode == MODE_COLORS_RANDOM;
    unsigned char red   = RGBGetRValue(zones[zone].colors[led]);
    unsigned char grn   = RGBGetGValue(zones[zone].colors[led]);
    unsigned char blu   = RGBGetBValue(zones[zone].colors[led]);
    MSI_MODE      mode  = (MSI_MODE)(modes[active_mode].value);
    MSI_SPEED     speed = (MSI_SPEED)(modes[active_mode].speed);
    
    controller->SetMode((MSI_ZONE)zones[zone].leds[led].value, mode, speed, MSI_BRIGHTNESS_LEVEL_100, random);
    controller->SetLEDColor((MSI_ZONE)zones[zone].leds[led].value, red, grn, blu, red, grn, blu);
}

void RGBController_MSIMysticLight162::SetupMode(const char *name, MSI_MODE mod, unsigned int flags)
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
        Mode.speed      = MSI_SPEED_MEDIUM;
        Mode.speed_max  = MSI_SPEED_HIGH;
        Mode.speed_min  = MSI_SPEED_LOW;
    }
    else
    {
        /*---------------------------------------------------------*\
        | For modes without speed this needs to be set to avoid     |
        | bad values in the saved profile which in turn corrupts    |
        | the brightness calculation when loading the profile       |
        \*---------------------------------------------------------*/
        Mode.speed      = 0;
        Mode.speed_max  = 0;
        Mode.speed_min  = 0;
    }

    modes.push_back(Mode);
}
