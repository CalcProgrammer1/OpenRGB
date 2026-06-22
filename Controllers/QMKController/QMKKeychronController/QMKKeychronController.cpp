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
#include "hsv.h"
#include "QMKKeychronController.h"
#include "QMKViaCommands.h"
#include "StringUtils.h"
#include "LogManager.h"

using namespace std::chrono_literals;

QMKKeychronController::QMKKeychronController(hid_device* dev_handle, const char *path)
{
    /*-----------------------------------------------------*\
    | Initialize controller fields                          |
    \*-----------------------------------------------------*/
    dev         = dev_handle;
    location    = path;
    supported   = false;

    /*-----------------------------------------------------*\
    | Read product string                                   |
    \*-----------------------------------------------------*/
    wchar_t product_string[256];

    int ret = hid_get_product_string(dev, product_string, 256);

    if(ret != 0)
    {
        name = "";
    }
    else
    {
        name = StringUtils::wstring_to_string(product_string);
    }

    /*-----------------------------------------------------*\
    | Read vendor string                                    |
    \*-----------------------------------------------------*/
    wchar_t vendor_string[256];

    ret = hid_get_manufacturer_string(dev, vendor_string, 256);

    if(ret != 0)
    {
        vendor = "";
    }
    else
    {
        vendor = StringUtils::wstring_to_string(vendor_string);
    }

    /*-----------------------------------------------------*\
    | Read serial string                                    |
    \*-----------------------------------------------------*/
    wchar_t serial_string[256];

    ret = hid_get_serial_number_string(dev, serial_string, 256);

    if(ret != 0)
    {
        serial = "";
    }
    else
    {
        serial = StringUtils::wstring_to_string(serial_string);
    }

    /*-----------------------------------------------------*\
    | Get VIA protocol version                              |
    \*-----------------------------------------------------*/
    CmdGetViaProtocolVersion(&via_protocol_version);

    /*-----------------------------------------------------*\
    | Get Keychron protocol version                         |
    \*-----------------------------------------------------*/
    CmdGetKeychronProtocolVersion(&kc_protocol_version);
}

QMKKeychronController::~QMKKeychronController()
{
    hid_close(dev);
}

std::string QMKKeychronController::GetLocation()
{
    return("HID: " + location);
}

std::string QMKKeychronController::GetName()
{
    return(name);
}

std::string QMKKeychronController::GetSerial()
{
    return(serial);
}

std::string QMKKeychronController::GetVendor()
{
    return(vendor);
}

std::string QMKKeychronController::GetVersion()
{
    /*-----------------------------------------------------*\
    | Format multi-line version text                        |
    \*-----------------------------------------------------*/
    return("VIA: " + std::to_string(via_protocol_version) + "\r\n" +
           "Keychron: " + std::to_string(kc_protocol_version));
}

bool QMKKeychronController::GetSupported()
{
    return(supported);
}

void QMKKeychronController::CmdGetKeychronProtocolVersion
    (
    unsigned char*      kc_protocol_version
    )
{
    ViaSendCommand(KC_GET_PROTOCOL_VERSION, NULL, 0, (unsigned char*)kc_protocol_version, sizeof(unsigned char));
}

void QMKKeychronController::CmdGetViaProtocolVersion
    (
    unsigned short*     via_protocol_version
    )
{
    ViaSendCommand(QMK_VIA_CMD_GET_PROTOCOL_VERSION, NULL, 0, (unsigned char*)via_protocol_version, sizeof(unsigned short));

    /*-----------------------------------------------------*\
    | The protocol version byte order is reversed           |
    \*-----------------------------------------------------*/
    *via_protocol_version = ((*via_protocol_version & 0x00FF) << 8) | ((*via_protocol_version & 0xFF00) >> 8);
}

unsigned int QMKKeychronController::GetLedCount()
{
    unsigned short led_count;

    ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_LED_COUNT, NULL, 0, (unsigned char*)&led_count, sizeof(unsigned short));

    /*-----------------------------------------------------*\
    | The LED count byte order is reversed                  |
    \*-----------------------------------------------------*/
    led_count = ((led_count & 0x00FF) << 8) | ((led_count & 0xFF00) >> 8);

    return(led_count);
}

std::vector<int> QMKKeychronController::GetLedNumbersByRow(unsigned char row)
{
    unsigned char args[4];
    unsigned char response[KEYCHRON_QHE_PACKET_SIZE - 2];

    args[0] = row;
    args[1] = 0xFF;
    args[2] = 0xFF;
    args[3] = 0xFF;

    int bytes_read = ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_LED_IDX, args, sizeof(args), response, sizeof(response));

    std::vector<int> result;

    if(bytes_read > 0)
    {
        for(int i = 0; i < bytes_read; i++)
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
    unsigned char args[KEYCHRON_QHE_PACKET_SIZE - 2];

    args[0]             = start;
    args[1]             = count;

    size_t data_offset  = 2;
    size_t copy_len     = hsv_data.size();

    if(data_offset + copy_len > (KEYCHRON_QHE_PACKET_SIZE - 2))
    {
        copy_len = (KEYCHRON_QHE_PACKET_SIZE - 2) - data_offset;
    }

    memcpy(&args[data_offset], hsv_data.data(), copy_len);

    ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_PER_KEY_SET_COLOR, args, sizeof(args), NULL, 0);
}

