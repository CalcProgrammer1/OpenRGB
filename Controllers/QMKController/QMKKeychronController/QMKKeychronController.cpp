/*---------------------------------------------------------*\
| QMKKeychronController.cpp                                 |
|                                                           |
|   Driver for Keychron QMK-based keyboards                 |
|                                                           |
|   Amadej Kastelic                             21 Jun 2026 |
|   Adam Honse <calcprogrammer1@gmail.com>      22 Jun 2026 |
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

/*---------------------------------------------------------*\
| Keychron Patch Types                                      |
|   The Keychron protocol does not provide RGB matrix X/Y   |
|   coordinates, only the position in the wiring matrix.    |
|   For most Keychron keyboards, the wiring matrix mostly   |
|   matches the physical position, but there are often a    |
|   few misplaced LEDs.  Implement a matrix patching system |
|   to apply keyboard-specific patches for these misplaced  |
|   LEDs.                                                   |
\*---------------------------------------------------------*/
typedef struct
{
    unsigned int                led_idx;
    unsigned int                row;
    unsigned int                col;
} keychron_patch_entry;

typedef struct
{
    unsigned short              pid;
    unsigned int                num_entries;
    const keychron_patch_entry* patch;
} keychron_patch;

/*---------------------------------------------------------*\
| Keychron Q2 ANSI Encoder                                  |
\*---------------------------------------------------------*/
#define KEYCHRON_Q2_ANSI_ENCODER_PATCH_ENTRIES_COUNT (sizeof(keychron_q2_ansi_encoder_patch_entries) / sizeof(keychron_patch_entry))

static const keychron_patch_entry keychron_q2_ansi_encoder_patch_entries[] =
{
    { 60,   4,      9 },
    { 61,   4,     10 },
    { 62,   4,     11 },
    { 63,   4,     12 },
    { 54,   3,     12 },
    { 55,   3,     13 },
    { 64,   4,     13 },
};

static const keychron_patch keychron_q2_ansi_encoder_patch =
{
    KEYCHRON_Q2_ANSI_ENCODER_PID,
    KEYCHRON_Q2_ANSI_ENCODER_PATCH_ENTRIES_COUNT,
    keychron_q2_ansi_encoder_patch_entries
};

/*---------------------------------------------------------*\
| Keychron Q6 ANSI Encoder                                  |
\*---------------------------------------------------------*/
#define KEYCHRON_Q6_ULTRA_8K_ANSI_PATCH_ENTRIES_COUNT (sizeof(keychron_q6_ultra_8k_ansi_patch_entries) / sizeof(keychron_patch_entry))

static const keychron_patch_entry keychron_q6_ultra_8k_ansi_patch_entries[] =
{
    { 19,   0,     20 },
    { 40,   1,     20 },
    { 77,   3,     20 },
    { 107,  5,     20 },
};

static const keychron_patch keychron_q6_ultra_8k_ansi_patch =
{
    KEYCHRON_Q6_ULTRA_8K_ANSI_PID,
    KEYCHRON_Q6_ULTRA_8K_ANSI_PATCH_ENTRIES_COUNT,
    keychron_q6_ultra_8k_ansi_patch_entries
};

/*---------------------------------------------------------*\
| List of all Keychron patches                              |
\*---------------------------------------------------------*/
#define KEYCHRON_PATCH_COUNT (sizeof(keychron_patches) / sizeof(keychron_patch*))

static const keychron_patch* keychron_patches[] =
{
    &keychron_q2_ansi_encoder_patch,
    &keychron_q6_ultra_8k_ansi_patch,
};

