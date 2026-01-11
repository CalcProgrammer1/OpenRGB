/*---------------------------------------------------------*\
| Mountain60KeyboardController.cpp                          |
|                                                           |
|   Driver for Mountain keyboard                            |
|                                                           |
|   O'D.Sæzl                                       Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include <thread>
#include "Mountain60KeyboardController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

Mountain60KeyboardController::Mountain60KeyboardController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

Mountain60KeyboardController::~Mountain60KeyboardController()
{
    hid_close(dev);
}

std::string Mountain60KeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string Mountain60KeyboardController::GetNameString()
{
    return(name);
}

std::string Mountain60KeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void Mountain60KeyboardController::UpdateData()
{
    unsigned char usb_buf[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    unsigned char read[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    memset(usb_buf, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);

    usb_buf[0x01] = MOUNTAIN60_KEYBOARD_CHECK_NUMPAD;
    usb_buf[0x02] = 0x46; //constant data
    usb_buf[0x03] = 0x23; //constant data
    usb_buf[0x04] = 0xEA; //constant data

    hid_send_feature_report(dev, usb_buf, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    memset(read, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    hid_get_feature_report(dev, read, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
}

void Mountain60KeyboardController::SendModeDetails(const mode* current_mode)
{
    unsigned char usb_buf[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    unsigned char color_mode [] = {MOUNTAIN60_KEYBOARD_COLOR_MODE_SINGLE,MOUNTAIN60_KEYBOARD_COLOR_MODE_DUAL};
    memset(usb_buf, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);

    usb_buf[0x01] = MOUNTAIN60_KEYBOARD_SEND_CMD;
    usb_buf[0x02] = 0x46; //constant data
    usb_buf[0x03] = 0x23; //constant data
    usb_buf[0x04] = 0xEA; //constant data

    usb_buf[0x05] = current_mode->value;
    usb_buf[0x07] = (current_mode->value == MOUNTAIN60_KEYBOARD_MODE_STATIC) ? 0x32 : current_mode->speed * 25;
    usb_buf[0x08] = current_mode->brightness * 25;
    usb_buf[0x09] = (current_mode->color_mode == MODE_COLORS_RANDOM) ? (unsigned char)MOUNTAIN60_KEYBOARD_COLOR_MODE_RAINBOW : color_mode[current_mode->colors.size() - 1];
    usb_buf[0x0A] = ConvertDirection(current_mode->direction,current_mode->value == MOUNTAIN60_KEYBOARD_MODE_TORNADO);

    for(std::size_t idx = 0; idx < current_mode->colors.size(); idx++)
    {
        std::size_t offset  = (12 + (idx * 3));
        usb_buf[offset]     = RGBGetRValue(current_mode->colors[idx]);
        usb_buf[offset+1]   = RGBGetGValue(current_mode->colors[idx]);
        usb_buf[offset+2]   = RGBGetBValue(current_mode->colors[idx]);
    }

    hid_send_feature_report(dev, usb_buf, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    std::this_thread::sleep_for(10ms);
    SaveData(current_mode->value);
}

void Mountain60KeyboardController::SelectMode(unsigned char mode_idx)
{
    unsigned char usb_buf[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    unsigned char read[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    memset(usb_buf, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);

    usb_buf[0x01] = MOUNTAIN60_KEYBOARD_SELECT_MODE_CMD;
    usb_buf[0x02] = 0x46; //constant data
    usb_buf[0x03] = 0x23; //constant data
    usb_buf[0x04] = 0xEA; //constant data
    usb_buf[0x05] = 0x01; //constant data

    usb_buf[0x09] = mode_idx;

    hid_send_feature_report(dev, usb_buf, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    memset(read, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    hid_get_feature_report(dev, read, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
}

void Mountain60KeyboardController::SaveData(unsigned char mode_idx)
{
    unsigned char usb_buf[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    unsigned char read[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    memset(usb_buf, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);

    usb_buf[0x01] = MOUNTAIN60_KEYBOARD_SAVE_CMD;
    usb_buf[0x02] = 0x46; //constant data
    usb_buf[0x03] = 0x23; //constant data
    usb_buf[0x04] = 0xEA; //constant data

    usb_buf[0x05] = mode_idx;

    hid_send_feature_report(dev, usb_buf, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    memset(read, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    hid_get_feature_report(dev, read, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
}

void Mountain60KeyboardController::SendDirectStartPacketCmd(unsigned int brightness)
{
    unsigned char usb_buf[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    unsigned char read[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    memset(usb_buf, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);

    usb_buf[0x01] = MOUNTAIN60_KEYBOARD_START_DIRECT_CMD;
    usb_buf[0x02] = 0x46; //constant data
    usb_buf[0x03] = 0x23; //constant data
    usb_buf[0x04] = 0xEA; //constant data
    usb_buf[0x06] = 0xC0; //constant data

    usb_buf[0x05] = brightness * 25;

    hid_send_feature_report(dev, usb_buf, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    memset(read, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    hid_get_feature_report(dev, read, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
}

void Mountain60KeyboardController::SendDirectPacketCmd(unsigned char stream_control, unsigned char *data, unsigned int data_size)
{
    unsigned char usb_buf[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    memset(usb_buf, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_HEADER_SIZE);

    usb_buf[0x01] = MOUNTAIN60_KEYBOARD_MAP_DIRECT_CMD;
    usb_buf[0x02] = 0x46; //constant data
    usb_buf[0x03] = 0x23; //constant data
    usb_buf[0x04] = 0xEA; //constant data

    usb_buf[0x05] = stream_control;

    if(data_size <= MOUNTAIN60_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE)
    {
        unsigned char read[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
        memcpy(&usb_buf[MOUNTAIN60_KEYBOARD_USB_BUFFER_HEADER_SIZE],data,data_size);

        if(data_size < MOUNTAIN60_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE)
        {
            memset(&usb_buf[MOUNTAIN60_KEYBOARD_USB_BUFFER_HEADER_SIZE + data_size],0xFF,MOUNTAIN60_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE-data_size);
        }

        hid_send_feature_report(dev, usb_buf, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
        memset(read, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
        hid_get_feature_report(dev, read, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    }
}

void Mountain60KeyboardController::SendDirectPacketFinishCmd()
{
    unsigned char usb_buf[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    unsigned char read[MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE];
    memset(usb_buf, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);

    usb_buf[0x01] = MOUNTAIN60_KEYBOARD_END_DIRECT_CMD;
    usb_buf[0x02] = 0x46; //constant data
    usb_buf[0x03] = 0x23; //constant data
    usb_buf[0x04] = 0xEA; //constant data

    hid_send_feature_report(dev, usb_buf, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    memset(read, 0x00, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
    hid_get_feature_report(dev, read, MOUNTAIN60_KEYBOARD_USB_BUFFER_SIZE);
}

void Mountain60KeyboardController::SendDirect(unsigned int brightness, unsigned char* color_data, unsigned int data_size)
{
    static unsigned char prv_buffer[MOUNTAIN60_KEYBOARD_TRANSFER_BUFFER_SIZE]   = {0xFF};
    unsigned char *data_ptr                                                     = color_data;
    unsigned char *prv_data_ptr                                                 = prv_buffer;
    unsigned int  data_len                                                      = data_size;
    unsigned char stream_control_flag                                           = 0x0E;

    SendDirectStartPacketCmd(brightness);

    while(data_len>0)
    {
        if(data_len >= MOUNTAIN60_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE)
        {
            Mountain60KeyboardController::SendDirectPacketCmd(stream_control_flag,data_ptr,MOUNTAIN60_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE);
            memcpy(prv_data_ptr,data_ptr,MOUNTAIN60_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE);

            data_ptr            += MOUNTAIN60_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE;
            prv_data_ptr        += MOUNTAIN60_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE;
            data_len            -= MOUNTAIN60_KEYBOARD_USB_MAX_DIRECT_PAYLOAD_SIZE;
        }
        else
        {
            stream_control_flag = 0x0A;
            Mountain60KeyboardController::SendDirectPacketCmd(stream_control_flag,data_ptr,data_len);
            memcpy(prv_data_ptr,data_ptr,data_len);
            data_len = 0;
        }
    }

    SendDirectPacketFinishCmd();
}

unsigned char Mountain60KeyboardController::ConvertDirection(unsigned int direction, bool rotation)
{
    unsigned char ret;
    switch(direction)
    {
        case MODE_DIRECTION_LEFT:
        {
            ret = rotation?MOUNTAIN60_KEYBOARD_DIRECTION_ANTICLK:MOUNTAIN60_KEYBOARD_DIRECTION_LEFT;
        }
        break;

        case MODE_DIRECTION_RIGHT:
        {
            ret = rotation?MOUNTAIN60_KEYBOARD_DIRECTION_CLK:MOUNTAIN60_KEYBOARD_DIRECTION_RIGHT;
        }
        break;

        case MODE_DIRECTION_UP:
        {
            ret = MOUNTAIN60_KEYBOARD_DIRECTION_UP;
        }
        break;

        case MODE_DIRECTION_DOWN:
        {
            ret = MOUNTAIN60_KEYBOARD_DIRECTION_DOWN;
        }
        break;

        default:
        {
            ret = MOUNTAIN60_KEYBOARD_DIRECTION_LEFT;
        }
        break;
    }
    return ret;
}
