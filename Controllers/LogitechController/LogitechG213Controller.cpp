/*-----------------------------------------*\
|  LogitechG213Controller.cpp               |
|                                           |
|  Generic RGB Interface for Logitech G213  |
|                                           |
|  Eric Samuelson (edbgon) 10/06/2020       |
\*-----------------------------------------*/

#include "LogitechG213Controller.h"
#include <cstring>

LogitechG213Controller::LogitechG213Controller(hid_device* dev_handle)
{
    dev = dev_handle;
}

LogitechG213Controller::~LogitechG213Controller()
{

}

void LogitechG213Controller::SetDirect
    (
    unsigned char       zone,
    unsigned char       r,
    unsigned char       g,
    unsigned char       b
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
    usb_buf[0x02]           = 0x0C;
    usb_buf[0x03]           = 0x3A;
    usb_buf[0x04]           = zone;
    usb_buf[0x05]           = 0x01;
    usb_buf[0x06]           = r;
    usb_buf[0x07]           = g;
    usb_buf[0x08]           = b;
    usb_buf[0x09]           = 0x02;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 20);
}