QMKKeychronController::QMKKeychronController(hid_device* dev_handle, const char *path, unsigned short dev_pid)
{
    /*-----------------------------------------------------*\
    | Initialize controller fields                          |
    \*-----------------------------------------------------*/
    dev                     = dev_handle;
    location                = path;
    kc_protocol_version     = 0;
    pid                     = dev_pid;
    supported_features      = 0;
    via_protocol_version    = 0;

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

    /*-----------------------------------------------------*\
    | Get Keychron firmware version                         |
    \*-----------------------------------------------------*/
    kc_firmware_version = CmdGetKeychronFirmwareVersion();

    /*-----------------------------------------------------*\
    | Get supported Keychron features                       |
    \*-----------------------------------------------------*/
    CmdGetSupportFeature(&supported_features);

    if(!GetSupported())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Get Keychron RGB protocol version                     |
    \*-----------------------------------------------------*/
    CmdGetKeychronRGBProtocolVersion(&kc_rgb_protocol_version);

    /*-----------------------------------------------------*\
    | Get count of LEDs                                     |
    \*-----------------------------------------------------*/
    CmdGetNumberLEDs(&number_leds);

    led_info.resize(number_leds);
    keycodes.resize(number_leds);

    for(std::size_t led_idx = 0; led_idx < led_info.size(); led_idx++)
    {
        led_info[led_idx].valid = false;
    }

    /*-----------------------------------------------------*\
    | Get info and keycode for all LEDs                     |
    \*-----------------------------------------------------*/
    for(unsigned char row = 0; row < 32; row++)
    {
        std::vector<unsigned char> row_leds = CmdGetLEDIndexByRow(row);

        for(unsigned char col = 0; col < row_leds.size(); col++)
        {
            if(row_leds[col] != 0xFF && row_leds[col] < led_info.size() && led_info[row_leds[col]].valid == false)
            {
                led_info[row_leds[col]].valid   = true;
                led_info[row_leds[col]].col     = col;
                led_info[row_leds[col]].row     = row;
            }
        }
    }

    for(unsigned short led_index = 0; led_index < number_leds; led_index++)
    {
        keycodes[led_index] = CmdGetKeycode(0, led_info[led_index].row, led_info[led_index].col);
    }

    /*-----------------------------------------------------*\
    | Apply matrix corrections                              |
    \*-----------------------------------------------------*/
    for(unsigned int patch_idx = 0; patch_idx < KEYCHRON_PATCH_COUNT; patch_idx++)
    {
        if(pid == keychron_patches[patch_idx]->pid)
        {
            for(unsigned int patch_entry_idx = 0; patch_entry_idx < keychron_patches[patch_idx]->num_entries; patch_entry_idx++)
            {
                led_info[keychron_patches[patch_idx]->patch[patch_entry_idx].led_idx].row = keychron_patches[patch_idx]->patch[patch_entry_idx].row;
                led_info[keychron_patches[patch_idx]->patch[patch_entry_idx].led_idx].col = keychron_patches[patch_idx]->patch[patch_entry_idx].col;
            }
        }
    }
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
    return("VIA: "          + std::to_string(via_protocol_version) + "\r\n" +
           "Keychron: "     + std::to_string(kc_protocol_version) + "\r\n" +
           "Keychron RGB: " + std::to_string(kc_rgb_protocol_version) + "\r\n" +
           "Keychron FW: "  + kc_firmware_version);
}

bool QMKKeychronController::GetSupported()
{
    return(supported_features & KC_FEATURE_KEYCHRON_RGB);
}

unsigned short QMKKeychronController::GetKeycode(unsigned short led_index)
{
    return(keycodes[led_index]);
}

unsigned short QMKKeychronController::GetLEDCount()
{
    return(number_leds);
}

qmk_rgb_matrix_led_info QMKKeychronController::GetLEDInfo(unsigned short led_index)
{
    return(led_info[led_index]);
}

void QMKKeychronController::SaveMode()
{
    CmdSaveMode();
}

void QMKKeychronController::SendLEDs(unsigned short number_leds, RGBColor* color_data)
{
    unsigned short      led_start_index     = 0;
    unsigned short      number_packet_leds  = 9;

    while(led_start_index < number_leds)
    {
        if((number_leds - led_start_index) < 9)
        {
            number_packet_leds = (number_leds - led_start_index);
        }

        CmdSendLEDs((unsigned char)led_start_index, (unsigned char)number_packet_leds, &color_data[led_start_index]);

        led_start_index += number_packet_leds;
    }
}

