/*---------------------------------------------------------*\
| LianLiUniHubSLController.cpp                              |
|                                                           |
|   Driver for Lian Li Uni Hub - SL                         |
|                                                           |
|   Muhamad Visat                               26 Jul 2025 |
|   Original work by Luca Lovisa & Oliver P                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "StringUtils.h"
#include "LianLiUniHubSLController.h"

using namespace std::chrono_literals;

LianLiUniHubSLController::LianLiUniHubSLController(hid_device *dev, const char *path)
{
    if(dev == nullptr)
    {
        return;
    }

    device = dev;
    location = "HID: " + std::string(path);
    is_merged_mode = false;
}

LianLiUniHubSLController::~LianLiUniHubSLController()
{
    if(device != nullptr)
    {
        hid_close(device);
        device = nullptr;
    }
}

std::string LianLiUniHubSLController::ReadVersion()
{
    wchar_t buf[40];
    int ret = hid_get_product_string(this->device, buf, 40);
    if(ret != 0)
    {
        return "";
    }

    /*------------------------------ -*\
    | Example: LianLi-UNI FAN-SL-v1.8  |
    | We just want the v1.8 part       |
    | without trailing spaces          |
    \*--------------------------------*/
    std::string version = StringUtils::wstring_to_string(buf);
    version = version.substr(version.find_last_of('-') + 1);
    return version.substr(0, version.find_last_not_of(' ') + 1);
}

std::string LianLiUniHubSLController::ReadSerial()
{
    wchar_t buf[20];
    int ret = hid_get_serial_number_string(this->device, buf, 20);
    if(ret != 0)
    {
        return "";
    }

    std::string serial = StringUtils::wstring_to_string(buf);
    return serial;
}

void LianLiUniHubSLController::UpdateMode(const std::vector<zone> &zones, const mode &active)
{
    /*---------------------------------*\
    | Activate all channels             |
    \*---------------------------------*/
    for(size_t channel = 0; channel < zones.size(); channel++)
    {
        this->SendActivate(channel, zones[channel].leds_count);
    }

    /*---------------------------------*\
    | Set merge mode if requested       |
    \*---------------------------------*/
    is_merged_mode = active.name.find("Merged") != std::string::npos;
    this->SendMerge();

    for (size_t channel = 0; channel < zones.size(); channel++)
    {
        UpdateZoneLEDs(channel, zones[channel], active);
    }
}

void LianLiUniHubSLController::UpdateZoneLEDs(size_t channel, const zone &z, const mode &active)
{
    /*---------------------------------*\
    | Handle per-LED color mode         |
    \*---------------------------------*/
    if(active.color_mode == MODE_COLORS_PER_LED)
    {
        this->SetPerLEDColor(channel, z, active);
        return;
    }

    /*-------------------------------------------------*\
    | In merged mode, only first channel takes control |
    \*-------------------------------------------------*/
    if(is_merged_mode && channel > 0)
    {
        return;
    }

    /*---------------------------------*\
    | Handle mode-specific color        |
    \*---------------------------------*/
    this->SetModeSpecificColor(channel, active);
}


void LianLiUniHubSLController::SetPerLEDColor(size_t channel, const zone &z, const mode &active)
{
    unsigned char color_buf[UNIHUB_SL_MAX_LED_PER_CHANNEL * 3];
    float brightness_scale = (float)(active.brightness) / active.brightness_max;

    memset(color_buf, 0, sizeof(color_buf));
    this->FillStaticColorBuffer(color_buf, z.colors, z.leds_count, brightness_scale);
    this->SendColor(channel, color_buf, sizeof(color_buf));
    this->SendMode(channel, active);
}

