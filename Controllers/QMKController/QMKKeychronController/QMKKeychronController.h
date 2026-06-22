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
| KC_RGB command group (0xA8)                               |
|                                                           |
|   Keychron-specific per-key RGB protocol.                  |
|   Matches the firmware enum in                            |
|   keyboards/keychron/common/rgb/keychron_rgb.c            |
\*---------------------------------------------------------*/
#define KEYCHRON_KC_RGB_CMD_GROUP                   0xA8

enum KeychronKCRGBCommand
{
    KEYCHRON_KC_RGB_PROTOCOL_VER       = 0x01,
    KEYCHRON_KC_RGB_SAVE               = 0x02,
    KEYCHRON_KC_RGB_GET_INDICATORS     = 0x03,
    KEYCHRON_KC_RGB_SET_INDICATORS     = 0x04,
    KEYCHRON_KC_RGB_LED_COUNT          = 0x05,
    KEYCHRON_KC_RGB_LED_IDX            = 0x06,
    KEYCHRON_KC_RGB_PER_KEY_GET_TYPE   = 0x07,
    KEYCHRON_KC_RGB_PER_KEY_SET_TYPE   = 0x08,
    KEYCHRON_KC_RGB_PER_KEY_GET_COLOR  = 0x09,
    KEYCHRON_KC_RGB_PER_KEY_SET_COLOR  = 0x0A,
    KEYCHRON_KC_RGB_MIXED_GET_INFO     = 0x0B,
    KEYCHRON_KC_RGB_MIXED_GET_REGIONS  = 0x0C,
    KEYCHRON_KC_RGB_MIXED_SET_REGIONS  = 0x0D,
    KEYCHRON_KC_RGB_MIXED_GET_EFFECTS  = 0x0E,
    KEYCHRON_KC_RGB_MIXED_SET_EFFECTS  = 0x0F,
};

/*---------------------------------------------------------*\
| Per-key RGB animation types (PER_KEY_RGB_SET_TYPE value)  |
\*---------------------------------------------------------*/
enum KeychronPerKeyRgbType
{
    KEYCHRON_PER_KEY_RGB_SOLID           = 0,
    KEYCHRON_PER_KEY_RGB_BREATHING       = 1,
    KEYCHRON_PER_KEY_RGB_REACTIVE_SIMPLE = 2,
    KEYCHRON_PER_KEY_RGB_REACTIVE_WIDE   = 3,
    KEYCHRON_PER_KEY_RGB_REACTIVE_SPLASH = 4,
};

/*---------------------------------------------------------*\
| VIA backlight config commands                             |
|                                                           |
|   Standard QMK/VIA rgb_matrix control.                    |
|   Format: [cmd, value_type, value_id, value]              |
|   value_type = 3 for rgb_matrix                           |
\*---------------------------------------------------------*/
enum KeychronVIABacklightCommand
{
    KEYCHRON_VIA_BACKLIGHT_SET_VALUE = 0x07,
    KEYCHRON_VIA_BACKLIGHT_GET_VALUE = 0x08,
    KEYCHRON_VIA_BACKLIGHT_SAVE      = 0x09,
};

/*---------------------------------------------------------*\
| VIA backlight value IDs                                    |
\*---------------------------------------------------------*/
#define KEYCHRON_VIA_BACKLIGHT_TYPE_RGB_MATRIX      0x03

enum KeychronVIABacklightValueID
{
    KEYCHRON_VIA_BACKLIGHT_BRIGHTNESS = 0x01,
    KEYCHRON_VIA_BACKLIGHT_EFFECT     = 0x02,
    KEYCHRON_VIA_BACKLIGHT_SPEED      = 0x03,
    KEYCHRON_VIA_BACKLIGHT_COLOR      = 0x04,
};

/*---------------------------------------------------------*\
| Q1 HE effect IDs                                          |
|                                                           |
|   Determined by the number of standard effects enabled    |
|   in the Q1 HE firmware (info.json animations) +           |
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
    QMKKeychronController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~QMKKeychronController();

    std::string                   GetDeviceLocation();
    std::string                   GetNameString();
    std::string                   GetSerialString();

    unsigned int                  GetLedCount();
    std::vector<int>              GetLedNumbersByRow(unsigned char row);
    std::vector<std::vector<int>> GetAllLedNumbers(unsigned char num_rows);

    void                          SetPerKeyRgbColor(unsigned char start, unsigned char count, const std::vector<unsigned char>& hsv_data);
    std::vector<unsigned char>    GetPerKeyRgbColor(unsigned char start, unsigned char count);

    void                          SetRgbMatrixMode(unsigned char mode);
    unsigned char                 GetRgbMatrixMode();

    void                          SetPerKeyRgbType(unsigned char type);

    void                          SetBrightness(unsigned char brightness);
    unsigned char                 GetBrightness();

    void                          SetSpeed(unsigned char speed);
    unsigned char                 GetSpeed();

    void                          SetColorHSV(unsigned char h, unsigned char s);
    void                          SaveLedConf();

private:
    hid_device*                 dev;
    std::string                 location;
    std::string                 name;

    void                        SendPacket(unsigned char* data, size_t len);
    int                         ReadPacket(unsigned char* buf, size_t buf_len);
};
