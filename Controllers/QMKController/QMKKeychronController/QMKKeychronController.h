/*---------------------------------------------------------*\
| QMKKeychronController.h                                   |
|                                                           |
|   Driver for Keychron QMK-based keyboards                 |
|                                                           |
|   Amadej Kastelic                             21 Jun 2026 |
|   Adam Honse <calcprogrammer1@gmail.com>      22 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "QMKCommon.h"
#include "RGBController.h"

/*---------------------------------------------------------*\
| Keychron vendor ID                                        |
\*---------------------------------------------------------*/
#define KEYCHRON_VID                                0x3434

/*---------------------------------------------------------*\
| Product IDs                                               |
\*---------------------------------------------------------*/
#define KEYCHRON_C1_PRO_ANSI_RGB_PID                0x0510
#define KEYCHRON_C1_PRO_8K_ANSI_PID                 0x0521
#define KEYCHRON_C1_PRO_8K_ISO_PID                  0x051D
#define KEYCHRON_C1_PRO_8K_JIS_PID                  0x051E
#define KEYCHRON_C1_PRO_V2_ANSI_RGB_PID             0x0516
#define KEYCHRON_C2_PRO_ANSI_RGB_PID                0x0520
#define KEYCHRON_C2_PRO_8K_ANSI_PID                 0x0522
#define KEYCHRON_C2_PRO_8K_ISO_PID                  0x052D
#define KEYCHRON_C2_PRO_V2_ANSI_RGB_PID             0x0526
#define KEYCHRON_C3_PRO_ANSI_RGB_PID                0x0433
#define KEYCHRON_C3_PRO_8K_ANSI_PID                 0x0530
#define KEYCHRON_C3_PRO_8K_ISO_PID                  0x0531
#define KEYCHRON_C3_PRO_8K_JIS_PID                  0x0532
#define KEYCHRON_K0_MAX_PID                         0x0A06
#define KEYCHRON_K1_MAX_ANSI_RGB_PID                0x0A10
#define KEYCHRON_K1_MAX_ISO_RGB_PID                 0x0A11
#define KEYCHRON_K1_MAX_JIS_RGB_PID                 0x0A12
#define KEYCHRON_K1_V6_ANSI_RGB_PID                 0x0D10
#define KEYCHRON_K1_V6_ISO_RGB_PID                  0x0D11
#define KEYCHRON_K1_V6_JIS_RGB_PID                  0x0D12
#define KEYCHRON_K2_HE_ANSI_PID                     0x0E20
#define KEYCHRON_K2_HE_ISO_PID                      0x0E21
#define KEYCHRON_K2_HE_JIS_PID                      0x0E22
#define KEYCHRON_K2_MAX_ANSI_RGB_PID                0x0A20
#define KEYCHRON_K2_MAX_ISO_RGB_PID                 0x0A21
#define KEYCHRON_K2_MAX_JIS_RGB_PID                 0x0A22
#define KEYCHRON_K2_V3_ANSI_RGB_PID                 0x0D20
#define KEYCHRON_K2_V3_ISO_RGB_PID                  0x0D21
#define KEYCHRON_K2_V3_JIS_RGB_PID                  0x0D22
#define KEYCHRON_K3_MAX_ANSI_RGB_PID                0x0A30
#define KEYCHRON_K3_MAX_ISO_RGB_PID                 0x0A31
#define KEYCHRON_K3_MAX_JIS_RGB_PID                 0x0A32
#define KEYCHRON_K3_V3_ANSI_RGB_PID                 0x0D30
#define KEYCHRON_K3_V3_ISO_RGB_PID                  0x0D31
#define KEYCHRON_K3_V3_JIS_RGB_PID                  0x0D32
#define KEYCHRON_K4_HE_ANSI_PID                     0x0E40
#define KEYCHRON_K4_HE_ISO_PID                      0x0E41
#define KEYCHRON_K4_HE_JIS_PID                      0x0E42
#define KEYCHRON_K4_MAX_ANSI_RGB_PID                0x0A40
#define KEYCHRON_K4_MAX_ISO_RGB_PID                 0x0A41
#define KEYCHRON_K4_MAX_JIS_RGB_PID                 0x0A42
#define KEYCHRON_K4_V3_ANSI_RGB_PID                 0x0D40
#define KEYCHRON_K4_V3_ISO_RGB_PID                  0x0D41
#define KEYCHRON_K4_V3_JIS_RGB_PID                  0x0D42
#define KEYCHRON_K5_MAX_ANSI_RGB_PID                0x0A50
#define KEYCHRON_K5_MAX_ISO_RGB_PID                 0x0A51
#define KEYCHRON_K5_MAX_JIS_RGB_PID                 0x0A52
#define KEYCHRON_K5_MAX_JIS_V2_RGB_PID              0x0A58
#define KEYCHRON_K6_HE_ANSI_PID                     0x0E60
#define KEYCHRON_K7_MAX_ANSI_RGB_PID                0x0A70
#define KEYCHRON_K7_MAX_ISO_RGB_PID                 0x0A71
#define KEYCHRON_K7_MAX_JIS_RGB_PID                 0x0A72
#define KEYCHRON_K7_MAX_JIS_V2_RGB_PID              0x0A76
#define KEYCHRON_K8_HE_ANSI_PID                     0x0E80
#define KEYCHRON_K8_HE_ISO_PID                      0x0E81
#define KEYCHRON_K8_HE_JIS_PID                      0x0E82
#define KEYCHRON_K8_MAX_ANSI_RGB_PID                0x0A80
#define KEYCHRON_K8_MAX_ISO_RGB_PID                 0x0A81
#define KEYCHRON_K8_MAX_JIS_RGB_PID                 0x0A82
#define KEYCHRON_K8_PRO_ANSI_RGB_PID                0x0280
#define KEYCHRON_K8_PRO_ISO_RGB_PID                 0x0281
#define KEYCHRON_K8_PRO_JIS_RGB_PID                 0x0282
#define KEYCHRON_K9_MAX_ANSI_RGB_PID                0x0A90
#define KEYCHRON_K10_HE_ANSI_PID                    0x0EA0
#define KEYCHRON_K10_HE_ISO_PID                     0x0EA1
#define KEYCHRON_K10_MAX_ANSI_RGB_PID               0x0AA0
#define KEYCHRON_K10_MAX_ISO_RGB_PID                0x0AA1
#define KEYCHRON_K10_MAX_JIS_RGB_PID                0x0AA2
#define KEYCHRON_K10_V2_ANSI_RGB_PID                0x0DA0
#define KEYCHRON_K10_V2_ISO_RGB_PID                 0x0DA1
#define KEYCHRON_K10_V2_JIS_RGB_PID                 0x0DA2
#define KEYCHRON_K11_MAX_ANSI_ENCODER_RGB_PID       0x0AB3
#define KEYCHRON_K11_MAX_ISO_ENCODER_RGB_PID        0x0AB4
#define KEYCHRON_K11_MAX_JIS_ENCODER_RGB_PID        0x0AB5
#define KEYCHRON_K13_MAX_ANSI_RGB_PID               0x0AD0
#define KEYCHRON_K13_MAX_ISO_RGB_PID                0x0AD1
#define KEYCHRON_K13_MAX_JIS_RGB_PID                0x0AD2
#define KEYCHRON_K15_MAX_ANSI_ENCODER_RGB_PID       0x0AF0
#define KEYCHRON_K15_MAX_ISO_ENCODER_RGB_PID        0x0AF1
#define KEYCHRON_K17_MAX_ANSI_ENCODER_RGB_PID       0x0A00
#define KEYCHRON_K17_MAX_ISO_ENCODER_RGB_PID        0x0A01
#define KEYCHRON_K17_MAX_JIS_ENCODER_RGB_PID        0x0A02
#define KEYCHRON_Q0_BASE_PID                        0x0130
#define KEYCHRON_Q0_PLUS_PID                        0x0131
#define KEYCHRON_Q0_MAX_ENCODER_PID                 0x0800
#define KEYCHRON_Q1_HE_ANSI_ENCODER_PID             0x0B10
#define KEYCHRON_Q1_HE_ISO_ENCODER_PID              0x0B11
#define KEYCHRON_Q1_HE_JIS_ENCODER_PID              0x0B12
#define KEYCHRON_Q1_MAX_ANSI_ENCODER_PID            0x0810
#define KEYCHRON_Q1_MAX_ISO_ENCODER_PID             0x0811
#define KEYCHRON_Q1_MAX_JIS_ENCODER_PID             0x0812
#define KEYCHRON_Q1_V1_ANSI_PID                     0x0100
#define KEYCHRON_Q1_V1_ANSI_ENCODER_PID             0x0101
#define KEYCHRON_Q1_V1_ISO_PID                      0x0102
#define KEYCHRON_Q1_V1_ISO_ENCODER_PID              0x0103
#define KEYCHRON_Q1_V2_ANSI_PID                     0x0106
#define KEYCHRON_Q1_V2_ANSI_ENCODER_PID             0x0107
#define KEYCHRON_Q1_V2_ISO_PID                      0x0108
#define KEYCHRON_Q1_V2_ISO_ENCODER_PID              0x0109
#define KEYCHRON_Q1_V2_JIS_PID                      0x010A
#define KEYCHRON_Q1_V2_JIS_ENCODER_PID              0x010B
#define KEYCHRON_Q2_ANSI_PID                        0x0110
#define KEYCHRON_Q2_ANSI_ENCODER_PID                0x0111
#define KEYCHRON_Q2_ISO_PID                         0x0112
#define KEYCHRON_Q2_ISO_ENCODER_PID                 0x0113
#define KEYCHRON_Q2_JIS_PID                         0x0114
#define KEYCHRON_Q2_JIS_ENCODER_PID                 0x0115
#define KEYCHRON_Q2_HE_ANSI_ENCODER_PID             0x0B20
#define KEYCHRON_Q2_MAX_ANSI_ENCODER_PID            0x0820
#define KEYCHRON_Q2_MAX_ISO_ENCODER_PID             0x0821
#define KEYCHRON_Q3_ANSI_PID                        0x0120
#define KEYCHRON_Q3_ANSI_ENCODER_PID                0x0121
#define KEYCHRON_Q3_ISO_PID                         0x0122
#define KEYCHRON_Q3_ISO_ENCODER_PID                 0x0123
#define KEYCHRON_Q3_JIS_PID                         0x0124
#define KEYCHRON_Q3_JIS_ENCODER_PID                 0x0125
#define KEYCHRON_Q3_HE_ANSI_ENCODER_PID             0x0B30
#define KEYCHRON_Q3_HE_ISO_ENCODER_PID              0x0B31
#define KEYCHRON_Q3_HE_JIS_ENCODER_PID              0x0B32
#define KEYCHRON_Q3_MAX_ANSI_ENCODER_PID            0x0830
#define KEYCHRON_Q3_MAX_ISO_ENCODER_PID             0x0831
#define KEYCHRON_Q4_ANSI_PID                        0x0140
#define KEYCHRON_Q4_ISO_PID                         0x0142
#define KEYCHRON_Q4_HE_ANSI_PID                     0x0B40
#define KEYCHRON_Q5_ANSI_PID                        0x0150
#define KEYCHRON_Q5_ANSI_ENCODER_PID                0x0151
#define KEYCHRON_Q5_ISO_PID                         0x0152
#define KEYCHRON_Q5_ISO_ENCODER_PID                 0x0153
#define KEYCHRON_Q5_HE_ANSI_ENCODER_PID             0x0B50
#define KEYCHRON_Q5_HE_ISO_ENCODER_PID              0x0B51
#define KEYCHRON_Q5_HE_JIS_ENCODER_PID              0x0B52
#define KEYCHRON_Q5_MAX_ANSI_ENCODER_PID            0x0850
#define KEYCHRON_Q5_MAX_ISO_ENCODER_PID             0x0851
#define KEYCHRON_Q5_MAX_JIS_ENCODER_PID             0x0852
#define KEYCHRON_Q6_ANSI_PID                        0x0160
#define KEYCHRON_Q6_ANSI_ENCODER_PID                0x0161
#define KEYCHRON_Q6_ISO_PID                         0x0162
#define KEYCHRON_Q6_ISO_ENCODER_PID                 0x0163
#define KEYCHRON_Q6_HE_ANSI_ENCODER_PID             0x0B60
#define KEYCHRON_Q6_HE_ISO_ENCODER_PID              0x0B61
#define KEYCHRON_Q6_HE_JIS_ENCODER_PID              0x0B62
#define KEYCHRON_Q6_MAX_ANSI_ENCODER_PID            0x0860
#define KEYCHRON_Q6_MAX_ISO_ENCODER_PID             0x0861
#define KEYCHRON_Q7_ANSI_PID                        0x0170
#define KEYCHRON_Q7_ISO_PID                         0x0172
#define KEYCHRON_Q8_ANSI_PID                        0x0180
#define KEYCHRON_Q8_ANSI_ENCODER_PID                0x0181
#define KEYCHRON_Q8_ISO_PID                         0x0182
#define KEYCHRON_Q8_ISO_ENCODER_PID                 0x0183
#define KEYCHRON_Q8_MAX_ANSI_ENCODER_PID            0x0880
#define KEYCHRON_Q9_ANSI_PID                        0x0190
#define KEYCHRON_Q9_ANSI_ENCODER_PID                0x0191
#define KEYCHRON_Q9_ISO_PID                         0x0192
#define KEYCHRON_Q9_ISO_ENCODER_PID                 0x0193
#define KEYCHRON_Q9_PLUS_ANSI_ENCODER_PID           0x0194
#define KEYCHRON_Q10_ANSI_ENCODER_PID               0x01A1
#define KEYCHRON_Q10_ISO_ENCODER_PID                0x01A3
#define KEYCHRON_Q10_MAX_ANSI_ENCODER_PID           0x08A0
#define KEYCHRON_Q10_MAX_ISO_ENCODER_PID            0x08A1
#define KEYCHRON_Q11_ANSI_ENCODER_PID               0x01E0
#define KEYCHRON_Q11_ISO_ENCODER_PID                0x01E1
#define KEYCHRON_Q12_ANSI_ENCODER_PID               0x01D1
#define KEYCHRON_Q12_ISO_ENCODER_PID                0x01D3
#define KEYCHRON_Q12_HE_ANSI_ENCODER_PID            0x0BC0
#define KEYCHRON_Q12_HE_ISO_ENCODER_PID             0x0BC1
#define KEYCHRON_Q12_MAX_ANSI_ENCODER_PID           0x08C3
#define KEYCHRON_Q12_MAX_ISO_ENCODER_PID            0x08C4
#define KEYCHRON_Q13_MAX_ANSI_ENCODER_PID           0x08D0
#define KEYCHRON_Q13_MAX_JIS_ENCODER_PID            0x08D2
#define KEYCHRON_Q14_MAX_ANSI_ENCODER_PID           0x08E0
#define KEYCHRON_Q15_MAX_ANSI_ENCODER_PID           0x08F0
#define KEYCHRON_Q60_MAX_ANSI_PID                   0x08C0
#define KEYCHRON_Q65_MAX_ANSI_ENCODER_PID           0x08B0

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

