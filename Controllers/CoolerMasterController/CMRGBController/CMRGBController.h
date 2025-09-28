/*---------------------------------------------------------*\
| CMRGBController.h                                         |
|                                                           |
|   Driver for Cooler Master RGB controller                 |
|                                                           |
|   Nic W (midgetspy)                           13 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define CM_RGBC_NUM_LEDS 4

#define REPORT_ID_OFFSET                        1
#define CM_RGBC_PACKET_SIZE                     64 + REPORT_ID_OFFSET //This needs to have one byte extra for the report ID thing
#define CM_RGBC_PACKET_OFFSET_OP                0x00
#define CM_RGBC_PACKET_OFFSET_TYPE              0x01
#define CM_RGBC_PACKET_OFFSET_MULTILAYER        0x02
#define CM_RGBC_PACKET_OFFSET_MODE              0x04
#define CM_RGBC_PACKET_OFFSET_SPEED             0x05
#define CM_RGBC_PACKET_OFFSET_BRIGHTNESS        0x09
#define CM_RGBC_PACKET_OFFSET_COLOR_1           0x0A
#define CM_RGBC_PACKET_OFFSET_COLOR_2           0x0D

#define CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_1  0x04
#define CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_2  0x07
#define CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_3  0x0A
#define CM_RGBC_PACKET_OFFSET_MULTIPLE_COLOR_4  0x0D

#define CM_RGBC_INTERRUPT_TIMEOUT               250

#define CM_RGBC_SPEED_NONE                      0x05
#define CM_RGBC_BRIGHTNESS_OFF                  0x03
#define HID_MAX_STR                             255

/*-------------------------------------------------*\
| OP OPCODES                                        |
\*-------------------------------------------------*/
enum
{
    CM_RGBC_OPCODE_OP_FLOW_CONTROL          = 0x41,
    CM_RGBC_OPCODE_OP_UNKNOWN_50            = 0x50,
    CM_RGBC_OPCODE_OP_WRITE                 = 0x51,
    CM_RGBC_OPCODE_OP_READ                  = 0x52,
};

/*-------------------------------------------------*\
| CONTROL FLOW OPCODES                              |
\*-------------------------------------------------*/
enum
{
    CM_RGBC_OPCODE_FLOW_00                  = 0x00,
    CM_RGBC_OPCODE_FLOW_01                  = 0x01,
    CM_RGBC_OPCODE_FLOW_80                  = 0x80,
};

/*-------------------------------------------------*\
| OP TYPE OPCODES                                   |
\*-------------------------------------------------*/
enum
{
    CM_RGBC_OPCODE_TYPE_MODE                = 0x28,
    CM_RGBC_OPCODE_TYPE_CONFIG_SIMPLIFIED   = 0x2B,
    CM_RGBC_OPCODE_TYPE_CONFIG_FULL         = 0x2C,
    CM_RGBC_OPCODE_TYPE_UNKNOWN_30          = 0x30,
    CM_RGBC_OPCODE_TYPE_UNKNOWN_55          = 0x55,
    CM_RGBC_OPCODE_TYPE_LED_INFO            = 0xA8,
};

/*-------------------------------------------------*\
| MODES                                             |
\*-------------------------------------------------*/
enum
{
    CM_RGBC_MODE_STATIC                     = 0x00,
    CM_RGBC_MODE_BREATHING                  = 0x01,
    CM_RGBC_MODE_COLOR_CYCLE                = 0x02,
    CM_RGBC_MODE_STAR                       = 0x03,
    CM_RGBC_MODE_MULTIPLE                   = 0x04,
    CM_RGBC_MODE_MULTILAYER                 = 0xE0,
    CM_RGBC_MODE_OFF                        = 0xFE,
};

/*-------------------------------------------------*\
| SPEED                                             |
\*-------------------------------------------------*/
enum
{
    CM_RGBC_SPEED_BREATHING_SLOWEST         = 0x3C,
    CM_RGBC_SPEED_BREATHING_FASTEST         = 0x26,
    CM_RGBC_SPEED_COLOR_CYCLE_SLOWEST       = 0x96,
    CM_RGBC_SPEED_COLOR_CYCLE_FASTEST       = 0x68,
    CM_RGBC_SPEED_STAR_SLOWEST              = 0x46,
    CM_RGBC_SPEED_STAR_FASTEST              = 0x32,
};

class CMRGBController
{
public:
    CMRGBController(hid_device* dev_handle, char* path);
    ~CMRGBController();

    std::string         GetDeviceName();
    std::string         GetSerial();
    std::string         GetLocation();

    unsigned char       GetMode();
    unsigned char       GetSpeed();
    unsigned char       GetBrightness();
    RGBColor            GetModeColor(int color_number);
    RGBColor            GetPortColor(int port_number);

    void                SetMode(unsigned char mode, unsigned char speed, unsigned char brightness, RGBColor color_1, RGBColor color_2);
    void                SetLedsDirect(RGBColor color_1, RGBColor color_2, RGBColor color_3, RGBColor color_4);

    void                ReadCurrentMode();
    void                ReadModeConfig(unsigned char mode);

private:
    std::string         device_name;
    std::string         location;
    hid_device*         dev;

    unsigned char       current_mode;
    unsigned char       current_speed;
    unsigned char       current_brightness;
    RGBColor            current_mode_color_1;
    RGBColor            current_mode_color_2;
    RGBColor            current_port1_color;
    RGBColor            current_port2_color;
    RGBColor            current_port3_color;
    RGBColor            current_port4_color;

    void                SendFlowControl(unsigned char byte_flag);
    void                SendApply();
    void                SendCustomColorStart();

    void                SendReadMode();
    void                SendSetMode(unsigned char mode);

    void                SendReadCustomColors();
    void                SendSetCustomColors(RGBColor color_1, RGBColor color_2, RGBColor color_3, RGBColor color_4);

    void                SendReadConfig(unsigned char mode);
    void                SendSetConfig(unsigned char mode, unsigned char speed, unsigned char brightness, RGBColor color_1, RGBColor color_2, bool simplified, bool multilayer);
};
