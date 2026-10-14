/*---------------------------------------------------------*\
| RoccatVulcanKeyboardController.cpp                        |
|                                                           |
|   Driver for Roccat Vulcan keyboard                       |
|                                                           |
|   Mola19                                      17 Dec 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include <math.h>
#include <thread>
#include <vector>
#include "LogManager.h"
#include "RoccatVulcanKeyboardController.h"
#include "StringUtils.h"

static const char* ROCCATVULCANKEYBOARD = "Roccat Vulcan Keyboard";

#define ROCCAT_VULCAN_II_MINI_LED_COUNT 69

static const uint8_t roccat_vulcan_ii_mini_slot_map[ROCCAT_VULCAN_II_MINI_LED_COUNT] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    18, 19, 20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
    34, 35, 36, 37, 38, 39, 40, 42, 43, 45, 46, 48, 49, 50, 51,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 64, 65, 66, 68,
    70, 71, 72, 73, 74, 75, 76
};

RoccatVulcanKeyboardController::RoccatVulcanKeyboardController(hid_device* dev_ctrl_handle, hid_device* dev_led_handle, char *path, uint16_t pid, std::string dev_name)
{
    dev_ctrl    = dev_ctrl_handle;
    dev_led     = dev_led_handle;
    location    = path;
    name        = dev_name;
    device_pid  = pid;
}

RoccatVulcanKeyboardController::~RoccatVulcanKeyboardController()
{
    hid_close(dev_ctrl);
    hid_close(dev_led);
}

std::string RoccatVulcanKeyboardController::GetLocation()
{
    return("HID: " + location);
}

std::string RoccatVulcanKeyboardController::GetName()
{
    return(name);
}

std::string RoccatVulcanKeyboardController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev_ctrl, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

device_info RoccatVulcanKeyboardController::InitDeviceInfo()
{
    uint8_t packet_length;
    uint8_t report_id;

    switch(device_pid)
    {
        case ROCCAT_PYRO_PID:
        case ROCCAT_MAGMA_PID:
        case ROCCAT_MAGMA_MINI_PID:
        case ROCCAT_VULCAN_PRO_PID:
        case ROCCAT_VULCAN_TKL_PRO_PID:
        case ROCCAT_VULCAN_II_PID:
        case ROCCAT_VULCAN_II_MINI_PID:
        case ROCCAT_VULCAN_II_MAX_PID:
        case TURTLE_BEACH_VULCAN_II_PID:
        case TURTLE_BEACH_VULCAN_II_TKL_PRO_PID:
            packet_length = 9;
            report_id     = 0x09;
            break;
        default:
            packet_length = 8;
            report_id     = 0x0F;
    }

    uint8_t* buf = new uint8_t[packet_length];
    memset(buf, 0x00, packet_length);

    buf[0] = report_id;
    /*-------------------------------------------------------------*\
    | Store the actual read bytes to verify data availability       |
    \*-------------------------------------------------------------*/
    int res = hid_get_feature_report(dev_ctrl, buf, packet_length);
    hid_get_feature_report(dev_ctrl, buf, packet_length);

    /*-------------------------------------------------------------*\
    | buf[2] is version e.g. 103 means v1.03                        |
    \*-------------------------------------------------------------*/
    dev_info.version_major = buf[2] / 100;
    dev_info.version_minor = buf[2] % 100;

    char version[5];
    snprintf(version, 5, "%d.%02d", dev_info.version_major, dev_info.version_minor);
    dev_info.version = version;

    if(device_pid == ROCCAT_MAGMA_PID || device_pid == ROCCAT_MAGMA_MINI_PID)
    {
        /*---------------------------------------------------------*\
        | This device doesn't need a layout,                        |
        | because it doesn't have per-led lighting.                 |
        | Taking us layout as placeholder instead                   |
        \*---------------------------------------------------------*/
        dev_info.layout_type    = ROCCAT_VULCAN_LAYOUT_US;
    }
    else
    {
        /*---------------------------------------------------------*\
        | Safe check for reported buffer length before accessing    |
        | buf[6] to prevent out-of-bounds array access.             |
        | Vulcan II Max (JP) returns 0x06 at buf[6] for JIS layout. |
        \*---------------------------------------------------------*/
        if(res >= 7)
        {
            dev_info.layout_type = buf[6];
            LOG_DEBUG("[%s]: Detected layout '0x%02X'", ROCCATVULCANKEYBOARD, buf[6]);
        }
        else
        {
            dev_info.layout_type = ROCCAT_VULCAN_LAYOUT_US;
            LOG_DEBUG("[%s]: Failed to detect layout, falling back to US", ROCCATVULCANKEYBOARD);
        }
    }
    LOG_INFO("[%s]: %s: firmware %s, layout 0x%02X", ROCCATVULCANKEYBOARD, name.c_str(), dev_info.version.c_str(), dev_info.layout_type);

    delete[] buf;
    return dev_info;
}

