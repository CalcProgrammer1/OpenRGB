/*-----------------------------------------*\
|  RGBController_Polychrome.cpp             |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ASRock ASR LED and Polychrome RGB Driver |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/15/2019  |
\*-----------------------------------------*/

#include "RGBController_Polychrome.h"

#define ASROCK_MAX_ZONES    4
#define ASROCK_MAX_LEDS     22

typedef struct
{
    const char*     zone_names[ASROCK_MAX_ZONES];
    const zone_type zone_types[ASROCK_MAX_ZONES];
    const int       zone_sizes[ASROCK_MAX_ZONES];
    const char*     led_names[ASROCK_MAX_LEDS];
} asrock_layout;

static const asrock_layout ASRock_B450_Steel_Legend =
{
    /*---------------------------------------------------------*\
    | zone_names                                                |
    \*---------------------------------------------------------*/
    {
        "AMD FAN LED Header",
        "RGB LED 1 Header",
        "PCH",
        "IO Cover",
    },
    /*---------------------------------------------------------*\
    | zone_types                                                |
    \*---------------------------------------------------------*/
    {
        ZONE_TYPE_SINGLE,
        ZONE_TYPE_SINGLE,
        ZONE_TYPE_LINEAR,
        ZONE_TYPE_LINEAR,
    },
    /*---------------------------------------------------------*\
    | zone_sizes                                                |
    \*---------------------------------------------------------*/
    {
        1,
        1,
        10,
        10,
    },
    /*---------------------------------------------------------*\
    | led_names                                                 |
    \*---------------------------------------------------------*/
    {
        "AMD FAN LED Header",
        "RGB LED 1 Header",
        "PCH 0",
        "PCH 1",
        "PCH 2",
        "PCH 3",
        "PCH 4",
        "PCH 5",
        "PCH 6",
        "PCH 7",
        "PCH 8",
        "PCH 9",
        "IO Cover 0",
        "IO Cover 1",
        "IO Cover 2",
        "IO Cover 3",
        "IO Cover 4",
        "IO Cover 5",
        "IO Cover 6",
        "IO Cover 7",
        "IO Cover 8",
        "IO Cover 9",
    }
};

static const asrock_layout ASRock_B450M_Steel_Legend =
{
    /*---------------------------------------------------------*\
    | zone_names                                                |
    \*---------------------------------------------------------*/
    {
        "AMD FAN LED Header",
        "RGB LED 1 Header",
        "PCH",
        "IO Cover",
    },
    /*---------------------------------------------------------*\
    | zone_types                                                |
    \*---------------------------------------------------------*/
    {
        ZONE_TYPE_SINGLE,
        ZONE_TYPE_SINGLE,
        ZONE_TYPE_LINEAR,
        ZONE_TYPE_LINEAR,
    },
    /*---------------------------------------------------------*\
    | zone_sizes                                                |
    \*---------------------------------------------------------*/
    {
        1,
        1,
        8,
        10,
    },
    /*---------------------------------------------------------*\
    | led_names                                                 |
    \*---------------------------------------------------------*/
    {
        "AMD FAN LED Header",
        "RGB LED 1 Header",
        "PCH 0",
        "PCH 1",
        "PCH 2",
        "PCH 3",
        "PCH 4",
        "PCH 5",
        "PCH 6",
        "PCH 7",
        "IO Cover 0",
        "IO Cover 1",
        "IO Cover 2",
        "IO Cover 3",
        "IO Cover 4",
        "IO Cover 5",
        "IO Cover 6",
        "IO Cover 7",
        "IO Cover 8",
        "IO Cover 9",
    }
};

