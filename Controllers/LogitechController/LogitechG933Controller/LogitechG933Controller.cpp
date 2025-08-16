/*---------------------------------------------------------*\
| LogitechG933Controller.cpp                                |
|                                                           |
|   Driver for Logitech G933                                |
|                                                           |
|   Edbgon                                      21 Jun 2021 |
|   Based on TheRogueZeta                       31 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "LogitechG933Controller.h"

using namespace std::chrono_literals;

LogitechG933Controller::LogitechG933Controller(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

LogitechG933Controller::~LogitechG933Controller()
{
    hid_close(dev);
}

std::string LogitechG933Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LogitechG933Controller::GetDeviceName()
{
    return(name);
}

void LogitechG933Controller::SetDirectMode(uint8_t zone)
{
    unsigned char usb_buf[LOGI_G933_LED_PACKET_SIZE];

    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x04;
    usb_buf[0x03]           = 0xCA;
    usb_buf[0x04]           = zone;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    fail_retry_write(dev, usb_buf, LOGI_G933_LED_PACKET_SIZE);
}

void LogitechG933Controller::SetOffMode(uint8_t zone)
{
    unsigned char usb_buf[LOGI_G933_LED_PACKET_SIZE];

    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x04;
    usb_buf[0x03]           = 0x3F;
    usb_buf[0x04]           = zone;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    fail_retry_write(dev, usb_buf, LOGI_G933_LED_PACKET_SIZE);
}

void LogitechG933Controller::SendHeadsetMode
    (
    unsigned char       zone,
    unsigned char       mode,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    unsigned char usb_buf[LOGI_G933_LED_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control                               |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x04;

    /*-----------------------------------------------------*\
    | This packet sets speaker into direct mode.  This mode |
    | is used by Lightsync Ambilight and Music Visualizer   |
    | realtime effect.                                      |
    \*-----------------------------------------------------*/
    usb_buf[0x03]           = 0x3A;

    /*-----------------------------------------------------*\
    | Set up mode and speed                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x04]           = zone;
    usb_buf[0x05]           = mode;

    /*-----------------------------------------------------*\
    | And set up the colors                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x06]           = red;
    usb_buf[0x07]           = green;
    usb_buf[0x08]           = blue;

    if(mode == LOGITECH_G933_MODE_DIRECT) //G933 only has Direct Mode.
    {
        usb_buf[0x09]       = 0x02;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    fail_retry_write(dev, usb_buf, LOGI_G933_LED_PACKET_SIZE);
}

void LogitechG933Controller::fail_retry_write(hid_device *device, const unsigned char *data, size_t length)
{
    unsigned char usb_buf_out[LOGI_G933_LED_PACKET_SIZE];
    unsigned int  write_max_retry = LOGI_G933_LED_COMMAND_SEND_RETRIES;
    do
    {
        std::this_thread::sleep_for(1ms);
        int ret = hid_write(device, data, length);

        /*-------------------------------------------------------------------------------------*\
        | HID write fails if a change led color and set volume command are sent at              |
        | the same time because RGB controller and volume control shares the same interface.    |
        \*-------------------------------------------------------------------------------------*/
        if(ret == 20)
        {
            std::this_thread::sleep_for(1ms);
            hid_read_timeout(dev, usb_buf_out, LOGI_G933_LED_PACKET_SIZE, 20);
            break;
        }
        else
        {
            write_max_retry--;
            std::this_thread::sleep_for(10ms);
        }

    }while (write_max_retry > 0);
}