void LianLiUniHubSLController::SetModeSpecificColor(size_t channel, const mode &active)
{
    unsigned char color_buf[UNIHUB_SL_MAX_LED_PER_CHANNEL * 3];
    memset(color_buf, 0, sizeof(color_buf));

    float brightness_scale = (float)(active.brightness) / active.brightness_max;

    switch(active.value)
    {
        case UNIHUB_SL_LED_MODE_RAINBOW:
        case UNIHUB_SL_LED_MODE_RAINBOW_MORPH:
        case UNIHUB_SL_LED_MODE_STACK_MULTI_COLOR:
        case UNIHUB_SL_LED_MODE_NEON:
            /*-------------------------*\
            | No need to set any value |
            \*-------------------------*/
            break;

        case UNIHUB_SL_LED_MODE_STATIC:
        case UNIHUB_SL_LED_MODE_BREATHING:
            this->FillStaticColorBuffer(color_buf, active.colors.data(), active.colors.size(), brightness_scale);
            break;

        case UNIHUB_SL_LED_MODE_COLOR_CYCLE:
        case UNIHUB_SL_LED_MODE_RUNWAY:
        case UNIHUB_SL_LED_MODE_STAGGERED:
        case UNIHUB_SL_LED_MODE_TIDE:
        case UNIHUB_SL_LED_MODE_METEOR:
        case UNIHUB_SL_LED_MODE_MIXING:
        case UNIHUB_SL_LED_MODE_STACK:
            this->FillDynamicColorBuffer(color_buf, active.colors.data(), active.colors.size(), brightness_scale);
            break;

        default:
            LOG_WARNING("[Lian Li Uni Hub - SL] Unknown mode value: %d", active.value);
            break;
    }

    this->SendColor(channel, color_buf, sizeof(color_buf));
    this->SendMode(channel, active);
}

void LianLiUniHubSLController::FillStaticColorBuffer(unsigned char *color_buf, const RGBColor *colors, size_t num_colors, float brightness_scale)
{
    size_t max_fans = (size_t)UNIHUB_SL_MAX_FAN_PER_CHANNEL;
    size_t max_idx = num_colors < max_fans ? num_colors : max_fans;

    for(size_t fan_idx = 0; fan_idx < max_idx; fan_idx++)
    {
        RGBColor color = colors[fan_idx];
        unsigned char r = (unsigned char)(RGBGetRValue(color) * brightness_scale);
        unsigned char g = (unsigned char)(RGBGetGValue(color) * brightness_scale);
        unsigned char b = (unsigned char)(RGBGetBValue(color) * brightness_scale);

        for(size_t led_idx = 0; led_idx < UNIHUB_SL_LED_PER_FAN; led_idx++)
        {
            size_t absolute_led_idx = fan_idx * UNIHUB_SL_LED_PER_FAN + led_idx;
            size_t idx = absolute_led_idx * 3;

            /*------------------------*\
            | The protocol uses R B G |
            \*------------------------*/
            color_buf[idx + 0] = r;
            color_buf[idx + 1] = b;
            color_buf[idx + 2] = g;
        }
    }
}

void LianLiUniHubSLController::FillDynamicColorBuffer(unsigned char *color_buf, const RGBColor *colors, size_t num_colors, float brightness_scale)
{
    size_t max_fans = (size_t)UNIHUB_SL_MAX_FAN_PER_CHANNEL;
    size_t max_idx = num_colors < max_fans ? num_colors : max_fans;

    for(size_t color_idx = 0; color_idx < max_idx; color_idx++)
    {
        RGBColor color = colors[color_idx];
        unsigned char r = (unsigned char)(RGBGetRValue(color) * brightness_scale);
        unsigned char g = (unsigned char)(RGBGetGValue(color) * brightness_scale);
        unsigned char b = (unsigned char)(RGBGetBValue(color) * brightness_scale);

        size_t idx = (color_idx * 3);

        /*------------------------*\
        | The protocol uses R B G |
        \*------------------------*/
        color_buf[idx + 0] = r;
        color_buf[idx + 1] = b;
        color_buf[idx + 2] = g;
    }
}

unsigned char LianLiUniHubSLController::ConvertBrightness(unsigned int brightness)
{
    switch(brightness)
    {
    case 0:
        return UNIHUB_SL_LED_BRIGHTNESS_000;

    case 1:
        return UNIHUB_SL_LED_BRIGHTNESS_025;

    case 2:
        return UNIHUB_SL_LED_BRIGHTNESS_050;

    case 3:
        return UNIHUB_SL_LED_BRIGHTNESS_075;

    case 4:
        return UNIHUB_SL_LED_BRIGHTNESS_100;

    default:
        return UNIHUB_SL_LED_BRIGHTNESS_100;
    }
}