RGBController_Polychrome::RGBController_Polychrome(PolychromeController* polychrome_ptr)
{
    polychrome = polychrome_ptr;

    name        = polychrome->GetDeviceName();
    version     = polychrome->GetFirmwareVersion();
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = "ASRock ASR LED/Polychrome Device";

    switch(polychrome->GetASRockType())
    {
        case ASROCK_TYPE_ASRLED:
            {
                mode Off;
                Off.name       = "Off";
                Off.value      = ASRLED_MODE_OFF;
                Off.flags      = 0;
                Off.color_mode = MODE_COLORS_NONE;
                modes.push_back(Off);

                mode Static;
                Static.name       = "Static";
                Static.value      = ASRLED_MODE_STATIC;
                Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
                Static.color_mode = MODE_COLORS_PER_LED;
                modes.push_back(Static);

                mode Breathing;
                Breathing.name       = "Breathing";
                Breathing.value      = ASRLED_MODE_BREATHING;
                Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
                Breathing.speed_min  = ASRLED_SPEED_MIN;
                Breathing.speed_max  = ASRLED_SPEED_MAX;
                Breathing.speed      = ASRLED_SPEED_DEFAULT;
                Breathing.color_mode = MODE_COLORS_PER_LED;
                modes.push_back(Breathing);

                mode Strobe;
                Strobe.name       = "Strobe";
                Strobe.value      = ASRLED_MODE_FLASHING;
                Strobe.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
                Strobe.speed_min  = ASRLED_SPEED_MIN;
                Strobe.speed_max  = ASRLED_SPEED_MAX;
                Strobe.speed      = ASRLED_SPEED_DEFAULT;
                Strobe.color_mode = MODE_COLORS_PER_LED;
                modes.push_back(Strobe);

                mode SpectrumCycle;
                SpectrumCycle.name       = "Spectrum Cycle";
                SpectrumCycle.value      = ASRLED_MODE_SPECTRUM_CYCLE;
                SpectrumCycle.flags      = MODE_FLAG_HAS_SPEED;
                SpectrumCycle.speed_min  = ASRLED_SPEED_MIN;
                SpectrumCycle.speed_max  = ASRLED_SPEED_MAX;
                SpectrumCycle.speed      = ASRLED_SPEED_DEFAULT;
                SpectrumCycle.color_mode = MODE_COLORS_NONE;
                modes.push_back(SpectrumCycle);

                mode Random;
                Random.name       = "Random";
                Random.value      = ASRLED_MODE_RANDOM;
                Random.flags      = MODE_FLAG_HAS_SPEED;
                Random.speed_min  = ASRLED_SPEED_MIN;
                Random.speed_max  = ASRLED_SPEED_MAX;
                Random.speed      = ASRLED_SPEED_DEFAULT;
                Random.color_mode = MODE_COLORS_NONE;
                modes.push_back(Random);

                mode Music;
                Music.name       = "Music";
                Music.value      = ASRLED_MODE_MUSIC;
                Music.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
                Music.color_mode = MODE_COLORS_PER_LED;
                modes.push_back(Music);

                mode Wave;
                Wave.name       = "Wave";
                Wave.value      = ASRLED_MODE_WAVE;
                Wave.flags      = MODE_FLAG_HAS_SPEED;
                Wave.speed_min  = ASRLED_SPEED_MIN;
                Wave.speed_max  = ASRLED_SPEED_MAX;
                Wave.speed      = ASRLED_SPEED_DEFAULT;
                Wave.color_mode = MODE_COLORS_NONE;
                modes.push_back(Wave);
            }
            break;

        case ASROCK_TYPE_POLYCHROME_V1:
        case ASROCK_TYPE_POLYCHROME_V2:
            {
                mode Off;
                Off.name       = "Off";
                Off.value      = POLYCHROME_MODE_OFF;
                Off.flags      = 0;
                Off.color_mode = MODE_COLORS_NONE;
                modes.push_back(Off);

                mode Static;
                Static.name       = "Static";
                Static.value      = POLYCHROME_MODE_STATIC;
                Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
                Static.color_mode = MODE_COLORS_PER_LED;
                modes.push_back(Static);

                mode Breathing;
                Breathing.name       = "Breathing";
                Breathing.value      = POLYCHROME_MODE_BREATHING;
                Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
                Breathing.speed_min  = POLYCHROME_SPEED_MIN;
                Breathing.speed_max  = POLYCHROME_SPEED_MAX;
                Breathing.speed      = POLYCHROME_SPEED_DEFAULT;
                Breathing.color_mode = MODE_COLORS_PER_LED;
                modes.push_back(Breathing);

                mode Strobe;
                Strobe.name       = "Strobe";
                Strobe.value      = POLYCHROME_MODE_FLASHING;
                Strobe.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
                Strobe.speed_min  = POLYCHROME_SPEED_MIN;
                Strobe.speed_max  = POLYCHROME_SPEED_MAX;
                Strobe.speed      = POLYCHROME_SPEED_DEFAULT;
                Strobe.color_mode = MODE_COLORS_PER_LED;
                modes.push_back(Strobe);

                mode SpectrumCycle;
                SpectrumCycle.name       = "Spectrum Cycle";
                SpectrumCycle.value      = POLYCHROME_MODE_SPECTRUM_CYCLE;
                SpectrumCycle.flags      = MODE_FLAG_HAS_SPEED;
                SpectrumCycle.speed_min  = POLYCHROME_SPEED_MIN;
                SpectrumCycle.speed_max  = POLYCHROME_SPEED_MAX;
                SpectrumCycle.speed      = POLYCHROME_SPEED_DEFAULT;
                SpectrumCycle.color_mode = MODE_COLORS_NONE;
                modes.push_back(SpectrumCycle);

                mode Random;
                Random.name       = "Random";
                Random.value      = POLYCHROME_MODE_RANDOM;
                Random.flags      = MODE_FLAG_HAS_SPEED;
                Random.speed_min  = POLYCHROME_SPEED_MIN;
                Random.speed_max  = POLYCHROME_SPEED_MAX;
                Random.speed      = POLYCHROME_SPEED_DEFAULT;
                Random.color_mode = MODE_COLORS_NONE;
                modes.push_back(Random);

                mode Wave;
                Wave.name       = "Wave";
                Wave.value      = POLYCHROME_MODE_WAVE;
                Wave.flags      = MODE_FLAG_HAS_SPEED;
                Wave.speed_min  = POLYCHROME_SPEED_MIN;
                Wave.speed_max  = POLYCHROME_SPEED_MAX;
                Wave.speed      = POLYCHROME_SPEED_DEFAULT;
                Wave.color_mode = MODE_COLORS_NONE;
                modes.push_back(Wave);

                mode Spring;
                Spring.name       = "Spring";
                Spring.value      = POLYCHROME_MODE_SPRING;
                Spring.flags      = MODE_FLAG_HAS_SPEED;
                Spring.speed_min  = POLYCHROME_SPEED_MIN;
                Spring.speed_max  = POLYCHROME_SPEED_MAX;
                Spring.speed      = POLYCHROME_SPEED_DEFAULT;
                Spring.color_mode = MODE_COLORS_NONE;
                modes.push_back(Spring);

                mode Stack;
                Stack.name       = "Stack";
                Stack.value      = POLYCHROME_MODE_STACK;
                Stack.flags      = MODE_FLAG_HAS_SPEED;
                Stack.speed_min  = POLYCHROME_SPEED_MIN;
                Stack.speed_max  = POLYCHROME_SPEED_MAX;
                Stack.speed      = POLYCHROME_SPEED_DEFAULT;
                Stack.color_mode = MODE_COLORS_NONE;
                modes.push_back(Stack);

                mode Cram;
                Cram.name       = "Cram";
                Cram.value      = POLYCHROME_MODE_CRAM;
                Cram.flags      = MODE_FLAG_HAS_SPEED;
                Cram.speed_min  = POLYCHROME_SPEED_MIN;
                Cram.speed_max  = POLYCHROME_SPEED_MAX;
                Cram.speed      = POLYCHROME_SPEED_DEFAULT;
                Cram.color_mode = MODE_COLORS_NONE;
                modes.push_back(Cram);

                mode Scan;
                Scan.name       = "Scan";
                Scan.value      = POLYCHROME_MODE_SCAN;
                Scan.flags      = MODE_FLAG_HAS_SPEED;
                Scan.speed_min  = POLYCHROME_SPEED_MIN;
                Scan.speed_max  = POLYCHROME_SPEED_MAX;
                Scan.speed      = POLYCHROME_SPEED_DEFAULT;
                Scan.color_mode = MODE_COLORS_NONE;
                modes.push_back(Scan);

                mode Neon;
                Neon.name       = "Neon";
                Neon.value      = POLYCHROME_MODE_NEON;
                Neon.flags      = 0;
                Neon.color_mode = MODE_COLORS_NONE;
                modes.push_back(Neon);

                mode Water;
                Water.name       = "Water";
                Water.value      = POLYCHROME_MODE_WATER;
                Water.flags      = MODE_FLAG_HAS_SPEED;
                Water.speed_min  = POLYCHROME_SPEED_MIN;
                Water.speed_max  = POLYCHROME_SPEED_MAX;
                Water.speed      = POLYCHROME_SPEED_DEFAULT;
                Water.color_mode = MODE_COLORS_NONE;
                modes.push_back(Water);

                mode Rainbow;
                Rainbow.name       = "Rainbow";
                Rainbow.value      = POLYCHROME_MODE_RAINBOW;
                Rainbow.flags      = MODE_FLAG_HAS_SPEED;
                Rainbow.speed_min  = POLYCHROME_SPEED_MIN;
                Rainbow.speed_max  = POLYCHROME_SPEED_MAX;
                Rainbow.speed      = POLYCHROME_SPEED_DEFAULT;
                Rainbow.color_mode = MODE_COLORS_NONE;
                modes.push_back(Rainbow);
            }
            break;
    }

    SetupZones();
}

