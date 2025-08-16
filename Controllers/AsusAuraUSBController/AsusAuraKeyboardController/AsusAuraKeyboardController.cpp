/*---------------------------------------------------------*\
| AsusAuraKeyboardController.cpp                            |
|                                                           |
|   Driver for ASUS Aura keyboard                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Mar 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "AsusAuraKeyboardController.h"
#include "StringUtils.h"

AuraKeyboardController::AuraKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

AuraKeyboardController::~AuraKeyboardController()
{
    hid_close(dev);
}

std::string AuraKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraKeyboardController::GetNameString()
{
    return(name);
}

std::string AuraKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void AuraKeyboardController::SendDirect
    (
    unsigned char       frame_count,
    unsigned char *     frame_data
    )
{
    unsigned char usb_buf[65];
    unsigned int  packet_count = frame_count / 15 + (((frame_count % 15) == 0) ? 0 : 1);
    unsigned int  total_frame_idx = 0;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0xC0;
    usb_buf[0x02]   = 0x81;
    usb_buf[0x03]   = packet_count * 15;
    usb_buf[0x04]   = 0x00;

    /*-----------------------------------------------------*\
    | Copy in frame data and send packets until all data    |
    | has been sent                                         |
    \*-----------------------------------------------------*/
    for(unsigned int packet_idx = 0; packet_idx < packet_count; packet_idx++)
    {
        unsigned int packet_data_size = 15 * 4;

        if((frame_count - total_frame_idx) < 15)
        {
            packet_data_size = (frame_count - total_frame_idx) * 4;
            memset(&usb_buf[0x05 + packet_data_size], 0xFF, (15*4) - packet_data_size);
        }

        memcpy(&usb_buf[0x05], &frame_data[packet_idx * 15 * 4], packet_data_size);

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_write(dev, usb_buf, 65);

        /*-----------------------------------------------------*\
        | Decrement remaining frame count                       |
        \*-----------------------------------------------------*/
        usb_buf[0x03] -= 0x0F;
        total_frame_idx += 0x0F;
    }

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0xC0;
    usb_buf[0x02]   = 0x81;
    usb_buf[0x03]   = 0x90;

    /*-----------------------------------------------------*\
    | Read to apply                                         |
    \*-----------------------------------------------------*/
    hid_read(dev,usb_buf, 65);
}