device_info RoccatVulcanKeyboardController::GetDeviceInfo()
{
    return dev_info;
}

bool RoccatVulcanKeyboardController::IsBigEndianDirectMode()
{
    switch(device_pid)
    {
        case ROCCAT_VULCAN_II_MINI_PID:
            return false;
        default:
            return (dev_info.version_major >= 1 && dev_info.version_minor >= 16);
    }
}

void RoccatVulcanKeyboardController::EnableDirect(bool on_off_switch)
{
    uint8_t* buf;
    int res;
    switch(device_pid)
    {
        case ROCCAT_PYRO_PID:
        case ROCCAT_MAGMA_PID:
        case ROCCAT_MAGMA_MINI_PID:
        case ROCCAT_VULCAN_PRO_PID:
        case ROCCAT_VULCAN_TKL_PRO_PID:
        case ROCCAT_VULCAN_II_PID:
        case ROCCAT_VULCAN_II_MINI_PID:
        case ROCCAT_VULCAN_II_MAX_PID:
        case TURTLE_BEACH_VULCAN_II_PID:
        case TURTLE_BEACH_VULCAN_II_TKL_PRO_PID:
            buf = new uint8_t[5] { 0x0E, 0x05, on_off_switch, 0x00, 0x00 };
            res = hid_send_feature_report(dev_ctrl, buf, 5);
            break;
        default:
            buf = new uint8_t[3] { 0x15, 0x00, on_off_switch };
            res = hid_send_feature_report(dev_ctrl, buf, 3);
    }
    delete[] buf;

    if(res < 0)
    {
        LOG_ERROR("[%s]: %s: failed to send direct mode %s report", ROCCATVULCANKEYBOARD, name.c_str(), on_off_switch ? "enable" : "disable");
    }
}

