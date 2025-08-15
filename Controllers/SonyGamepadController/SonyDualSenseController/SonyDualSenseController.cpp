/*---------------------------------------------------------*\
| SonyDualSenseController.cpp                               |
|                                                           |
|   Driver for Sony DualSense                               |
|                                                           |
|   Flora Aubry                                 01 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <CRC.h>
#include <hidapi.h>
#include "SonyDualSenseController.h"
#include "StringUtils.h"

SonyDualSenseController::SonyDualSenseController(hid_device * device_handle, const char * device_path, bool is_bluetooth, std::string dev_name)
{
    dev                 = device_handle;
    location            = device_path;
    name                = dev_name;
    this->is_bluetooth  = is_bluetooth;
}

SonyDualSenseController::~SonyDualSenseController()
{
    hid_close(dev);
}

std::string SonyDualSenseController::GetLocation()
{
    return("HID: " + location);
}

std::string SonyDualSenseController::GetName()
{
    return(name);
}

std::string SonyDualSenseController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void SonyDualSenseController::SetColors(std::vector<RGBColor> colors, unsigned char brightness, unsigned char mode_value)
{
    if(is_bluetooth)
    {
        unsigned char buffer[SONY_DUALSENSE_BT_PACKET_SIZE + 1];
        memset(buffer, 0x00, SONY_DUALSENSE_BT_PACKET_SIZE + 1);

        buffer[0]      = 0xA2;
        buffer[1]      = 0x31;
        buffer[2]      = 0x02;
        buffer[3]      = 0x0F;
        buffer[4]      = 0x55;

        buffer[11]     = mode_value;
        buffer[41]     = 0xFF; // Must be > 0x00 to control birghtness
        buffer[44]     = 0x02; // bypass default blue color when connected to bluetooth
        buffer[45]     = 0x02 - brightness;
        buffer[46]     = 0x20
                       + ((colors[1] > 0)     )
                       + ((colors[2] > 0) << 1)
                       + ((colors[3] > 0) << 2)
                       + ((colors[4] > 0) << 3)
                       + ((colors[5] > 0) << 4);

        buffer[47]     = RGBGetRValue(colors[0]);
        buffer[48]     = RGBGetGValue(colors[0]);
        buffer[49]     = RGBGetBValue(colors[0]);

        uint32_t crc = CRCPP::CRC::Calculate(buffer, SONY_DUALSENSE_BT_PACKET_SIZE - 3, CRCPP::CRC::CRC_32());
        unsigned char outbuffer[SONY_DUALSENSE_BT_PACKET_SIZE];

        /*-------------------------------------------------*\
        | The report has to be signed with the byte 0xa2.   |
        | However, hidapi already adds 0xa2 to the report,  |
        | so we need to remove it from the buffer.          |
        \*-------------------------------------------------*/
        for(unsigned int i = 1; i < SONY_DUALSENSE_BT_PACKET_SIZE + 1; i++)
        {
            outbuffer[i - 1] = buffer[i];
        }

        /*-------------------------------------------------*\
        | Add the crc32 to the end of the buffer            |
        \*-------------------------------------------------*/
        outbuffer[74] = (0x000000FF & crc);
        outbuffer[75] = (0x0000FF00 & crc) >> 8;
        outbuffer[76] = (0x00FF0000 & crc) >> 16;
        outbuffer[77] = (0xFF000000 & crc) >> 24;

        hid_write(dev, outbuffer, SONY_DUALSENSE_BT_PACKET_SIZE);
    }
    else
    {
        unsigned char usb_buf[SONY_DUALSENSE_USB_PACKET_SIZE];
        memset(usb_buf, 0x00, SONY_DUALSENSE_USB_PACKET_SIZE);

        usb_buf[0]      = 0x02;
        usb_buf[1]      = 0x0F;
        usb_buf[2]      = 0x55;
        usb_buf[9]      = mode_value;
        usb_buf[39]     = 0xFF; // Must be > 0x00 to control birghtness
        usb_buf[43]     = 0x02 - brightness;
        usb_buf[44]     = 0x20
                        + ((colors[1] > 0)     )
                        + ((colors[2] > 0) << 1)
                        + ((colors[3] > 0) << 2)
                        + ((colors[4] > 0) << 3)
                        + ((colors[5] > 0) << 4);

        usb_buf[45]     = RGBGetRValue(colors[0]);
        usb_buf[46]     = RGBGetGValue(colors[0]);
        usb_buf[47]     = RGBGetBValue(colors[0]);

        hid_write(dev, usb_buf, SONY_DUALSENSE_USB_PACKET_SIZE);
    }
}

bool SonyDualSenseController::IsBluetooth()
{
    return(is_bluetooth);
}
