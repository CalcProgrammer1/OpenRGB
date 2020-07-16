/*-------------------------------------------------------------------*\
|  RGBController_EKController.cpp                                     |
|                                                                     |
|  Driver for EK Loop Connect                                         |
|                                                                     |
|  Chris M (Dr_No)          16th Jul 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/


#include "RGBController_EKController.h"

RGBController_EKController::RGBController_EKController(EKController* _dev)
{
    EK_dev  = _dev;

    name        = EK_dev->GetDeviceName();
    type        = DEVICE_TYPE_LEDSTRIP;
    description = EK_dev->GetDeviceName();
    version     = "1.0";
    serial      = EK_dev->GetSerial();
    location    = EK_dev->GetLocation();

    mode Static;
    Static.name             = "Static";
    Static.value            = EK_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = EK_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min  = EK_SPEED_SLOWEST;
    Breathing.speed_max  = EK_SPEED_FASTEST;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    Breathing.speed      = EK_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_EKController::~RGBController_EKController()
{

}

void RGBController_EKController::SetupZones()
{
    zone EK_zone;
    EK_zone.name          = "Loop Connect";
    EK_zone.type          = ZONE_TYPE_SINGLE;
    EK_zone.leds_min      = 1;
    EK_zone.leds_max      = 1;
    EK_zone.leds_count    = 1;
    EK_zone.matrix_map    = NULL;
    zones.push_back(EK_zone);

    led EK_led;
    EK_led.name = "EK LED";
    leds.push_back(EK_led);

    SetupColors();
}

void RGBController_EKController::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | ToDo                                                      |
    \*---------------------------------------------------------*/
}

void RGBController_EKController::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);
    EK_dev->SetColor(red, grn, blu);
}

void RGBController_EKController::UpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);
    EK_dev->SetColor(red, grn, blu);
}

void RGBController_EKController::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_EKController::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_EKController::UpdateMode()
{
    EK_dev->SetMode(modes[active_mode].value, modes[active_mode].speed);
}
