/*-----------------------------------------*\
|  RGBController_AsusAuraSMBus.cpp          |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Asus Aura SMBus driver                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/13/2019   |
\*-----------------------------------------*/

#include "RGBController_AsusAuraSMBus.h"

int RGBController_AuraSMBus::GetDeviceMode()
{
    int  dev_mode  = aura->AuraRegisterRead(AURA_REG_MODE);
    int color_mode = MODE_COLORS_PER_LED;

    if (aura->AuraRegisterRead(AURA_REG_DIRECT))
    {
        dev_mode = 0xFFFF;
    }

    switch(dev_mode)
    {
    case AURA_MODE_OFF:
    case AURA_MODE_RAINBOW:
    case AURA_MODE_SPECTRUM_CYCLE:
    case AURA_MODE_RANDOM_FLICKER:
        color_mode = MODE_COLORS_NONE;
        break;

    case AURA_MODE_SPECTRUM_CYCLE_CHASE:
        dev_mode   = AURA_MODE_CHASE;
        color_mode = MODE_COLORS_RANDOM;
        break;

    case AURA_MODE_SPECTRUM_CYCLE_BREATHING:
        dev_mode   = AURA_MODE_BREATHING;
        color_mode = MODE_COLORS_RANDOM;
        break;

    case AURA_MODE_SPECTRUM_CYCLE_CHASE_FADE:
        dev_mode   = AURA_MODE_CHASE_FADE;
        color_mode = MODE_COLORS_RANDOM;
        break;
    }

    for(std::size_t mode = 0; mode < modes.size(); mode++)
    {
        if(modes[mode].value == dev_mode)
        {
            active_mode            = mode;
            modes[mode].color_mode = color_mode;
        }
    }

    return(active_mode);
}

void RGBController_AuraSMBus::DeviceUpdateLEDs()
{
    if(GetMode() == 0)
    {
        aura->SetAllColorsDirect(&colors[0]);
    }
    else
    {
        aura->SetAllColorsEffect(&colors[0]);
    }
}

void RGBController_AuraSMBus::UpdateZoneLEDs(int zone)
{
    for (std::size_t led_idx = 0; led_idx < zones[zone].leds_count; led_idx++)
    {
        int           led   = zones[zone].leds[led_idx].value;
        RGBColor      color = colors[led];
        unsigned char red   = RGBGetRValue(color);
        unsigned char grn   = RGBGetGValue(color);
        unsigned char blu   = RGBGetBValue(color);

        if (GetMode() == 0)
        {
            aura->SetLEDColorDirect(led, red, grn, blu);
        }
        else
        {
            aura->SetLEDColorEffect(led, red, grn, blu);
        }
    }
}

void RGBController_AuraSMBus::UpdateSingleLED(int led)
{
    RGBColor color    = colors[led];
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if (GetMode() == 0)
    {
        aura->SetLEDColorDirect(led, red, grn, blu);
    }
    else
    {
        aura->SetLEDColorEffect(led, red, grn, blu);
    }
}

RGBController_AuraSMBus::RGBController_AuraSMBus(AuraSMBusController * aura_ptr)
{
    aura = aura_ptr;

    version = aura->GetDeviceName();
    location = aura->GetDeviceLocation();
    if((version.find("DIMM_LED") != std::string::npos) || (version.find("AUDA") != std::string::npos) )
    {
        type = DEVICE_TYPE_DRAM;
        name = "ASUS Aura DRAM";
    }
    else
    {
        type = DEVICE_TYPE_MOTHERBOARD;
        name = "ASUS Aura Motherboard";
    }
    vendor      = "ASUS";
    description = "ASUS Aura SMBus Device";

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name       = "Off";
    Off.value      = AURA_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name       = "Static";
    Static.value      = AURA_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);
    
    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = AURA_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = AURA_MODE_FLASHING;
    Flashing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = AURA_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags      = 0;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = AURA_MODE_RAINBOW;
    Rainbow.flags      = 0;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode ChaseFade;
    ChaseFade.name       = "Chase Fade";
    ChaseFade.value      = AURA_MODE_CHASE_FADE;
    ChaseFade.flags      = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_PER_LED_COLOR;
    ChaseFade.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(ChaseFade);

    mode Chase;
    Chase.name       = "Chase";
    Chase.value      = AURA_MODE_CHASE;
    Chase.flags      = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_PER_LED_COLOR;
    Chase.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Chase);

    mode RandomFlicker;
    RandomFlicker.name       = "Random Flicker";
    RandomFlicker.value      = AURA_MODE_RANDOM_FLICKER;
    RandomFlicker.flags      = 0;
    RandomFlicker.color_mode = MODE_COLORS_NONE;
    modes.push_back(RandomFlicker);

    SetupZones();

    // Initialize active mode
    active_mode = GetDeviceMode();
}

