/*-------------------------------------------------------------------*\
|  RGBController_CMR6000Controller.cpp                                |
|                                                                     |
|  Driver for Coolermaster based AMD Radeon GPU (6000 series)         |
|                                                                     |
|  Eric S (edbgon)          2nd Feb 2021                              |
\*-------------------------------------------------------------------*/

#include "RGBController_CMR6000Controller.h"

/**------------------------------------------------------------------*\
    @name AMD Radeon 6000
    @category GPU
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterGPU
    @comment Similar to the Wraith Spire before it the AMD branded Radeon
        GPUs have an RGB controller provided by Coolermaster.
\*-------------------------------------------------------------------*/

RGBController_CMR6000Controller::RGBController_CMR6000Controller(CMR6000Controller* controller_ptr)
{
    controller          = controller_ptr;
    unsigned char speed = controller->GetLedSpeed();

    name                = "AMD RX 6xxx GPU";
    vendor              = "Cooler Master";
    type                = DEVICE_TYPE_GPU;
    description         = controller->GetDeviceName();
    serial              = controller->GetSerial();
    location            = controller->GetLocation();

    mode Off;
    Off.name            = "Off";
    Off.flags           = 0;
    Off.value           = CM_MR6000_MODE_OFF;
    Off.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = CM_MR6000_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR| MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.colors_min       = 1;
    Direct.colors_max       = 1;
    Direct.colors.resize(1);
    Direct.brightness_min   = 0x00;
    Direct.brightness_max   = 0xFF;
    Direct.brightness       = 0xFF;
    modes.push_back(Direct);

    mode ColorCycle;
    ColorCycle.name             = "Color Cycle";
    ColorCycle.value            = CM_MR6000_MODE_COLOR_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    ColorCycle.speed_min        = MR6000_CYCLE_SPEED_SLOWEST;
    ColorCycle.speed            = MR6000_CYCLE_SPEED_NORMAL;
    ColorCycle.speed_max        = MR6000_CYCLE_SPEED_FASTEST;
    ColorCycle.color_mode       = MODE_COLORS_NONE;
    ColorCycle.speed            = speed;
    ColorCycle.brightness_min   = 0x00;
    ColorCycle.brightness_max   = 0xFF;
    ColorCycle.brightness       = 0x7F;
    modes.push_back(ColorCycle);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = CM_MR6000_MODE_BREATHE;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.speed_min  = MR6000_BREATHE_SPEED_SLOWEST;
    Breathing.speed      = MR6000_BREATHE_SPEED_NORMAL;
    Breathing.speed_max  = MR6000_BREATHE_SPEED_FASTEST;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.colors.resize(1);
    Breathing.speed      = speed;
    modes.push_back(Breathing);

    SetupZones();
    active_mode = 1;
}

RGBController_CMR6000Controller::~RGBController_CMR6000Controller()
{
    delete controller;
}

void RGBController_CMR6000Controller::SetupZones()
{
    zone GP_zone;
    GP_zone.name        = "GPU";
    GP_zone.type        = ZONE_TYPE_SINGLE;
    GP_zone.leds_min    = 1;
    GP_zone.leds_max    = 1;
    GP_zone.leds_count  = 1;
    GP_zone.matrix_map  = NULL;
    zones.push_back(GP_zone);

    led GP_led;
    GP_led.name         = "Logo";
    GP_led.value        = 0;
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

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        red = RGBGetRValue(colors[0]);
        grn = RGBGetGValue(colors[0]);
        blu = RGBGetBValue(colors[0]);
    }

    unsigned char rnd = (modes[active_mode].color_mode == MODE_COLORS_RANDOM) ? 0xA0 : 0x20;
    unsigned char bri = (modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS) ? modes[active_mode].brightness : 0xFF;

    controller->SetMode(modes[active_mode].value, modes[active_mode].speed, red, grn, blu, rnd, bri);
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