void RoccatVulcanKeyboardController::SendColors(std::vector<led_color> colors)
{
    unsigned short packet_length;
    unsigned char  column_length;
    unsigned char  protocol_version;

    switch(device_pid)
    {
    case ROCCAT_MAGMA_PID:
    case ROCCAT_MAGMA_MINI_PID:
        packet_length = 64;
        column_length = 5;
        protocol_version = 2;
        break;
    case ROCCAT_PYRO_PID:
        packet_length = 378;
        column_length = 1;
        protocol_version = 2;
        break;
    case ROCCAT_VULCAN_PRO_PID:
    case ROCCAT_VULCAN_TKL_PRO_PID:
        packet_length = 384;
        column_length = 12;
        protocol_version = 2;
        break;
    case ROCCAT_VULCAN_II_PID:
    case TURTLE_BEACH_VULCAN_II_PID:
        packet_length = 396;
        column_length = 1;
        protocol_version = 2;
        break;
    case ROCCAT_VULCAN_II_MAX_PID:
        packet_length = 567;
        column_length = 1;
        protocol_version = 2;
        break;
    case ROCCAT_VULCAN_II_MINI_PID:
        packet_length = 345;
        column_length = 1;
        protocol_version = 2;
        break;
    case TURTLE_BEACH_VULCAN_II_TKL_PRO_PID:
        packet_length = 320;
        column_length = 1;
        protocol_version = 2;
        break;
    default:
        packet_length = 436;
        column_length = 12;
        protocol_version = 1;
    }

    unsigned char packet_num = (unsigned char)(ceil((float)packet_length / 64));
    std::vector<std::vector<uint8_t>> bufs(packet_num);

    for(int p = 0; p < packet_num; p++)
    {
        bufs[p].resize(65);
        memset(&bufs[p][0], 0x00, sizeof(bufs[p][0]) * bufs[p].size());
    }

    if(protocol_version > 1)
    {
        for(unsigned int i = 0; i < packet_num; i++)
        {
            bufs[i][1] = 0xA1;
            bufs[i][2] = i + 1;
        }
    }
    else
    {
        bufs[0][1] = 0xA1;
        bufs[0][2] = 0x01;
    }

    unsigned char header_length_first = (packet_length > 255) ? 4 : 3;

    if(header_length_first == 3)
    {
        bufs[0][3] = (uint8_t)packet_length;
    }
    else
    {
        if(IsBigEndianDirectMode())
        {
            bufs[0][3] = (packet_length >> 8) & 0xFF;
            bufs[0][4] = packet_length & 0xFF;
        }
        else
        {
            bufs[0][3] = packet_length & 0xFF;
            bufs[0][4] = (packet_length >> 8) & 0xFF;
        }
    }

    unsigned int data_length_packet = 64 - header_length_first;

    unsigned int hw_leds = 0;
    if(device_pid == TURTLE_BEACH_VULCAN_II_TKL_PRO_PID)
    {
        for(unsigned int i = 0; i < colors.size(); i++)
        {
            if(colors[i].value > hw_leds)
            {
                hw_leds = colors[i].value;
            }
        }
        hw_leds += 1;
    }

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        if(device_pid == TURTLE_BEACH_VULCAN_II_TKL_PRO_PID)
        {
            const unsigned int CHANNEL_OFFSET = hw_leds;

            unsigned int led_index = colors[i].value;
            if(led_index >= hw_leds)
            {
                continue;
            }

            unsigned int logical_pos_r = led_index;
            unsigned int p_idx_r       = logical_pos_r / data_length_packet;
            unsigned int b_idx_r       = logical_pos_r % data_length_packet;

            if(p_idx_r < bufs.size())
            {
                bufs[p_idx_r][b_idx_r + header_length_first + 1] = RGBGetRValue(colors[i].color);
            }

            unsigned int logical_pos_g = led_index + CHANNEL_OFFSET;
            unsigned int p_idx_g       = logical_pos_g / data_length_packet;
            unsigned int b_idx_g       = logical_pos_g % data_length_packet;

            if(p_idx_g < bufs.size())
            {
                bufs[p_idx_g][b_idx_g + header_length_first + 1] = RGBGetGValue(colors[i].color);
            }

            unsigned int logical_pos_b = led_index + 2 * CHANNEL_OFFSET;
            unsigned int p_idx_b       = logical_pos_b / data_length_packet;
            unsigned int b_idx_b       = logical_pos_b % data_length_packet;

            if(p_idx_b < bufs.size())
            {
                bufs[p_idx_b][b_idx_b + header_length_first + 1] = RGBGetBValue(colors[i].color);
            }
        }

        else
        {
            unsigned int led_index = colors[i].value;

            if(device_pid == ROCCAT_VULCAN_II_MINI_PID)
            {
                if(led_index >= ROCCAT_VULCAN_II_MINI_LED_COUNT)
                {
                    continue;
                }

                led_index = roccat_vulcan_ii_mini_slot_map[led_index];
            }

            int column  = (int)(floor(led_index / column_length));
            int row     = led_index % column_length;

            if(protocol_version == 1)
            {
                int offset = column * 3 * column_length + row + header_length_first;
                bufs[offset / 64][offset % 64 + 1] = RGBGetRValue(colors[i].color);
                offset += column_length;
                bufs[offset / 64][offset % 64 + 1] = RGBGetGValue(colors[i].color);
                offset += column_length;
                bufs[offset / 64][offset % 64 + 1] = RGBGetBValue(colors[i].color);
            }
            else
            {
                int offset = column * 3 * column_length + row;

                bufs[offset / data_length_packet][offset % data_length_packet + header_length_first + 1] = RGBGetRValue(colors[i].color);

                offset += column_length;
                bufs[offset / data_length_packet][offset % data_length_packet + header_length_first + 1] = RGBGetGValue(colors[i].color);

                offset += column_length;
                bufs[offset / data_length_packet][offset % data_length_packet + header_length_first + 1] = RGBGetBValue(colors[i].color);
            }
        }
    }

    for(int p = 0; p < packet_num; p++)
    {
        if(hid_write(dev_led, &bufs[p][0], 65) < 0)
        {
            LOG_ERROR("[%s]: %s: failed to send color frame packet %d", ROCCATVULCANKEYBOARD, name.c_str(), p);
        }
    }

    ClearResponses();
    AwaitResponse(20);
}

