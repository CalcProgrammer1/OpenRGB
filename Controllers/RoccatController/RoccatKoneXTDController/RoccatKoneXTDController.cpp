/*---------------------------------------------------------*\
| RoccatKoneXTDController.cpp                               |
|                                                           |
|   Driver for Roccat Kone XTD                              |
|                                                           |
|   Julian                                      14 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include <thread>
#include "RoccatKoneXTDController.h"
#include "StringUtils.h"

/*---------------------------------------------------------*\
| Feature reports on interface 0 (usage page 0x0B), same    |
| family as the Kone[+]:                                    |
|   0x04   3 bytes  control: write 04 PP 80 to select the   |
|                   profile 0x06 reads; read byte 1 =       |
|                   1 ok, 2 error, 3 busy                   |
|   0x05   3 bytes  05 03 PP active profile (0-4)           |
|   0x06  43 bytes  profile settings                        |
|   0x09   6 bytes  info, byte 2 = firmware version * 100   |
|   0x10  16 bytes  live colors:                            |
|                   10 10 02 01 R1 G1 B1 .. R4 G4 B4        |
|                   10 10 00 FF 00.. releases the LEDs      |
|                                                           |
| Profile settings (0x06):                                  |
|   0x00  06 2B PP                                          |
|   0x14  lights_enabled, bit n = LED n                     |
|   0x15  light_effect_mode (color source)                  |
|   0x16  color_flow_effect                                 |
|   0x17  light_effect_type                                 |
|   0x18  light_effect_speed                                |
|   0x19  4 x { palette index, R, G, B }                    |
|   0x29  16-bit sum of bytes 0x00-0x28                     |
\*---------------------------------------------------------*/
#define KONE_XTD_REPORT_CONTROL         0x04
#define KONE_XTD_REPORT_PROFILE         0x05
#define KONE_XTD_REPORT_SETTINGS        0x06
#define KONE_XTD_REPORT_INFO            0x09
#define KONE_XTD_REPORT_TALK            0x10

#define KONE_XTD_CONTROL_LENGTH         3
#define KONE_XTD_PROFILE_LENGTH         3
#define KONE_XTD_SETTINGS_LENGTH        0x2B
#define KONE_XTD_INFO_LENGTH            6
#define KONE_XTD_TALK_LENGTH            0x10

#define KONE_XTD_CONTROL_READ_SETTINGS  0x80
#define KONE_XTD_STATUS_OK              1
#define KONE_XTD_STATUS_BUSY            3

#define KONE_XTD_LIGHTS_ENABLED_OFFSET  0x14
#define KONE_XTD_EFFECT_MODE_OFFSET     0x15
#define KONE_XTD_COLOR_FLOW_OFFSET      0x16
#define KONE_XTD_EFFECT_TYPE_OFFSET     0x17
#define KONE_XTD_EFFECT_SPEED_OFFSET    0x18
#define KONE_XTD_LIGHTS_OFFSET          0x19
#define KONE_XTD_CHECKSUM_OFFSET        0x29

RoccatKoneXTDController::RoccatKoneXTDController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    unsigned char buf[KONE_XTD_INFO_LENGTH];
    memset(buf, 0x00, KONE_XTD_INFO_LENGTH);
    buf[0x00] = KONE_XTD_REPORT_INFO;

    if(hid_get_feature_report(dev, buf, KONE_XTD_INFO_LENGTH) > 2)
    {
        char version_str[8];
        snprintf(version_str, sizeof(version_str), "%.2f", buf[0x02] / 100.0);
        version = version_str;
    }
}

RoccatKoneXTDController::~RoccatKoneXTDController()
{
    hid_close(dev);
}

std::string RoccatKoneXTDController::GetLocation()
{
    return("HID: " + location);
}

std::string RoccatKoneXTDController::GetName()
{
    return(name);
}

std::string RoccatKoneXTDController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string RoccatKoneXTDController::GetVersion()
{
    return(version);
}

/*---------------------------------------------------------*\
| Poll the control report while it says busy                |
\*---------------------------------------------------------*/
bool RoccatKoneXTDController::WaitReady()
{
    for(unsigned int attempt = 0; attempt < 100; attempt++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        unsigned char status[KONE_XTD_CONTROL_LENGTH];
        status[0] = KONE_XTD_REPORT_CONTROL;

        int ret = hid_get_feature_report(dev, status, KONE_XTD_CONTROL_LENGTH);

        if(ret < 2)
        {
            return(false);
        }

        if(status[1] != KONE_XTD_STATUS_BUSY)
        {
            return(status[1] == KONE_XTD_STATUS_OK);
        }
    }

    return(false);
}

unsigned char RoccatKoneXTDController::GetActiveProfile()
{
    unsigned char buf[KONE_XTD_PROFILE_LENGTH];
    memset(buf, 0x00, KONE_XTD_PROFILE_LENGTH);
    buf[0x00] = KONE_XTD_REPORT_PROFILE;

    if(hid_get_feature_report(dev, buf, KONE_XTD_PROFILE_LENGTH) < KONE_XTD_PROFILE_LENGTH)
    {
        return(0);
    }

    return(buf[0x02]);
}

