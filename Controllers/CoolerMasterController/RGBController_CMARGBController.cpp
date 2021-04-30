/*-------------------------------------------------------------------*\
|  RGBController_CMARGBController.cpp                                 |
|                                                                     |
|  Driver for Coolermaster ARGB Controller                            |
|                                                                     |
|  Chris M (Dr_No)          14th Oct 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController_CMARGBController.h"

RGBController_CMARGBController::RGBController_CMARGBController(CMARGBController *cmargb_ptr)
{
    cmargb                  = cmargb_ptr;
    unsigned char speed     = cmargb->GetLedSpeed();

    name                    = argb_header_data[cmargb->GetZoneIndex()].name;
    vendor                  = "Cooler Master";
    type                    = DEVICE_TYPE_LEDSTRIP;
    description             = cmargb->GetDeviceName();
    version                 = "2.0 for FW0023";
    serial                  = cmargb->GetSerial();
    location                = cmargb->GetLocation();

    if(argb_header_data[cmargb->GetZoneIndex()].digital)
    {
        mode Off;
        Off.name                = "Turn Off";
        Off.value               = CM_ARGB_MODE_OFF;
        Off.color_mode          = MODE_COLORS_NONE;
        modes.push_back(Off);

        mode Reload;
        Reload.name             = "Reload";
        Reload.value            = CM_ARGB_MODE_RELOAD;
        Reload.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Reload.colors_min       = 1;
        Reload.colors_max       = 1;
        Reload.colors.resize(Reload.colors_max);
        Reload.speed_min        = CM_ARGB_SPEED_SLOWEST;
        Reload.speed_max        = CM_ARGB_SPEED_FASTEST;
        Reload.color_mode       = MODE_COLORS_MODE_SPECIFIC;
        Reload.speed            = speed;
        modes.push_back(Reload);

        mode Recoil;
        Recoil.name             = "Recoil";
        Recoil.value            = CM_ARGB_MODE_RECOIL;
        Recoil.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Recoil.colors_min       = 1;
        Recoil.colors_max       = 1;
        Recoil.colors.resize(Recoil.colors_max);
        Recoil.speed_min        = CM_ARGB_SPEED_SLOWEST;
        Recoil.speed_max        = CM_ARGB_SPEED_FASTEST;
        Recoil.color_mode       = MODE_COLORS_MODE_SPECIFIC;
        Recoil.speed            = speed;
        modes.push_back(Recoil);

        mode Breathing;
        Breathing.name          = "Breathing";
        Breathing.value         = CM_ARGB_MODE_BREATHING;
        Breathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Breathing.colors_min    = 1;
        Breathing.colors_max    = 1;
        Breathing.colors.resize(Breathing.colors_max);
        Breathing.speed_min     = CM_ARGB_SPEED_SLOWEST;
        Breathing.speed_max     = CM_ARGB_SPEED_FASTEST;
        Breathing.color_mode    = MODE_COLORS_MODE_SPECIFIC;
        Breathing.speed         = speed;
        modes.push_back(Breathing);

        mode Refill;
        Refill.name             = "Refill";
        Refill.value            = CM_ARGB_MODE_REFILL;
        Refill.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Refill.colors_min       = 1;
        Refill.colors_max       = 1;
        Refill.colors.resize(Refill.colors_max);
        Refill.speed_min        = CM_ARGB_SPEED_SLOWEST;
        Refill.speed_max        = CM_ARGB_SPEED_FASTEST;
        Refill.color_mode       = MODE_COLORS_MODE_SPECIFIC;
        Refill.speed            = speed;
        modes.push_back(Refill);

        mode Demo;
        Demo.name               = "Demo";
        Demo.value              = CM_ARGB_MODE_DEMO;
        Demo.flags              = MODE_FLAG_HAS_SPEED;
        Demo.speed_min          = CM_ARGB_SPEED_SLOWEST;
        Demo.speed_max          = CM_ARGB_SPEED_FASTEST;
        Demo.color_mode         = MODE_COLORS_NONE;
        Demo.speed              = speed;
        modes.push_back(Demo);

        mode Spectrum;
        Spectrum.name           = "Spectrum";
        Spectrum.value          = CM_ARGB_MODE_SPECTRUM;
        Spectrum.flags          = MODE_FLAG_HAS_SPEED;
        Spectrum.speed_min      = CM_ARGB_SPEED_SLOWEST;
        Spectrum.speed_max      = CM_ARGB_SPEED_FASTEST;
        Spectrum.color_mode     = MODE_COLORS_NONE;
        Spectrum.speed          = speed;
        modes.push_back(Spectrum);

        mode FillFlow;
        FillFlow.name           = "Fill Flow";
        FillFlow.value          = CM_ARGB_MODE_FILLFLOW;
        FillFlow.flags          = MODE_FLAG_HAS_SPEED;
        FillFlow.speed_min      = CM_ARGB_SPEED_SLOWEST;
        FillFlow.speed_max      = CM_ARGB_SPEED_FASTEST;
        FillFlow.color_mode     = MODE_COLORS_NONE;
        FillFlow.speed          = speed;
        modes.push_back(FillFlow);

        mode Rainbow;
        Rainbow.name            = "Rainbow";
        Rainbow.value           = CM_ARGB_MODE_RAINBOW;
        Rainbow.flags           = MODE_FLAG_HAS_SPEED;
        Rainbow.speed_min       = CM_ARGB_SPEED_SLOWEST;
        Rainbow.speed_max       = CM_ARGB_SPEED_FASTEST;
        Rainbow.color_mode      = MODE_COLORS_NONE;
        Rainbow.speed           = speed;
        modes.push_back(Rainbow);

        mode Static;
        Static.name             = "Static";
        Static.value            = CM_ARGB_MODE_STATIC;
        Static.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
        Static.colors_min       = 1;
        Static.colors_max       = 1;
        Static.colors.resize(Static.colors_max);
        Static.speed_min        = CM_ARGB_SPEED_SLOWEST;
        Static.speed_max        = CM_ARGB_SPEED_FASTEST;
        Static.color_mode       = MODE_COLORS_MODE_SPECIFIC;
        Static.speed            = speed;
        modes.push_back(Static);

        mode Direct;
        Direct.name             = "Direct";
        Direct.value            = CM_ARGB_MODE_DIRECT;
        Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode       = MODE_COLORS_PER_LED;
        modes.push_back(Direct);

        mode PassThru;
        PassThru.name           = "Pass Thru";
        PassThru.value          = CM_ARGB_MODE_PASSTHRU;
        PassThru.flags          = 0;
        PassThru.color_mode     = MODE_COLORS_NONE;
        modes.push_back(PassThru);
    }
    else
    {
        mode Static;
        Static.name             = "Static";
        Static.value            = CM_RGB_MODE_STATIC;
        Static.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
        Static.colors_min       = 1;
        Static.colors_max       = 1;
        Static.colors.resize(Static.colors_max);
        Static.color_mode       = MODE_COLORS_MODE_SPECIFIC;
        Static.speed            = 0;
        modes.push_back(Static);

        mode Breathing;
        Breathing.name          = "Breathing";
        Breathing.value         = CM_RGB_MODE_BREATHING;
        Breathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Breathing.colors_min    = 1;
        Breathing.colors_max    = 1;
        Breathing.colors.resize(Breathing.colors_max);
        Breathing.speed_min     = CM_ARGB_SPEED_SLOWEST;
        Breathing.speed_max     = CM_ARGB_SPEED_FASTEST;
        Breathing.color_mode    = MODE_COLORS_MODE_SPECIFIC;
        Breathing.speed         = CM_ARGB_SPEED_NORMAL;
        modes.push_back(Breathing);

        mode Flash;
        Flash.name              = "Flash";
        Flash.value             = CM_RGB_MODE_FLASH;
        Flash.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Flash.colors_min        = 1;
        Flash.colors_max        = 1;
        Flash.colors.resize(Flash.colors_max);
        Flash.speed_min         = CM_ARGB_SPEED_SLOWEST;
        Flash.speed_max         = CM_ARGB_SPEED_FASTEST;
        Flash.color_mode        = MODE_COLORS_MODE_SPECIFIC;
        Flash.speed             = CM_ARGB_SPEED_NORMAL;
        modes.push_back(Flash);

        mode Mirage;
        Mirage.name             = "Mirage";
        Mirage.value            = CM_RGB_MODE_MIRAGE;
        Mirage.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mirage.colors_min       = 1;
        Mirage.colors_max       = 1;
        Mirage.colors.resize(Mirage.colors_max);
        Mirage.speed_min        = CM_ARGB_SPEED_SLOWEST;
        Mirage.speed_max        = CM_ARGB_SPEED_FASTEST;
        Mirage.color_mode       = MODE_COLORS_MODE_SPECIFIC;
        Mirage.speed            = CM_ARGB_SPEED_NORMAL;
        modes.push_back(Mirage);

        mode PassThru;
        PassThru.name           = "Pass Thru";
        PassThru.value          = CM_RGB_MODE_PASSTHRU;
        PassThru.color_mode     = MODE_COLORS_NONE;
        modes.push_back(PassThru);

        mode Off;
        Off.name                = "Turn Off";
        Off.value               = CM_RGB_MODE_OFF;
        Off.color_mode          = MODE_COLORS_NONE;
        modes.push_back(Off);
    }

    Init_Controller();
    SetupZones();

    int temp_mode   = cmargb->GetMode();
    for(std::size_t mode_idx = 0; mode_idx < modes.size() ; mode_idx++)
    {
        if (temp_mode == modes[mode_idx].value)
        {
            active_mode = mode_idx;
            break;
        }
    }
    if (modes[active_mode].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        modes[active_mode].colors[0] = ToRGBColor(cmargb->GetLedRed(), cmargb->GetLedGreen(), cmargb->GetLedBlue());
    }
    modes[active_mode].color_mode = (cmargb->GetRandomColours()) ? MODE_COLORS_RANDOM : MODE_COLORS_MODE_SPECIFIC;
    if (modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        modes[active_mode].speed = cmargb->GetLedSpeed();
    }
}

RGBController_CMARGBController::~RGBController_CMARGBController()
{
    delete cmargb;
}

void RGBController_CMARGBController::Init_Controller()
{
    int zone_idx            = cmargb->GetZoneIndex();
    int zone_led_count      = argb_header_data[zone_idx].count;

    /*-------------------------------------------------*\
    | If argb_header_data[zone_idx].count == 1 then the |
    | zone is ZONE_TYPE_SINGLE                          |
    \*-------------------------------------------------*/
    bool boolSingleLED      = ( zone_led_count == 1 );

    zone ARGB_zone;
    ARGB_zone.name          = std::to_string(zone_idx);
    ARGB_zone.type          = (boolSingleLED) ? ZONE_TYPE_SINGLE : ZONE_TYPE_LINEAR;
    ARGB_zone.leds_min      = 4;
    ARGB_zone.leds_max      = 48;
    ARGB_zone.leds_count    = zone_led_count;
    ARGB_zone.matrix_map    = NULL;
    zones.push_back(ARGB_zone);
}