void RoccatVulcanKeyboardController::SendMode(unsigned int mode, unsigned int speed, unsigned int brightness, std::vector<led_color> colors)
{
    if(speed      == 0) speed      = ROCCAT_VULCAN_SPEED_DEFAULT;
    if(brightness == 0) brightness = ROCCAT_VULCAN_BRIGHTNESS_DEFAULT;

    unsigned short packet_length;
    unsigned char  protocol_version;
    unsigned char  column_length;

    switch(device_pid)
    {
        case ROCCAT_PYRO_PID:
            protocol_version = 2;
            packet_length = 365;
            column_length = 1;
            break;
        case ROCCAT_MAGMA_PID:
        case ROCCAT_MAGMA_MINI_PID:
            protocol_version = 2;
            packet_length = 26;
            column_length = 5;
            break;
        case ROCCAT_VULCAN_PRO_PID:
        case ROCCAT_VULCAN_TKL_PRO_PID:
            protocol_version = 2;
            packet_length = 371;
            column_length = 12;
            break;
        case ROCCAT_VULCAN_II_PID:
        case TURTLE_BEACH_VULCAN_II_PID:
            protocol_version = 2;
            packet_length = 377;
            column_length = 1;
            break;
        case ROCCAT_VULCAN_II_MAX_PID:
            protocol_version = 2;
            packet_length = 542;
            column_length = 1;
            break;
        case ROCCAT_VULCAN_II_MINI_PID:
            protocol_version = 2;
            packet_length = 332;
            column_length = 1;
            break;
        case TURTLE_BEACH_VULCAN_II_TKL_PRO_PID:
            packet_length = 284;
            column_length = 1;
            protocol_version = 2;
            break;
        default:
            protocol_version = 1;
            packet_length = 443;
            column_length = 12;
    }


    uint8_t* buf = new uint8_t[packet_length];
    memset(buf, 0x00, packet_length);

    unsigned char header_length = (packet_length > 255) ? 2 : 1;

    buf[0] = (protocol_version == 1) ? 0x0D : 0x11;

    if(header_length == 1)
    {
        buf[1] = (uint8_t)packet_length;
    }
    else
    {
        buf[1] = packet_length % 256;
        buf[2] = packet_length / 256;
    }

    unsigned char offset = header_length + 1;

    buf[0 + offset] = 0x00;
    buf[1 + offset] = mode;
    buf[2 + offset] = speed;

    if(protocol_version == 1)
    {
        buf[3 + offset] = 0x00;
        buf[4 + offset] = brightness;
        buf[5 + offset] = 0x00;
    }
    else
    {
        buf[3 + offset] = brightness;
        buf[4 + offset] = 0x00;
        buf[5 + offset] = 0x00;
    }

    if(device_pid == ROCCAT_VULCAN_II_MINI_PID)
    {
        buf[0 + offset] = 0x02;
        buf[4 + offset] = 0xFB;
    }

    if(device_pid == TURTLE_BEACH_VULCAN_II_TKL_PRO_PID)
    {
        buf[0 + offset] = 0x03;
        buf[4 + offset] = 0x0B;
        buf[5 + offset] = 0x01;
    }

    unsigned int hw_leds = 0;
    if(device_pid == TURTLE_BEACH_VULCAN_II_TKL_PRO_PID)
    {
        for(unsigned int i = 0; i < colors.size(); i++)
        {
            if(colors[i].value > hw_leds)
            {
                hw_leds = colors[i].value;
            }
        }
        hw_leds += 1;
    }

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        if(device_pid == TURTLE_BEACH_VULCAN_II_TKL_PRO_PID)
        {
            const int HEADER_OFFSET = 9;
            const unsigned int CHANNEL_OFFSET = hw_leds;

            unsigned int led_index = colors[i].value;
            if(led_index >= hw_leds) continue;

            int pos_r = HEADER_OFFSET + led_index;
            int pos_g = HEADER_OFFSET + led_index + CHANNEL_OFFSET;
            int pos_b = HEADER_OFFSET + led_index + (2 * CHANNEL_OFFSET);

            if (pos_b < packet_length - 2)
            {
                buf[pos_r] = RGBGetRValue(colors[i].color);
                buf[pos_g] = RGBGetGValue(colors[i].color);
                buf[pos_b] = RGBGetBValue(colors[i].color);
            }
        }
        else
        {
            unsigned int led_index = colors[i].value;

            if(device_pid == ROCCAT_VULCAN_II_MINI_PID)
            {
                if(led_index >= ROCCAT_VULCAN_II_MINI_LED_COUNT)
                {
                    continue;
                }

                led_index = roccat_vulcan_ii_mini_slot_map[led_index];
            }

            int column  = (int)(floor(led_index / column_length));
            int row     = led_index % column_length;
            int pos = column * 3 * column_length + row + 9;

            if(pos + (2 * column_length) < packet_length - 2)
            {
                buf[pos] = RGBGetRValue(colors[i].color);
                pos += column_length;
                buf[pos] = RGBGetGValue(colors[i].color);
                pos += column_length;
                buf[pos] = RGBGetBValue(colors[i].color);
            }
        }
    }

    unsigned short total = 0;
    for(int i = 0; i < packet_length - 2; i++) total += buf[i];

    buf[packet_length - 2] = total & 0xFF;
    buf[packet_length - 1] = total >> 8;

    if(hid_send_feature_report(dev_ctrl, buf, packet_length) < 0)
    {
        LOG_ERROR("[%s]: %s: failed to send mode report", ROCCATVULCANKEYBOARD, name.c_str());
    }

    /*---------------------------------------------------------*\
    | The Vulcan II Mini only commits a static 0x11 report      |
    | while the 0x13 keepalive is being sent.                   |
    \*---------------------------------------------------------*/
    if(device_pid == ROCCAT_VULCAN_II_MINI_PID && mode == ROCCAT_VULCAN_MODE_STATIC)
    {
        uint8_t keepalive[123];
        memset(keepalive, 0x00, sizeof(keepalive));
        keepalive[0]          = 0x13;
        keepalive[1]          = 0x7B;   /* 123 bytes */
        keepalive[2]          = 0x1E;   /* 30 four-byte entries */
        keepalive[3 + 4 * 4]  = 0xA4;
        keepalive[3 + 4 * 5]  = 0xA5;
        keepalive[3 + 4 * 16] = 0xB0;
        keepalive[3 + 4 * 22] = 0xB6;

        WaitUntilReady();

        for(int i = 0; i < 3; i++)
        {
            if(hid_send_feature_report(dev_ctrl, keepalive, sizeof(keepalive)) < 0)
            {
                LOG_ERROR("[%s]: %s: failed to send keepalive report", ROCCATVULCANKEYBOARD, name.c_str());
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }
    }

    delete[] buf;
}

