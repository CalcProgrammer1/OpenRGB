/*-----------------------------------------*\
|  RGBController_RGBFusion.cpp              |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion Driver               |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/11/2019  |
\*-----------------------------------------*/

#include "RGBController_RGBFusion.h"

int RGBController_RGBFusion::GetMode()
{
    return(rgb_fusion->GetMode());
}

void RGBController_RGBFusion::SetMode(int mode)
{
    rgb_fusion->SetMode(mode);
}

void RGBController_RGBFusion::SetCustomMode()
{
    rgb_fusion->SetMode(RGB_FUSION_MODE_STATIC);
}

void RGBController_RGBFusion::SetAllLEDs(RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);
    
    rgb_fusion->SetAllColors(red, grn, blu);
}

void RGBController_RGBFusion::SetAllZoneLEDs(int zone, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    rgb_fusion->SetLEDColor(zone, red, grn, blu);
}

void RGBController_RGBFusion::SetLED(int led, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);
    
    rgb_fusion->SetLEDColor(led, red, grn, blu);
}

void RGBController_RGBFusion::UpdateLEDs()
{
    for (int led = 0; led < colors.size(); led++)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        rgb_fusion->SetLEDColor(led, red, grn, blu);
    }
}

static const char* rgb_fusion_zone_names[] =
{
    "Motherboard",
    "RGB Header"
};

RGBController_RGBFusion::RGBController_RGBFusion(RGBFusionController* rgb_fusion_ptr)
{
    rgb_fusion = rgb_fusion_ptr;

    name = rgb_fusion->GetDeviceName();
    description = "RGB Fusion 1.0";
    location = rgb_fusion->GetDeviceLocation();

    type = DEVICE_TYPE_MOTHERBOARD;

    mode rgb_fusion_modes[RGB_FUSION_NUMBER_MODES];

    rgb_fusion_modes[0].name = "Static";
    rgb_fusion_modes[1].name = "Breathing";
    rgb_fusion_modes[2].name = "Flashing";

    for (int i = 0; i < RGB_FUSION_NUMBER_MODES; i++)
    {
        modes.push_back(rgb_fusion_modes[i]);
    }

    // Search through all LEDs and create zones for each channel type
    for (int i = 0; i < rgb_fusion->GetLEDCount(); i++)
    {
        zone* new_zone = new zone();
        led*  new_led  = new led();

        std::vector<int>* zone_row = new std::vector<int>();

        // Set zone name to channel name
        new_zone->name = rgb_fusion_zone_names[i];
        new_led->name  = rgb_fusion_zone_names[i];

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
