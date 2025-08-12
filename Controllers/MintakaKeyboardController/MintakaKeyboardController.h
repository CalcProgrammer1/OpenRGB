/*---------------------------------------------------------*\
| MintakaKeyboardController.h                               |
|                                                           |
|   Driver for VSG Mintaka Devices keyboard lighting        |
|   Based on KeychronKeyboardController                     |
|                                                           |
|   Federico Scodelaro (pudymody)               08 Oct 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/
#pragma once

#include "RGBController.h"
#include <string>
#include <hidapi.h>

#define REPORT_ID                   0x00
#define PACKET_DATA_LENGTH          64
#define COLOR_BUF_SIZE              576
#define EFFECT_PAGE_LENGTH          16
#define LED_SPECIAL_EFFECT_PACKETS  0x12
#define PACKET_HEADER               0x04
#define EFFECT_PAGE_CHECK_CODE_L    0xAA
#define EFFECT_PAGE_CHECK_CODE_H    0x55

/*-----------------------------------------*\
|  Commands                                 |
\*-----------------------------------------*/
enum
{
    COMMUNICATION_END_COMMAND                   = 0x02,
    GET_BASIC_INFO_COMMAND                      = 0x05,
    READ_KEY_DEFINITION_AREA_COMMAND            = 0x10,
    WRITE_KEY_DEFINITION_AREA_COMMAND           = 0x11,
    READ_LED_EFFECT_DEFINITION_AREA_COMMAND     = 0x12,
    WRITE_LED_SPECIAL_EFFECT_AREA_COMMAND       = 0x13,
    READ_MACRO_DEFINITION_AREA_COMMAND          = 0x14,
    WRITE_MACRO_DEFINITION_AREA_COMMAND         = 0x15,
    READ_GAME_MODE_AREA_COMMAND                 = 0x16,
    WRITE_GAME_MODE_AREA_COMMAND                = 0x17,
    TURN_ON_CUSTOMIZATION_COMMAND               = 0x18,
    TURN_OFF_CUSTOMIZATION_COMMAND              = 0x19,
    LED_EFFECT_START_COMMAND                    = 0xF0,
    LED_SYNC_INITIAL_COMMAND                    = 0xF1,
    LED_SYNC_START_COMMAND                      = 0xF2,
    LED_SYNC_STOP_COMMAND                       = 0xF3,
    RANDOM_PACKET_START_COMMAND                 = 0xAB,
};

/*-----------------------------------------*\
|  Modes                                    |
\*-----------------------------------------*/
enum
{
    CUSTOM_MODE_VALUE                           = 0x00,
    STATIC_MODE_VALUE                           = 0x01,
    KEYSTROKE_LIGHT_UP_MODE_VALUE               = 0x02,
    KEYSTROKE_DIM_MODE_VALUE                    = 0x03,
    SPARKLE_MODE_VALUE                          = 0x04,
    RAIN_MODE_VALUE                             = 0x05,
    RANDOM_COLORS_MODE_VALUE                    = 0x06,
    BREATHING_MODE_VALUE                        = 0x07,
    SPECTRUM_CYCLE_MODE_VALUE                   = 0x08,
    RING_GRADIENT_MODE_VALUE                    = 0x09,
    VERTICAL_GRADIENT_MODE_VALUE                = 0x0A,
    HORIZONTAL_GRADIENT_WAVE_MODE_VALUE         = 0x0B,
    AROUND_EDGES_MODE_VALUE                     = 0x0C,
    KEYSTROKE_HORIZONTAL_LINES_VALUE            = 0x0D,
    KEYSTROKE_TITLED_LINES_MODE_VALUE           = 0x0E,
    KEYSTROKE_RIPPLES_MODE_VALUE                = 0x0F,
    SEQUENCE_MODE_VALUE                         = 0x10,
    WAVE_LINE_MODE_VALUE                        = 0x11,
    TILTED_LINES_MODE_VALUE                     = 0x12,
    BACK_AND_FORTH_MODE_VALUE                   = 0x13,
    LIGHTS_OFF_MODE_VALUE                       = 0x80,
};

/*-----------------------------------------*\
|  Other settings                           |
\*-----------------------------------------*/
enum
{
    MINTAKA_MIN_SPEED      = 0x00,
    MINTAKA_MAX_SPEED      = 0x0F,
    MINTAKA_MIN_BRIGHTNESS = 0x00,
    MINTAKA_MAX_BRIGHTNESS = 0x0F,
};


class MintakaKeyboardController
{
public:
    MintakaKeyboardController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~MintakaKeyboardController();

    std::string                 GetDeviceLocation();
    std::string                 GetNameString();
    std::string                 GetSerialString();

    void                        SetLedSequencePositions(std::vector<unsigned int> positions);
    void                        SetMode(std::vector<mode> modes, int active_mode, std::vector<RGBColor> colors);

protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 name;
    std::string                 serial_number;
    std::string                 version;
    std::vector<unsigned int>   led_sequence_positions;

    void                        SetCustomization(bool state);
    void                        StartEffectPage();
    void                        StartEffectCommand();
    void                        EndCommunication();

    void                        Read();
    void                        Send(unsigned char data[PACKET_DATA_LENGTH]);
};
