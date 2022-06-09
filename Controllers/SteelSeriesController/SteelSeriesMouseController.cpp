/*-------------------------------------------------------------------*\
|  SteelSeriesMouseController.cpp                                     |
|                                                                     |
|  OpenRGB abstract driver for SteelSeries Mice                       |
|                                                                     |
|  Chris M (Dr_No)         9th June 2022                              |
\*-------------------------------------------------------------------*/

#include <cstring>
#include "SteelSeriesMouseController.h"

SteelSeriesMouseController::SteelSeriesMouseController(hid_device*  dev_handle, steelseries_type proto_type, const char* path)
{
    dev         = dev_handle;
    location    = path;
    proto       = proto_type;
}

SteelSeriesMouseController::~SteelSeriesMouseController()
{

}

std::string SteelSeriesMouseController::GetDeviceLocation()
{
    return("HID: " + location);
}

char* SteelSeriesMouseController::GetDeviceName()
{
    return device_name;
}

std::string SteelSeriesMouseController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if (ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

steelseries_type SteelSeriesMouseController::GetMouseType()
{
    return proto;
}

void SteelSeriesMouseController::Save()
{
    const uint8_t   SAVE_BUFFER_SIZE            = 10;
    uint8_t         usb_buf[SAVE_BUFFER_SIZE]   = { 0x00, 0x09 };

    hid_write(dev, usb_buf, SAVE_BUFFER_SIZE);
}
