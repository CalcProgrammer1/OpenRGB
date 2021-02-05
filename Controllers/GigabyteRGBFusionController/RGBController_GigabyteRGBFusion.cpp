/*-----------------------------------------*\
|  RGBController_GigabyteRGBFusion.cpp      |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion Driver               |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/11/2019  |
\*-----------------------------------------*/

#include "RGBController_GigabyteRGBFusion.h"

static const char* rgb_fusion_zone_names[] =
{
    "Motherboard",
    "RGB Header"
};

RGBController_RGBFusion::RGBController_RGBFusion(RGBFusionController* rgb_fusion_ptr)
{
    rgb_fusion = rgb_fusion_ptr;

    name        = rgb_fusion->GetDeviceName();
    vendor      = "Gigabyte";
    description = "RGB Fusion 1.0";
    location    = rgb_fusion->GetDeviceLocation();

    type = DEVICE_TYPE_MOTHERBOARD;

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = RGB_FUSION_MODE_STATIC;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = RGB_FUSION_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min  = RGB_FUSION_SPEED_SLOW;
    Breathing.speed_max  = RGB_FUSION_SPEED_FAST;
    Breathing.speed      = RGB_FUSION_SPEED_NORMAL;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = RGB_FUSION_MODE_FLASHING;
    Flashing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.speed_min  = RGB_FUSION_SPEED_SLOW;
    Flashing.speed_max  = RGB_FUSION_SPEED_FAST;
    Flashing.speed      = RGB_FUSION_SPEED_NORMAL;
    Flashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    SetupZones();

    // Initialize active mode
    active_mode = GetDeviceMode();
}

RGBController_RGBFusion::~RGBController_RGBFusion()
{
    delete rgb_fusion;
}

void RGBController_RGBFusion::SetupZones()
{
    /*---------------------------------------------------------*\
    | Search through all LEDs and create zones for each channel |
    | type                                                      |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < rgb_fusion->GetLEDCount(); zone_idx++)
    {
        zone* new_zone = new zone();

        /*---------------------------------------------------------*\
        | Set zone name to channel name                             |
        \*---------------------------------------------------------*/
        new_zone->name          = rgb_fusion_zone_names[zone_idx];
        new_zone->leds_min      = 1;
        new_zone->leds_max      = 1;
        new_zone->leds_count    = 1;
        new_zone->matrix_map    = NULL;

        /*---------------------------------------------------------*\
        | Push new zone to zones vector                             |
        \*---------------------------------------------------------*/
        zones.push_back(*new_zone);
    }

    for(unsigned int led_idx = 0; led_idx < zones.size(); led_idx++)
    {
        led* new_led = new led();

        /*---------------------------------------------------------*\
        | Set LED name to channel name                              |
        \*---------------------------------------------------------*/
        new_led->name           = rgb_fusion_zone_names[led_idx];

        /*---------------------------------------------------------*\
        | Push new LED to LEDs vector                               |
        \*---------------------------------------------------------*/
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_RGBFusion::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RGBFusion::DeviceUpdateLEDs()
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

int RGBController_RGBFusion::GetDeviceMode()
{
    int dev_mode = rgb_fusion->GetMode();

    for(std::size_t mode = 0; mode < modes.size(); mode++)
    {
        if(modes[mode].value == dev_mode)
        {
            return(mode);
        }
    }

    return(0);
}

void RGBController_RGBFusion::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_RGBFusion::DeviceUpdateMode()
{
    rgb_fusion->SetMode(modes[active_mode].value, modes[active_mode].speed);
}
