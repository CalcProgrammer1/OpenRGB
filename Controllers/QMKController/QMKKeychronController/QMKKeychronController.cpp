/*---------------------------------------------------------*\
| QMKKeychronController.cpp                                 |
|                                                           |
|   Driver for Keychron QMK-based keyboards                 |
|   (Q1 HE and other KEYCHRON_RGB-enabled models)           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string.h>
#include "QMKKeychronController.h"
#include "StringUtils.h"
#include "LogManager.h"

using namespace std::chrono_literals;

QMKKeychronController::QMKKeychronController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev         = dev_handle;
    location    = info.path;
    name        = dev_name;
}

QMKKeychronController::~QMKKeychronController()
{
    hid_close(dev);
}

std::string QMKKeychronController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string QMKKeychronController::GetNameString()
{
    return(name);
}

std::string QMKKeychronController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void QMKKeychronController::SendPacket(unsigned char* data, size_t len)
{
    unsigned char usb_buf[KEYCHRON_QHE_PACKET_SIZE + 1];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0] = 0x00;

    size_t copy_len = len;
    if(copy_len > KEYCHRON_QHE_PACKET_SIZE)
    {
        copy_len = KEYCHRON_QHE_PACKET_SIZE;
    }

    memcpy(&usb_buf[1], data, copy_len);

    hid_write(dev, usb_buf, KEYCHRON_QHE_PACKET_SIZE + 1);

    std::this_thread::sleep_for(5ms);
}

int QMKKeychronController::ReadPacket(unsigned char* buf, size_t buf_len)
{
    unsigned char usb_buf[KEYCHRON_QHE_PACKET_SIZE + 1];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    int bytes_read = hid_read_timeout(dev, usb_buf, KEYCHRON_QHE_PACKET_SIZE + 1, KEYCHRON_QHE_HID_READ_TIMEOUT);

    if(bytes_read > 0)
    {
        size_t copy_len = (size_t)bytes_read;
        if(copy_len > buf_len)
        {
            copy_len = buf_len;
        }
        memcpy(buf, usb_buf, copy_len);
    }

    return bytes_read;
}

unsigned int QMKKeychronController::GetLedCount()
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_KC_RGB_CMD_GROUP;
    cmd[1] = KEYCHRON_KC_RGB_LED_COUNT;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);

    unsigned char response[KEYCHRON_QHE_PACKET_SIZE];
    memset(response, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    int bytes_read = ReadPacket(response, KEYCHRON_QHE_PACKET_SIZE);

    if(bytes_read > 3 && response[0] == KEYCHRON_KC_RGB_CMD_GROUP && response[1] == KEYCHRON_KC_RGB_LED_COUNT)
    {
        return response[3];
    }

    return 0;
}

std::vector<int> QMKKeychronController::GetLedNumbersByRow(unsigned char row)
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_KC_RGB_CMD_GROUP;
    cmd[1] = KEYCHRON_KC_RGB_LED_IDX;
    cmd[2] = row;
    cmd[3] = 0xFF;
    cmd[4] = 0xFF;
    cmd[5] = 0xFF;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);

    unsigned char response[KEYCHRON_QHE_PACKET_SIZE];
    memset(response, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    int bytes_read = ReadPacket(response, KEYCHRON_QHE_PACKET_SIZE);

    std::vector<int> result;

    if(bytes_read > 3 && response[0] == KEYCHRON_KC_RGB_CMD_GROUP && response[1] == KEYCHRON_KC_RGB_LED_IDX)
    {
        for(int i = 3; i < bytes_read; i++)
        {
            result.push_back(response[i] == 0xFF ? -1 : response[i]);
        }
    }

    return result;
}

std::vector<std::vector<int>> QMKKeychronController::GetAllLedNumbers(unsigned char num_rows)
{
    std::vector<std::vector<int>> all_rows;

    for(unsigned char row = 0; row < num_rows; row++)
    {
        std::vector<int> row_data = GetLedNumbersByRow(row);
        all_rows.push_back(row_data);
    }

    return all_rows;
}

void QMKKeychronController::SetPerKeyRgbColor(unsigned char start, unsigned char count, const std::vector<unsigned char>& hsv_data)
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_KC_RGB_CMD_GROUP;
    cmd[1] = KEYCHRON_KC_RGB_PER_KEY_SET_COLOR;
    cmd[2] = start;
    cmd[3] = count;

    size_t data_offset = 4;
    size_t copy_len = hsv_data.size();
    if(data_offset + copy_len > KEYCHRON_QHE_PACKET_SIZE)
    {
        copy_len = KEYCHRON_QHE_PACKET_SIZE - data_offset;
    }

    memcpy(&cmd[data_offset], hsv_data.data(), copy_len);

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);

    unsigned char response[KEYCHRON_QHE_PACKET_SIZE];
    ReadPacket(response, KEYCHRON_QHE_PACKET_SIZE);
}

std::vector<unsigned char> QMKKeychronController::GetPerKeyRgbColor(unsigned char start, unsigned char count)
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_KC_RGB_CMD_GROUP;
    cmd[1] = KEYCHRON_KC_RGB_PER_KEY_GET_COLOR;
    cmd[2] = start;
    cmd[3] = count;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);

    unsigned char response[KEYCHRON_QHE_PACKET_SIZE];
    memset(response, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    int bytes_read = ReadPacket(response, KEYCHRON_QHE_PACKET_SIZE);

    std::vector<unsigned char> result;

    if(bytes_read > 3 && response[0] == KEYCHRON_KC_RGB_CMD_GROUP && response[1] == KEYCHRON_KC_RGB_PER_KEY_GET_COLOR)
    {
        size_t color_bytes = (size_t)count * 3;
        if(3 + color_bytes <= (size_t)bytes_read)
        {
            for(size_t i = 3; i < 3 + color_bytes; i++)
            {
                result.push_back(response[i]);
            }
        }
    }

    return result;
}

void QMKKeychronController::SetPerKeyRgbType(unsigned char type)
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_KC_RGB_CMD_GROUP;
    cmd[1] = KEYCHRON_KC_RGB_PER_KEY_SET_TYPE;
    cmd[2] = type;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);

    unsigned char response[KEYCHRON_QHE_PACKET_SIZE];
    ReadPacket(response, KEYCHRON_QHE_PACKET_SIZE);
}

void QMKKeychronController::SetRgbMatrixMode(unsigned char mode)
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_VIA_BACKLIGHT_SET_VALUE;
    cmd[1] = KEYCHRON_VIA_BACKLIGHT_TYPE_RGB_MATRIX;
    cmd[2] = KEYCHRON_VIA_BACKLIGHT_EFFECT;
    cmd[3] = mode;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);

    unsigned char response[KEYCHRON_QHE_PACKET_SIZE];
    ReadPacket(response, KEYCHRON_QHE_PACKET_SIZE);
}

unsigned char QMKKeychronController::GetRgbMatrixMode()
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_VIA_BACKLIGHT_GET_VALUE;
    cmd[1] = KEYCHRON_VIA_BACKLIGHT_TYPE_RGB_MATRIX;
    cmd[2] = KEYCHRON_VIA_BACKLIGHT_EFFECT;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);

    unsigned char response[KEYCHRON_QHE_PACKET_SIZE];
    memset(response, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    int bytes_read = ReadPacket(response, KEYCHRON_QHE_PACKET_SIZE);

    if(bytes_read > 3)
    {
        return response[3];
    }

    return 0;
}

void QMKKeychronController::SetBrightness(unsigned char brightness)
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_VIA_BACKLIGHT_SET_VALUE;
    cmd[1] = KEYCHRON_VIA_BACKLIGHT_TYPE_RGB_MATRIX;
    cmd[2] = KEYCHRON_VIA_BACKLIGHT_BRIGHTNESS;
    cmd[3] = brightness;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);
}

unsigned char QMKKeychronController::GetBrightness()
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_VIA_BACKLIGHT_GET_VALUE;
    cmd[1] = KEYCHRON_VIA_BACKLIGHT_TYPE_RGB_MATRIX;
    cmd[2] = KEYCHRON_VIA_BACKLIGHT_BRIGHTNESS;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);

    unsigned char response[KEYCHRON_QHE_PACKET_SIZE];
    memset(response, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    int bytes_read = ReadPacket(response, KEYCHRON_QHE_PACKET_SIZE);

    if(bytes_read > 3)
    {
        return response[3];
    }

    return 0;
}

void QMKKeychronController::SetSpeed(unsigned char speed)
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_VIA_BACKLIGHT_SET_VALUE;
    cmd[1] = KEYCHRON_VIA_BACKLIGHT_TYPE_RGB_MATRIX;
    cmd[2] = KEYCHRON_VIA_BACKLIGHT_SPEED;
    cmd[3] = speed;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);
}

unsigned char QMKKeychronController::GetSpeed()
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_VIA_BACKLIGHT_GET_VALUE;
    cmd[1] = KEYCHRON_VIA_BACKLIGHT_TYPE_RGB_MATRIX;
    cmd[2] = KEYCHRON_VIA_BACKLIGHT_SPEED;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);

    unsigned char response[KEYCHRON_QHE_PACKET_SIZE];
    memset(response, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    int bytes_read = ReadPacket(response, KEYCHRON_QHE_PACKET_SIZE);

    if(bytes_read > 3)
    {
        return response[3];
    }

    return 0;
}

void QMKKeychronController::SetColorHSV(unsigned char h, unsigned char s)
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_VIA_BACKLIGHT_SET_VALUE;
    cmd[1] = KEYCHRON_VIA_BACKLIGHT_TYPE_RGB_MATRIX;
    cmd[2] = KEYCHRON_VIA_BACKLIGHT_COLOR;
    cmd[3] = h;
    cmd[4] = s;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);
}

void QMKKeychronController::SaveLedConf()
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = KEYCHRON_KC_RGB_CMD_GROUP;
    cmd[1] = KEYCHRON_KC_RGB_SAVE;

    SendPacket(cmd, KEYCHRON_QHE_PACKET_SIZE);

    unsigned char response[KEYCHRON_QHE_PACKET_SIZE];
    ReadPacket(response, KEYCHRON_QHE_PACKET_SIZE);
}
