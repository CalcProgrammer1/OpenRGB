#include "LenovoDevices.h"
#include "RGBController_LenovoUSB.h"

#include <vector>
#include <string>
#include <utility>
using namespace std;

/*--------------------------------------------------------------------------------------*\
|note: the RGBController_LenovoUSB constructor determines which list of leds to pull from|
\*--------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------*\
    @name Lenovo USB
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLenovoLegionUSBControllers
    @comment Tested on Lenovo Legion 7/7i gen. 6 ANSI and ISO models
        Hardware modes are not implented
        PLEASE UPDATE YOUR BIOS IF YOU HAVE ISSUES WITH HARDWARE MODES AFTER CLOSING OPENRGB

        If you have other models beside the Legion 7 gen 6 and want to test if RGB can be added to OpenRGB,
        you can do so if you're on Windows by running
        [this Powershell script](https://gitlab.com/pvazny/legion7-rgb-ps) with the `-test` parameter:

        ```powershell
        legion7-rgb.ps1 -test -v 0x048D -p 0xC935 -up 0xFF89 -u 0x07 -c 0xa1 -start 0x15 -stop 0xa1
        ```

        This script will iterate through each LED one by one, in bank `$c = 0xa1`, between IDs
        `-start 0x15` and `-stop 0xa1`, on the HID device with VID `-v 0x048D` PID `-p 0xC935` usage page
        `-up 0xFF89` and usage `-u 0x07`. It will default to the Legion 7 Gen 6 description, however you can
        enter a new description for each LED and at the end it will generate the differences.

        To check if you have an eligible HID device that could potentially be iterated by the script please
        open the powershell command prompt and executre the follow:

        ```powershell
        Get-PnpDevice -Class 'HIDClass' | ForEach-Object { [PSCustomObject]@{Name = $_.FriendlyName; InstanceId = $_.InstanceId; HardwareId = ($_.HardwareId | Where-Object {$_ -like 'HID_DEVICE_UP:*' })}} | Where-Object { $_.HardwareId -ne $null } | Sort-Object InstanceId
        ```

        If the script is successful please create a [new device issue](https://gitlab.com/CalcProgrammer1/OpenRGB/-/issues/new?issuable_template=New%20Device#)
        and attach the relevant details to request support for your device.
\*-------------------------------------------------------------------*/

/*--------------------------------------------------------*\
| Legion 7 gen 6: 6 zones                                  |
|                                                          |
|Note: the device has 4 zones in the protocol however, the |
|vent lights have been split into 4 zones to improve ease  |
|of use                                                    |
\*--------------------------------------------------------*/

/*------*\
|keyboard|
\*------*/
lenovo_zone lenovo_legion_7_gen_6_kbd
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    10,
    22,
    0,
    146,
};
/*------*\
|logo    |
\*------*/
lenovo_zone lenovo_legion_7_gen_6_logo
{
    "Logo",
    ZONE_TYPE_LINEAR,
    1,
    13,
    147,
    159,
};
/*------*\
|vents   |
\*------*/
lenovo_zone lenovo_legion_7_gen_6_vent_left
{
    "Left vent",
    ZONE_TYPE_LINEAR,
    1,
    26,
    160,
    186,
};
lenovo_zone lenovo_legion_7_gen_6_vent_right
{
    "Right vent",
    ZONE_TYPE_LINEAR,
    1,
    26,
    187,
    213,
};
lenovo_zone lenovo_legion_7_gen_6_vent_back_right
{
    "Back Right vent",
    ZONE_TYPE_LINEAR,
    1,
    25,
    214,
    239,
};
lenovo_zone lenovo_legion_7_gen_6_vent_back_left
{
    "Back Left vent",
    ZONE_TYPE_LINEAR,
    1,
    25,
    240,
    265,
};
/*------*\
|neon    |
\*------*/
lenovo_zone lenovo_legion_7_gen_6_neon
{
    "Neon",
    ZONE_TYPE_LINEAR,
    1,
    99,
    266,
    364,
};

RGBController_LenovoUSB::RGBController_LenovoUSB(LenovoUSBController* controller_ptr)
{
    controller        = controller_ptr;

    mode Direct;
    Direct.name       = "Direct";
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    name    = controller->getName();
    type    = DEVICE_TYPE_KEYBOARD;
    vendor  = "Lenovo";

    switch(controller->getPid())
    {
        case LEGION_7_6_PID_1:
        case LEGION_7_6_PID_2:
        case LEGION_7_6_PID_3:
            description         = "Lenovo Legion 7 gen 6, ANSI VERSION";
            lenovo_leds         = legion_7_g_6_ansi_leds;
            lenovo_size_of_leds = sizeof(legion_7_g_6_ansi_leds)/sizeof(lenovo_led);
        break;
    }

    for(unsigned int i = 0; i < lenovo_size_of_leds; i++)
    {
        leds.push_back({lenovo_leds[i].name, i});
    }
    leds.shrink_to_fit();

    SetupZones();
}

RGBController_LenovoUSB::~RGBController_LenovoUSB()
{
    /*--------------------------------*\
    | see LenovoUSBController.cpp for  |
    | details                          |
    \*--------------------------------*/
    controller->setDeviceHardwareMode();

    delete controller;
}