void QMKKeychronController::SetMode(unsigned short mode, unsigned char speed, unsigned char hue, unsigned char sat, unsigned char val)
{
    if(mode == 0xFFFF)
    {
        CmdSetRGBMatrixMode(KEYCHRON_QHE_PER_KEY_RGB_EFFECT);
        CmdSetPerKeyRGBType(KEYCHRON_PER_KEY_RGB_SOLID);
    }
    else
    {
        CmdSetRGBMatrixMode((unsigned char)mode);
        CmdSetColorHS(hue, sat);
        CmdSetBrightness(val);
        CmdSetSpeed(speed);
    }
}

unsigned short QMKKeychronController::CmdGetKeycode
    (
    unsigned char       layer,
    unsigned char       row,
    unsigned char       col
    )
{
    unsigned char       args[3];
    unsigned char       response[5];
    unsigned short      keycode;

    args[0] = layer;
    args[1] = row;
    args[2] = col;

    ViaSendCommand(QMK_VIA_CMD_VIA_DYNAMIC_KEYMAP_GET_KEYCODE, args, sizeof(args), response, sizeof(response));

    keycode = ( response[3] << 8 )| response[4];

    return(keycode);
}


std::string QMKKeychronController::CmdGetKeychronFirmwareVersion()
{
    char                response[30];

    ViaSendCommand(KC_GET_FIRMWARE_VERSION, NULL, 0, (unsigned char*)response, sizeof(response));

    /*-----------------------------------------------------*\
    | Ensure response null termination                      |
    \*-----------------------------------------------------*/
    response[29] = 0;

    return(std::string(response));
}

void QMKKeychronController::CmdGetKeychronProtocolVersion
    (
    unsigned char*      kc_protocol_version
    )
{
    ViaSendCommand(KC_GET_PROTOCOL_VERSION, NULL, 0, (unsigned char*)kc_protocol_version, sizeof(unsigned char));
}

void QMKKeychronController::CmdGetKeychronRGBProtocolVersion(unsigned short* kc_rgb_protocol_version)
{
    ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_PROTOCOL_VER, NULL, 0, (unsigned char*)kc_rgb_protocol_version, sizeof(unsigned short));

    /*-----------------------------------------------------*\
    | The RGB protocol version byte order is reversed       |
    \*-----------------------------------------------------*/
    *kc_rgb_protocol_version = ((*kc_rgb_protocol_version & 0x00FF) << 8) | ((*kc_rgb_protocol_version & 0xFF00) >> 8);
}

std::vector<unsigned char> QMKKeychronController::CmdGetLEDIndexByRow(unsigned char row)
{
    unsigned char args[4];
    unsigned char response[KEYCHRON_QHE_PACKET_SIZE - 2];

    args[0] = row;
    args[1] = 0xFF;
    args[2] = 0xFF;
    args[3] = 0xFF;

    int bytes_read = ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_LED_IDX, args, sizeof(args), response, sizeof(response));

    std::vector<unsigned char> result;

    if(bytes_read > 0)
    {
        for(int i = 1; i < bytes_read; i++)
        {
            result.push_back(response[i] == 0xFF ? -1 : response[i]);
        }
    }

    return result;
}

void QMKKeychronController::CmdGetNumberLEDs
    (
    unsigned short*     number_leds
    )
{
    ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_LED_COUNT, NULL, 0, (unsigned char*)number_leds, sizeof(unsigned short));

    /*-----------------------------------------------------*\
    | The LED count byte order is reversed                  |
    \*-----------------------------------------------------*/
    *number_leds = ((*number_leds & 0x00FF) << 8) | ((*number_leds & 0xFF00) >> 8);
}

