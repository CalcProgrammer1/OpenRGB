/*-----------------------------------------*\
|  RGBController_Polychrome.cpp             |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ASRock ASR LED and Polychrome RGB Driver |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/15/2019  |
\*-----------------------------------------*/

#include "RGBController_Polychrome.h"

static const char* polychrome_zone_names[] =
{
    "Motherboard"
};

RGBController_Polychrome::RGBController_Polychrome(PolychromeController* polychrome_ptr)
{
    polychrome = polychrome_ptr;

    name        = polychrome->GetDeviceName();
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = "ASRock Polychrome Device";

    if(polychrome->IsAsrLed())
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
    else
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

    SetupZones();
}

void RGBController_Polychrome::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for (unsigned int i = 0; i < polychrome->GetLEDCount(); i++)
    {
        zone* new_zone = new zone();

        /*---------------------------------------------------------*\
        | Set zone name to channel name                             |
        \*---------------------------------------------------------*/
        new_zone->name          = polychrome_zone_names[i];
        new_zone->type          = ZONE_TYPE_SINGLE;
        new_zone->leds_min      = 1;
        new_zone->leds_max      = 1;
        new_zone->leds_count    = 1;
        new_zone->matrix_map    = NULL;

        /*---------------------------------------------------------*\
        | Push new zone to zones vector                             |
        \*---------------------------------------------------------*/
        zones.push_back(*new_zone);
    }

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        /*---------------------------------------------------------*\
        | Each zone only has one LED                                |
        \*---------------------------------------------------------*/
        led* new_led = new led();

        new_led->name = polychrome_zone_names[zone_idx];

        /*---------------------------------------------------------*\
        | Push new LED to LEDs vector                               |
        \*---------------------------------------------------------*/
        leds.push_back(*new_led);
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
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        polychrome->SetColorsAndSpeed(red, grn, blu, modes[active_mode].speed);
    }
}

void RGBController_Polychrome::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Polychrome::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Polychrome::SetCustomMode()
{
    active_mode = 1;
}

void RGBController_Polychrome::DeviceUpdateMode()
{
    polychrome->SetMode(modes[active_mode].value);

    DeviceUpdateLEDs();
}
