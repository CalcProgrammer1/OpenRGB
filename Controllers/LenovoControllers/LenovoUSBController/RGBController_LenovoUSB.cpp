/*---------------------------------------------------------*\
| RGBController_Lenovo_USB.cpp                              |
|                                                           |
|   RGBController for Lenovo USB devices                    |
|                                                           |
|   Cooper Hall (geobot19)                      17 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "LenovoDevices.h"
#include "RGBController_LenovoUSB.h"
#include "LogManager.h"

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

    if(LogManager::get()->getLoglevel() >= LL_TRACE)
    {
        DumpControllerInformation();
    }

    std::vector<uint8_t> response;

    /*-----------------------*\
    |Default to ANSI keyboard |
    \*-----------------------*/
    keyboard_type = ANSI;

    switch(controller->getPid())
    {
        case LEGION_Y740:
            response = controller->getInformation(0x01);
            if(response.size() > 4 && response[4] <= 100)
            {
                chasis_size = FIFTEEN;
            }
            else
            {
                chasis_size = SEVENTEEN;
            }

            response = controller->getInformation(0x04);
            if(response.size() > 4)
            {
                if(response[4] >= 16 && response[4] <=48)
                {
                    keyboard_type = ISO;
                }
            }

            description = "Lenovo Y740 " + sizeToString(chasis_size) + " " + keyboardToString(keyboard_type);

            break;

        case LEGION_Y750:
            response = controller->getInformation(0x04);
            if(response.size() > 4)
            {
                if(response[4] == 41)
                {
                    keyboard_type = JAPAN;
                }
                else if(response[4] >= 16 && response[4] <=40)
                {
                    keyboard_type = ISO;
                }
            }

            description = "Lenovo Y750 " + keyboardToString(keyboard_type);

            break;

        case LEGION_Y750S:
            response = controller->getInformation(0x01);
            if(response.size() > 4)
            {
                if(response[4] == 0x97)
                {
                    keyboard_type = JAPAN;
                }
                else if(response[4] == 0x91)
                {
                    keyboard_type = ISO;
                }
            }

            description = "Lenovo Y750S " + keyboardToString(keyboard_type);

            break;

        case LEGION_Y760:
            response = controller->getInformation(0x07);
            if(response.size() > 4)
            {
                if(response[4] == 41)
                {
                    keyboard_type = JAPAN;
                }
                else if(response[4] >= 16 && response[4] <=40)
                {
                    keyboard_type = ISO;
                }
            }

            description = "Lenovo Y760 " + keyboardToString(keyboard_type);

            break;

        case LEGION_Y760S:
            response = controller->getInformation(0x01);
            if(response.size() > 4)
            {
                if(response[4] == 0x97)
                {
                    keyboard_type = JAPAN;
                }
                else if(response[4] == 0x8F)
                {
                    keyboard_type = ISO;
                }
            }

            description = "Lenovo Y760S " + keyboardToString(keyboard_type);
    }

    LOG_DEBUG("[Lenovo Controller] detected: %s", description.c_str());

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
        case LEGION_Y740:
            switch(chasis_size)
            {
                case FIFTEEN:
                    switch(keyboard_type)
                    {
                        case ISO:
                            lenovo_zones.push_back(lenovo_legion_Y740_15_kbd_iso);
                            break;

                        default:
                            lenovo_zones.push_back(lenovo_legion_Y740_15_kbd_ansi);
                            break;
                    }
                    break;

                case SEVENTEEN:
                default:
                    switch(keyboard_type)
                    {
                        case ISO:
                            lenovo_zones.push_back(lenovo_legion_Y740_17_kbd_iso);
                            break;

                        default:
                            lenovo_zones.push_back(lenovo_legion_Y740_17_kbd_ansi);
                            break;
                    }
                    break;
            }
            lenovo_zones.push_back(lenovo_legion_Y740_logo);
            lenovo_zones.push_back(lenovo_legion_Y740_pwrbtn);
            lenovo_zones.push_back(lenovo_legion_Y740_vents);
            lenovo_zones.push_back(lenovo_legion_Y740_ports);
            break;

        case LEGION_Y750:
            switch(keyboard_type)
            {
                case JAPAN:
                    lenovo_zones.push_back(lenovo_legion_Y760_kbd_jp);
                    break;

                case ISO:
                    lenovo_zones.push_back(lenovo_legion_Y760_kbd_iso);
                    break;

                default:
                    lenovo_zones.push_back(lenovo_legion_Y760_kbd_ansi);
                    break;
            }
            lenovo_zones.push_back(lenovo_legion_Y750_logo);
            lenovo_zones.push_back(lenovo_legion_Y750_vents);
            lenovo_zones.push_back(lenovo_legion_Y750_neon);
            break;
        case LEGION_Y750S:
        case LEGION_Y760S:
            switch(keyboard_type)
            {
                case JAPAN:
                    lenovo_zones.push_back(lenovo_legion_Y760_kbd_jp);
                    break;

                case ISO:
                    lenovo_zones.push_back(lenovo_legion_Y760_kbd_iso);
                    break;

                default:
                    lenovo_zones.push_back(lenovo_legion_Y760_kbd_ansi);
                    break;
            }
            break;
        case LEGION_Y760:
            switch(keyboard_type)
            {
                case JAPAN:
                    lenovo_zones.push_back(lenovo_legion_Y760_kbd_jp);
                    break;

                case ISO:
                    lenovo_zones.push_back(lenovo_legion_Y760_kbd_iso);
                    break;

                default:
                    lenovo_zones.push_back(lenovo_legion_Y760_kbd_ansi);
                    break;
            }
            lenovo_zones.push_back(lenovo_legion_Y760_logo);
            lenovo_zones.push_back(lenovo_legion_Y760_vent_left);
            lenovo_zones.push_back(lenovo_legion_Y760_vent_right);
            lenovo_zones.push_back(lenovo_legion_Y760_vent_back_right);
            lenovo_zones.push_back(lenovo_legion_Y760_vent_back_left);
            lenovo_zones.push_back(lenovo_legion_Y760_neon);
            break;
        case LEGION_S7GEN7:
            lenovo_zones.push_back(legion7_gen7and8_kbd_ansi);
            break;
        case LEGION_7GEN7:
            lenovo_zones.push_back(legion7_gen7and8_kbd_ansi);
            lenovo_zones.push_back(lenovo_legion_7gen7_logo);
            lenovo_zones.push_back(lenovo_legion_7gen7_vents);
            lenovo_zones.push_back(legion7_gen7and8_neon);
            break;
        case LEGION_7GEN8:
        case LEGION_7GEN9:
        case LEGION_7GEN9_H:
            lenovo_zones.push_back(legion7_gen7and8_kbd_ansi);
            lenovo_zones.push_back(legion7_gen7and8_neon);
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

        LOG_DEBUG("[Lenovo Controller] adding zone: %s with %u LEDs", new_zone.name.c_str(), new_zone.leds_count);

        if(lenovo_zones[i].type == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = new matrix_map_type;
            new_zone.matrix_map->height = lenovo_zones[i].height;
            new_zone.matrix_map->width  = lenovo_zones[i].width;
            new_zone.matrix_map->map    = (unsigned int *) lenovo_zones[i].matrix_map;
        }
        else
        {
            new_zone.matrix_map = NULL;
        }

        zones.push_back(new_zone);

        for(unsigned int led_idx = lenovo_zones[i].start; led_idx <= lenovo_zones[i].end; led_idx++ )
        {
            led new_led;
            new_led.name = lenovo_zones[i].leds[led_idx].name;
            new_led.value = ( lenovo_zones[i].id << 8 ) + lenovo_zones[i].leds[led_idx].led_num;
            leds.push_back(new_led);
        }
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
        controller->setSingleLED(leds[led].value >> 8, leds[led].value & 0xFF, colors[led]);
    }
}