void RGBController_Polychrome::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for (unsigned int i = 0; i < 4; i++)
    {
        zone* new_zone = new zone();

        /*---------------------------------------------------------*\
        | Set zone name to channel name                             |
        \*---------------------------------------------------------*/
        new_zone->name          = ASRock_B450_Steel_Legend.zone_names[i];
        new_zone->type          = ASRock_B450_Steel_Legend.zone_types[i];
        new_zone->leds_min      = ASRock_B450_Steel_Legend.zone_sizes[i];
        new_zone->leds_max      = ASRock_B450_Steel_Legend.zone_sizes[i];
        new_zone->leds_count    = ASRock_B450_Steel_Legend.zone_sizes[i];
        new_zone->matrix_map    = NULL;

        /*---------------------------------------------------------*\
        | Push new zone to zones vector                             |
        \*---------------------------------------------------------*/
        zones.push_back(*new_zone);
    }

    unsigned int led_count = 0;
    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            /*---------------------------------------------------------*\
            | Each zone only has one LED                                |
            \*---------------------------------------------------------*/
            led* new_led = new led();

            new_led->name = ASRock_B450_Steel_Legend.led_names[led_count];

            /*---------------------------------------------------------*\
            | Push new LED to LEDs vector                               |
            \*---------------------------------------------------------*/
            leds.push_back(*new_led);

            led_count++;
        }
    }

    SetupColors();
}

void RGBController_Polychrome::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Polychrome::DeviceUpdateLEDs()
{
    for (std::size_t led = 0; led < colors.size(); led++)
    {
        UpdateSingleLED(led);
    }
}

void RGBController_Polychrome::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Polychrome::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    polychrome->SetColorsAndSpeed(led, red, grn, blu);
}

void RGBController_Polychrome::SetCustomMode()
{
    active_mode = 1;
}

void RGBController_Polychrome::DeviceUpdateMode()
{
    polychrome->SetMode(modes[active_mode].value, modes[active_mode].speed);

    DeviceUpdateLEDs();
}
