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
    int dev_mode = rgb_fusion->GetMode();

    for(int mode = 0; mode < modes.size(); mode++)
    {
        if(modes[mode].value == dev_mode)
        {
            return(mode);
        }
    }

    return(0);
}

void RGBController_RGBFusion::SetMode(int mode)
{
    rgb_fusion->SetMode(modes[mode].value);
}

void RGBController_RGBFusion::SetCustomMode()
{
    rgb_fusion->SetMode(RGB_FUSION_MODE_STATIC);
}

void RGBController_RGBFusion::UpdateLEDs()
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

void RGBController_RGBFusion::UpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    rgb_fusion->SetLEDColor(zone, red, grn, blu);
}

void RGBController_RGBFusion::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
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

    mode Static;
    Static.name  = "Static";
    Static.value = RGB_FUSION_MODE_STATIC;
    Static.flags = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name  = "Breathing";
    Breathing.value = RGB_FUSION_MODE_BREATHING;
    Breathing.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name  = "Flashing";
    Flashing.value = RGB_FUSION_MODE_FLASHING;
    Flashing.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR;
    modes.push_back(Flashing);

    colors.resize(rgb_fusion->GetLEDCount());

    // Search through all LEDs and create zones for each channel type
    for (unsigned int i = 0; i < rgb_fusion->GetLEDCount(); i++)
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
