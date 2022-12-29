/*-----------------------------------------*\
|  GigabyteAorusMouseController.cpp         |
|                                           |
|  Driver for Gigabyte Aorus Mouse lighting |
|  controller                               |
|                                           |
|  Guimard Morgan (morg) 12/29/2022         |
\*-----------------------------------------*/
#include "GigabyteAorusMouseController.h"
#include <string.h>

GigabyteAorusMouseController::GigabyteAorusMouseController(hid_device* dev_handle, const hid_device_info& info)
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

GigabyteAorusMouseController::~GigabyteAorusMouseController()
{
    hid_close(dev);
}

std::string GigabyteAorusMouseController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string GigabyteAorusMouseController::GetSerialString()
{
    return(serial_number);
}

std::string GigabyteAorusMouseController::GetFirmwareVersion()
{
    return(version);
}

void GigabyteAorusMouseController::SetMode(RGBColor color, uint8_t mode_value, uint8_t brightness, uint8_t speed)
{
    uint8_t usb_buf[GIGABYTE_AORUS_MOUSE_REPORT_SIZE];

    usb_buf[0] = GIGABYTE_AORUS_MOUSE_HARDWARE_CMD;
    usb_buf[1] = mode_value;
    usb_buf[2] = brightness;
    usb_buf[3] = RGBGetRValue(color);
    usb_buf[4] = RGBGetGValue(color);
    usb_buf[5] = RGBGetBValue(color);
    usb_buf[6] = speed;
    usb_buf[7] = 0x00;

    hid_send_feature_report(dev, usb_buf, GIGABYTE_AORUS_MOUSE_REPORT_SIZE);
}

void GigabyteAorusMouseController::SendDirect(RGBColor color)
{
    uint8_t usb_buf[8];

    memset(usb_buf, 0x00, GIGABYTE_AORUS_MOUSE_REPORT_SIZE);

    usb_buf[0] = GIGABYTE_AORUS_MOUSE_DIRECT_CMD;
    usb_buf[2] = RGBGetRValue(color);
    usb_buf[3] = RGBGetGValue(color);
    usb_buf[4] = RGBGetBValue(color);

    hid_send_feature_report(dev, usb_buf, GIGABYTE_AORUS_MOUSE_REPORT_SIZE);
}
