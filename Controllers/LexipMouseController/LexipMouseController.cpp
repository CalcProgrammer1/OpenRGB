/*-----------------------------------------*\
|  LexipMouseController.cpp                 |
|                                           |
|  Driver for Lexip mouse lighting          |
|  controller                               |
|                                           |
|  Guimard Morgan (morg) 2/21/2022          |
\*-----------------------------------------*/
#include "LexipMouseController.h"
#include <string.h>

LexipMouseController::LexipMouseController(hid_device* dev_handle, const hid_device_info& info)
{
    dev                 = dev_handle;
    location            = info.path;
    version             = "";

    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        serial_number = "";
    }
    else
    {
        std::wstring return_wstring = serial_string;
        serial_number = std::string(return_wstring.begin(), return_wstring.end());
    }

}

LexipMouseController::~LexipMouseController()
{
    hid_close(dev);
}

std::string LexipMouseController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LexipMouseController::GetSerialString()
{
    return(serial_number);
}

std::string LexipMouseController::GetFirmwareVersion()
{
    return(version);
}

void LexipMouseController::SetDirect(RGBColor color)
{
    /*-----------------------------------------*\
    |  Send a change color packet               |
    |                                           |
    |  URB INTERRUPT OUT, pad a leading zero    |
    |                                           |
    |  00 24 01 RR GG BB 00 64 80 00 .... 00    |
    \*-----------------------------------------*/
    unsigned char usb_buf[PACKET_DATA_LENGTH];

    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x01] = 0x24;                   // constant data
    usb_buf[0x02] = 0x01;                   // constant data

    usb_buf[0x03] = RGBGetRValue(color);    // red channel
    usb_buf[0x04] = RGBGetGValue(color);    // green channel
    usb_buf[0x05] = RGBGetBValue(color);    // blue channel

    usb_buf[0x07] = 0x64;                   // constant data
    usb_buf[0x08] = 0x80;                   // constant data

    hid_write(dev, usb_buf, PACKET_DATA_LENGTH);
}
