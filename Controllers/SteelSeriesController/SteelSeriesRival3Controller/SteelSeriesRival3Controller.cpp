/*-----------------------------------------*\
|  SteelSeriesRival3Controller.h            |
|                                           |
|  Definitions and types for SteelSeries    |
|  Rival 3 lighting controller              |
|                                           |
|  B Horn (bahorn) 29/8/2021                |
\*-----------------------------------------*/

#include "SteelSeriesRival3Controller.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

SteelSeriesRival3Controller::SteelSeriesRival3Controller
    (
    hid_device*         dev_handle,
    steelseries_type    proto_type,
    const char*         path
    ) : SteelSeriesMouseController(dev_handle, proto_type, path)
{
    //dev         = dev_handle;
    //location    = path;
    //proto       = proto_type;
}

SteelSeriesRival3Controller::~SteelSeriesRival3Controller()
{
    hid_close(dev);
}

/*
std::string SteelSeriesRival3Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

char* SteelSeriesRival3Controller::GetDeviceName()
{
    return device_name;
}

std::string SteelSeriesRival3Controller::GetSerialString()
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
*/

std::string SteelSeriesRival3Controller::GetFirmwareVersion()
{
    const uint8_t   FW_BUFFER_SIZE              = 3;
    uint8_t         usb_buf[FW_BUFFER_SIZE]     = { 0x00, 0x10, 0x00 };
    uint16_t        version;
    std::string     return_string;

    hid_write(dev, usb_buf, FW_BUFFER_SIZE);
    hid_read(dev, (unsigned char *)&version, 2);

    return_string = std::to_string(version);
    return return_string;
}

steelseries_mouse SteelSeriesRival3Controller::GetMouse()
{
    return rival_3;
}

/*
steelseries_type SteelSeriesRival3Controller::GetMouseType()
{
    return proto;
}

void SteelSeriesRival3Controller::Save()
{
    const uint8_t   SAVE_BUFFER_SIZE            = 10;
    uint8_t         usb_buf[SAVE_BUFFER_SIZE]   = { 0x00, 0x09 };

    hid_write(dev, usb_buf, SAVE_BUFFER_SIZE);
}
*/

void SteelSeriesRival3Controller::SetLightEffectAll(uint8_t effect)
{
    const uint8_t   EFFECT_BUFFER_SIZE          = 4;
    uint8_t         usb_buf[EFFECT_BUFFER_SIZE] = { 0x00, 0x06, 0x00, effect };

    hid_write(dev, usb_buf, EFFECT_BUFFER_SIZE);
}

void SteelSeriesRival3Controller::SetColor
    (
    unsigned char   zone_id,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue,
    unsigned char   brightness
    )
{
    const uint8_t   COLOR_BUFFER_SIZE           = 8;
    uint8_t         usb_buf[COLOR_BUFFER_SIZE];

    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = 0x05;
    usb_buf[0x02] = 0x00;
    usb_buf[0x03] = zone_id;

    usb_buf[0x04] = red;
    usb_buf[0x05] = green;
    usb_buf[0x06] = blue;
    usb_buf[0x07] = brightness;

    hid_write(dev, usb_buf, COLOR_BUFFER_SIZE);
}
