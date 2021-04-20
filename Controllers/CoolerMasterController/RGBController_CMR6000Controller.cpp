/*-------------------------------------------------------------------*\
|  RGBController_CMR6000Controller.cpp                                |
|                                                                     |
|  Driver for Coolermaster based AMD Radeon GPU (6000 series)         |
|                                                                     |
|  Eric S (edbgon)          2nd Feb 2021                              |
\*-------------------------------------------------------------------*/

#include "RGBController_CMR6000Controller.h"

RGBController_CMR6000Controller::RGBController_CMR6000Controller(CMR6000Controller* cmmp_ptr)
{
    cmr6000             = cmmp_ptr;
    unsigned char speed = cmr6000->GetLedSpeed();

    name                = "AMD RX 6xxx GPU";
    vendor              = "Cooler Master";
    type                = DEVICE_TYPE_GPU;
    description         = cmr6000->GetDeviceName();
    version             = "1.0";
    serial              = cmr6000->GetSerial();
    location            = cmr6000->GetLocation();

    mode Off;
    Off.name            = "Off";
    Off.flags           = 0;
    Off.value           = CM_MR6000_MODE_OFF;
    Off.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name         = "Static";
    Static.value        = CM_MR6000_MODE_STATIC;
    Static.flags        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min   = 1;
    Static.colors_max   = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode ColorCycle;
    ColorCycle.name       = "Color Cycle";
    ColorCycle.value      = CM_MR6000_MODE_COLOR_CYCLE;
    ColorCycle.flags      = MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min  = MR6000_CYCLE_SPEED_SLOWEST;
    ColorCycle.speed      = MR6000_CYCLE_SPEED_NORMAL;
    ColorCycle.speed_max  = MR6000_CYCLE_SPEED_FASTEST;
    ColorCycle.color_mode = MODE_COLORS_NONE;
    ColorCycle.speed      = speed;
    modes.push_back(ColorCycle);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = CM_MR6000_MODE_BREATHE;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.speed_min  = MR6000_BREATHE_SPEED_SLOWEST;
    Breathing.speed      = MR6000_BREATHE_SPEED_NORMAL;
    Breathing.speed_max  = MR6000_BREATHE_SPEED_FASTEST;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.colors.resize(1);
    Breathing.speed      = speed;
    modes.push_back(Breathing);    

    SetupZones();
    active_mode = cmr6000->GetMode();
    if (modes[active_mode].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        modes[active_mode].colors[0] = ToRGBColor(cmr6000->GetLedRed(), cmr6000->GetLedGreen(), cmr6000->GetLedBlue());
    }
    if (modes[active_mode].flags & MODE_FLAG_HAS_RANDOM_COLOR)
    {
        modes[active_mode].color_mode = (cmr6000->GetRandomColours()) ? MODE_COLORS_RANDOM : MODE_COLORS_MODE_SPECIFIC;
    }
    if (modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        modes[active_mode].speed = cmr6000->GetLedSpeed();
    }
}

RGBController_CMR6000Controller::~RGBController_CMR6000Controller()
{
    delete cmr6000;
}

void RGBController_CMR6000Controller::SetupZones()
{
    zone GP_zone;
    GP_zone.name          = "GPU";
    GP_zone.type          = ZONE_TYPE_SINGLE;
    GP_zone.leds_min      = 1;
    GP_zone.leds_max      = 1;
    GP_zone.leds_count    = 1;
    GP_zone.matrix_map    = NULL;
    zones.push_back(GP_zone);

    led GP_led;
    GP_led.name = "Logo";
    leds.push_back(GP_led);

    SetupColors();

}

void RGBController_CMR6000Controller::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CMR6000Controller::DeviceUpdateLEDs()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    unsigned char rnd = (modes[active_mode].color_mode == MODE_COLORS_RANDOM) ? 0xA0 : 0x20;

    cmr6000->SetMode(modes[active_mode].value, modes[active_mode].speed, red, grn, blu, rnd);
}

void RGBController_CMR6000Controller::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMR6000Controller::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMR6000Controller::SetCustomMode()
{
    active_mode = 1;
}

void RGBController_CMR6000Controller::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
