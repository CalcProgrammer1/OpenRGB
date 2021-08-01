/*-----------------------------------------*\
|  RGBController_GigabyteRGBFusion2SMBus.cpp|
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion 2 SMBUS Driver       |
|                                           |
|  Matt Harper (5/5/2020)		            |
\*-----------------------------------------*/

#include "RGBController_GigabyteRGBFusion2SMBus.h"

/* TODO - Validate all of these
 *	CPU
 *	???
 *	Mobo logo - Verified
 *	Case rear
 *	Case
 *	???
 *	???
 *	ARGB header 1
 *	ARGB header 2
 *
 *	Do ??? actually map to anything? Are they even supported?
 *	If not, what is an elegant way to display but not wreck existing logic?
 */
static const char* rgb_fusion_zone_names[] =
{
    "CPU",
    "???",
    "Motherboard Logo",
    "Case Rear",
    "Case",
    "???",
    "???",
    "ARGB Header 1",
    "ARGB Header 2",
    "???"
};

RGBController_RGBFusion2SMBus::RGBController_RGBFusion2SMBus(RGBFusion2SMBusController* rgb_fusion_ptr)
{
    rgb_fusion = rgb_fusion_ptr;

    name        = rgb_fusion->GetDeviceName();
    vendor      = "Gigabyte";
    description = "RGB Fusion 2 SMBus";
    location    = rgb_fusion->GetDeviceLocation();

    type = DEVICE_TYPE_MOTHERBOARD;

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = RGB_FUSION_2_MODE_STATIC;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Pulse;
    Pulse.name       = "Pulse";
    Pulse.value      = RGB_FUSION_2_MODE_PULSE;
    Pulse.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Pulse.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    Pulse.speed_max  = RGB_FUSION_2_SPEED_FAST;
    Pulse.speed      = RGB_FUSION_2_SPEED_NORMAL;
    Pulse.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Pulse);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = RGB_FUSION_2_MODE_FLASHING;
    Flashing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.speed_min  = 0x01;
    Flashing.speed_max  = 0x04;
    Flashing.speed      = 0x02;
    Flashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    mode ColorCycle;
    ColorCycle.name       = "Color Cycle";
    ColorCycle.value      = RGB_FUSION_2_MODE_COLOR_CYCLE;
    ColorCycle.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    ColorCycle.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    ColorCycle.speed_max  = RGB_FUSION_2_SPEED_FAST;
    ColorCycle.speed      = RGB_FUSION_2_SPEED_NORMAL;
    ColorCycle.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(ColorCycle);

    mode DigitalWave;
    DigitalWave.name       = "Digital Wave";
    DigitalWave.value      = RGB_FUSION_2_MODE_DIGITAL_WAVE;
    DigitalWave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalWave.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    DigitalWave.speed_max  = RGB_FUSION_2_SPEED_FAST;
    DigitalWave.speed      = RGB_FUSION_2_SPEED_NORMAL;
    DigitalWave.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalWave);

    mode DigitalA;
    DigitalA.name       = "Digital A";
    DigitalA.value      = RGB_FUSION_2_MODE_DIGITAL_A;
    DigitalA.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalA.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    DigitalA.speed_max  = RGB_FUSION_2_SPEED_FAST;
    DigitalA.speed      = RGB_FUSION_2_SPEED_NORMAL;
    DigitalA.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalA);

    mode DigitalB;
    DigitalB.name       = "Digital B";
    DigitalB.value      = RGB_FUSION_2_MODE_DIGITAL_B;
    DigitalB.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalB.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    DigitalB.speed_max  = RGB_FUSION_2_SPEED_FAST;
    DigitalB.speed      = RGB_FUSION_2_SPEED_NORMAL;
    DigitalB.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalB);

    mode DigitalC;
    DigitalC.name       = "Digital C";
    DigitalC.value      = RGB_FUSION_2_MODE_DIGITAL_C;
    DigitalC.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalC.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    DigitalC.speed_max  = RGB_FUSION_2_SPEED_FAST;
    DigitalC.speed      = RGB_FUSION_2_SPEED_NORMAL;
    DigitalC.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalC);

    mode DigitalD;
    DigitalD.name       = "Digital D";
    DigitalD.value      = RGB_FUSION_2_MODE_DIGITAL_D;
    DigitalD.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalD.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    DigitalD.speed_max  = RGB_FUSION_2_SPEED_FAST;
    DigitalD.speed      = RGB_FUSION_2_SPEED_NORMAL;
    DigitalD.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalD);

    mode DigitalE;
    DigitalE.name       = "Digital E";
    DigitalE.value      = RGB_FUSION_2_MODE_DIGITAL_E;
    DigitalE.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalE.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    DigitalE.speed_max  = RGB_FUSION_2_SPEED_FAST;
    DigitalE.speed      = RGB_FUSION_2_SPEED_NORMAL;
    DigitalE.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalE);

    mode DigitalF;
    DigitalF.name       = "Digital F";
    DigitalF.value      = RGB_FUSION_2_MODE_DIGITAL_F;
    DigitalF.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalF.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    DigitalF.speed_max  = RGB_FUSION_2_SPEED_FAST;
    DigitalF.speed      = RGB_FUSION_2_SPEED_NORMAL;
    DigitalF.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalF);

    mode DigitalG;
    DigitalG.name       = "Digital G";
    DigitalG.value      = RGB_FUSION_2_MODE_DIGITAL_G;
    DigitalG.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalG.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    DigitalG.speed_max  = RGB_FUSION_2_SPEED_FAST;
    DigitalG.speed      = RGB_FUSION_2_SPEED_NORMAL;
    DigitalG.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalG);

    SetupZones();

    // Initialize active mode
    // TODO - broken. Need to complete GetDeviceMode
    active_mode = GetDeviceMode();
}

