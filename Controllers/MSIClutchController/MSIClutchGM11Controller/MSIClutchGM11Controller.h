/*---------------------------------------------------------*\
| MSIClutchGM11Controller.h                                 |
|                                                           |
|   Driver for MSI Clutch GM11 Gaming Mouse                 |
|                                                           |
|   Anatoly U. (TTolikoLL)                      20 SEP 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define MSI_CLUTCH_GM11_REPORT_SIZE             8
#define MSI_CLUTCH_GM11_LEDS_COUNT              1
#define MSI_CLUTCH_GM11_PALETTE_SIZE            33
#define MSI_CLUTCH_GM11_PACKET_DELAY            5

#define MSI_CLUTCH_GM11_CMD_HANDSHAKE           0x03
#define MSI_CLUTCH_GM11_CMD_LED_CONTROL         0x05
#define MSI_CLUTCH_GM11_CMD_PALETTE_PREP        0x10
#define MSI_CLUTCH_GM11_CMD_MODE_TRIGGER        0x85

#define MSI_CLUTCH_GM11_REG_BRIGHTNESS          0x00
#define MSI_CLUTCH_GM11_REG_PARAMS              0x08

#define MSI_CLUTCH_GM11_SLOT_CUSTOM             0x07
#define MSI_CLUTCH_GM11_COLOR_RANDOM            0x00
#define MSI_CLUTCH_GM11_PALETTE_TERMINATOR      0xD0

#define MSI_CLUTCH_GM11_DIR_UP                  0x00
#define MSI_CLUTCH_GM11_DIR_DOWN                0x01

enum
{
    MSI_CLUTCH_GM11_MODE_OFF                    = 0x00,
    MSI_CLUTCH_GM11_MODE_WAVE                   = 0x01,
    MSI_CLUTCH_GM11_MODE_STATIC                 = 0x02,
    MSI_CLUTCH_GM11_MODE_BREATHING              = 0x03,
    MSI_CLUTCH_GM11_MODE_RADAR                  = 0x04,
    MSI_CLUTCH_GM11_MODE_HORIZON                = 0x05,
    MSI_CLUTCH_GM11_MODE_RIPPLE                 = 0x06,
    MSI_CLUTCH_GM11_MODE_REACTIVE               = 0x07
};

enum
{
    MSI_CLUTCH_GM11_BRIGHTNESS_MIN              = 0,
    MSI_CLUTCH_GM11_BRIGHTNESS_MAX              = 255
};

enum
{
    MSI_CLUTCH_GM11_SPEED_MIN                   = 1,
    MSI_CLUTCH_GM11_SPEED_MAX                   = 3
};

class MSIClutchGM11Controller
{
public:
    MSIClutchGM11Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~MSIClutchGM11Controller();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            SetMode(RGBColor color,
                            unsigned char mode_value,
                            unsigned char brightness,
                            unsigned char speed,
                            unsigned char direction,
                            unsigned int color_mode);

protected:
    hid_device*     dev;

private:
    void            SendPacket(unsigned char* data);
    void            SendHandshake();
    void            SendModeTrigger(unsigned char mode_value);
    void            SendBrightness(unsigned char mode_value, unsigned char brightness);
    void            SendModeParameters(unsigned char mode_value, unsigned char param);
    void            SendCustomPalette(unsigned char red, unsigned char green, unsigned char blue);
    unsigned char   GetColourIndex(unsigned char red, unsigned char green, unsigned char blue);

    std::string     location;
    std::string     name;
};