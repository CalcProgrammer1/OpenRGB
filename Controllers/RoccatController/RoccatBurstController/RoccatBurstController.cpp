/*---------------------------------------------------------*\
| RoccatBurstController.cpp                                 |
|                                                           |
|   Driver for Roccat Burst                                 |
|                                                           |
|   Morgan Guimard (morg)                       24 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "RoccatBurstController.h"
#include "StringUtils.h"

RoccatBurstController::RoccatBurstController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;

    SetupDirectMode();
}

RoccatBurstController::~RoccatBurstController()
{
    hid_close(dev);
}

std::string RoccatBurstController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RoccatBurstController::GetNameString()
{
    return(name);
}

std::string RoccatBurstController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void RoccatBurstController::SetupDirectMode()
{
    SwitchControl(true);
}

void RoccatBurstController::SwitchControl(bool direct)
{
    unsigned char usb_buf[ROCCAT_BURST_CONTROL_MODE_PACKET_LENGTH];

    usb_buf[0x00] = 0x0E;
    usb_buf[0x01] = 0x06;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = direct ? 0x01 : 0x00;
    usb_buf[0x04] = 0x00;
    usb_buf[0x05] = 0xFF;

    hid_send_feature_report(dev, usb_buf, ROCCAT_BURST_CONTROL_MODE_PACKET_LENGTH);
}

void RoccatBurstController::SendDirect(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[ROCCAT_BURST_DIRECT_MODE_PACKET_LENGTH];

    memset(usb_buf, 0x00, ROCCAT_BURST_DIRECT_MODE_PACKET_LENGTH);

    usb_buf[0x00] = ROCCAT_BURST_DIRECT_MODE_REPORT_ID;
    usb_buf[0x01] = ROCCAT_BURST_DIRECT_MODE_BYTE;

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        usb_buf[0x02 + 3 * i] = RGBGetRValue(colors[i]);
        usb_buf[0x03 + 3 * i] = RGBGetGValue(colors[i]);
        usb_buf[0x04 + 3 * i] = RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, usb_buf, ROCCAT_BURST_DIRECT_MODE_PACKET_LENGTH);
}

void RoccatBurstController::SetMode(std::vector<RGBColor> colors, unsigned char mode_value, unsigned char speed, unsigned char brightness, unsigned int color_mode, unsigned int mode_flags)
{
    /*---------------------------------------------------------*\
    | 1. Read from flash                                        |
    \*---------------------------------------------------------*/
    unsigned char usb_buf[ROCCAT_BURST_FLASH_PACKET_LENGTH];
    memset(usb_buf, 0x00, ROCCAT_BURST_FLASH_PACKET_LENGTH);

    usb_buf[0x00] = 0x06;

    hid_get_feature_report(dev, usb_buf, ROCCAT_BURST_FLASH_PACKET_LENGTH);

    /*---------------------------------------------------------*\
    | 2. Update needed bytes                                    |
    \*---------------------------------------------------------*/
    usb_buf[0x01] = 0x3F;
    usb_buf[0x03] = 0x06;
    usb_buf[0x04] = 0x06;
    usb_buf[0x05] = 0x1F;

    usb_buf[30] = mode_value;
    usb_buf[31] = mode_flags & MODE_FLAG_HAS_SPEED ? speed : 0xFF;
    usb_buf[32] = brightness;

    usb_buf[34] = 0xFF;

    if(color_mode & MODE_COLORS_MODE_SPECIFIC)
    {
        usb_buf[36] = 0x14;
        usb_buf[37] = 0xFF;
        for(unsigned int i = 0; i < colors.size(); i++)
        {
            usb_buf[38 + 10 * i] = RGBGetRValue(colors[i]);
            usb_buf[39 + 10 * i] = RGBGetGValue(colors[i]);
            usb_buf[40 + 10 * i] = RGBGetBValue(colors[i]);
        }
    }
    else if (color_mode & MODE_COLORS_NONE)
    {
        for(unsigned int i = 0; i < colors.size(); i++)
        {
            usb_buf[38 + 10 * i] = 0xF4;
            usb_buf[39 + 10 * i] = 0x00;
            usb_buf[40 + 10 * i] = 0x00;
        }
    }

    unsigned int crc = CalculateCRC(&usb_buf[0]);

    usb_buf[61] = (unsigned char) crc;
    usb_buf[62] = crc >> 8;

    /*---------------------------------------------------------*\
    | 3. Send to flash                                          |
    \*---------------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, ROCCAT_BURST_FLASH_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    /*---------------------------------------------------------*\
    | 4. Switch to built-in mode                                |
    \*---------------------------------------------------------*/
    SwitchControl(false);
}

unsigned int RoccatBurstController::CalculateCRC(unsigned char* bytes)
{
    unsigned int crc = 0;

    for(unsigned int i = 0; i < ROCCAT_BURST_FLASH_PACKET_LENGTH - 2; i++)
    {
        crc += bytes[i];
    }

    return crc;
}
