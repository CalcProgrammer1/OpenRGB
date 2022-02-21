/*-----------------------------------------*\
|  NvidiaESAController.cpp                  |
|                                           |
|  Driver for Nvidia ESA lighting           |
|  controller                               |
|                                           |
|  Guimard Morgan (morg) 2/18/2022          |
\*-----------------------------------------*/
#include "NvidiaESAController.h"
#include <string.h>

NvidiaESAController::NvidiaESAController(hid_device* dev_handle, const hid_device_info& info)
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

NvidiaESAController::~NvidiaESAController()
{
    hid_close(dev);
}

std::string NvidiaESAController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string NvidiaESAController::GetSerialString()
{
    return(serial_number);
}

std::string NvidiaESAController::GetFirmwareVersion()
{
    return(version);
}

void NvidiaESAController::SetZoneColor(unsigned int zone_idx, RGBColor color)
{
    unsigned char red = 0x0F - 0x0F * RGBGetRValue(color) / 255.0;
    unsigned char grn = 0x0F - 0x0F * RGBGetGValue(color) / 255.0;
    unsigned char blu = 0x0F - 0x0F * RGBGetBValue(color) / 255.0;

    unsigned char usb_buf[4];

    usb_buf[0x00] = 0x42 + zone_idx;

    usb_buf[0x01] = red;
    usb_buf[0x02] = grn;
    usb_buf[0x03] = blu;

    hid_write(dev, usb_buf, 4);
}
