/*---------------------------------------------------------*\
| SinowealthController.cpp                                  |
|                                                           |
|   Driver for Sinowealth mice, including Glorious          |
|                                                           |
|   Niels Westphal (crashniels)                 20 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogManager.h"
#include "SinowealthController.h"
#include "StringUtils.h"

SinowealthController::SinowealthController(hid_device* dev_data_handle, hid_device* dev_cmd_handle, char *_path, std::string dev_name)
{
    dev_data            = dev_data_handle;
    dev_cmd             = dev_cmd_handle;
    location            = _path;
    name                = dev_name;

    led_count           = 1;

    current_mode        = GLORIOUS_MODE_STATIC;
    current_speed       = GLORIOUS_SPEED_NORMAL;
    current_direction   = GLORIOUS_DIRECTION_UP;
}

SinowealthController::~SinowealthController()
{
    hid_close(dev_data);

    /*---------------------------------------------------------------------*\
    | If the dev_cmd handle was passed in as the same device as dev_data    |
    |   then attempting to close it a second time will segfault             |
    \*---------------------------------------------------------------------*/
    if(dev_cmd)
    {
        hid_close(dev_cmd);
    }
}

std::string SinowealthController::GetLocation()
{
    return("HID: " + location);
}

std::string SinowealthController::GetName()
{
    return(name);
}

unsigned int SinowealthController::GetLEDCount()
{
    return(led_count);
}

std::string SinowealthController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev_cmd, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string SinowealthController::GetFirmwareVersion()
{
    unsigned char usb_buf[SINOWEALTH_COMMAND_REPORT_SIZE + 1]; // Additional byte for null-terminator
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0] = 5;
    usb_buf[1] = 1;

    int ret = hid_send_feature_report(dev_cmd, usb_buf, SINOWEALTH_COMMAND_REPORT_SIZE);
    if(ret < 0) return("");

    usb_buf[1] = 0;
    ret = hid_get_feature_report(dev_cmd, usb_buf, SINOWEALTH_COMMAND_REPORT_SIZE);
    if(ret < 0) return("");

    return std::string(reinterpret_cast<char*>(usb_buf) + 2); // Skip report and command byte
}

void SinowealthController::SetLEDColor
    (
    RGBColor* color_buf
    )
{
    if (GetProfile() < SINOWEALTH_CONFIG_SIZE_MIN) return;

    unsigned char usb_buf[SINOWEALTH_CONFIG_REPORT_SIZE];
    memcpy(usb_buf, device_configuration, SINOWEALTH_CONFIG_SIZE); // Yes, we only copy 167 bytes back, for now - if anything weird starts happening use SINOWEALTH_CONFIG_REPORT_SIZE

    usb_buf[0x03] = 0x7B;   //write to device
    usb_buf[0x06] = 0x00;

    usb_buf[0x35] = GLORIOUS_MODE_STATIC;
    usb_buf[0x38] = 0x40; //max brightness
    usb_buf[0x39] = RGBGetRValue(color_buf[0]);
    usb_buf[0x3A] = RGBGetBValue(color_buf[0]);
    usb_buf[0x3B] = RGBGetGValue(color_buf[0]);

    hid_send_feature_report(dev_data, usb_buf, SINOWEALTH_CONFIG_REPORT_SIZE);
}