void RoccatKoneXTDController::SendDirectColors(const std::vector<RGBColor>& colors)
{
    unsigned char buf[KONE_XTD_TALK_LENGTH];
    memset(buf, 0x00, KONE_XTD_TALK_LENGTH);

    buf[0x00] = KONE_XTD_REPORT_TALK;
    buf[0x01] = KONE_XTD_TALK_LENGTH;
    buf[0x02] = 0x02;
    buf[0x03] = 0x01;

    for(unsigned int led = 0; led < ROCCAT_KONE_XTD_NUMBER_OF_LEDS && led < colors.size(); led++)
    {
        buf[0x04 + led * 3 + 0] = RGBGetRValue(colors[led]);
        buf[0x04 + led * 3 + 1] = RGBGetGValue(colors[led]);
        buf[0x04 + led * 3 + 2] = RGBGetBValue(colors[led]);
    }

    hid_send_feature_report(dev, buf, KONE_XTD_TALK_LENGTH);
}

void RoccatKoneXTDController::ReleaseDirect()
{
    unsigned char buf[KONE_XTD_TALK_LENGTH];
    memset(buf, 0x00, KONE_XTD_TALK_LENGTH);

    buf[0x00] = KONE_XTD_REPORT_TALK;
    buf[0x01] = KONE_XTD_TALK_LENGTH;
    buf[0x02] = 0x00;
    buf[0x03] = 0xFF;

    hid_send_feature_report(dev, buf, KONE_XTD_TALK_LENGTH);
}

void RoccatKoneXTDController::SetProfileLighting(bool                         lights_on,
                                                 unsigned char                effect_type,
                                                 unsigned char                color_flow,
                                                 unsigned char                color_mode,
                                                 unsigned char                speed,
                                                 const std::vector<RGBColor>& colors)
{
    /*-----------------------------------------------------*\
    | Read the active profile's settings so everything      |
    | that is not lighting (DPI, polling rate...) is kept   |
    \*-----------------------------------------------------*/
    unsigned char profile = GetActiveProfile();

    unsigned char control[KONE_XTD_CONTROL_LENGTH];
    control[0x00] = KONE_XTD_REPORT_CONTROL;
    control[0x01] = profile;
    control[0x02] = KONE_XTD_CONTROL_READ_SETTINGS;

    hid_send_feature_report(dev, control, KONE_XTD_CONTROL_LENGTH);

    if(!WaitReady())
    {
        return;
    }

    unsigned char buf[KONE_XTD_SETTINGS_LENGTH];
    memset(buf, 0x00, KONE_XTD_SETTINGS_LENGTH);
    buf[0x00] = KONE_XTD_REPORT_SETTINGS;

    if(hid_get_feature_report(dev, buf, KONE_XTD_SETTINGS_LENGTH) < KONE_XTD_SETTINGS_LENGTH
    || buf[0x01] != KONE_XTD_SETTINGS_LENGTH)
    {
        return;
    }

    buf[0x02]                           = profile;
    buf[KONE_XTD_LIGHTS_ENABLED_OFFSET] = lights_on ? 0x0F : 0x00;
    buf[KONE_XTD_EFFECT_MODE_OFFSET]    = color_mode;
    buf[KONE_XTD_COLOR_FLOW_OFFSET]     = color_flow;
    buf[KONE_XTD_EFFECT_TYPE_OFFSET]    = effect_type;
    buf[KONE_XTD_EFFECT_SPEED_OFFSET]   = speed;

    /*-----------------------------------------------------*\
    | Keep each light's palette index, only replace RGB     |
    \*-----------------------------------------------------*/
    for(unsigned int led = 0; led < ROCCAT_KONE_XTD_NUMBER_OF_LEDS && led < colors.size(); led++)
    {
        buf[KONE_XTD_LIGHTS_OFFSET + led * 4 + 1] = RGBGetRValue(colors[led]);
        buf[KONE_XTD_LIGHTS_OFFSET + led * 4 + 2] = RGBGetGValue(colors[led]);
        buf[KONE_XTD_LIGHTS_OFFSET + led * 4 + 3] = RGBGetBValue(colors[led]);
    }

    unsigned int sum = 0;

    for(unsigned int i = 0; i < KONE_XTD_CHECKSUM_OFFSET; i++)
    {
        sum += buf[i];
    }

    buf[KONE_XTD_CHECKSUM_OFFSET]     = sum & 0xFF;
    buf[KONE_XTD_CHECKSUM_OFFSET + 1] = (sum >> 8) & 0xFF;

    hid_send_feature_report(dev, buf, KONE_XTD_SETTINGS_LENGTH);

    WaitReady();
}