unsigned char LianLiUniHubSLController::ConvertSpeed(unsigned int speed)
{
    switch(speed)
    {
    case 0:
        return UNIHUB_SL_LED_SPEED_000;

    case 1:
        return UNIHUB_SL_LED_SPEED_025;

    case 2:
        return UNIHUB_SL_LED_SPEED_050;

    case 3:
        return UNIHUB_SL_LED_SPEED_075;

    case 4:
        return UNIHUB_SL_LED_SPEED_100;

    default:
        return UNIHUB_SL_LED_SPEED_050;
    }
}

unsigned char LianLiUniHubSLController::ConvertDirection(unsigned int direction)
{
    switch(direction)
    {
    case MODE_DIRECTION_LEFT:
        return UNIHUB_SL_LED_DIRECTION_LTR;

    case MODE_DIRECTION_RIGHT:
        return UNIHUB_SL_LED_DIRECTION_RTL;

    default:
        return UNIHUB_SL_LED_DIRECTION_LTR;
    }
}

void LianLiUniHubSLController::SendActivate(size_t channel, unsigned char num_fans)
{
    unsigned char buf[11];
    memset(buf, 0x00, sizeof(buf));

    buf[0x00] = UNIHUB_SL_REPORT_ID;
    buf[0x01] = 0x10;
    buf[0x02] = 0x32;
    buf[0x03] = 0x10 * channel + num_fans;

    this->LogBuffer("SendActivate", buf, sizeof(buf));

    hid_write(this->device, buf, sizeof(buf));
    std::this_thread::sleep_for(5ms);
}

void LianLiUniHubSLController::SendMerge()
{
    unsigned char buf[11];
    memset(buf, 0x00, sizeof(buf));

    buf[0x00] = UNIHUB_SL_REPORT_ID;
    buf[0x01] = 0x10;
    if(is_merged_mode)
    {
        buf[0x02] = 0x33;
        buf[0x03] = 0x00;
        buf[0x04] = 0x01;
        buf[0x05] = 0x02;
        buf[0x06] = 0x03;
        buf[0x07] = 0x08;
    }
    else
    {
        buf[0x02] = 0x34;
    }

    this->LogBuffer("SendMerge", buf, sizeof(buf));

    hid_write(this->device, buf, sizeof(buf));
    std::this_thread::sleep_for(5ms);
}

void LianLiUniHubSLController::SendColor(size_t channel, const unsigned char *colors, size_t num_colors)
{
    unsigned char* buf = new unsigned char[2 + num_colors];
    memset(buf, 0x00, sizeof(buf));

    buf[0x00] = UNIHUB_SL_REPORT_ID;
    buf[0x01] = 0x30 + channel; // Channel 1: 0x30, Channel 2: 0x31, etc.

    memcpy(&buf[0x02], colors, num_colors);

    this->LogBuffer("SendColor", buf, sizeof(buf));

    hid_write(this->device, buf, sizeof(buf));
    std::this_thread::sleep_for(5ms);

    delete[] buf;
}

void LianLiUniHubSLController::SendMode(size_t channel, const mode &active)
{
    unsigned char buf[11];
    memset(buf, 0x00, sizeof(buf));

    buf[0x00] = UNIHUB_SL_REPORT_ID;
    buf[0x01] = 0x10 + channel; // Channel 1: 0x10, Channel 2: 0x11, etc.
    buf[0x02] = active.value;
    buf[0x03] = this->ConvertSpeed(active.speed);
    buf[0x04] = this->ConvertDirection(active.direction);
    buf[0x05] = this->ConvertBrightness(active.brightness);

    this->LogBuffer("SendMode", buf, sizeof(buf));

    hid_write(this->device, buf, sizeof(buf));
    std::this_thread::sleep_for(5ms);
}

void LianLiUniHubSLController::LogBuffer(const char *operation, const unsigned char *buf, size_t buf_len)
{
    std::string hex_string;
    for(size_t i = 0; i < buf_len; i++)
    {
        char hex_byte[3];
        snprintf(hex_byte, sizeof(hex_byte), "%02X", buf[i]);
        hex_string += hex_byte;
    }
    LOG_DEBUG("[Lian Li Uni Hub - SL] %s buffer: %s", operation, hex_string.c_str());
}
