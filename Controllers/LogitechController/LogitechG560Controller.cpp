/*-----------------------------------------*\
|  LogitechG560Controller.cpp               |
|                                           |
|  Driver for Logitech G560 RGB Speaker     |
|  Charging System                          |
|                                           |
|  Cheerpipe     10/28/2020                 |
|  Based on                                 |
|  TheRogueZeta   8/31/2020                 |
\*-----------------------------------------*/

#include "LogitechG560Controller.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;

LogitechG560Controller::LogitechG560Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

LogitechG560Controller::~LogitechG560Controller()
{
    hid_close(dev);
}

std::string LogitechG560Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

void LogitechG560Controller::SetDirectMode(uint8_t zone)
{
    unsigned char usb_buf[20];

    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x04;
    usb_buf[0x03]           = 0xCA;
    usb_buf[0x04]           = zone;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    fail_retry_write(dev, usb_buf, 20);
}

void LogitechG560Controller::SetOffMode(uint8_t zone)
{
    unsigned char usb_buf[20];

    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x04;
    usb_buf[0x03]           = 0x3F;
    usb_buf[0x04]           = zone;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    fail_retry_write(dev, usb_buf, 20);
}

void LogitechG560Controller::SendSpeakerMode
    (
    unsigned char       zone,           //0x04
    unsigned char       mode,           //0x05
    unsigned char       speed,          //0x0B // not working so far
    unsigned char       red,            //0x06
    unsigned char       green,          //0x07
    unsigned char       blue            //0x08
    )
{
    unsigned char usb_buf[20];

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

    if(mode == LOGITECH_G560_MODE_DIRECT) //G560 only has Direct Mode.
    {
        usb_buf[0x09]       = 0x02;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    fail_retry_write(dev, usb_buf, 20);
}

void LogitechG560Controller::fail_retry_write(hid_device *device, const unsigned char *data, size_t length)
{
    unsigned char usb_buf_out[33];
    uint8_t write_max_retry=3;
    do
    {
        std::this_thread::sleep_for(1ms);
        int ret = hid_write(device, data, length);

        /*-------------------------------------------------------------------------------------*\
        | HID write fails if a change led color and set volume command are sent at              |
        | the same time because RGB controller and volume control shares the same interface.    |
        \*-------------------------------------------------------------------------------------*/
        if (ret > 0)
        {
            std::this_thread::sleep_for(1ms);
            hid_read_timeout(dev, usb_buf_out, 33, 100);
            break;
        }
        else
        {
            write_max_retry--;
        }

    }while (write_max_retry > 0);
}
