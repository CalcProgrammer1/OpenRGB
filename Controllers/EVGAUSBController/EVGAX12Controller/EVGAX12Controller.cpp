/*---------------------------------------------------------*\
| EVGAX12Controller.cpp                                     |
|                                                           |
|   Driver for the EVGA X12 gaming mouse                    |
|                                                           |
|   QuickMythril                               08 Aug 2026  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include <thread>
#include "EVGAX12Controller.h"
#include "LogManager.h"
#include "StringUtils.h"

#define EVGA_X12_HEADER                     0xEA
#define EVGA_X12_DIRECTION_GET              0x01
#define EVGA_X12_DIRECTION_SET              0x02
#define EVGA_X12_COMMAND_MODE               0x01
#define EVGA_X12_COMMAND_STATIC             0x02
#define EVGA_X12_START_WITH_PROFILE         0x01
#define EVGA_X12_READ_ATTEMPTS              5
#define EVGA_X12_READ_DELAY_MS              5
#define EVGA_X12_WRITE_ATTEMPTS             3

static const unsigned int EVGA_X12_LED_TO_FIRMWARE_SLOT[EVGA_X12_LED_COUNT] =
{
    EVGA_X12_FIRMWARE_SLOT_HEAD_LEFT,
    EVGA_X12_FIRMWARE_SLOT_WHEEL,
    EVGA_X12_FIRMWARE_SLOT_LOGO,
};

EVGAX12Controller::EVGAX12Controller(hid_device* dev_handle, const hid_device_info& info, const std::string& dev_name)
{
    dev         = dev_handle;
    location    = info.path;
    name        = dev_name;
}

EVGAX12Controller::~EVGAX12Controller()
{
    hid_close(dev);
}

std::string EVGAX12Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string EVGAX12Controller::GetNameString()
{
    return(name);
}

std::string EVGAX12Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

EVGAX12LightingState EVGAX12Controller::GetLightingState()
{
    std::lock_guard<std::mutex> lock(device_mutex);
    EVGAX12LightingState state = {};
    unsigned char packet[EVGA_X12_REPORT_SIZE];

    state.modes_valid = GetReport(EVGA_X12_COMMAND_MODE, packet);

    if(state.modes_valid)
    {
        for(unsigned int led_idx = 0; led_idx < EVGA_X12_LED_COUNT; led_idx++)
        {
            unsigned int firmware_slot = EVGA_X12_LED_TO_FIRMWARE_SLOT[led_idx];
            state.modes[led_idx]        = packet[4 + firmware_slot];
        }
    }

    state.static_valid = GetReport(EVGA_X12_COMMAND_STATIC, packet);

    if(state.static_valid)
    {
        for(unsigned int led_idx = 0; led_idx < EVGA_X12_LED_COUNT; led_idx++)
        {
            unsigned int firmware_slot  = EVGA_X12_LED_TO_FIRMWARE_SLOT[led_idx];
            unsigned int offset         = 4 + (firmware_slot * 4);
            state.brightness[led_idx]   = packet[offset];
            state.colors[led_idx]       = ToRGBColor(packet[offset + 1], packet[offset + 2], packet[offset + 3]);
        }
    }

    return(state);
}

bool EVGAX12Controller::SetMode(unsigned char mode)
{
    std::lock_guard<std::mutex> lock(device_mutex);
    return(SetModeReport(mode));
}

bool EVGAX12Controller::SetModeReport(unsigned char mode)
{
    unsigned char packet[EVGA_X12_REPORT_SIZE] = {};

    packet[0] = EVGA_X12_REPORT_ID;
    packet[1] = EVGA_X12_HEADER;
    packet[2] = EVGA_X12_DIRECTION_SET;
    packet[3] = EVGA_X12_COMMAND_MODE;

    for(unsigned int led_idx = 0; led_idx < EVGA_X12_LED_COUNT; led_idx++)
    {
        unsigned int firmware_slot  = EVGA_X12_LED_TO_FIRMWARE_SLOT[led_idx];
        packet[4 + firmware_slot]   = mode;
    }

    return(SendReport(packet));
}

bool EVGAX12Controller::SetStatic(const std::vector<RGBColor>& colors, unsigned char brightness)
{
    if(colors.size() < EVGA_X12_LED_COUNT)
    {
        LOG_ERROR("[%s] Cannot set static lighting with only %u colors", name.c_str(), (unsigned int)colors.size());
        return(false);
    }

    std::lock_guard<std::mutex> lock(device_mutex);
    return(SetStaticReport(colors, brightness));
}

bool EVGAX12Controller::SetStaticAndActivate(const std::vector<RGBColor>& colors, unsigned char brightness)
{
    if(colors.size() < EVGA_X12_LED_COUNT)
    {
        LOG_ERROR("[%s] Cannot set static lighting with only %u colors", name.c_str(), (unsigned int)colors.size());
        return(false);
    }

    std::lock_guard<std::mutex> lock(device_mutex);

    if(!SetStaticReport(colors, brightness))
    {
        return(false);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(EVGA_X12_READ_DELAY_MS));
    return(SetModeReport(EVGA_X12_MODE_STATIC));
}

bool EVGAX12Controller::SetStaticReport(const std::vector<RGBColor>& colors, unsigned char brightness)
{
    unsigned char packet[EVGA_X12_REPORT_SIZE] = {};

    packet[0] = EVGA_X12_REPORT_ID;
    packet[1] = EVGA_X12_HEADER;
    packet[2] = EVGA_X12_DIRECTION_SET;
    packet[3] = EVGA_X12_COMMAND_STATIC;

    for(unsigned int firmware_slot = 0; firmware_slot < EVGA_X12_FIRMWARE_SLOT_COUNT; firmware_slot++)
    {
        packet[20 + firmware_slot] = EVGA_X12_START_WITH_PROFILE;
    }

    for(unsigned int led_idx = 0; led_idx < EVGA_X12_LED_COUNT; led_idx++)
    {
        unsigned int firmware_slot  = EVGA_X12_LED_TO_FIRMWARE_SLOT[led_idx];
        unsigned int offset         = 4 + (firmware_slot * 4);
        packet[offset]              = brightness;
        packet[offset + 1]          = RGBGetRValue(colors[led_idx]);
        packet[offset + 2]          = RGBGetGValue(colors[led_idx]);
        packet[offset + 3]          = RGBGetBValue(colors[led_idx]);
    }

    for(unsigned int attempt = 0; attempt < EVGA_X12_WRITE_ATTEMPTS; attempt++)
    {
        if(!SendReport(packet))
        {
            return(false);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(EVGA_X12_READ_DELAY_MS));

        unsigned char reply[EVGA_X12_REPORT_SIZE];

        if(GetReport(EVGA_X12_COMMAND_STATIC, reply))
        {
            bool matches = true;

            for(unsigned int firmware_slot = 0; firmware_slot < EVGA_X12_FIRMWARE_SLOT_COUNT; firmware_slot++)
            {
                unsigned int offset = 4 + (firmware_slot * 4);

                if(std::memcmp(&packet[offset], &reply[offset], 4) != 0)
                {
                    matches = false;
                    break;
                }
            }

            if(matches)
            {
                return(true);
            }
        }
    }

    LOG_VERBOSE("[%s] Retries exhausted setting static lighting", name.c_str());
    return(false);
}

bool EVGAX12Controller::GetReport(unsigned char command, unsigned char* packet)
{
    std::memset(packet, 0, EVGA_X12_REPORT_SIZE);
    packet[0] = EVGA_X12_REPORT_ID;
    packet[1] = EVGA_X12_HEADER;
    packet[2] = EVGA_X12_DIRECTION_GET;
    packet[3] = command;

    if(!SendReport(packet))
    {
        return(false);
    }

    for(unsigned int attempt = 0; attempt < EVGA_X12_READ_ATTEMPTS; attempt++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(EVGA_X12_READ_DELAY_MS));
        std::memset(packet, 0, EVGA_X12_REPORT_SIZE);
        packet[0] = EVGA_X12_REPORT_ID;

        int bytes_read = hid_get_feature_report(dev, packet, EVGA_X12_REPORT_SIZE);

        if(bytes_read == EVGA_X12_REPORT_SIZE && packet[1] == EVGA_X12_HEADER && packet[3] == command)
        {
            return(true);
        }

        if(bytes_read < 0)
        {
            LOG_DEBUG("[%s] Error reading feature report: %ls", name.c_str(), hid_error(dev));
            return(false);
        }
    }

    LOG_VERBOSE("[%s] Retries exhausted reading command 0x%02X", name.c_str(), command);
    return(false);
}

bool EVGAX12Controller::SendReport(const unsigned char* packet)
{
    int bytes_written = hid_send_feature_report(dev, packet, EVGA_X12_REPORT_SIZE);

    if(bytes_written != EVGA_X12_REPORT_SIZE)
    {
        LOG_DEBUG("[%s] Error writing feature report: %ls", name.c_str(), hid_error(dev));
        return(false);
    }

    return(true);
}
