/*---------------------------------------------------------*\
| MSIMonitorController.cpp                                  |
|                                                           |
|   Driver for MSI monitor (gaming controller)              |
|                                                           |
|   Andy Herbert                              2026 June  4  |
|   Ken Sanislo                               2026 July 20  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string.h>
#include "MSIMonitorController.h"
#include "StringUtils.h"

MSIMonitorController::MSIMonitorController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

MSIMonitorController::~MSIMonitorController()
{
    hid_close(dev);
}

std::string MSIMonitorController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIMonitorController::GetNameString()
{
    return(name);
}

std::string MSIMonitorController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void MSIMonitorController::Set(uint8_t mode_value, const std::vector<RGBColor> colors, uint8_t last_bit)
{
    /*---------------------------------------------------------*\
    | Prepare color data                                        |
    \*---------------------------------------------------------*/
    uint8_t data[MSI_MONITOR_PACKET_SIZE];
    memset(data, 0x00, MSI_MONITOR_PACKET_SIZE);

    unsigned int offset = 0;

    data[offset++] = 0x71;
    data[offset++] = 0x01;
    for(int i = 0; i < 3; i++)
    {
        data[offset++] = 0x00;
    }

    data[offset++] = 0x01;
    data[offset++] = 0x64;
    for(int i = 0; i < 5; i++)
    {
        data[offset++] = 0x00;
    }

    /*---------------------------------------------------------*\
    | put mode_value                                            |
    \*---------------------------------------------------------*/
    data[offset++] = mode_value;

    for(int i = 0; i < 3; i++)
    {
        data[offset++] = 0x00;
    }
    data[offset++] = 0x01;
    data[offset++] = 0x64;
    for(int i = 0; i < 5; i++)
    {
        data[offset++] = 0x00;
    }

    /*-----------------------------------------------------------------------*\
    | this data looks like placeholder for additional LEDs in other monitors  |
    \*-----------------------------------------------------------------------*/
    for(int i = 0; i < 9; i++)
    {
        data[offset++] = 0xff;
        data[offset++] = 0x00;
        data[offset++] = 0x00;
    }

    //RGB values begin
    for(const RGBColor color: colors)
    {
        data[offset++] = RGBGetRValue(color);
        data[offset++] = RGBGetGValue(color);
        data[offset++] = RGBGetBValue(color);
    }

    /*-------------------------------------------------------------------------------*\
    | last bit is probably a write to device bit- 0x01 saves to device, 0x00 doesn't. |
    | For direct mode, bit is set to 0x00, otherwise lights will flicker              |
    \*-------------------------------------------------------------------------------*/

    data[offset++] = last_bit;

    /*---------------------------------------------------------*\
    | Send the data (1 packet)                                  |
    \*---------------------------------------------------------*/

    hid_send_feature_report(dev, data, MSI_MONITOR_PACKET_SIZE);
}

