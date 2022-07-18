/*-----------------------------------------*\
|  SteelSeriesApexTZoneController.cpp       |
|                                           |
|  Edbgon 06.10.21                          |
\*-----------------------------------------*/


#include "SteelSeriesApexTZoneController.h"
#include <cstring>

SteelSeriesApexTZoneController::SteelSeriesApexTZoneController
    (
    hid_device*         dev_handle,
    steelseries_type    proto_type,
    const char*         path
    )
{
    dev         = dev_handle;
    location    = path;
    proto       = proto_type;
}

SteelSeriesApexTZoneController::~SteelSeriesApexTZoneController()
{
    hid_close(dev);
}

std::string SteelSeriesApexTZoneController::GetDeviceLocation()
{
    return("HID: " + location);
}

char* SteelSeriesApexTZoneController::GetDeviceName()
{
    return device_name;
}

std::string SteelSeriesApexTZoneController::GetSerialString()
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

steelseries_type SteelSeriesApexTZoneController::GetKeyboardType()
{
    return proto;
}

void SteelSeriesApexTZoneController::Save()
{
    unsigned char buf[STEELSERIES_TZ_WRITE_PACKET_SIZE] = { 0x00, 0x06, 0x00, 0x08 };

    hid_write(dev, buf, STEELSERIES_TZ_WRITE_PACKET_SIZE);

    buf[0x01]           = 0x09;
    buf[0x03]           = 0x00;
    hid_write(dev, buf, STEELSERIES_TZ_WRITE_PACKET_SIZE);
}

void SteelSeriesApexTZoneController::SetColor(std::vector<RGBColor> colors, unsigned char brightness)
{
    unsigned char buf[STEELSERIES_TZ_WRITE_PACKET_SIZE] = { 0x00 };

    /*-----------------------------------------------------*\
    | Zero out buffer, set up packet and send               |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, STEELSERIES_TZ_WRITE_PACKET_SIZE);

    buf[0x01]           = 0x0A;
    buf[0x03]           = brightness;
    hid_write(dev, buf, STEELSERIES_TZ_WRITE_PACKET_SIZE);

    buf[0x01]           = 0x0B;

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        uint8_t index   = i * 3;

        buf[index + 3]  = RGBGetRValue(colors[i]);;
        buf[index + 4]  = RGBGetGValue(colors[i]);;
        buf[index + 5]  = RGBGetBValue(colors[i]);;
    }

    hid_write(dev, buf, STEELSERIES_TZ_WRITE_PACKET_SIZE);
}
