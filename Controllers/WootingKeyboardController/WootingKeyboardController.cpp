/*---------------------------------------------------------*\
| WootingKeyboardController.cpp                             |
|                                                           |
|   Driver for Wooting keyboard                             |
|                                                           |
|   Chris M (Dr_No)                             09 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "WootingKeyboardController.h"

WootingKeyboardController::WootingKeyboardController()
{

}

WootingKeyboardController::~WootingKeyboardController()
{

}

std::string WootingKeyboardController::GetName()
{
    return name;
}

std::string WootingKeyboardController::GetVendor()
{
    return vendor;
}

std::string WootingKeyboardController::GetLocation()
{
    return("HID: " + location);
}

std::string WootingKeyboardController::GetDescription()
{
    return description;
}

std::string WootingKeyboardController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

WOOTING_DEVICE_TYPE WootingKeyboardController::GetWootingType()
{
    return wooting_type;
}

void WootingKeyboardController::SendInitialize()
{
    wooting_usb_send_feature(WOOTING_COLOR_INIT_COMMAND, 0,0,0,0);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

bool WootingKeyboardController::wooting_usb_send_feature(uint8_t commandId, uint8_t parameter0, uint8_t parameter1, uint8_t parameter2, uint8_t parameter3)
{
    uint8_t feature_buffer[WOOTING_COMMAND_SIZE] = { 0, 0xD0, 0xDA };

    /*---------------------------------------------------------*\
    | Set up the Send Feature packet                            |
    \*---------------------------------------------------------*/
    feature_buffer[3] = commandId;
    feature_buffer[4] = parameter3;
    feature_buffer[5] = parameter2;
    feature_buffer[6] = parameter1;
    feature_buffer[7] = parameter0;

    /*---------------------------------------------------------*\
    | Send packet                                               |
    \*---------------------------------------------------------*/
    uint8_t report_size = hid_send_feature_report(dev, feature_buffer, WOOTING_COMMAND_SIZE);
    LOG_DEBUG("%sSend feature returned - %04i expected %04i", WOOTING_CONTROLLER_NAME, report_size, WOOTING_COMMAND_SIZE);
    return (report_size == WOOTING_COMMAND_SIZE);
}
