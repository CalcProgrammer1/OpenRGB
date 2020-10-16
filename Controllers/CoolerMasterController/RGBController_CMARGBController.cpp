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
    type                    = DEVICE_TYPE_LEDSTRIP;
    description             = cmargb->GetDeviceName();
    version                 = "1.0";
    serial                  = cmargb->GetSerial();
    location                = cmargb->GetLocation();

    mode Off;
    Off.name                = "Turn Off";
    Off.value               = CM_ARGB_MODE_OFF;
    Off.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Spectrum;
    Spectrum.name           = "Spectrum";
    Spectrum.value          = CM_ARGB_MODE_SPECTRUM;
    Spectrum.flags          = MODE_FLAG_HAS_SPEED;
    Spectrum.speed_min      = CM_ARGB_SPEED_SLOWEST;
    Spectrum.speed_max      = CM_ARGB_SPEED_FASTEST;
    Spectrum.color_mode     = MODE_COLORS_NONE;
    Spectrum.speed          = speed;
    modes.push_back(Spectrum);

    mode Reload;
    Reload.name             = "Reload";
    Reload.value            = CM_ARGB_MODE_RELOAD;
    Reload.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Reload.speed_min        = CM_ARGB_SPEED_SLOWEST;
    Reload.speed_max        = CM_ARGB_SPEED_FASTEST;
    Reload.color_mode       = MODE_COLORS_PER_LED;
    Reload.speed            = speed;
    modes.push_back(Reload);

    mode Recoil;
    Recoil.name             = "Recoil";
    Recoil.value            = CM_ARGB_MODE_RECOIL;
    Recoil.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Recoil.speed_min        = CM_ARGB_SPEED_SLOWEST;
    Recoil.speed_max        = CM_ARGB_SPEED_FASTEST;
    Recoil.color_mode       = MODE_COLORS_PER_LED;
    Recoil.speed            = speed;
    modes.push_back(Recoil);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = CM_ARGB_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min     = CM_ARGB_SPEED_SLOWEST;
    Breathing.speed_max     = CM_ARGB_SPEED_FASTEST;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    Breathing.speed         = speed;
    modes.push_back(Breathing);

    mode Refill;
    Refill.name             = "Refill";
    Refill.value            = CM_ARGB_MODE_REFILL;
    Refill.flags            = MODE_FLAG_HAS_SPEED;
    Refill.speed_min        = CM_ARGB_SPEED_SLOWEST;
    Refill.speed_max        = CM_ARGB_SPEED_FASTEST;
    Refill.color_mode       = MODE_COLORS_NONE;
    Refill.speed            = speed;
    modes.push_back(Refill);

    Init_Controller();         //Only processed on first run
    SetupZones();
    //active_mode = cmargb->GetMode();
}

RGBController_CMARGBController::~RGBController_CMARGBController()
{

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
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);
    cmargb->SetColor(red, grn, blu);
}

void RGBController_CMARGBController::UpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);
    cmargb->SetColor(red, grn, blu);
}

void RGBController_CMARGBController::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_CMARGBController::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CMARGBController::DeviceUpdateMode()
{
    cmargb->SetMode( modes[active_mode].value, modes[active_mode].speed );
}
