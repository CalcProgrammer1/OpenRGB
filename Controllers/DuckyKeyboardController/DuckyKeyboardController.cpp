/*---------------------------------------------------------*\
| DuckyKeyboardController.cpp                               |
|                                                           |
|   Driver for Ducky keyboard                               |
|                                                           |
|   Adam Honse (CalcProgrammer1)                04 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "DuckyKeyboardController.h"
#include "StringUtils.h"

DuckyKeyboardController::DuckyKeyboardController(hid_device* dev_handle, const char* path, const unsigned short pid, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    usb_pid     = pid;

    SendInitialize();
}

DuckyKeyboardController::~DuckyKeyboardController()
{
    hid_close(dev);
}

std::string DuckyKeyboardController::GetLocationString()
{
    return("HID: " + location);
}

std::string DuckyKeyboardController::GetNameString()
{
    return(name);
}

std::string DuckyKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned short DuckyKeyboardController::GetUSBPID()
{
    return(usb_pid);
}

void DuckyKeyboardController::SendColors
    (
    unsigned char*  color_data,
    unsigned int    color_data_size
    )
{
    unsigned int    bytes_sent;
    unsigned char*  color_data_ptr = color_data;

    SendInitializeColorPacket();

    for(int i = 0; i < 8; i++)
    {
        bytes_sent = SendColorDataPacket(i, color_data_ptr, color_data_size);

        color_data_ptr += bytes_sent;
        color_data_size -= bytes_sent;
    }

    SendTerminateColorPacket();
}

void DuckyKeyboardController::SendInitialize()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Initialize Direct Mode packet                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x41;
    usb_buf[0x02]   = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void DuckyKeyboardController::SendInitializeColorPacket()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Initialize Color packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x56;
    usb_buf[0x02]   = 0x81;
    usb_buf[0x05]   = 0x01;
    usb_buf[0x09]   = 0x08;
    usb_buf[0x0D]   = 0xAA;
    usb_buf[0x0E]   = 0xAA;
    usb_buf[0x0F]   = 0xAA;
    usb_buf[0x10]   = 0xAA;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

unsigned int DuckyKeyboardController::SendColorDataPacket
    (
    unsigned char   packet_id,
    unsigned char*  color_data,
    unsigned int    color_size
    )
{
    unsigned int bytes_sent;
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Color Data packet                              |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x56;
    usb_buf[0x02]   = 0x83;
    usb_buf[0x03]   = packet_id;

    if(packet_id == 0x00)
    {
        usb_buf[0x05]   = 0x01;
        usb_buf[0x09]   = 0x80;
        usb_buf[0x0A]   = 0x01;
        usb_buf[0x0C]   = 0xC1;
        usb_buf[0x11]   = 0xFF;
        usb_buf[0x12]   = 0xFF;
        usb_buf[0x13]   = 0xFF;
        usb_buf[0x14]   = 0xFF;
    }

    /*-----------------------------------------------------*\
    | Copy in color data                                    |
    \*-----------------------------------------------------*/
    if(packet_id == 0x00)
    {
        bytes_sent = 65 - 0x19;

        if(color_size < bytes_sent)
        {
            bytes_sent = color_size;
        }

        memcpy(&usb_buf[0x19], color_data, bytes_sent);
    }
    else
    {
        bytes_sent = 65 - 0x05;

        if(color_size < bytes_sent)
        {
            bytes_sent = color_size;
        }

        memcpy(&usb_buf[0x05], color_data, bytes_sent);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    return(bytes_sent);
}

void DuckyKeyboardController::SendTerminateColorPacket()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Terminate Color packet                         |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x28;
    usb_buf[0x05]   = 0xFF;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}
