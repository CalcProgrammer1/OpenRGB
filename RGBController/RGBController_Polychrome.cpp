/*-----------------------------------------*\
|  RGBController_Polychrome.cpp             |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ASRock ASR LED and Polychrome RGB Driver |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/15/2019  |
\*-----------------------------------------*/

#include "RGBController_Polychrome.h"


void RGBController_Polychrome::UpdateLEDs()
{
    for (std::size_t led = 0; led < colors.size(); led++)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        polychrome->SetColor(red, grn, blu);
    }
}

void RGBController_Polychrome::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_Polychrome::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

static const char* polychrome_zone_names[] =
{
    "Motherboard"
};

RGBController_Polychrome::RGBController_Polychrome(PolychromeController* polychrome_ptr)
{
    polychrome = polychrome_ptr;

    name = polychrome->GetDeviceName();

    if(polychrome->IsAsrLed())
    {
        mode Off;
        Off.name = "Off";
        Off.value = ASRLED_MODE_OFF;
        Off.flags = 0;
        modes.push_back(Off);

        mode Static;
        Static.name = "Static";
        Static.value = ASRLED_MODE_STATIC;
        Static.flags = MODE_FLAG_HAS_COLOR;
        modes.push_back(Static);

        mode Breathing;
        Breathing.name = "Breathing";
        Breathing.value = ASRLED_MODE_BREATHING;
        Breathing.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR;
        modes.push_back(Breathing);

        mode Strobe;
        Strobe.name = "Strobe";
        Strobe.value = ASRLED_MODE_FLASHING;
        Strobe.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR;
        modes.push_back(Strobe);

        mode SpectrumCycle;
        SpectrumCycle.name = "Spectrum Cycle";
        SpectrumCycle.value = ASRLED_MODE_SPECTRUM_CYCLE;
        SpectrumCycle.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(SpectrumCycle);

        mode Random;
        Random.name = "Random";
        Random.value = ASRLED_MODE_RANDOM;
        Random.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(Random);
        
        mode Music;
        Music.name = "Music";
        Music.value = ASRLED_MODE_MUSIC;
        Music.flags = MODE_FLAG_HAS_COLOR;
        modes.push_back(Music);

        mode Wave;
        Wave.name = "Wave";
        Wave.value = ASRLED_MODE_WAVE;
        Wave.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(Wave);
    }
    else
    {
        mode Off;
        Off.name = "Off";
        Off.value = POLYCHROME_MODE_OFF;
        Off.flags = 0;
        modes.push_back(Off);

        mode Static;
        Static.name = "Static";
        Static.value = POLYCHROME_MODE_STATIC;
        Static.flags = MODE_FLAG_HAS_COLOR;
        modes.push_back(Static);

        mode Breathing;
        Breathing.name = "Breathing";
        Breathing.value = POLYCHROME_MODE_BREATHING;
        Breathing.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR;
        modes.push_back(Breathing);

        mode Strobe;
        Strobe.name = "Strobe";
        Strobe.value = POLYCHROME_MODE_FLASHING;
        Strobe.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR;
        modes.push_back(Strobe);

        mode SpectrumCycle;
        SpectrumCycle.name = "Spectrum Cycle";
        SpectrumCycle.value = POLYCHROME_MODE_SPECTRUM_CYCLE;
        SpectrumCycle.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(SpectrumCycle);

        mode Random;
        Random.name = "Random";
        Random.value = POLYCHROME_MODE_RANDOM;
        Random.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(Random);

        mode Wave;
        Wave.name = "Wave";
        Wave.value = POLYCHROME_MODE_WAVE;
        Wave.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(Wave);

        mode Spring;
        Spring.name = "Spring";
        Spring.value = POLYCHROME_MODE_SPRING;
        Spring.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(Spring);

        mode Stack;
        Stack.name = "Stack";
        Stack.value = POLYCHROME_MODE_STACK;
        Stack.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(Stack);

        mode Cram;
        Cram.name = "Cram";
        Cram.value = POLYCHROME_MODE_CRAM;
        Cram.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(Cram);

        mode Scan;
        Scan.name = "Scan";
        Scan.value = POLYCHROME_MODE_SCAN;
        Scan.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(Scan);

        mode Neon;
        Neon.name = "Neon";
        Neon.value = POLYCHROME_MODE_NEON;
        Neon.flags = 0;
        modes.push_back(Neon);

        mode Water;
        Water.name = "Water";
        Water.value = POLYCHROME_MODE_WATER;
        Water.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(Water);

        mode Rainbow;
        Rainbow.name = "Rainbow";
        Rainbow.value = POLYCHROME_MODE_RAINBOW;
        Rainbow.flags = MODE_FLAG_HAS_SPEED;
        modes.push_back(Rainbow);
    }

    colors.resize(polychrome->GetLEDCount());

    // Search through all LEDs and create zones for each channel type
    for (unsigned int i = 0; i < polychrome->GetLEDCount(); i++)
    {
        zone* new_zone = new zone();
        led* new_led = new led();

        std::vector<int>* zone_row = new std::vector<int>();

        // Set zone name to channel name
        new_zone->name = polychrome_zone_names[i];
        new_led->name = polychrome_zone_names[i];

        zone_row->push_back(i);

        // Aura devices can be either single or linear, never matrix
        // That means only one row is needed
        new_zone->map.push_back(*zone_row);

        // Push new LED to LEDs vector
        leds.push_back(*new_led);

        // Push new zone to zones vector
        zones.push_back(*new_zone);
    }
}

void RGBController_Polychrome::SetCustomMode()
{
    SetMode(1);
}

void RGBController_Polychrome::UpdateMode()
{
    polychrome->SetMode(modes[active_mode].value);
}
