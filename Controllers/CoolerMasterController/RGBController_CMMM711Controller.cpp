/*-------------------------------------------------------------------*\
|  RGBController_CMMM711Controller.cpp                                |
|                                                                     |
|  Driver for Coolermaster MM711 Controller                           |
|                                                                     |
|  Chris M (Dr_No)          14th Feb 2021                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#define applyBrightness(c, bright) ((RGBColor) ((RGBGetBValue(c) * bright / CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT) << 16 | (RGBGetGValue(c) * bright / CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT) << 8 | (RGBGetRValue(c) * bright / CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT)))

#include "RGBController_CMMM711Controller.h"

RGBController_CMMM711Controller::RGBController_CMMM711Controller(CMMM711Controller *cmmm711_ptr)
{
    cmmm711                         = cmmm711_ptr;
    uint8_t speed                   = cmmm711->GetLedSpeed();

    name                            = cmmm711->GetDeviceName();
    vendor                          = "Cooler Master";
    type                            = DEVICE_TYPE_MOUSE;
    description                     = cmmm711->GetDeviceName();
    version                         = "1.0";
    serial                          = cmmm711->GetSerial();
    location                        = cmmm711->GetLocation();

    mode Custom;
    Custom.name                     = "Direct";
    Custom.value                    = CM_MM711_MODE_CUSTOM;
    Custom.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Custom.brightness_min           = CM_MM_ARGB_BRIGHTNESS_MIN;
    Custom.brightness_max           = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Custom.brightness               = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Custom.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = CM_MM711_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.brightness_min           = CM_MM_ARGB_BRIGHTNESS_MIN;
    Static.brightness_max           = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Static.brightness               = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(Static.colors_max);
    Static.speed_min                = CM_MM711_SPEED_SLOWEST;
    Static.speed_max                = CM_MM711_SPEED_FASTEST;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.speed                    = CM_MM711_SPEED_NORMAL;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = CM_MM711_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.brightness_min        = CM_MM_ARGB_BRIGHTNESS_MIN;
    Breathing.brightness_max        = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Breathing.brightness            = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(Breathing.colors_max);
    Breathing.speed_min             = CM_MM711_SPEED_SLOWEST;
    Breathing.speed_max             = CM_MM711_SPEED_FASTEST;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed                 = CM_MM711_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Spectrum_Cycle;
    Spectrum_Cycle.name             = "Spectrum Cycle";
    Spectrum_Cycle.value            = CM_MM711_MODE_SPECTRUM_CYCLE;
    Spectrum_Cycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Spectrum_Cycle.brightness_min   = CM_MM_ARGB_BRIGHTNESS_MIN;
    Spectrum_Cycle.brightness_max   = CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM;
    Spectrum_Cycle.brightness       = CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM;
    Spectrum_Cycle.speed_min        = CM_MM711_SPEED_SLOWEST;
    Spectrum_Cycle.speed_max        = CM_MM711_SPEED_FASTEST;
    Spectrum_Cycle.color_mode       = MODE_COLORS_NONE;
    Spectrum_Cycle.speed            = CM_MM711_SPEED_NORMAL;
    modes.push_back(Spectrum_Cycle);

    mode Indicator;
    Indicator.name                  = "Indicator";
    Indicator.value                 = CM_MM711_MODE_INDICATOR;
    Indicator.flags                 = MODE_FLAG_MANUAL_SAVE;
    Indicator.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Indicator);

    mode Off;
    Off.name                        = "Turn Off";
    Off.value                       = CM_MM711_MODE_OFF;
    Off.flags                       = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    Init_Controller();         //Only processed on first run
    SetupZones();

    uint8_t temp_mode           = cmmm711->GetMode();
    for(std::size_t mode_index = 0; mode_index < modes.size(); mode_index++)
    {
        if (modes[mode_index].value == temp_mode)
        {
            active_mode = mode_index;
            break;
        }
    }
    if (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        modes[active_mode].colors[0] = ToRGBColor(cmmm711->GetLedRed(),cmmm711->GetLedGreen(),cmmm711->GetLedBlue());
    }
    colors[0] = cmmm711->GetWheelColour();
    colors[1] = cmmm711->GetLogoColour();
}

RGBController_CMMM711Controller::~RGBController_CMMM711Controller()
{
    delete cmmm711;
}

void RGBController_CMMM711Controller::Init_Controller()
{
    zone mouse_zone;
    mouse_zone.name         = name;
    mouse_zone.type         = ZONE_TYPE_LINEAR;
    mouse_zone.leds_min     = 2;
    mouse_zone.leds_max     = 2;
    mouse_zone.leds_count   = 2;
    mouse_zone.matrix_map   = NULL;
    zones.push_back(mouse_zone);

    led wheel_led;
    wheel_led.name          = "Scroll Wheel LED";
    wheel_led.value         = 0;
    leds.push_back(wheel_led);

    led logo_led;
    logo_led.name           = "Logo LED";
    logo_led.value          = 1;
    leds.push_back(logo_led);
}

void RGBController_CMMM711Controller::SetupZones()
{
    SetupColors();
}

void RGBController_CMMM711Controller::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CMMM711Controller::DeviceUpdateLEDs()
{
    RGBColor wheel  = applyBrightness(colors[0], modes[active_mode].brightness);
    RGBColor logo   = applyBrightness(colors[1], modes[active_mode].brightness);

    cmmm711->SetLedsDirect( wheel, logo);
}

void RGBController_CMMM711Controller::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMMM711Controller::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMMM711Controller::SetCustomMode()
{
    for(std::size_t mode_index = 0; mode_index < modes.size(); mode_index++)
    {
        if (modes[mode_index].value == CM_MM711_MODE_CUSTOM)
        {
            active_mode = mode_index;
            break;
        }
    }
}

void RGBController_CMMM711Controller::DeviceUpdateMode()
{
    RGBColor colour = 0;

    if (modes[active_mode].value != CM_MM711_MODE_CUSTOM)
    {
        if( modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC )
        {
            colour = modes[active_mode].colors[0];
        }

        cmmm711->SendUpdate(modes[active_mode].value, modes[active_mode].speed, colour, modes[active_mode].brightness);
    }
}

void RGBController_CMMM711Controller::DeviceSaveMode()
{
    DeviceUpdateMode();
    cmmm711->SendSavePacket();
}