void RGBController_LenovoUSB::SetupZones()
{
    vector<lenovo_zone> lenovo_zones;

    switch(controller->getPid())
    {
        case LEGION_7_6_PID_1:
        case LEGION_7_6_PID_2:
        case LEGION_7_6_PID_3:
            lenovo_zones.push_back(lenovo_legion_7_gen_6_kbd);
            lenovo_zones.push_back(lenovo_legion_7_gen_6_logo);
            lenovo_zones.push_back(lenovo_legion_7_gen_6_vent_left);
            lenovo_zones.push_back(lenovo_legion_7_gen_6_vent_right);
            lenovo_zones.push_back(lenovo_legion_7_gen_6_vent_back_right);
            lenovo_zones.push_back(lenovo_legion_7_gen_6_vent_back_left);
            lenovo_zones.push_back(lenovo_legion_7_gen_6_neon);
        break;
    }

    for(unsigned int i = 0; i < lenovo_zones.size(); i++)
    {
        zone new_zone;
        new_zone.name       = lenovo_zones[i].name;
        new_zone.type       = lenovo_zones[i].type;
        new_zone.leds_count = lenovo_zones[i].end - lenovo_zones[i].start + 1;
        new_zone.leds_max   = new_zone.leds_count;
        new_zone.leds_min   = new_zone.leds_count;

        if(lenovo_zones[i].type == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = new matrix_map_type;
            new_zone.matrix_map->height = lenovo_zones[i].height;
            new_zone.matrix_map->width  = lenovo_zones[i].width;
            new_zone.matrix_map->map    = new unsigned int[new_zone.matrix_map->height * new_zone.matrix_map->width];

            /*----------------------------------------------------*\
            | This section sets up the maps of zones where elements|
            | may have an oddly aligned on the matrix              |
            | please be sure to document in some way which zones   |
            | in a device you may be implementing have this poperty|
            | (Example in the LENOVO_7_6 section bellow)           |
            \*----------------------------------------------------*/

            switch(controller->getPid())
            {
                case LEGION_7_6_PID_1:
                case LEGION_7_6_PID_2:
                case LEGION_7_6_PID_3:
                    /*----------------------------------------------------------------------*\
                    | for this device, only the keyboard map (lenovo_legion_7_gen_6_kbd) uses|
                    | a custom map, so that map is setup here                                |
                    \*----------------------------------------------------------------------*/

                    new_zone.matrix_map->map = new unsigned int[lenovo_zones[i].width * lenovo_zones[i].height]
                    {
                        0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  NA,  NA,  16,  17,  18,  19,
                        20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  NA,  NA,  NA,  NA,  53,  33,  34,  35,  36,
                        37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  NA,  NA,  68,  69,  70,  NA,
                        54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  90,  65,  91,  66,  92,  67,  NA,  93,  94,  95,  71,
                        72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  85,  84,  87,  86,  88,  89,  111, 113, 114, 96,
                        97,  98,  99,  100, 101, 102, 103, 104, 105, 116, 106, 117, 107, 118, 108, 109, 110, NA,  112, NA,  115, NA,
                        126, 127, 128, 129, 130, NA,  NA,  NA,  NA,  131, 132, 133, NA,  NA,  NA,  136, NA,  NA,  119, 121, 123, 125,
                        NA,  146, 145, NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  135, NA,  NA,  120, 122, 124, 144,
                        NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  134, 137, 139, NA,  140, 141, 143, NA,
                        NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  138, NA,  NA,  NA,  NA,  142,  NA
                    };
                break;
            }
        }
        else
        {
            new_zone.matrix_map = NULL;
        }

        zones.push_back(new_zone);
    }

    SetupColors();
}

void RGBController_LenovoUSB::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LenovoUSB::UpdateSingleLED(int led)
{
    if(led != (int)NA)
    {
        controller->setSingleLED(lenovo_leds[led].zone_num, lenovo_leds[led].led_num, colors[led]);
    }
}

void RGBController_LenovoUSB::UpdateZoneLEDs(int zone)
{
    uint8_t device_zone = lenovo_leds[zones[zone].start_idx].zone_num;
    vector<pair<uint8_t, RGBColor>> color_map;

    for(unsigned int i = 0; i < zones[zone].leds_count; i++)
    {
        int index = zones[zone].start_idx+i;

        color_map.push_back({lenovo_leds[index].led_num, colors[index]});
    }

    color_map.shrink_to_fit();

    controller->setZoneLeds(device_zone, color_map);
}

void RGBController_LenovoUSB::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < lenovo_size_of_leds;)
    {
        uint8_t curr_device_zone = lenovo_leds[i].zone_num;
        vector<pair<uint8_t, RGBColor>> curr_color_map;

        for(; i < lenovo_size_of_leds && lenovo_leds[i].zone_num == curr_device_zone && i < colors.size(); i++)
        {
            curr_color_map.push_back({lenovo_leds[i].led_num, colors[i]});
        }

        controller->setZoneLeds(curr_device_zone, curr_color_map);
    }
}

void RGBController_LenovoUSB::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | This device does not support multiple modes                |
    \*---------------------------------------------------------*/
}

void RGBController_LenovoUSB::DeviceSaveMode()
{
    /*---------------------------------------------------------*\
    | This device does not support saving or multiple modes     |
    \*---------------------------------------------------------*/
}
