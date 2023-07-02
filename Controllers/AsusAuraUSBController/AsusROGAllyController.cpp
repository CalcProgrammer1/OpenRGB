/*-----------------------------------------*\
|  AsusROGAllyController.cpp                |
|                                           |
|  Driver for ASUS ROG Ally lighting        |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/12/2023   |
\*-----------------------------------------*/

#include "AsusROGAllyController.h"
#include <cstring>

ROGAllyController::ROGAllyController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

ROGAllyController::~ROGAllyController()
{
    hid_close(dev);
}

std::string ROGAllyController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ROGAllyController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

std::string ROGAllyController::GetVersion()
{
    return("");
}

void ROGAllyController::UpdateLeds
    (
    std::vector<RGBColor>    colors
    )
{
    unsigned char usb_buf[64];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5A;
    usb_buf[0x01]   = 0xD1;
    usb_buf[0x02]   = 0x08;
    usb_buf[0x03]   = 0x0C;

    for(unsigned int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        usb_buf[color_idx * 3 + 4]  = RGBGetRValue(colors[color_idx]);
        usb_buf[color_idx * 3 + 5]  = RGBGetGValue(colors[color_idx]);
        usb_buf[color_idx * 3 + 6]  = RGBGetBValue(colors[color_idx]);
    }

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ROGAllyController::UpdateDevice
    (
    unsigned char           mode,
    std::vector<RGBColor>   colors,
    unsigned char           speed,
    unsigned char           brightness,
    unsigned char           direction
    )
{
    unsigned char usb_buf[64];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5A;
    usb_buf[0x01]   = 0xB3;
    usb_buf[0x02]   = 0x00;
    usb_buf[0x03]   = mode;
    if(colors.size() > 0)
    {
        usb_buf[0x04]   = RGBGetRValue(colors[0]);
        usb_buf[0x05]   = RGBGetGValue(colors[0]);
        usb_buf[0x06]   = RGBGetBValue(colors[0]);
    }
    usb_buf[0x07]   = speed;
    usb_buf[0x08]   = direction;
    if(colors.size() > 1)
    {
        usb_buf[0x0A]   = RGBGetRValue(colors[1]);
        usb_buf[0x0B]   = RGBGetGValue(colors[1]);
        usb_buf[0x0C]   = RGBGetBValue(colors[1]);
    }

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5A;
    usb_buf[0x01]   = 0xB5;

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ROGAllyController::SaveMode()
{
    unsigned char usb_buf[64];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x5A;
    usb_buf[0x01]   = 0xB4;

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

