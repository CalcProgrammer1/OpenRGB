/*---------------------------------------------------------*\
| CMMP750Controller.h                                       |
|                                                           |
|   Driver for Cooler Master MP750 mousemat                 |
|                                                           |
|   Chris M (Dr_No)                             16 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <array>
#include <string>
#include <hidapi.h>

#define CM_COLOUR_MODE_DATA_SIZE (sizeof(colour_mode_data[0]) / sizeof(colour_mode_data[0][0]))
#define CM_INTERRUPT_TIMEOUT 250
#define CM_DEVICE_NAME_SIZE (sizeof(device_name) / sizeof(device_name[ 0 ]))
#define CM_SERIAL_SIZE (sizeof(serial) / sizeof(serial[ 0 ]))
#define HID_MAX_STR 255

/*-------------------------------------------------------------------*\
|  Simple RGB device with 5 modes                                     |
|  BYTE0 = Mode (0x01 thru 0x05                                       |
|  BYTE1 = ?? Must be set to 0x04 for colour modes otherwise ignored  |
|  BYTE2 = Colour Modes: RED    else Cycle SPEED                      |
|  BYTE3 = Colour Modes: GREEN  else ignored                          |
|  BYTE4 = Colour Modes: BLUE   else ignored                          |
|  BYTE5 = Colour Modes: SPEED  else ignored                          |
\*-------------------------------------------------------------------*/

enum
{
    CM_MODE_BYTE   = 1,
    CM_LENGTH_BYTE = 2,
    CM_RED_BYTE    = 3,
    CM_GREEN_BYTE  = 4,
    CM_BLUE_BYTE   = 5,
    CM_SPEED_BYTE  = 6
};

enum
{
    CM_MP750_MODE_OFF           = 0x00, //Off
    CM_MP750_MODE_STATIC        = 0x01, //Static Mode
    CM_MP750_MODE_BLINK         = 0x02, //Blinking Mode
    CM_MP750_MODE_BREATHING     = 0x03, //Breathing Mode
    CM_MP750_MODE_COLOR_CYCLE   = 0x04, //Color Cycle Mode
    CM_MP750_MODE_BREATH_CYCLE  = 0x05  //Breathing Cycle Mode
};

enum
{
    MP750_SPEED_SLOWEST              = 0x00, /* Slowest speed                */
    MP750_SPEED_SLOWER               = 0x01, /* Slower speed                 */
    MP750_SPEED_SLOW                 = 0x02, /* Slow speed                   */
    MP750_SPEED_SLOWISH              = 0x03, /* Slowish speed                */
    MP750_SPEED_NORMAL               = 0x04, /* Normal speed                 */
    MP750_SPEED_FASTISH              = 0x05, /* Fastish speed                */
    MP750_SPEED_FAST                 = 0x06, /* Fast speed                   */
    MP750_SPEED_FASTER               = 0x07, /* Faster speed                 */
    MP750_SPEED_FASTEST              = 0x08, /* Fastest speed                */
};

class CMMP750Controller
{
public:
    CMMP750Controller(hid_device* dev_handle, char *_path);
    ~CMMP750Controller();

    std::string GetDeviceName();
    std::string GetSerial();
    std::string GetLocation();

    unsigned char GetMode();
    unsigned char GetLedRed();
    unsigned char GetLedGreen();
    unsigned char GetLedBlue();
    unsigned char GetLedSpeed();
    void SetMode(unsigned char mode, unsigned char speed);
    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    std::string             device_name;
    std::string             location;
    hid_device*             dev;

    unsigned char           current_mode;
    unsigned char           current_speed;

    unsigned char           current_red;
    unsigned char           current_green;
    unsigned char           current_blue;

    void GetStatus();
    void SendUpdate();
};