RGBController_RGBFusion2SMBus::~RGBController_RGBFusion2SMBus()
{
    delete rgb_fusion;
}

void RGBController_RGBFusion2SMBus::SetupZones()
{
    /*---------------------------------------------------------*\
    | Search through all LEDs and create zones for each channel |
    | type                                                      |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < rgb_fusion->GetLEDCount(); zone_idx++)
    {
        zone* new_zone = new zone();

        // Set zone name to channel name
        new_zone->name          = rgb_fusion_zone_names[zone_idx];
        new_zone->leds_min      = 1;
        new_zone->leds_max      = 1;
        new_zone->leds_count    = 1;

        // Push new zone to zones vector
        zones.push_back(*new_zone);
    }

    for(unsigned int led_idx = 0; led_idx < zones.size(); led_idx++)
    {
        led* new_led = new led();

        // Set LED name to channel name
        new_led->name           = rgb_fusion_zone_names[led_idx];

        // Push new LED to LEDs vector
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_RGBFusion2SMBus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RGBFusion2SMBus::DeviceUpdateLEDs()
{
    for (std::size_t led = 0; led < colors.size(); led++)
    {
        RGBColor      color = colors[led];
        unsigned char red   = RGBGetRValue(color);
        unsigned char grn   = RGBGetGValue(color);
        unsigned char blu   = RGBGetBValue(color);

        int mode = modes[active_mode].value;
        unsigned int speed = modes[active_mode].speed;

        rgb_fusion->SetLEDEffect(led, mode, speed, red, grn, blu);
    }

    rgb_fusion->Apply();
}

void RGBController_RGBFusion2SMBus::UpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    int mode = modes[active_mode].value;
    unsigned int speed = modes[active_mode].speed;

    rgb_fusion->SetLEDEffect(zone, mode, speed, red, grn, blu);
    rgb_fusion->Apply();
}

void RGBController_RGBFusion2SMBus::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

// TODO - Research if possible to read device state
int RGBController_RGBFusion2SMBus::GetDeviceMode()
{
    return(0);
}

void RGBController_RGBFusion2SMBus::SetCustomMode()
{

}

void RGBController_RGBFusion2SMBus::DeviceUpdateMode()
{

}

