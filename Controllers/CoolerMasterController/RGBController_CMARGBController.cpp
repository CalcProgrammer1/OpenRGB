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
    version                 = "1.0";
    serial                  = cmargb->GetSerial();
    location                = cmargb->GetLocation();

    if ( argb_header_data[cmargb->GetZoneIndex()].digital)
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
        Reload.color_mode       = MODE_COLORS_RANDOM;
        Reload.speed            = speed;
        modes.push_back(Reload);

        mode Recoil;
        Recoil.name             = "Recoil";
        Recoil.value            = CM_ARGB_MODE_RECOIL;
        Recoil.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Recoil.colors_min       = 1;
        Recoil.colors_max       = 1;
        Recoil.colors.resize(Reload.colors_max);
        Recoil.speed_min        = CM_ARGB_SPEED_SLOWEST;
        Recoil.speed_max        = CM_ARGB_SPEED_FASTEST;
        Recoil.color_mode       = MODE_COLORS_RANDOM;
        Recoil.speed            = speed;
        modes.push_back(Recoil);

        mode Breathing;
        Breathing.name          = "Breathing";
        Breathing.value         = CM_ARGB_MODE_BREATHING;
        Breathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Breathing.colors_min    = 1;
        Breathing.colors_max    = 1;
        Breathing.colors.resize(Reload.colors_max);
        Breathing.speed_min     = CM_ARGB_SPEED_SLOWEST;
        Breathing.speed_max     = CM_ARGB_SPEED_FASTEST;
        Breathing.color_mode    = MODE_COLORS_RANDOM;
        Breathing.speed         = speed;
        modes.push_back(Breathing);

        mode Refill;
        Refill.name             = "Refill";
        Refill.value            = CM_ARGB_MODE_REFILL;
        Refill.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Refill.colors_min       = 1;
        Refill.colors_max       = 1;
        Refill.colors.resize(Reload.colors_max);
        Refill.speed_min        = CM_ARGB_SPEED_SLOWEST;
        Refill.speed_max        = CM_ARGB_SPEED_FASTEST;
        Refill.color_mode       = MODE_COLORS_RANDOM;
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

        mode Direct;
        Direct.name             = "Direct";
        Direct.value            = CM_ARGB_MODE_DIRECT;
        Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode       = MODE_COLORS_PER_LED;
        modes.push_back(Direct);

        mode PassThru;
        PassThru.name           = "Pass Thru";
        PassThru.value          = CM_ARGB_MODE_PASSTHRU;
        PassThru.color_mode     = MODE_COLORS_NONE;
        modes.push_back(PassThru);
    }
    else
    {
        mode Off;
        Off.name                = "Turn Off";
        Off.value               = CM_RGB_MODE_OFF;
        Off.color_mode          = MODE_COLORS_NONE;
        modes.push_back(Off);

        mode Breathing;
        Breathing.name          = "Breathing";
        Breathing.value         = CM_RGB_MODE_BREATHING;
        Breathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Breathing.colors_min    = 1;
        Breathing.colors_max    = 1;
        Breathing.colors.resize(Breathing.colors_max);
        Breathing.speed_min     = CM_ARGB_SPEED_SLOWEST;
        Breathing.speed_max     = CM_ARGB_SPEED_FASTEST;
        Breathing.color_mode    = MODE_COLORS_RANDOM;
        Breathing.speed         = speed;
        modes.push_back(Breathing);

        mode Flash;
        Breathing.name          = "Flash";
        Breathing.value         = CM_RGB_MODE_FLASH;
        Breathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Breathing.colors_min    = 1;
        Breathing.colors_max    = 1;
        Breathing.colors.resize(Breathing.colors_max);
        Breathing.speed_min     = CM_ARGB_SPEED_SLOWEST;
        Breathing.speed_max     = CM_ARGB_SPEED_FASTEST;
        Breathing.color_mode    = MODE_COLORS_RANDOM;
        Breathing.speed         = speed;
        modes.push_back(Breathing);

        mode ColourCycle;
        ColourCycle.name          = "Colour Cycle";
        ColourCycle.value         = CM_RGB_MODE_COLOUR_CYCLE;
        ColourCycle.flags         = MODE_FLAG_HAS_SPEED;
        ColourCycle.speed_min     = CM_ARGB_SPEED_SLOWEST;
        ColourCycle.speed_max     = CM_ARGB_SPEED_FASTEST;
        ColourCycle.color_mode    = MODE_COLORS_NONE;
        ColourCycle.speed         = speed;
        modes.push_back(ColourCycle);

        mode PassThru;
        PassThru.name           = "Pass Thru";
        PassThru.value          = CM_RGB_MODE_PASSTHRU;
        PassThru.color_mode     = MODE_COLORS_NONE;
        modes.push_back(PassThru);
    }

    Init_Controller();         //Only processed on first run
    SetupZones();
    //active_mode = cmargb->GetMode();
}

RGBController_CMARGBController::~RGBController_CMARGBController()
{
    delete cmargb;
}

void RGBController_CMARGBController::Init_Controller()
{
    int zone_idx            = cmargb->GetZoneIndex();
    int zone_led_count      = argb_header_data[zone_idx].count;
    bool boolSingleLED      = ( zone_led_count == 1 );          //If argb_header_data[zone_idx].count == 1 then the zone is ZONE_TYPE_SINGLE

    zone ARGB_zone;
    ARGB_zone.name          = std::to_string(zone_idx);
    ARGB_zone.type          = (boolSingleLED) ? ZONE_TYPE_SINGLE : ZONE_TYPE_LINEAR;
    ARGB_zone.leds_min      = 0;
    ARGB_zone.leds_max      = 64;
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
        bool boolSingleLED = (zones[zone_idx].type == ZONE_TYPE_SINGLE);    //Calculated for later use

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

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_CMARGBController::DeviceUpdateLEDs()
{
    /*unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);
    cmargb->SetColor(red, grn, blu);*/

    //this one
    cmargb->SetMode( modes[active_mode].value, modes[active_mode].speed );
    cmargb->SetLedsDirect( zones[0].colors, zones[0].leds_count );
}

void RGBController_CMARGBController::UpdateZoneLEDs(int zone)
{
    RGBColor      colour    = colors[zone];
    unsigned char red       = RGBGetRValue(colour);
    unsigned char grn       = RGBGetGValue(colour);
    unsigned char blu       = RGBGetBValue(colour);
    cmargb->SetColor(red, grn, blu);
}

void RGBController_CMARGBController::UpdateSingleLED(int led)
{
        //cmargb->SetMode( modes[active_mode].value, modes[active_mode].speed );
        //cmargb->SetLedsDirect( zones[0].colors, zones[0].leds_count );
}

void RGBController_CMARGBController::SetCustomMode()
{
    active_mode = CM_ARGB_MODE_DIRECT;
}

void RGBController_CMARGBController::DeviceUpdateMode()
{
    if( modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC )
    {
        unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
        unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
        unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);
        cmargb->SetColor(red, grn, blu);
    }
    else
    {
        cmargb->SetColor(0, 0, 0);        //If the mode is not colour specific then set colour to black for the random index
    }
    cmargb->SetMode( modes[active_mode].value, modes[active_mode].speed );
}