void QMKKeychronController::CmdGetSupportFeature(unsigned short* supported_features)
{
    unsigned char response[3];

    ViaSendCommand(KC_GET_SUPPORT_FEATURE, NULL, 0, response, sizeof(response));

    /*-----------------------------------------------------*\
    | The QMK and ZMK variants of this packet differ by one |
    | byte position                                         |
    \*-----------------------------------------------------*/
    if(response[0] == 0)
    {
        *supported_features = (response[2] << 8) | response[1];
    }
    else
    {
        *supported_features = (response[1] << 8) | response[0];
    }
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

void QMKKeychronController::CmdSaveMode()
{
    ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_SAVE, NULL, 0, NULL, 0);
}

void QMKKeychronController::CmdSendLEDs(unsigned char start_index, unsigned char number_leds, RGBColor* color_data)
{
    unsigned char args[KEYCHRON_QHE_PACKET_SIZE - 2];

    args[0]         = start_index;
    args[1]         = number_leds;

    if(number_leds > 9)
    {
        number_leds = 9;
    }

    for(unsigned char led_index = 0; led_index < number_leds; led_index++)
    {
        /*-------------------------------------------------*\
        | VialRGB sends direct packets in HSV for some      |
        | inexplicable reason, so do the RGB to HSV         |
        | conversion before sending                         |
        \*-------------------------------------------------*/
        hsv_t hsv_color;
        rgb2hsv(color_data[led_index], &hsv_color);

        args[2 + (led_index * 3)]   = (unsigned char)((float)hsv_color.hue * (256.0f / 360.0f));
        args[3 + (led_index * 3)]   = hsv_color.saturation;
        args[4 + (led_index * 3)]   = hsv_color.value;
    }

    ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_PER_KEY_SET_COLOR, args, sizeof(args), NULL, 0);
}

void QMKKeychronController::CmdSetBrightness(unsigned char brightness)
{
    unsigned char args[3];

    args[0] = QMK_VIA_RGB_MATRIX_CHANNEL;
    args[1] = QMK_VIA_RGB_MATRIX_BRIGHTNESS;
    args[2] = brightness;

    ViaSendCommand(QMK_VIA_CMD_CUSTOM_SET_VALUE, args, sizeof(args), NULL, 0);
}

void QMKKeychronController::CmdSetColorHS(unsigned char h, unsigned char s)
{
    unsigned char args[4];

    args[0] = QMK_VIA_RGB_MATRIX_CHANNEL;
    args[1] = QMK_VIA_RGB_MATRIX_COLOR;
    args[2] = h;
    args[3] = s;

    ViaSendCommand(QMK_VIA_CMD_CUSTOM_SET_VALUE, args, sizeof(args), NULL, 0);
}

void QMKKeychronController::CmdSetPerKeyRGBType(unsigned char type)
{
    unsigned char args[1];

    args[0] = type;

    ViaSendCommandSub(KC_KEYCHRON_RGB, KEYCHRON_RGB_PER_KEY_SET_TYPE, args, sizeof(args), NULL, 0);
}

void QMKKeychronController::CmdSetRGBMatrixMode(unsigned char mode)
{
    unsigned char args[3];

    args[0] = QMK_VIA_RGB_MATRIX_CHANNEL;
    args[1] = QMK_VIA_RGB_MATRIX_EFFECT;
    args[2] = mode;

    ViaSendCommand(QMK_VIA_CMD_CUSTOM_SET_VALUE, args, sizeof(args), NULL, 0);
}

void QMKKeychronController::CmdSetSpeed(unsigned char speed)
{
    unsigned char args[3];

    args[0] = QMK_VIA_RGB_MATRIX_CHANNEL;
    args[1] = QMK_VIA_RGB_MATRIX_EFFECT_SPEED;
    args[2] = speed;

    ViaSendCommand(QMK_VIA_CMD_CUSTOM_SET_VALUE, args, sizeof(args), NULL, 0);
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
