/*---------------------------------------------------------*\
| CMR6000Controller.h                                       |
|                                                           |
|   Driver for Cooler Master AMD Radeon 6000 series GPU     |
|                                                           |
|   Eric S (edbgon)                             02 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <array>
#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define COOLERMASTER_RADEON_6000_PID            0x014D
#define COOLERMASTER_RADEON_6900_PID            0x015B

#define CM_6K_PACKET_SIZE           65  //Includes extra first byte for non HID Report packets
#define CM_6K_INTERRUPT_TIMEOUT     250
#define CM_6K_DEVICE_NAME_SIZE      (sizeof(device_name) / sizeof(device_name[ 0 ]))
#define CM_6K_SERIAL_SIZE           (sizeof(serial) / sizeof(serial[ 0 ]))
#define HID_MAX_STR                 255

enum
{
    CM_MR6000_MODE_DIRECT           = 0x00, //Direct Mode
    CM_MR6000_MODE_BREATHE          = 0x01, //Breathe Mode
    CM_MR6000_MODE_COLOR_CYCLE      = 0x02, //Color cycle

    CM_MR6000_MODE_RAINBOW          = 0x07, //Rainbow
    CM_MR6000_MODE_BOUNCE           = 0x08, //Bounce
    CM_MR6000_MODE_CHASE            = 0x09, //Chase
    CM_MR6000_MODE_SWIRL            = 0x0A, //Swirl

    CM_MR6000_MODE_OFF              = 0xFF, //Off
};

enum
{
    MR6000_CYCLE_SPEED_SLOWEST              = 0x96, /* Slowest speed                */
    MR6000_CYCLE_SPEED_SLOW                 = 0x8C, /* Slow speed                   */
    MR6000_CYCLE_SPEED_NORMAL               = 0x80, /* Normal speed                 */
    MR6000_CYCLE_SPEED_FAST                 = 0x6E, /* Fast speed                   */
    MR6000_CYCLE_SPEED_FASTEST              = 0x68, /* Fastest speed                */

    MR6000_RAINBOW_SPEED_SLOWEST            = 0x78, /* Slowest speed                */
    MR6000_RAINBOW_SPEED_NORMAL             = 0x6B, /* Normal speed                */
    MR6000_RAINBOW_SPEED_FASTEST            = 0x60, /* Fastest speed                */

    MR6000_BREATHE_SPEED_SLOWEST            = 0x3C, /* Slowest speed                */
    MR6000_BREATHE_SPEED_SLOW               = 0x37, /* Slow speed                   */
    MR6000_BREATHE_SPEED_NORMAL             = 0x31, /* Normal speed                 */
    MR6000_BREATHE_SPEED_FAST               = 0x2C, /* Fast speed                   */
    MR6000_BREATHE_SPEED_FASTEST            = 0x26, /* Fastest speed                */
};

class CMR6000Controller
{
public:
    CMR6000Controller(hid_device* dev_handle, char *_path, uint16_t _pid);
    ~CMR6000Controller();

    std::string     GetDeviceName();
    std::string     GetSerial();
    std::string     GetLocation();

    unsigned char   GetMode();
    unsigned char   GetLedSpeed();
    unsigned char   GetBrightness();
    bool            GetRandomColours();
    uint16_t        GetPID();

    void            SetMode(unsigned char mode, unsigned char speed, RGBColor color1, RGBColor color2, unsigned char random, unsigned char brightness);

private:
    std::string     device_name;
    std::string     location;
    hid_device*     dev;
    uint16_t        pid;

    unsigned char   current_mode;
    unsigned char   current_speed;
    unsigned char   current_random;

    unsigned char   current_brightness;
    RGBColor        primary;
    RGBColor        secondary;

    void            SendUpdate();
    void            SendEnableCommand();
    void            SendApplyCommand();
    void            SendColourConfig();
    void            SendSecondColour();
};