void SinowealthController::SetMode
    (
    unsigned char   mode,
    unsigned char   speed,
    unsigned char   direction,
    RGBColor*       color_buf
    )
{
    if (GetProfile() < SINOWEALTH_CONFIG_SIZE_MIN) return;

    unsigned char usb_buf[SINOWEALTH_CONFIG_REPORT_SIZE];
    memcpy(usb_buf, device_configuration, SINOWEALTH_CONFIG_SIZE); // Yes, we only copy 167 bytes back, for now - if anything weird starts happening use SINOWEALTH_CONFIG_REPORT_SIZE

    usb_buf[0x03] = 0x7B;   //write to device
    usb_buf[0x06] = 0x00;

    usb_buf[0x35] = mode;

    switch (mode)
    {
    case GLORIOUS_MODE_OFF:
        usb_buf[0x81] = 0x00; //mode 0 either 0x00 or 0x03
        break;
    case GLORIOUS_MODE_RAINBOW:
        usb_buf[0x36] = speed;
        usb_buf[0x37] = direction;
        break;
    case GLORIOUS_MODE_SPECTRUM_BREATING:
        //colours not yet researched
        usb_buf[0x3C] = speed; //speed for mode 3
        usb_buf[0x3D] = 0x07;  //maybe some kind of bank change?+
        //usb_buf[0x3D] = 0x06;
        usb_buf[0x3E] = RGBGetRValue(color_buf[0]);  //mode 3 red 1
        usb_buf[0x3F] = RGBGetBValue(color_buf[0]);  //mode 3 blue 1
        usb_buf[0x40] = RGBGetGValue(color_buf[0]);  //mode 3 green 1
        usb_buf[0x41] = RGBGetRValue(color_buf[1]);  //mode 3 red 2
        usb_buf[0x42] = RGBGetBValue(color_buf[1]);  //mode 3 blue 2
        usb_buf[0x43] = RGBGetGValue(color_buf[1]);  //mode 3 green 2
        usb_buf[0x44] = RGBGetRValue(color_buf[2]);  //mode 3 red 3
        usb_buf[0x45] = RGBGetBValue(color_buf[2]);  //mode 3 blue 3
        usb_buf[0x46] = RGBGetGValue(color_buf[2]);  //mode 3 green 3
        usb_buf[0x47] = RGBGetRValue(color_buf[3]);  //mode 3 red 4
        usb_buf[0x48] = RGBGetBValue(color_buf[3]);  //mode 3 blue 4
        usb_buf[0x49] = RGBGetGValue(color_buf[3]);  //mode 3 green 4
        usb_buf[0x4A] = RGBGetRValue(color_buf[4]);  //mode 3 red 5
        usb_buf[0x4B] = RGBGetBValue(color_buf[4]);  //mode 3 blue 5
        usb_buf[0x4C] = RGBGetGValue(color_buf[4]);  //mode 3 green 5
        usb_buf[0x4D] = RGBGetRValue(color_buf[5]);  //mode 3 red 6
        usb_buf[0x4E] = RGBGetBValue(color_buf[5]);  //mode 3 blue 6
        usb_buf[0x4F] = RGBGetGValue(color_buf[5]);  //mode 3 green 6
        usb_buf[0x50] = RGBGetRValue(color_buf[6]);  //mode 3 red 7
        usb_buf[0x51] = RGBGetBValue(color_buf[6]);  //mode 3 blue 7
        usb_buf[0x52] = RGBGetGValue(color_buf[6]);  //mode 3 green 7
        break;
    case GLORIOUS_MODE_TAIL:
        usb_buf[0x53] = speed; //Speed for mode 4
        break;
    case GLORIOUS_MODE_SPECTRUM_CYCLE:
        usb_buf[0x54] = speed; //Speed for mode 1,2,5
        break;
    case GLORIOUS_MODE_RAVE:
        usb_buf[0x74] = speed; //Speed for mode 7
        usb_buf[0x75] = RGBGetRValue(color_buf[0]); //mode 7 red 1
        usb_buf[0x76] = RGBGetBValue(color_buf[0]); //mode 7 blue 1
        usb_buf[0x77] = RGBGetGValue(color_buf[0]); //mode 7 green 1
        usb_buf[0x78] = RGBGetRValue(color_buf[1]); //mode 7 red 2
        usb_buf[0x79] = RGBGetBValue(color_buf[1]); //mode 7 blue 2
        usb_buf[0x7A] = RGBGetGValue(color_buf[1]); //mode 7 green 2
        break;
    case GLORIOUS_MODE_WAVE:
        usb_buf[0x7C] = speed; //Speed for mode 9
        break;
    case GLORIOUS_MODE_BREATHING:
        usb_buf[0x7D] = speed;
        usb_buf[0x7E] = RGBGetRValue(color_buf[0]); //mode 0a red
        usb_buf[0x7F] = RGBGetBValue(color_buf[0]); //mode 0a blue
        usb_buf[0x80] = RGBGetGValue(color_buf[0]); //mode 0a green
    default:
        break;
    }

    hid_send_feature_report(dev_data, usb_buf, SINOWEALTH_CONFIG_REPORT_SIZE);
}

int SinowealthController::GetProfile()
{
    int actual;
    unsigned char usb_buf[SINOWEALTH_COMMAND_REPORT_SIZE];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0] = 0x05;
    usb_buf[1] = 0x11;

    actual = hid_send_feature_report(dev_cmd, usb_buf, sizeof(usb_buf));

    if (actual != SINOWEALTH_COMMAND_REPORT_SIZE)
    {
        LOG_ERROR("[Sinowealth Mouse] Error sending read request!");
        return -1;
    }
    else
    {
        memset(device_configuration, 0x00, SINOWEALTH_CONFIG_REPORT_SIZE);
        device_configuration[0] = 0x04;

        actual = hid_get_feature_report(dev_data, device_configuration, SINOWEALTH_CONFIG_REPORT_SIZE);

        if (actual < 0)
        {
            LOG_ERROR("[Sinowealth Mouse] Error reading device configuration!");
        }
    }

    return actual;
}
