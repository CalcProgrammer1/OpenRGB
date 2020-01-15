/*-----------------------------------------*\
|  RGBController_RGBFusion2.cpp             |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion 2.0 Driver           |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/15/2020   |
\*-----------------------------------------*/

#include "RGBController_RGBFusion2.h"

int RGBController_RGBFusion2::GetMode()
{
    return(rgb_fusion->GetMode());
}

void RGBController_RGBFusion2::SetMode(int mode)
{
    rgb_fusion->SetMode(mode);
}

void RGBController_RGBFusion2::SetCustomMode()
{
    rgb_fusion->SetMode(0);
}

void RGBController_RGBFusion2::UpdateLEDs()
{
    for (std::size_t led = 0; led < colors.size(); led++)
    {
        RGBColor      color = colors[led];
        unsigned char red   = RGBGetRValue(color);
        unsigned char grn   = RGBGetGValue(color);
        unsigned char blu   = RGBGetBValue(color);

        rgb_fusion->SetLEDColor(led, red, grn, blu);
    }
}

void RGBController_RGBFusion2::UpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    rgb_fusion->SetLEDColor(zone, red, grn, blu);
}

void RGBController_RGBFusion2::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

RGBController_RGBFusion2::RGBController_RGBFusion2(RGBFusion2Controller* rgb_fusion_ptr)
{
    rgb_fusion = rgb_fusion_ptr;

    name = rgb_fusion->GetDeviceName();
    description = "RGB Fusion 2.0";
    location = rgb_fusion->GetDeviceLocation();

    type = DEVICE_TYPE_MOTHERBOARD;

    mode rgb_fusion_modes[1];

    rgb_fusion_modes[0].name = "Static";

    for (int i = 0; i < 1; i++)
    {
        modes.push_back(rgb_fusion_modes[i]);
    }

    colors.resize(rgb_fusion->GetLEDCount());

    // Search through all LEDs and create zones for each channel type
    for (unsigned int i = 0; i < rgb_fusion->GetLEDCount(); i++)
    {
        zone* new_zone = new zone();
        led*  new_led  = new led();

        std::vector<int>* zone_row = new std::vector<int>();

        // Set zone name to channel name
        new_zone->name = "Zone";
        new_led->name  = "LED";

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