RGBController_AuraSMBus::~RGBController_AuraSMBus()
{
    delete aura;
}

void RGBController_AuraSMBus::SetupZones()
{
    std::vector<int>    aura_led_map;

    /*---------------------------------------------------------*\
    | Search through all LEDs and create zones for each channel |
    | type                                                      |
    \*---------------------------------------------------------*/
    for (std::size_t led_idx = 0; led_idx < aura->GetLEDCount(); led_idx++)
    {
        bool matched = false;

        /*---------------------------------------------------------*\
        | Search through existing zones to make sure we don't       |
        | create a duplicate zone                                   |
        \*---------------------------------------------------------*/
        for (std::size_t existing_zone_idx = 0; existing_zone_idx < zones.size(); existing_zone_idx++)
        {
            if (aura->GetChannelName(led_idx) == zones[existing_zone_idx].name)
            {
                matched = true;
            }
        }

        /*---------------------------------------------------------*\
        | If zone does not already exist, create it                 |
        \*---------------------------------------------------------*/
        if (matched == false)
        {
            zone* new_zone = new zone();

            /*---------------------------------------------------------*\
            | Set zone name to channel name                             |
            \*---------------------------------------------------------*/
            new_zone->name = aura->GetChannelName(led_idx);

            new_zone->leds_count = 0;

            /*---------------------------------------------------------*\
            | Find all LEDs with this channel type and add them to zone |
            \*---------------------------------------------------------*/
            for (std::size_t zone_led_idx = 0; zone_led_idx < aura->GetLEDCount(); zone_led_idx++)
            {
                if (aura->GetChannelName(zone_led_idx) == new_zone->name)
                {
                    new_zone->leds_count++;
                    aura_led_map.push_back(zone_led_idx);
                }
            }

            /*---------------------------------------------------------*\
            | Aura zones have fixed size, so set min and max to count   |
            \*---------------------------------------------------------*/
            new_zone->leds_min = new_zone->leds_count;
            new_zone->leds_max = new_zone->leds_count;

            /*---------------------------------------------------------*\
            | If this zone has more than one LED, mark it as linear type|
            \*---------------------------------------------------------*/
            if(new_zone->leds_count > 1)
            {
                new_zone->type = ZONE_TYPE_LINEAR;
            }
            else
            {
                new_zone->type = ZONE_TYPE_SINGLE;
            }

            new_zone->matrix_map = NULL;
            
            /*---------------------------------------------------------*\
            | Push new zone to zones vector                             |
            \*---------------------------------------------------------*/
            zones.push_back(*new_zone);
        }
    }

    /*---------------------------------------------------------*\
    | Create LED entries for each zone                          |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led* new_led = new led();

            new_led->name = zones[zone_idx].name + " LED ";
            new_led->name.append(std::to_string(led_idx + 1));

            new_led->value = aura_led_map[led_idx];

            leds.push_back(*new_led);
        }
    }

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize colors for each LED                            |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned int  led = leds[led_idx].value;
        unsigned char red = aura->GetLEDRed(led);
        unsigned char grn = aura->GetLEDGreen(led);
        unsigned char blu = aura->GetLEDBlue(led);

        colors[led_idx] = ToRGBColor(red, grn, blu);
    }
}

void RGBController_AuraSMBus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AuraSMBus::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AuraSMBus::DeviceUpdateMode()
{
    if (modes[active_mode].value == 0xFFFF)
    {
        aura->SetDirect(true);
    }
    else
    {
        int new_mode = modes[active_mode].value;

        if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
        {
            switch(new_mode)
            {
            case AURA_MODE_CHASE:
                new_mode = AURA_MODE_SPECTRUM_CYCLE_CHASE;
                break;
            case AURA_MODE_BREATHING:
                new_mode = AURA_MODE_SPECTRUM_CYCLE_BREATHING;
                break;
            case AURA_MODE_CHASE_FADE:
                new_mode = AURA_MODE_SPECTRUM_CYCLE_CHASE_FADE;
                break;
            }
        }

        aura->SetMode(new_mode);
        aura->SetDirect(false);
    }
}
