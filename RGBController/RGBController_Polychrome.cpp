/*-----------------------------------------*\
|  RGBController_Polychrome.cpp             |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ASRock ASR LED and Polychrome RGB Driver |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/15/2019  |
\*-----------------------------------------*/

#include "RGBController_Polychrome.h"

int RGBController_Polychrome::GetMode()
{
    return(polychrome->GetMode());
}

void RGBController_Polychrome::SetMode(int mode)
{
    polychrome->SetMode(mode);
}

void RGBController_Polychrome::SetCustomMode()
{
    polychrome->SetMode(POLYCHROME_MODE_STATIC);
}

void RGBController_Polychrome::SetAllLEDs(RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    polychrome->SetAllColors(red, grn, blu);
}

void RGBController_Polychrome::SetAllZoneLEDs(int zone, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    polychrome->SetLEDColor(zone, red, grn, blu);
}

void RGBController_Polychrome::SetLED(int led, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    polychrome->SetLEDColor(led, red, grn, blu);
}

void RGBController_Polychrome::UpdateLEDs()
{
    for (int led = 0; led < colors.size(); led++)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        polychrome->SetLEDColor(led, red, grn, blu);
    }
}

static const char* polychrome_zone_names[] =
{
    "Motherboard"
};

RGBController_Polychrome::RGBController_Polychrome(PolychromeController* polychrome_ptr)
{
    polychrome = polychrome_ptr;

    name = polychrome->GetDeviceName();

    mode polychrome_modes[POLYCHROME_NUM_MODES];

    polychrome_modes[0].name = "Static";
    polychrome_modes[1].name = "Breathing";
    polychrome_modes[2].name = "Flashing";

    for (int i = 0; i < POLYCHROME_NUM_MODES; i++)
    {
        modes.push_back(polychrome_modes[i]);
    }

    // Search through all LEDs and create zones for each channel type
    for (int i = 0; i < polychrome->GetLEDCount(); i++)
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
