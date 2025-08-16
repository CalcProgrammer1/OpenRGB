/*---------------------------------------------------------*\
| RoccatSenseAimoController.cpp                             |
|                                                           |
|   Driver for Roccat Sense Aimo                            |
|                                                           |
|   Mola19                                      09 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogManager.h"
#include "RoccatSenseAimoController.h"
#include "StringUtils.h"

RoccatSenseAimoController::RoccatSenseAimoController(hid_device* dev_handle, char *path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

RoccatSenseAimoController::~RoccatSenseAimoController()
{
    hid_close(dev);
}

std::string RoccatSenseAimoController::GetLocation()
{
    return("HID: " + location);
}

std::string RoccatSenseAimoController::GetName()
{
    return(name);
}

std::string RoccatSenseAimoController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string RoccatSenseAimoController::GetVersion()
{
    uint8_t buf[8] = { 0x01 };
    int return_length = hid_get_feature_report(dev, buf, 5);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Sense Aimo]: Could not fetch mode. HIDAPI Error: %ls", hid_error(dev));
        return std::string("Unknown");
    }

    char version[6];
    snprintf(version, 6, "%2X.%02X", buf[1], buf[2]);

    return std::string(version);
}

mode_struct RoccatSenseAimoController::GetMode()
{
    uint8_t buf[19] = { 0x02 };
    int return_length = hid_get_feature_report(dev, buf, 19);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Sense Aimo]: Could not fetch mode. HIDAPI Error: %ls", hid_error(dev));
        mode_struct default_mode;

        default_mode.profile = 0;
        default_mode.mode = ROCCAT_SENSE_AIMO_MODE_STATIC;
        default_mode.speed = 0;
        default_mode.brightness = ROCCAT_SENSE_AIMO_BRIGHTNESS_DEFAULT;
        default_mode.left = ToRGBColor(0, 0, 0);
        default_mode.right = ToRGBColor(0, 0, 0);

        return default_mode;
    }

    mode_struct active_mode;

    active_mode.profile = buf[1];
    active_mode.mode = buf[2];
    active_mode.speed = buf[3];
    active_mode.brightness = buf[4];
    active_mode.left = ToRGBColor(buf[6], buf[7], buf[8]);
    active_mode.right = ToRGBColor(buf[14], buf[15], buf[16]);

    return active_mode;
}

void RoccatSenseAimoController::SetMode(uint8_t profile, uint8_t mode, uint8_t speed, uint8_t brightness, std::vector<RGBColor> colors)
{
    uint8_t buf[19];
    memset(buf, 0x00, 19);

    buf[0x00] = 0x02;
    buf[0x01] = profile;

    for(uint8_t i = 0; i < 2; i++)
    {
        buf[0x02 + i * 8] = mode; // this device has per led modes
        buf[0x03 + i * 8] = speed;
        buf[0x04 + i * 8] = brightness;
        buf[0x05 + i * 8] = 0x00;
        buf[0x06 + i * 8] = RGBGetRValue(colors[i]);
        buf[0x07 + i * 8] = RGBGetGValue(colors[i]);
        buf[0x08 + i * 8] = RGBGetBValue(colors[i]);
        buf[0x09 + i * 8] = 0xFF; // this device uses RGBA, but OpenRGB doesn't allow it, so it is always max
    }

    buf[0x12] = 0x00; // this stores the swarm theme and first bit is a flag if custom is active in swarm. No usage outside Swarm

    int return_length = hid_send_feature_report(dev, buf, 19);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Sense Aimo]: Could not send mode. HIDAPI Error: %ls", hid_error(dev));
    }
}

void RoccatSenseAimoController::SendDirect(std::vector<RGBColor> colors)
{
    uint8_t buf[9];
    memset(buf, 0x00, 9);

    buf[0x00] = 0x03;

    for(uint8_t i = 0; i < 2; i++)
    {
        buf[0x01 + i * 4] = RGBGetRValue(colors[i]);
        buf[0x02 + i * 4] = RGBGetGValue(colors[i]);
        buf[0x03 + i * 4] = RGBGetBValue(colors[i]);
        buf[0x04 + i * 4] = 0xFF; // this device uses RGBA, but OpenRGB doesn't allow it, so it is always max
    }

    int return_length = hid_send_feature_report(dev, buf, 9);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Sense Aimo]: Could not send direct. HIDAPI Error: %ls", hid_error(dev));
    }
}
