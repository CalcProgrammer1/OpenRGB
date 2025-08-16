/*---------------------------------------------------------*\
| RoccatKoneProController.cpp                               |
|                                                           |
|   Driver for Roccat Kone Pro                              |
|                                                           |
|   Garrett Denham (GardenOfWyers)              12 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "RoccatKoneProController.h"
#include "StringUtils.h"

RoccatKoneProController::RoccatKoneProController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;

    SetupDirectMode();
}

RoccatKoneProController::~RoccatKoneProController()
{
    hid_close(dev);
}

std::string RoccatKoneProController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RoccatKoneProController::GetNameString()
{
    return(name);
}

std::string RoccatKoneProController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void RoccatKoneProController::SetupDirectMode()
{
    SwitchControl(true);
}

void RoccatKoneProController::SwitchControl(bool direct)
{
    unsigned char usb_buf[ROCCAT_KONE_PRO_CONTROL_MODE_PACKET_LENGTH];

    usb_buf[0x00] = 0x0E;
    usb_buf[0x01] = 0x06;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = direct ? 0x01 : 0x00;
    usb_buf[0x04] = 0x00;
    usb_buf[0x05] = 0xFF;

    hid_send_feature_report(dev, usb_buf, ROCCAT_KONE_PRO_CONTROL_MODE_PACKET_LENGTH);
}

void RoccatKoneProController::SendDirect(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[ROCCAT_KONE_PRO_DIRECT_MODE_PACKET_LENGTH];

    memset(usb_buf, 0x00, ROCCAT_KONE_PRO_DIRECT_MODE_PACKET_LENGTH);

    usb_buf[0x00] = ROCCAT_KONE_PRO_DIRECT_MODE_REPORT_ID;
    usb_buf[0x01] = ROCCAT_KONE_PRO_DIRECT_MODE_BYTE;

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        usb_buf[0x02 + 3 * i] = RGBGetRValue(colors[i]);
        usb_buf[0x03 + 3 * i] = RGBGetGValue(colors[i]);
        usb_buf[0x04 + 3 * i] = RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, usb_buf, ROCCAT_KONE_PRO_DIRECT_MODE_PACKET_LENGTH);
}

void RoccatKoneProController::SetMode(std::vector<RGBColor> colors, unsigned char mode_value, unsigned char speed, unsigned char brightness, unsigned int color_mode, unsigned int mode_flags)
{
    /*---------------------------------------------------------*\
    | 1. Read from flash                                        |
    \*---------------------------------------------------------*/
    unsigned char usb_buf[ROCCAT_KONE_PRO_FLASH_PACKET_LENGTH];
    memset(usb_buf, 0x00, ROCCAT_KONE_PRO_FLASH_PACKET_LENGTH);

    hid_get_feature_report(dev, usb_buf, ROCCAT_KONE_PRO_FLASH_PACKET_LENGTH);

    /*---------------------------------------------------------*\
    | 2. Update needed bytes                                    |
    \*---------------------------------------------------------*/
    usb_buf[0x00] = 0x06;
    usb_buf[0x01] = 0x45;

    usb_buf[0x03] = 0x06;
    usb_buf[0x04] = 0x06;
    usb_buf[0x05] = 0x1F;

    usb_buf[0x1E] = mode_value;
    usb_buf[0x1F] = mode_flags & MODE_FLAG_HAS_SPEED ? speed : 0xFF;
    usb_buf[0x20] = brightness;

    if(color_mode & MODE_COLORS_MODE_SPECIFIC)
    {
        for(unsigned int i = 0; i < colors.size(); i++)
        {
            usb_buf[0x24 + 5 * i] = 0x14;
            usb_buf[0x25 + 5 * i] = 0xFF;
            usb_buf[0x26 + 5 * i] = RGBGetRValue(colors[i]);
            usb_buf[0x27 + 5 * i] = RGBGetGValue(colors[i]);
            usb_buf[0x28 + 5 * i] = RGBGetBValue(colors[i]);
        }
    }
    else if(color_mode & MODE_COLORS_NONE)
    {
        for(unsigned int i = 0; i < colors.size(); i++)
        {
            usb_buf[0x24 + 5 * i] = 0x14;
            usb_buf[0x25 + 5 * i] = 0x00;
            usb_buf[0x26 + 5 * i] = 0x00;
            usb_buf[0x27 + 5 * i] = 0x00;
            usb_buf[0x28 + 5 * i] = 0x00;
        }
    }

    usb_buf[0x2E] = 0x14;
    usb_buf[0x2F] = 0xFF;

    unsigned int crc = CalculateCRC(&usb_buf[0x00]);

    usb_buf[0x43] = (unsigned char) crc;
    usb_buf[0x44] = crc >> 8;

    /*---------------------------------------------------------*\
    | 3. Send to flash                                          |
    \*---------------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, ROCCAT_KONE_PRO_FLASH_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    /*---------------------------------------------------------*\
    | 4. Switch to built-in mode                                |
    \*---------------------------------------------------------*/
    SwitchControl(false);
}

unsigned int RoccatKoneProController::CalculateCRC(unsigned char* bytes)
{
    unsigned int crc = 0;

    for(unsigned int i = 0; i < ROCCAT_KONE_PRO_FLASH_PACKET_LENGTH - 2; i++)
    {
        crc += bytes[i];
    }

    return crc;
}
