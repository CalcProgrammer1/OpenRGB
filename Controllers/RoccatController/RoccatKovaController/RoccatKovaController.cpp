/*---------------------------------------------------------*\
| RoccatKovaController.cpp                                  |
|                                                           |
|   Driver for Roccat Kova                                  |
|                                                           |
|   Gustash                                     01 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "LogManager.h"
#include "RoccatKovaController.h"
#include "StringUtils.h"

RoccatKovaController::RoccatKovaController(hid_device* dev_handle, char *path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    SendInitialPacket();
    FetchFirmwareVersion();
}

RoccatKovaController::~RoccatKovaController()
{
    hid_close(dev);
}

std::string RoccatKovaController::GetLocation()
{
    return("HID: " + location);
}

std::string RoccatKovaController::GetName()
{
    return(name);
}

std::string RoccatKovaController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string RoccatKovaController::GetVersion()
{
    return(version);
}

void RoccatKovaController::SetColor(RGBColor color_wheel,
                                    RGBColor color_stripe,
                                    uint8_t mode,
                                    uint8_t speed,
                                    bool color_flow)
{
    bool is_off                                               = mode == ROCCAT_KOVA_MODE_OFF;
    uint8_t report_buf[ROCCAT_KOVA_PROFILE_WRITE_PACKET_SIZE] {00};
    FetchProfileData(report_buf);

    report_buf[0x0]                                           = ROCCAT_KOVA_PROFILE_REPORT_ID;
    report_buf[0x1]                                           = ROCCAT_KOVA_PROFILE_WRITE_PACKET_SIZE;

    report_buf[ROCCAT_KOVA_FLAGS_IDX]                        |= ROCCAT_KOVA_USE_CUSTOM_COLORS_MASK;
    if(is_off)
    {
        report_buf[ROCCAT_KOVA_FLAGS_IDX]                    &= ~ROCCAT_KOVA_LIGHTS_ON_MASK;
    }
    else
    {
        report_buf[ROCCAT_KOVA_FLAGS_IDX]                    |= ROCCAT_KOVA_LIGHTS_ON_MASK;
    }

    /*-------------------------------------------------*\
    | Set colors for each LED and reset the selected    |
    | preset color to ensure consistency                |
    \*-------------------------------------------------*/
    report_buf[ROCCAT_KOVA_WHEEL_IDX]                         = 0x0;
    report_buf[ROCCAT_KOVA_WHEEL_IDX + ROCCAT_KOVA_R_OFFSET]  = RGBGetRValue(color_wheel);
    report_buf[ROCCAT_KOVA_WHEEL_IDX + ROCCAT_KOVA_G_OFFSET]  = RGBGetGValue(color_wheel);
    report_buf[ROCCAT_KOVA_WHEEL_IDX + ROCCAT_KOVA_B_OFFSET]  = RGBGetBValue(color_wheel);
    report_buf[ROCCAT_KOVA_PIPE_IDX]                          = 0x0;
    report_buf[ROCCAT_KOVA_PIPE_IDX  + ROCCAT_KOVA_R_OFFSET]  = RGBGetRValue(color_stripe);
    report_buf[ROCCAT_KOVA_PIPE_IDX  + ROCCAT_KOVA_G_OFFSET]  = RGBGetGValue(color_stripe);
    report_buf[ROCCAT_KOVA_PIPE_IDX  + ROCCAT_KOVA_B_OFFSET]  = RGBGetBValue(color_stripe);

    report_buf[ROCCAT_KOVA_COLOR_FLOW_IDX]                    = color_flow;
    if(!is_off)
    {
        report_buf[ROCCAT_KOVA_MODE_IDX]                      = mode;
    }
    report_buf[ROCCAT_KOVA_EFFECT_SPEED_IDX]                  = speed;

    uint16_t checksum                                         = GenerateChecksum(report_buf, sizeof(report_buf) - 2);

    report_buf[ROCCAT_KOVA_CHECKSUM_IDX]                      = checksum & 0xFF;
    report_buf[ROCCAT_KOVA_CHECKSUM_IDX + 1]                  = checksum >> 8;

    hid_send_feature_report(dev, report_buf, ROCCAT_KOVA_PROFILE_WRITE_PACKET_SIZE);
}

void RoccatKovaController::SendInitialPacket()
{
    uint8_t buf[ROCCAT_KOVA_INIT_WRITE_PACKET_SIZE] {00};
    buf[0x00]                                       = ROCCAT_KOVA_INIT_REPORT_ID;
    buf[0x01]                                       = 0x00;
    buf[0x02]                                       = 0x80;
    hid_send_feature_report(dev, buf, ROCCAT_KOVA_INIT_WRITE_PACKET_SIZE);
}

void RoccatKovaController::FetchFirmwareVersion()
{
    uint8_t buf[ROCCAT_KOVA_VERSION_READ_PACKET_SIZE]      {00};
    buf[0x0]                                               = ROCCAT_KOVA_VERSION_REPORT_ID;

    hid_get_feature_report(dev, buf, ROCCAT_KOVA_VERSION_READ_PACKET_SIZE);

    uint8_t fw_version                                     = buf[ROCCAT_KOVA_FIRMWARE_VERSION_IDX];
    char version_str[5]                                    {00};
    snprintf(version_str, 5, "%.2f", fw_version / 100.);
    version                                                = version_str;
}

void RoccatKovaController::FetchProfileData(uint8_t *buf)
{
    buf[0x00] = ROCCAT_KOVA_PROFILE_REPORT_ID;
    hid_get_feature_report(dev, buf, ROCCAT_KOVA_PROFILE_WRITE_PACKET_SIZE);
}

uint16_t RoccatKovaController::GenerateChecksum(uint8_t *buf, size_t length)
{
    uint16_t checksum  = 0x0;
    for (uint8_t idx   = 0; idx < length; idx++)
    {
        checksum      += buf[idx];
    }
    return checksum;
}
