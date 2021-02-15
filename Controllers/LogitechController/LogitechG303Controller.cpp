/*-----------------------------------------*\
|  LogitechG303Controller.cpp               |
|                                           |
|  Driver for Logitech G303 Daedalus Apex   |
|  mouse lighting controller                |
|                                           |
|  TheRogueZeta   02/14/2021                |
\*-----------------------------------------*/

#include "LogitechG303Controller.h"

#include <cstring>

LogitechG303Controller::LogitechG303Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

LogitechG303Controller::~LogitechG303Controller()
{
    hid_close(dev);
}

std::string LogitechG303Controller::GetDeviceLocation()
{
    return(location);
}

void LogitechG303Controller::SendMouseMode
    (
    unsigned char       mode,
    std::uint16_t       speed,
    unsigned char       channel,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    unsigned char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x0E;
    usb_buf[0x03]           = 0x3C;
    usb_buf[0x04]           = channel;

    usb_buf[0x05]           = mode;

    usb_buf[0x06]           = red;
    usb_buf[0x07]           = green;
    usb_buf[0x08]           = blue;

    speed = 100 * speed;
    if(mode == LOGITECH_G303_MODE_CYCLE)
    {
        usb_buf[0x0B]   = speed >> 8;
        usb_buf[0x0C]   = speed & 0xFF;
        usb_buf[0x0D]   = 0x64;
    }
    else if(mode == LOGITECH_G303_MODE_BREATHING)
    {
        usb_buf[0x09]   = speed >> 8;
        usb_buf[0x0A]   = speed & 0xFF;
        usb_buf[0x0C]   = 0x64;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 20);
    hid_read(dev, usb_buf, 20);
}