std::vector<unsigned char> QMKKeychronController::GetPerKeyRgbColor(unsigned char start, unsigned char count)
{
    unsigned char args[2];
    unsigned char response[KEYCHRON_QHE_PACKET_SIZE - 2];

    args[0] = start;
    args[1] = count;

    int bytes_read = ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_PER_KEY_GET_COLOR, args, sizeof(args), response, sizeof(response));

    std::vector<unsigned char> result;
    size_t color_bytes = (size_t)count * 3;

    if(color_bytes <= (size_t)bytes_read)
    {
        for(size_t i = 0; i < color_bytes; i++)
        {
            result.push_back(response[i]);
        }
    }

    return result;
}

void QMKKeychronController::SetPerKeyRgbType(unsigned char type)
{
    unsigned char args[1];

    args[0] = type;

    ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_PER_KEY_SET_TYPE, args, sizeof(args), NULL, 0);
}

void QMKKeychronController::SetRgbMatrixMode(unsigned char mode)
{
    unsigned char args[3];

    args[0] = QMK_VIA_RGB_MATRIX_CHANNEL;
    args[1] = QMK_VIA_RGB_MATRIX_EFFECT;
    args[2] = mode;

    ViaSendCommand(QMK_VIA_CMD_CUSTOM_SET_VALUE, args, sizeof(args), NULL, 0);
}

unsigned char QMKKeychronController::GetRgbMatrixMode()
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = QMK_VIA_CMD_CUSTOM_GET_VALUE;
    cmd[1] = QMK_VIA_RGB_MATRIX_CHANNEL;
    cmd[2] = QMK_VIA_RGB_MATRIX_EFFECT;

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
    unsigned char args[3];

    args[0] = QMK_VIA_RGB_MATRIX_CHANNEL;
    args[1] = QMK_VIA_RGB_MATRIX_BRIGHTNESS;
    args[2] = brightness;

    ViaSendCommand(QMK_VIA_CMD_CUSTOM_SET_VALUE, args, sizeof(args), NULL, 0);
}

unsigned char QMKKeychronController::GetBrightness()
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = QMK_VIA_CMD_CUSTOM_GET_VALUE;
    cmd[1] = QMK_VIA_RGB_MATRIX_CHANNEL;
    cmd[2] = QMK_VIA_RGB_MATRIX_BRIGHTNESS;

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
    unsigned char args[3];

    args[0] = QMK_VIA_RGB_MATRIX_CHANNEL;
    args[1] = QMK_VIA_RGB_MATRIX_EFFECT_SPEED;
    args[2] = speed;

    ViaSendCommand(QMK_VIA_CMD_CUSTOM_SET_VALUE, args, sizeof(args), NULL, 0);
}

unsigned char QMKKeychronController::GetSpeed()
{
    unsigned char cmd[KEYCHRON_QHE_PACKET_SIZE];
    memset(cmd, 0x00, KEYCHRON_QHE_PACKET_SIZE);

    cmd[0] = QMK_VIA_CMD_CUSTOM_GET_VALUE;
    cmd[1] = QMK_VIA_RGB_MATRIX_CHANNEL;
    cmd[2] = QMK_VIA_RGB_MATRIX_EFFECT_SPEED;

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
    unsigned char args[4];

    args[0] = QMK_VIA_RGB_MATRIX_CHANNEL;
    args[1] = QMK_VIA_RGB_MATRIX_COLOR;
    args[2] = h;
    args[3] = s;

    ViaSendCommand(QMK_VIA_CMD_CUSTOM_SET_VALUE, args, sizeof(args), NULL, 0);
}

void QMKKeychronController::SaveLedConf()
{
    ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_SAVE, NULL, 0, NULL, 0);
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

int QMKKeychronController::ViaSendCommand
    (
    unsigned char       cmd,
    unsigned char*      data_in,
    unsigned char       data_in_size,
    unsigned char*      data_out,
    unsigned char       data_out_size
    )
{
    /*-----------------------------------------------------*\
    | Standard VIA command with no sub-command              |
    |                                                       |
    | Byte 0: Command                                       |
    | Byte 1+: Data                                         |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[KEYCHRON_QHE_PACKET_SIZE + 1];

    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Write command, offsetting by 1 for HID report ID      |
    \*-----------------------------------------------------*/
    usb_buf[1] = cmd;
    memcpy(&usb_buf[2], data_in, data_in_size);

    hid_write(dev, usb_buf, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Read response                                         |
    \*-----------------------------------------------------*/
    int bytes_received = hid_read_timeout(dev, usb_buf, sizeof(usb_buf) - 1, 1000);

    if(usb_buf[0] != cmd)
    {
        return(-1);
    }

    memcpy(data_out, &usb_buf[1], data_out_size);

    return(bytes_received - 1);
}

int QMKKeychronController::ViaSendCommandSub
    (
    unsigned char       cmd,
    unsigned char       subcmd,
    unsigned char*      data_in,
    unsigned char       data_in_size,
    unsigned char*      data_out,
    unsigned char       data_out_size
    )
{
    /*-----------------------------------------------------*\
    | Standard VIA command with sub-command                 |
    |                                                       |
    | Byte 0: Command                                       |
    | Byte 1: Sub-Command                                   |
    | Byte 2+: Data                                         |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[KEYCHRON_QHE_PACKET_SIZE + 1];

    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Write command, offsetting by 1 for HID report ID      |
    \*-----------------------------------------------------*/
    usb_buf[1] = cmd;
    usb_buf[2] = subcmd;
    memcpy(&usb_buf[3], data_in, data_in_size);

    hid_write(dev, usb_buf, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Read response                                         |
    \*-----------------------------------------------------*/
    int bytes_received = hid_read_timeout(dev, usb_buf, sizeof(usb_buf) - 1, 1000);

    if(usb_buf[0] != cmd || usb_buf[1] != subcmd)
    {
        return(-1);
    }

    memcpy(data_out, &usb_buf[2], data_out_size);

    return(bytes_received - 2);
}