enum
{
    KC_FEATURE_DEFAULT_LAYER                        = ( 1 << 0 ),
    KC_FEATURE_BLUETOOTH                            = ( 1 << 1 ),
    KC_FEATURE_P24G                                 = ( 1 << 2 ),
    KC_FEATURE_ANALOG_MATRIX                        = ( 1 << 3 ),
    KC_FEATURE_STATE_NOTIFY                         = ( 1 << 4 ),
    KC_FEATURE_DYNAMIC_DEBOUNCE                     = ( 1 << 5 ),
    KC_FEATURE_SNAP_CLICK                           = ( 1 << 6 ),
    KC_FEATURE_KEYCHRON_RGB                         = ( 1 << 7 ),
    KC_FEATURE_QUICK_START                          = ( 1 << 8 ),
    KC_FEATURE_NKRO                                 = ( 1 << 9 ),
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

    std::string                             GetLocation();
    std::string                             GetName();
    std::string                             GetSerial();
    std::string                             GetVendor();
    std::string                             GetVersion();

    bool                                    GetSupported();

    unsigned short                          GetKeycode(unsigned short led_index);
    unsigned short                          GetLEDCount();
    qmk_rgb_matrix_led_info                 GetLEDInfo(unsigned short led_index);

    void                                    SendLEDs(unsigned short number_leds, RGBColor* color_data);
    void                                    SetMode(unsigned short mode, unsigned char speed, unsigned char hue, unsigned char sat, unsigned char val);

