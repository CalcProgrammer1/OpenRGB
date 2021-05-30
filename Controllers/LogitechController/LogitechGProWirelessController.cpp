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
    delete lightspeed;
}

std::string LogitechGProWirelessController::GetDeviceLocation()
{
    return("HID: " + location + " (Receiver) \r\nWireless Index: " + std::to_string(lightspeed->device_index));
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
    lightspeed->setMode(mode, speed, zone, red, green, blue, 0x64);
}