void RGBController_LenovoUSB::UpdateZoneLEDs(int zone)
{
    uint8_t zone_id = zones[zone].leds_count > 0 ? leds[zones[zone].start_idx].value >> 8 : 0;
    vector<pair<uint8_t, RGBColor>> color_map;

    for(unsigned int i = 0; i < zones[zone].leds_count; i++)
    {
        int index = zones[zone].start_idx+i;

        color_map.push_back({(uint8_t)leds[index].value & 0xFF, colors[index]});
    }

    color_map.shrink_to_fit();

    controller->setZoneLeds(zone_id, color_map);
}

void RGBController_LenovoUSB::DeviceUpdateLEDs()
{
    uint8_t zone_id = 0;
    uint8_t prev_zone_id = 0;
    vector<pair<uint8_t, RGBColor>> curr_color_map;

    for(unsigned int i = 0; i < leds.size(); i++)
    {
        zone_id = leds[i].value >> 8;

        if((zone_id != prev_zone_id) && (prev_zone_id != 0))
        {
            controller->setZoneLeds(prev_zone_id, curr_color_map);
            curr_color_map.clear();
        }

        prev_zone_id = zone_id;

        curr_color_map.push_back({(uint8_t)(leds[i].value & 0xFF), colors[i]});
    }

    if(curr_color_map.size() > 0)
    {
        controller->setZoneLeds(prev_zone_id, curr_color_map);
    }
}

void RGBController_LenovoUSB::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | This device does not support multiple modes               |
    \*---------------------------------------------------------*/
}

void RGBController_LenovoUSB::DeviceSaveMode()
{
    /*---------------------------------------------------------*\
    | This device does not support saving or multiple modes     |
    \*---------------------------------------------------------*/
}

std::string RGBController_LenovoUSB::ConvertBytesToHex(const std::vector<uint8_t> &input)
{
    std::ostringstream temp_stream;
    for(const uint8_t &oneInputByte : input)
    {
        temp_stream << (temp_stream.tellp()==0 ? "" : " ") << std::setw(2) << std::setfill('0') << std::hex << (int)oneInputByte;
    }
    return temp_stream.str();
}

std::string RGBController_LenovoUSB::keyboardToString(LENOVO_KEYBOARD kb)
{
    switch(kb)
    {
        case LENOVO_KEYBOARD::ANSI:
            return "ANSI";
        case LENOVO_KEYBOARD::ISO:
            return "ISO";
        case LENOVO_KEYBOARD::JAPAN:
            return "JAPAN";
        default:
            return "Unknown";
    }
}

std::string RGBController_LenovoUSB::sizeToString(LENOVO_SIZE size)
{
    switch(size)
    {
        case LENOVO_SIZE::FIFTEEN:
            return "15\"";
        case LENOVO_SIZE::SEVENTEEN:
            return "17\"";
        default:
            return "Unknown";
    }
}

void RGBController_LenovoUSB::DumpControllerInformation()
{
    for(uint8_t i=1;i<=7;i++)
    {
        std::vector<uint8_t> response = controller->getInformation(i);
        LOG_TRACE("[Lenovo Controller] Read values [%02x]: %s", i, ConvertBytesToHex(response).c_str());
    }
}
