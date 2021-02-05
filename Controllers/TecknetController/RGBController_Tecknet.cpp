/*-------------------------------------------------------------------*\
|  RGBController_Tecknet.cpp                                          |
|                                                                     |
|  Driver for Tecknet Devices                                         |
|                                                                     |
|  Chris M (Dr_No)          29th Jul 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController_Tecknet.h"

RGBController_Tecknet::RGBController_Tecknet(TecknetController *_dev)
{
    Tecknet_dev = _dev;

    name        = Tecknet_dev->GetDeviceName();
    vendor      = "Tecknet";
    type        = DEVICE_TYPE_MOUSE;
    description = Tecknet_dev->GetDeviceName();
    version     = "1.0";
    serial      = Tecknet_dev->GetSerial();
    location    = Tecknet_dev->GetLocation();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = TECKNET_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.speed_min                = TECKNET_SPEED_OFF;
    Direct.speed_max                = TECKNET_SPEED_OFF;
    Direct.speed                    = TECKNET_SPEED_OFF;
    modes.push_back(Direct);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = TECKNET_MODE_OFF;
    Off.color_mode                  = MODE_COLORS_NONE;
    Off.speed_min                   = TECKNET_SPEED_OFF;
    Off.speed_max                   = TECKNET_SPEED_OFF;
    Off.speed                       = TECKNET_SPEED_OFF;
    modes.push_back(Off);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = TECKNET_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.speed_min             = TECKNET_SPEED_SLOW;
    Breathing.speed_max             = TECKNET_SPEED_FAST;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.speed                 = TECKNET_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_Tecknet::~RGBController_Tecknet()
{
    delete Tecknet_dev;
}

void RGBController_Tecknet::SetupZones()
{
    zone Tecknet_zone;
    Tecknet_zone.name          = "Logo";
    Tecknet_zone.type          = ZONE_TYPE_SINGLE;
    Tecknet_zone.leds_min      = 1;
    Tecknet_zone.leds_max      = 1;
    Tecknet_zone.leds_count    = 1;
    Tecknet_zone.matrix_map    = NULL;
    zones.push_back(Tecknet_zone);

    led Tecknet_led;
    Tecknet_led.name = "Logo";
    leds.push_back(Tecknet_led);

    SetupColors();
}

void RGBController_Tecknet::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | Not implemented for thiis device                          |
    \*---------------------------------------------------------*/
}

void RGBController_Tecknet::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);
    Tecknet_dev->SetColor(red, grn, blu);
}

void RGBController_Tecknet::UpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);
    Tecknet_dev->SetColor(red, grn, blu);
}

void RGBController_Tecknet::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_Tecknet::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_Tecknet::DeviceUpdateMode()
{
    //If active_mode is "Off" then set brightness to off otherwise high
    unsigned char brightness = (active_mode == TECKNET_MODE_OFF) ? TECKNET_BRIGHTNESS_OFF : TECKNET_BRIGHTNESS_HIGH;

    Tecknet_dev->SetMode(modes[active_mode].value, modes[active_mode].speed, brightness);
}
