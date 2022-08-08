/*-----------------------------------------*\
|  AsusStrixClawController.cpp              |
|                                           |
|  Driver for ASUS Strix USB                |
|  lighting controller                      |
|                                           |
|  Mola19 08/06/2022                        |
\*-----------------------------------------*/

#include "AsusStrixClawController.h"

#include <cstring>
#include <string>

StrixClawController::StrixClawController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

StrixClawController::~StrixClawController()
{
    hid_close(dev);
}

std::string StrixClawController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string StrixClawController::GetSerialString()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

std::string StrixClawController::GetVersion()
{

    // asking the device to prepare version information
    unsigned char usb_buf_out[9];

    memset(usb_buf_out, 0x00, 9);

    usb_buf_out[0x00] = 0x07;
    usb_buf_out[0x01] = 0x80;

    hid_send_feature_report(dev, usb_buf_out, 9);

    // retrieving the version information
    unsigned char usb_buf_in[9];

    memset(usb_buf_in, 0x00, 9);

    usb_buf_in[0x00] = 0x07;

    hid_get_feature_report(dev, usb_buf_in, 9);

    return (std::to_string(usb_buf_in[1]) + std::to_string(usb_buf_in[2]));
}

void StrixClawController::SetScrollWheelLED(bool OnOff)
{
    unsigned char usb_buf[9];

    memset(usb_buf, 0x00, 9);

    usb_buf[0x00] = 0x07;
    usb_buf[0x01] = 0x07;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = OnOff;
    hid_send_feature_report(dev, usb_buf, 9);
}

void StrixClawController::SetLogoLED(uint8_t brightness)
{
    unsigned char usb_buf[9];

    memset(usb_buf, 0x00, 9);

    usb_buf[0x00] = 0x07;
    usb_buf[0x01] = 0x0a;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = 0x00;
    usb_buf[0x04] = brightness;
    hid_send_feature_report(dev, usb_buf, 9);
}
