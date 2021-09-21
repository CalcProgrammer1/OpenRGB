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
    if (lightspeed->device_index == 255 && lightspeed->wireless)
    {
        LOG_DEBUG("[%s] Skipped get serial number as this is the reciever", lightspeed->device_name.c_str());
        return("");
    }
    else
    {
        wchar_t serial_string[128];
        //int ret = hid_get_serial_number_string(dev, serial_string, 128);
        //LOG_DEBUG("[%s] hid_get_serial_number_string Returned status - %i : %s", lightspeed->device_name.c_str(), ret, ((ret == 0) ? "SUCCESS" : "FAILED"));

        //if(ret != 0)
        {
            return("");
        }

        std::wstring return_wstring = serial_string;
        std::string return_string(return_wstring.begin(), return_wstring.end());

        return(return_string);
    }
}

void LogitechLightspeedController::SendMouseMode
    (
    unsigned char       mode,
    uint16_t            speed,
    unsigned char       zone,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue,
    unsigned char       brightness,
    bool                bright_cycle_swap
    )
{
    lightspeed->setMode(mode, speed, zone, red, green, blue, brightness, bright_cycle_swap);
}