void RGBController_CMARGBController::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        bool boolSingleLED = (zones[zone_idx].type == ZONE_TYPE_SINGLE);

        if (!boolSingleLED)
        {
            cmargb->SetLedCount( std::stoi(zones[zone_idx].name), zones[zone_idx].leds_count);
        }

        for(unsigned int lp_idx = 0; lp_idx < zones[zone_idx].leds_count; lp_idx++)
        {
            led     new_led;
            unsigned int i = std::stoi(zones[zone_idx].name);

            if(boolSingleLED)
            {
                new_led.name  = i;
                new_led.value = argb_header_data[i].header;
            }
            else
            {
                new_led.name = i;
                new_led.name.append(" LED " + std::to_string(lp_idx));
                new_led.value = argb_header_data[i].header;
            }

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CMARGBController::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }
    uint8_t end_zone = last_zone(zone);

    for(std::size_t zone_idx = first_zone(zone); zone_idx < end_zone; zone_idx++)
    {
        if(((unsigned int)new_size >= zones[zone_idx].leds_min) && ((unsigned int)new_size <= zones[zone_idx].leds_max))
        {
            zones[zone_idx].leds_count = new_size;
        }
    }

    SetupZones();
}

void RGBController_CMARGBController::DeviceUpdateLEDs()
{
    uint8_t end_zone = last_zone(cmargb->GetZoneIndex());

    for(std::size_t zone_idx = first_zone(cmargb->GetZoneIndex()); zone_idx < end_zone; zone_idx++)
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_CMARGBController::UpdateZoneLEDs(int /*zone*/)
{
    //cmargb->SetLedsDirect( zones[zone].colors, zones[zone].leds_count );
}

void RGBController_CMARGBController::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(GetLED_Zone(led));
}

void RGBController_CMARGBController::SetCustomMode()
{
    for(std::size_t mode_idx = 0; mode_idx < modes.size() ; mode_idx++)
    {
        if (modes[mode_idx].value == CM_ARGB_MODE_DIRECT)
        {
            active_mode = mode_idx;
            break;
        }
    }
}

void RGBController_CMARGBController::DeviceUpdateMode()
{
    bool random_colours     = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    RGBColor colour         = (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC) ? modes[active_mode].colors[0] : 0;

    cmargb->SetMode( modes[active_mode].value, modes[active_mode].speed, colour, random_colours );
}

int RGBController_CMARGBController::GetLED_Zone(int led_idx)
{
    /*---------------------------------------------------------*\
    | This may be more useful in the abstract RGBController.cpp |
    \*---------------------------------------------------------*/
    for(size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        int zone_start  = zones[zone_idx].start_idx;
        int zone_end    = zone_start + zones[zone_idx].leds_count - 1;

        if( zone_start <= led_idx && zone_end >= led_idx)
        {
            return(zone_idx);
        }
    }

    return -1;
}