    void                                    SaveMode();

private:
    hid_device*                             dev;
    unsigned char                           kc_protocol_version;
    unsigned short                          kc_rgb_protocol_version;
    std::vector<unsigned short>             keycodes;
    std::vector<qmk_rgb_matrix_led_info>    led_info;
    std::string                             location;
    std::string                             name;
    unsigned short                          number_leds;
    std::string                             serial;
    unsigned short                          supported_features;
    std::string                             vendor;
    unsigned short                          via_protocol_version;

    unsigned short                          CmdGetKeycode(unsigned char layer, unsigned char row, unsigned char col);
    void                                    CmdGetKeychronProtocolVersion(unsigned char* kc_protocol_version);
    void                                    CmdGetKeychronRGBProtocolVersion(unsigned short* kc_rgb_protocol_version);
    std::vector<unsigned char>              CmdGetLEDIndexByRow(unsigned char row);
    void                                    CmdGetNumberLEDs(unsigned short* number_leds);
    void                                    CmdGetSupportFeature(unsigned short* supported_features);
    void                                    CmdGetViaProtocolVersion(unsigned short* via_protocol_version);
    void                                    CmdSaveMode();
    void                                    CmdSendLEDs(unsigned char start_index, unsigned char number_leds, RGBColor* color_data);
    void                                    CmdSetBrightness(unsigned char brightness);
    void                                    CmdSetColorHS(unsigned char h, unsigned char s);
    void                                    CmdSetPerKeyRGBType(unsigned char type);
    void                                    CmdSetRGBMatrixMode(unsigned char mode);
    void                                    CmdSetSpeed(unsigned char speed);

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
