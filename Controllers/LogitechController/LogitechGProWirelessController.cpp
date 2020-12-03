/*-----------------------------------------*\
|  LogitechGProWirelessController.cpp       |
|                                           |
|  Driver for Logitech G Pro Wireless Gaming|
|  Mouse lighting controller                |
|                                           |
|  TheRogueZeta   8/5/2020                  |
\*-----------------------------------------*/

#include "LogitechGProWirelessController.h"

#include <cstring>

LogitechGProWirelessController::LogitechGProWirelessController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

LogitechGProWirelessController::~LogitechGProWirelessController()
{
    hid_close(dev);
}

std::string LogitechGProWirelessController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LogitechGProWirelessController::GetSerialString()
{
    wchar_t serial_string[128];
    hid_get_serial_number_string(dev, serial_string, 128);

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void LogitechGProWirelessController::SendMouseMode
    (
    unsigned char       mode,
    std::uint16_t       speed,
    unsigned char       zone,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
  //  unsigned char       brightness
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
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = 0x07;
    usb_buf[0x03]           = 0x3C;

    usb_buf[0x04]           = zone;
    usb_buf[0x05]           = mode;

    usb_buf[0x06]           = red;
    usb_buf[0x07]           = green;
    usb_buf[0x08]           = blue;

    speed = 100 * speed;
    if(mode == LOGITECH_G_PRO_WIRELESS_MODE_STATIC)
    {
        usb_buf[0x09]       = 0x02;
    }
    if(mode == LOGITECH_G_PRO_WIRELESS_MODE_CYCLE)
    {
        usb_buf[0x0B]       = speed >> 8;
        usb_buf[0x0C]       = speed & 0xFF;
        //usb_buf[0x0D]       = brightness;
        usb_buf[0x0D]       = 0x64;
    }
    else if(mode == LOGITECH_G_PRO_WIRELESS_MODE_BREATHING)
    {
        usb_buf[0x09]       = speed >> 8;
        usb_buf[0x0A]       = speed & 0xFF;
        //usb_buf[0x0C]       = brightness;
        usb_buf[0x0C]       = 0x64;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 20);
    hid_read(dev, usb_buf, 20);
}
