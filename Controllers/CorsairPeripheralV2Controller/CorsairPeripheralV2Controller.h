/*---------------------------------------------------------*\
| CorsairPeripheralV2Controller.h                           |
|                                                           |
|   Driver for Corsair V2 peripherals                       |
|                                                           |
|   Chris M (Dr_No)                             07 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "LogManager.h"
#include "RGBController.h"
#include "CorsairPeripheralV2Devices.h"

#define NA                              0xFFFFFFFF
#define HID_MAX_STR                     255

#define CORSAIR_V2_TIMEOUT              50
#define CORSAIR_V2_TIMEOUT_SHORT        3
#define CORSAIR_V2_VALUE_MODE           3
#define CORSAIR_V2_WRITE_WIRED_ID       8
#define CORSAIR_V2_WRITE_WIRELESS_ID    9
#define CORSAIR_V2_WRITE_SIZE           65
#define CORSAIR_V2_PACKET_SIZE          1024

#define CORSAIR_V2_LIGHT_CTRL1          1
#define CORSAIR_V2_LIGHT_CTRL2          34          /* 0x22                             */
#define CORSAIR_V2_UPDATE_PERIOD        30000
#define CORSAIR_V2_SLEEP_PERIOD         12500ms

#define CORSAIR_V2_BRIGHTNESS_MIN       0
#define CORSAIR_V2_BRIGHTNESS_MAX       0xFF

enum corsair_v2_cmd
{
    CORSAIR_V2_CMD_SET                  = 0x01,     /* Command for setting values       */
    CORSAIR_V2_CMD_GET                  = 0x02,     /* Command for getting values       */
    CORSAIR_V2_CMD_STOP_TX              = 0x05,     /* Finish Transaction               */
    CORSAIR_V2_CMD_BLK_W1               = 0x06,     /* Block write packet 1             */
    CORSAIR_V2_CMD_BLK_WN               = 0x07,     /* Block write remaining packets    */
    CORSAIR_V2_CMD_START_TX             = 0x0D,     /* Start Transaction                */
};

enum corsair_v2_mode
{
    CORSAIR_V2_MODE_DIRECT              = 0x0012,
    CORSAIR_V2_MODE_STATIC              = 0x207E,
    CORSAIR_V2_MODE_FLASHING            = 0xAD4F,
    CORSAIR_V2_MODE_BREATHING           = 0xA5FA,
    CORSAIR_V2_MODE_SPECTRUM            = 0x7BFF,
    CORSAIR_V2_MODE_RAINBOW             = 0xB94C,
    CORSAIR_V2_MODE_RAIN                = 0xA07E,
    CORSAIR_V2_MODE_SPIRAL              = 0xAB87,
    CORSAIR_V2_MODE_WATERCOLOR          = 0x0022,
    CORSAIR_V2_MODE_REACTIVE            = 0xB1F9,
    CORSAIR_V2_MODE_RIPPLE              = 0x09A2,
    CORSAIR_V2_MODE_VISOR               = 0x90C0
};

enum corsair_v2_color
{
    CORSAIR_V2_COLOR_NONE               = 0x00,
    CORSAIR_V2_COLOR_SPECIFIC           = 0x01,
    CORSAIR_V2_COLOR_RANDOM             = 0x02,
    CORSAIR_V2_COLOR_UNKNOWN            = 0x03
};


class CorsairPeripheralV2Controller
{
public:
    CorsairPeripheralV2Controller(hid_device* dev_handle, const char* path, std::string name);
    virtual ~CorsairPeripheralV2Controller();

    std::string                     GetDeviceLocation();
    std::string                     GetErrorString(uint8_t err);
    std::string                     GetFirmwareString();
    std::string                     GetName();
    std::string                     GetSerialString();
    const corsair_v2_device*        GetDeviceData();
    unsigned int                    GetKeyboardLayout();

    void                            SetRenderMode(corsair_v2_device_mode mode);
    void                            LightingControl(uint8_t opt1);
    void                            SetLEDs(uint8_t *data, uint16_t data_size);
    void                            UpdateHWMode(uint16_t mode, corsair_v2_color color_mode, uint8_t speed,
                                                 uint8_t direction, uint8_t brightness, std::vector<RGBColor> colors);

    virtual void                    SetLedsDirect(std::vector<RGBColor *> colors)                   = 0;

protected:
    uint16_t                        device_index;
    std::string                     device_name;
    uint8_t                         light_ctrl          = CORSAIR_V2_LIGHT_CTRL2;

private:
    void                            ClearPacketBuffer();
    unsigned int                    GetAddress(uint8_t address);
    unsigned char                   StartTransaction(uint8_t opt1);
    void                            StopTransaction(uint8_t opt1);

    hid_device*                     dev;

    uint8_t                         write_cmd           = CORSAIR_V2_WRITE_WIRED_ID;
    uint16_t                        pkt_sze             = CORSAIR_V2_WRITE_SIZE;
    std::string                     firmware_version;
    std::string                     location;
};
