/*---------------------------------------------------------*\
| LogitechLightspeedController.cpp                          |
|                                                           |
|   Driver for Logitech Lightspeed                          |
|                                                           |
|   TheRogueZeta                                05 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogitechLightspeedController.h"
#include "StringUtils.h"

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

        std::string return_string(StringUtils::wstring_to_string(serial_string));

        return(return_string);
    }
}

void LogitechLightspeedController::SendMouseMode
    (
        uint8_t         mode,
        uint16_t        speed,
        uint8_t         zone,
        uint8_t         red,
        uint8_t         green,
        uint8_t         blue,
        uint8_t         brightness
    )
{
    lightspeed->setMode(mode, speed, zone, red, green, blue, brightness);
}
