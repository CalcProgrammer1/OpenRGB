/*-----------------------------------------*\
|  LogitechG203Controller.cpp               |
|                                           |
|  Driver for Logitech G203 Prodigy mouse   |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/17/2020   |
\*-----------------------------------------*/

#include "LogitechG203Controller.h"

#include <cstring>

LogitechG203Controller::LogitechG203Controller(hid_device* dev_handle)
{
    dev = dev_handle;
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void LogitechG203Controller::SendMouseMode
    (
    unsigned char       mode,
    unsigned short      speed,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    char usb_buf[20];

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
    usb_buf[0x04]           = 0x00;

    usb_buf[0x05]           = mode;

    usb_buf[0x06]           = red;
    usb_buf[0x07]           = green;
    usb_buf[0x08]           = blue;

    speed = 1000 + 4750 * (LOGITECH_G203_SPEED_FASTEST - speed);
    if(mode == LOGITECH_G203_MODE_CYCLE)
    {
        usb_buf[0x0B]   = speed >> 8;
        usb_buf[0x0C]   = speed & 0xFF;
        usb_buf[0x0D]   = 0x64;
    }
    else if(mode == LOGITECH_G203_MODE_BREATHING)
    {
        usb_buf[0x09]   = speed >> 8;
        usb_buf[0x0A]   = speed & 0xFF;
        usb_buf[0x0C]   = 0x64;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 20);
    hid_read(dev, (unsigned char *)usb_buf, 20);
}