void RoccatVulcanKeyboardController::WaitUntilReady()
{
    unsigned short packet_length;

    switch(device_pid)
    {
        case ROCCAT_PYRO_PID:
        case ROCCAT_MAGMA_PID:
        case ROCCAT_MAGMA_MINI_PID:
        case ROCCAT_VULCAN_PRO_PID:
        case ROCCAT_VULCAN_TKL_PRO_PID:
            packet_length = 4;
            break;
        default:
            packet_length = 3;
    }

    uint8_t* buf = new uint8_t[packet_length];
    memset(buf, 0x00, packet_length);

    buf[0] = 0x04;

    for(unsigned char i = 0; buf[1] != 1 && i < 100; i++)
    {
        if(i != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }

        hid_get_feature_report(dev_ctrl, buf, packet_length);
    }

    if(buf[1] != 1)
    {
        LOG_WARNING("[%s]: %s: device did not report ready", ROCCATVULCANKEYBOARD, name.c_str());
    }

    delete[] buf;
}

void RoccatVulcanKeyboardController::AwaitResponse(int ms)
{
    unsigned char usb_buf_out[65];
    hid_read_timeout(dev_led, usb_buf_out, 65, ms);
}

void RoccatVulcanKeyboardController::ClearResponses()
{
    int result = 1;
    unsigned char usb_buf_flush[65];
    while(result > 0)
    {
        result = hid_read_timeout(dev_led, usb_buf_flush, 65, 0);
    }
}
