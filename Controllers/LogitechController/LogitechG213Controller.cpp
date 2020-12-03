/*-----------------------------------------*\
|  LogitechG213Controller.cpp               |
|                                           |
|  Generic RGB Interface for Logitech G213  |
|                                           |
|  Eric Samuelson (edbgon) 10/06/2020       |
\*-----------------------------------------*/

#include "LogitechG213Controller.h"
#include <cstring>

LogitechG213Controller::LogitechG213Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

LogitechG213Controller::~LogitechG213Controller()
{
    hid_close(dev);
}

std::string LogitechG213Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LogitechG213Controller::GetSerialString()
{
    wchar_t serial_string[128];
    hid_get_serial_number_string(dev, serial_string, 128);

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
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
