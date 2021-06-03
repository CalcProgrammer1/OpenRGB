/*-----------------------------------------*\
|  LogitechLightspeedController.cpp         |
|                                           |
|  Driver for Logitech Lightspeed Wireless  |
|  Gaming Mice lighting controller          |
|                                           |
|  TheRogueZeta   8/5/2020                  |
\*-----------------------------------------*/

#include "LogitechLightspeedController.h"

#include <cstring>

LogitechLightspeedController::LogitechLightspeedController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

LogitechLightspeedController::~LogitechLightspeedController()
{
    delete lightspeed;
}

std::string LogitechLightspeedController::GetDeviceLocation()
{
    return("HID: " + location + " (Receiver) \r\nWireless Index: " + std::to_string(lightspeed->device_index));
}

std::string LogitechLightspeedController::GetSerialString()
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

void LogitechLightspeedController::SendMouseMode
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
