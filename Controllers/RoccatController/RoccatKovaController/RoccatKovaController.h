/*---------------------------------------------------------*\
| RoccatKovaController.h                                    |
|                                                           |
|   Driver for Roccat Kova                                  |
|                                                           |
|   Gustash                                     01 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

#define ROCCAT_KOVA_HID_MAX_STR               255
#define ROCCAT_KOVA_LED_COUNT                 2
#define ROCCAT_KOVA_SPEED_MIN                 1
#define ROCCAT_KOVA_SPEED_MAX                 3
#define ROCCAT_KOVA_INIT_REPORT_ID            4
#define ROCCAT_KOVA_INIT_WRITE_PACKET_SIZE    3
#define ROCCAT_KOVA_PROFILE_REPORT_ID         6
#define ROCCAT_KOVA_PROFILE_WRITE_PACKET_SIZE 28
#define ROCCAT_KOVA_VERSION_REPORT_ID         9
#define ROCCAT_KOVA_VERSION_READ_PACKET_SIZE  8
/*#define NUM_OF_DPI_SWITCHES                 5*/

enum
{
    ROCCAT_KOVA_FIRMWARE_VERSION_IDX          = 2,
    /*ROCCAT_KOVA_SELECTED_PROFILE_IDX        = 2,*/
    /*ROCCAT_KOVA_UNKNOWN_3_IDX               = 3,*/
    /*ROCCAT_KOVA_UNKNOWN_4_IDX               = 4,*/
    /*ROCCAT_KOVA_ORIENTATION_IDX             = 5,*/
    /*ROCCAT_KOVA_DPI_SWITCHER_IDX            = 6,*/
    /*ROCCAT_KOVA_DPI_SPEED_IDX               = 7,*/
    /*ROCCAT_KOVA_SELECTED_DPI_IDX            = 12,*/
    /*ROCCAT_KOVA_POLLING_RATE_IDX            = 13,*/
    ROCCAT_KOVA_FLAGS_IDX                     = 14,
    ROCCAT_KOVA_COLOR_FLOW_IDX                = 15,
    ROCCAT_KOVA_MODE_IDX                      = 16,
    ROCCAT_KOVA_EFFECT_SPEED_IDX              = 17,
    ROCCAT_KOVA_PIPE_IDX                      = 18,
    ROCCAT_KOVA_WHEEL_IDX                     = 22,
    ROCCAT_KOVA_CHECKSUM_IDX                  = 26,
};

#define ROCCAT_KOVA_R_OFFSET                  1
#define ROCCAT_KOVA_G_OFFSET                  2
#define ROCCAT_KOVA_B_OFFSET                  3

#define ROCCAT_KOVA_USE_CUSTOM_COLORS_MASK    0b00110000
#define ROCCAT_KOVA_LIGHTS_ON_MASK            0b00000011

enum
{
    ROCCAT_KOVA_MODE_OFF                      = 0x00,
    ROCCAT_KOVA_MODE_STATIC                   = 0x01,
    ROCCAT_KOVA_MODE_FLASHING                 = 0x02,
    ROCCAT_KOVA_MODE_BREATHING                = 0x03,
    ROCCAT_KOVA_MODE_COLOR_FLOW               = 0xFF,
};

class RoccatKovaController
{
public:
    RoccatKovaController(hid_device* dev_handle, char *path, std::string dev_name);
    ~RoccatKovaController();

    std::string GetLocation();
    std::string GetName();
    std::string GetSerial();
    std::string GetVersion();

    void SetColor(RGBColor color_wheel,
                  RGBColor color_stripe,
                  uint8_t mode,
                  uint8_t speed,
                  bool color_flow);

private:
    hid_device* dev;
    std::string location;
    std::string name;
    std::string version;

    void SendInitialPacket();
    void FetchProfileData(uint8_t *buf);
    void FetchFirmwareVersion();

    uint16_t GenerateChecksum(uint8_t *buf, size_t length);
};
