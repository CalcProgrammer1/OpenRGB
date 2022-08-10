#ifdef _WIN32

#include "RGBController_AsusTUFLaptopWMI.h"

#include "acpiwmi.h"
#include "ResourceManager.h"
#include "Detector.h"
#include "wmi.h"
#include <string>

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name Asus TUF Laptop
    @category Keyboard
    @type WMI
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusTUFLaptopWMIControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AsusTUFLaptopWMI::RGBController_AsusTUFLaptopWMI()
{
    name        = "ASUS TUF Keyboard";
    vendor      = "ASUS";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "WMI Device";
    location    = "\\\\.\\ATKACPI";

    modes.resize(5);
    modes[0].name       = "Direct";
    modes[0].value      = 4;
    modes[0].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[0].color_mode = MODE_COLORS_PER_LED;

    modes[1].name       = "Static";
    modes[1].value      = 0;
    modes[1].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[1].color_mode = MODE_COLORS_PER_LED;

    modes[2].name       = "Breathing";
    modes[2].value      = 1;
    modes[2].flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    modes[2].speed_min  = 0;
    modes[2].speed_max  = 2;
    modes[2].color_mode = MODE_COLORS_PER_LED;
    modes[2].speed      = 1;

    modes[3].name       = "Color Cycle";
    modes[3].value      = 2;
    modes[3].flags      = MODE_FLAG_HAS_SPEED;
    modes[3].speed_min  = 0;
    modes[3].speed_max  = 2;
    modes[3].color_mode = MODE_COLORS_NONE;
    modes[3].speed      = 1;

    modes[4].name       = "Strobe";
    modes[4].value      = 0x0A;
    modes[4].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[4].color_mode = MODE_COLORS_PER_LED;

    SetupZones();
}

RGBController_AsusTUFLaptopWMI::~RGBController_AsusTUFLaptopWMI()
{
    AsWMI_Close();
}

void RGBController_AsusTUFLaptopWMI::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zones.resize(1);
    zones[0].type           = ZONE_TYPE_SINGLE;
    zones[0].name           = "Keyboard Backlight zone";
    zones[0].leds_min       = 1;
    zones[0].leds_max       = 1;
    zones[0].leds_count     = 1;
    zones[0].matrix_map     = NULL;

    /*---------------------------------------------------------*\
    | Set up LED                                                |
    \*---------------------------------------------------------*/
    leds.resize(1);
    leds[0].name = "Keyboard Backlight LED";

    SetupColors();
}

void RGBController_AsusTUFLaptopWMI::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AsusTUFLaptopWMI::DeviceUpdateLEDs()
{
    uint8_t red   = RGBGetRValue(colors[0]);
    uint8_t green = RGBGetGValue(colors[0]);
    uint8_t blue  = RGBGetBValue(colors[0]);
    uint8_t speed_byte = 0;
    uint8_t mode = modes[active_mode].value;
    uint8_t inv = 0;
    if(mode == 4)
    {
        mode = 1;
        inv = 4; // Any invalid mode, i.e. anything other than 0, 1, 2 and 10
    }
    if(mode == 1 || mode == 2)
    {
        switch(modes[active_mode].speed)
        {
        case 0: speed_byte = 0xE1; break;
        case 1: speed_byte = 0xEB; break;
        case 2: speed_byte = 0xF5; break;
        }
    }
    int high = ((mode | ((red | (green << 8)) << 8)) << 8) | 0xB3;
    int low = blue | (speed_byte << 8);
    AsWMI_NB_DeviceControl_2arg(0x100056, high, low);
    if(inv)
    {
        //std::this_thread::sleep_for(10ms);
        high = ((inv | ((red | (green << 8)) << 8)) << 8) | 0xB3;
        AsWMI_NB_DeviceControl_2arg(0x100056, high, low);
    }
}

void RGBController_AsusTUFLaptopWMI::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusTUFLaptopWMI::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusTUFLaptopWMI::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

#endif