uint8_t MSIMonitorController::GetLayoutVersion()
{
    /*-----------------------------------------------------*\
    | Ask the MCU which buffer layout it speaks.  Request   |
    | is [0x01][opcode] zero padded, reply is [0x01][0x5A]  |
    | then data with the version at byte 3.                 |
    \*-----------------------------------------------------*/
    uint8_t request[MSI_MONITOR_QUERY_PACKET_SIZE];
    uint8_t reply[MSI_MONITOR_QUERY_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Unimplemented opcodes never answer, so a reply left   |
    | queued by an earlier exchange would be read here and  |
    | parsed as this query's answer.  Drain first.          |
    \*-----------------------------------------------------*/
    while(hid_read_timeout(dev, reply, MSI_MONITOR_QUERY_PACKET_SIZE, 0) > 0)
    {
        continue;
    }

    memset(request, 0x00, MSI_MONITOR_QUERY_PACKET_SIZE);

    request[0] = MSI_MONITOR_QUERY_REPORT_ID;
    request[1] = MSI_MONITOR_OPCODE_GET_VERSION;

    if(hid_write(dev, request, MSI_MONITOR_QUERY_PACKET_SIZE) < 0)
    {
        return(MSI_MONITOR_VERSION_UNKNOWN);
    }

    memset(reply, 0x00, MSI_MONITOR_QUERY_PACKET_SIZE);

    if(hid_read_timeout(dev, reply, MSI_MONITOR_QUERY_PACKET_SIZE, 500) <= 0)
    {
        return(MSI_MONITOR_VERSION_UNKNOWN);
    }

    if(reply[1] != MSI_MONITOR_QUERY_ACK)
    {
        return(MSI_MONITOR_VERSION_UNKNOWN);
    }

    return(reply[3]);
}

void MSIMonitorController::SetMode72(uint8_t mode_value, uint8_t speed, uint8_t brightness, RGBColor color1, RGBColor color2, bool user_palette, bool save, const std::vector<RGBColor>& led_colors, bool fill_both_arrays)
{
    uint8_t data[MSI_MONITOR_72_PACKET_SIZE];
    memset(data, 0x00, MSI_MONITOR_72_PACKET_SIZE);

    data[0] = 0x72;

    /*-----------------------------------------------------*\
    | Identical 11 byte control blocks at index 1 and 12    |
    \*-----------------------------------------------------*/
    for(unsigned int base = 1; base <= 12; base += 11)
    {
        data[base + 0]  = mode_value;
        data[base + 1]  = RGBGetRValue(color1);
        data[base + 2]  = RGBGetGValue(color1);
        data[base + 3]  = RGBGetBValue(color1);
        data[base + 4]  = speed;
        data[base + 5]  = brightness;
        data[base + 6]  = RGBGetRValue(color2);
        data[base + 7]  = RGBGetGValue(color2);
        data[base + 8]  = RGBGetBValue(color2);
        data[base + 9]  = user_palette ? 0x80 : 0x00;
        data[base + 10] = 0x00;
    }

    /*-----------------------------------------------------*\
    | The engine paints from the second inline colour array |
    | at index 95, not from Colour1 and not from the first  |
    | array at index 23: on a 322URX a buffer carrying only |
    | Colour1 renders black, and walking one lit slot       |
    | through the buffer only lights LEDs for the second    |
    | array.  That pairs with the control block at index 12 |
    | being the operative one.  Built in palette modes      |
    | leave the array clear so the firmware effect is not   |
    | painted over.                                         |
    |                                                       |
    | fill_both_arrays also writes the first array at index |
    | 23, for the dual block versions (11/13/15) whose two  |
    | control blocks each drive their own run.  On a single |
    | block layout the first array is inert, so filling it  |
    | is harmless.  Those versions also route colour index  |
    | above 15 to a separate DragonShield emblem region not |
    | handled here - one reason they stay disabled.         |
    \*-----------------------------------------------------*/
    if(user_palette)
    {
        for(unsigned int slot = 0; slot < MSI_MONITOR_72_ARRAY_SLOTS && slot < led_colors.size(); slot++)
        {
            unsigned int offset_b = MSI_MONITOR_72_ARRAY_B_INDEX + (slot * 3);

            data[offset_b + 0] = RGBGetRValue(led_colors[slot]);
            data[offset_b + 1] = RGBGetGValue(led_colors[slot]);
            data[offset_b + 2] = RGBGetBValue(led_colors[slot]);

            if(fill_both_arrays)
            {
                unsigned int offset_a = MSI_MONITOR_72_ARRAY_A_INDEX + (slot * 3);

                data[offset_a + 0] = RGBGetRValue(led_colors[slot]);
                data[offset_a + 1] = RGBGetGValue(led_colors[slot]);
                data[offset_a + 2] = RGBGetBValue(led_colors[slot]);
            }
        }
    }

    /*-----------------------------------------------------*\
    | Final byte is store to flash: 0x01 persists across    |
    | power cycles, 0x00 applies without a flash write.     |
    | Keep 0x00 for streaming to avoid flicker and wear.    |
    \*-----------------------------------------------------*/
    data[MSI_MONITOR_72_STORE_INDEX] = save ? 0x01 : 0x00;

    hid_send_feature_report(dev, data, MSI_MONITOR_72_PACKET_SIZE);
}
