/*---------------------------------------------------------*\
| QMKKeychronController.h                                   |
|                                                           |
|   Driver for Keychron QMK-based keyboards                 |
|   (Q1 HE and other KEYCHRON_RGB-enabled models)           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <string>
#include <hidapi.h>

/*---------------------------------------------------------*\
| Keychron vendor ID                                        |
\*---------------------------------------------------------*/
#define KEYCHRON_VID                                0x3434

/*---------------------------------------------------------*\
| Product IDs                                               |
\*---------------------------------------------------------*/
#define KEYCHRON_Q1_HE_PID                          0x0B10
#define KEYCHRON_Q2_PID                             0x0111

/*---------------------------------------------------------*\
| QMK raw HID usage page/usage                              |
\*---------------------------------------------------------*/
#define KEYCHRON_QMK_USAGE_PAGE                     0xFF60
#define KEYCHRON_QMK_USAGE                          0x61

/*---------------------------------------------------------*\
| HID packet constants                                      |
\*---------------------------------------------------------*/
#define KEYCHRON_QHE_PACKET_SIZE                    32
#define KEYCHRON_QHE_HID_READ_TIMEOUT               1000

/*---------------------------------------------------------*\
| Keychron-specific VIA protocol extension                  |
\*---------------------------------------------------------*/
enum
{
    KC_GET_PROTOCOL_VERSION                         = 0xA0,
    KC_GET_FIRMWARE_VERSION                         = 0xA1,
    KC_GET_SUPPORT_FEATURE                          = 0xA2,
    KC_GET_DEFAULT_LAYER                            = 0xA3,
    KC_MISC_CMD_GROUP                               = 0xA7,
    KC_KEYCHRON_RGB                                 = 0xA8,
    KC_ANALOG_MATRIX                                = 0xA9,
    KC_WIRELESS_DFU                                 = 0xAA,
    KC_FACTORY_TEST                                 = 0xAB
};

enum KeychronKCRGBCommand
{
    KEYCHRON_RGB_PROTOCOL_VER                       = 0x01,
    KEYCHRON_RGB_SAVE                               = 0x02,
    KEYCHRON_RGB_GET_INDICATORS                     = 0x03,
    KEYCHRON_RGB_SET_INDICATORS                     = 0x04,
    KEYCHRON_RGB_LED_COUNT                          = 0x05,
    KEYCHRON_RGB_LED_IDX                            = 0x06,
    KEYCHRON_RGB_PER_KEY_GET_TYPE                   = 0x07,
    KEYCHRON_RGB_PER_KEY_SET_TYPE                   = 0x08,
    KEYCHRON_RGB_PER_KEY_GET_COLOR                  = 0x09,
    KEYCHRON_RGB_PER_KEY_SET_COLOR                  = 0x0A,
    KEYCHRON_RGB_MIXED_GET_INFO                     = 0x0B,
    KEYCHRON_RGB_MIXED_GET_REGIONS                  = 0x0C,
    KEYCHRON_RGB_MIXED_SET_REGIONS                  = 0x0D,
    KEYCHRON_RGB_MIXED_GET_EFFECTS                  = 0x0E,
    KEYCHRON_RGB_MIXED_SET_EFFECTS                  = 0x0F,
};

/*---------------------------------------------------------*\
| Per-key RGB animation types (PER_KEY_RGB_SET_TYPE value)  |
\*---------------------------------------------------------*/
enum KeychronPerKeyRgbType
{
    KEYCHRON_PER_KEY_RGB_SOLID                      = 0,
    KEYCHRON_PER_KEY_RGB_BREATHING                  = 1,
    KEYCHRON_PER_KEY_RGB_REACTIVE_SIMPLE            = 2,
    KEYCHRON_PER_KEY_RGB_REACTIVE_WIDE              = 3,
    KEYCHRON_PER_KEY_RGB_REACTIVE_SPLASH            = 4,
};

/*---------------------------------------------------------*\
| VIA backlight value IDs                                   |
\*---------------------------------------------------------*/
#define KEYCHRON_VIA_BACKLIGHT_TYPE_RGB_MATRIX      0x03

enum KeychronVIABacklightValueID
{
    KEYCHRON_VIA_BACKLIGHT_BRIGHTNESS               = 0x01,
    KEYCHRON_VIA_BACKLIGHT_EFFECT                   = 0x02,
    KEYCHRON_VIA_BACKLIGHT_SPEED                    = 0x03,
    KEYCHRON_VIA_BACKLIGHT_COLOR                    = 0x04,
};

/*---------------------------------------------------------*\
| Q1 HE effect IDs                                          |
|                                                           |
|   Determined by the number of standard effects enabled    |
|   in the Q1 HE firmware (info.json animations) +          |
|   2 custom effects (PER_KEY_RGB, MIXED_RGB).              |
|   May need adjustment for other firmware versions.        |
\*---------------------------------------------------------*/
#define KEYCHRON_QHE_PER_KEY_RGB_EFFECT             23

/*---------------------------------------------------------*\
| Brightness and speed ranges                               |
\*---------------------------------------------------------*/
#define KEYCHRON_QHE_MIN_BRIGHTNESS                 0x00
#define KEYCHRON_QHE_MAX_BRIGHTNESS                 0xFF
#define KEYCHRON_QHE_MIN_SPEED                      0x00
#define KEYCHRON_QHE_MAX_SPEED                      0xFF

class QMKKeychronController
{
public:
    QMKKeychronController(hid_device* dev_handle, const char *path);
    ~QMKKeychronController();

    std::string                     GetLocation();
    std::string                     GetName();
    std::string                     GetSerial();
    std::string                     GetVendor();
    std::string                     GetVersion();

    bool                            GetSupported();

    unsigned int                    GetLedCount();
    std::vector<int>                GetLedNumbersByRow(unsigned char row);
    std::vector<std::vector<int>>   GetAllLedNumbers(unsigned char num_rows);

    void                            SetPerKeyRgbColor(unsigned char start, unsigned char count, const std::vector<unsigned char>& hsv_data);
    std::vector<unsigned char>      GetPerKeyRgbColor(unsigned char start, unsigned char count);

    void                            SetRgbMatrixMode(unsigned char mode);
    unsigned char                   GetRgbMatrixMode();

    void                            SetPerKeyRgbType(unsigned char type);

    void                            SetBrightness(unsigned char brightness);
    unsigned char                   GetBrightness();

    void                            SetSpeed(unsigned char speed);
    unsigned char                   GetSpeed();

    void                            SetColorHSV(unsigned char h, unsigned char s);
    void                            SaveLedConf();

private:
    hid_device*                     dev;
    unsigned char                   kc_protocol_version;
    std::string                     location;
    std::string                     name;
    std::string                     serial;
    bool                            supported;
    std::string                     vendor;
    unsigned short                  via_protocol_version;

    void CmdGetKeychronProtocolVersion
        (
        unsigned char*      kc_protocol_version
        );

    void CmdGetViaProtocolVersion
        (
        unsigned short*     via_protocol_version
        );

    void SendPacket(unsigned char* data, size_t len);
    int  ReadPacket(unsigned char* buf, size_t buf_len);

    int ViaSendCommand
        (
        unsigned char       cmd,
        unsigned char*      data_in,
        unsigned char       data_in_size,
        unsigned char*      data_out,
        unsigned char       data_out_size
        );

    int ViaSendCommandSub
        (
        unsigned char       cmd,
        unsigned char       subcmd,
        unsigned char*      data_in,
        unsigned char       data_in_size,
        unsigned char*      data_out,
        unsigned char       data_out_size
        );
};
