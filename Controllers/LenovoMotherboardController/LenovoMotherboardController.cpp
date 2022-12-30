/*-----------------------------------------*\
|  LenovoMotherboardController.cpp          |
|                                           |
|  Driver for Lenovo motherboards lighting  |
|  controller                               |
|                                           |
|  Guimard Morgan (morg) 12/26/2022         |
\*-----------------------------------------*/
#include "LenovoMotherboardController.h"
#include <string.h>

LenovoMotherboardController::LenovoMotherboardController(hid_device* dev_handle, const hid_device_info& info)
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

LenovoMotherboardController::~LenovoMotherboardController()
{
    hid_close(dev);
}

std::string LenovoMotherboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LenovoMotherboardController::GetSerialString()
{
    return(serial_number);
}

std::string LenovoMotherboardController::GetFirmwareVersion()
{
    return(version);
}

void LenovoMotherboardController::SetMode(uint8_t zone, uint8_t mode, uint8_t brightness, uint8_t speed, RGBColor color)
{
    uint8_t r = RGBGetRValue(color);
    uint8_t g = RGBGetGValue(color);
    uint8_t b = RGBGetBValue(color);

    uint8_t buffer[LENOVO_MB_PACKET_LENGTH];
    memset(buffer, 0x00, LENOVO_MB_PACKET_LENGTH);

    buffer[0]   = LENOVO_MB_REPORT_ID;

    buffer[1]   = zone;
    buffer[2]   = mode;
    buffer[3]   = speed;
    buffer[4]   = brightness;

    buffer[5]   = r;
    buffer[6]   = g;
    buffer[7]   = b;

    hid_send_feature_report(dev, buffer, LENOVO_MB_PACKET_LENGTH);

    memset(buffer, 0x00, LENOVO_MB_PACKET_LENGTH);

    buffer[0]   = LENOVO_MB_REPORT_ID;
    buffer[1]   = 0x28;
    buffer[2]   = 0x06;

    buffer[33]  = zone;
    buffer[34]  = mode;
    buffer[35]  = speed;
    buffer[36]  = brightness;

    buffer[37]  = r;
    buffer[38]  = g;
    buffer[39]  = b;

    hid_send_feature_report(dev, buffer, LENOVO_MB_PACKET_LENGTH);
}
