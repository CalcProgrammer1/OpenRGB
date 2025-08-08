/*---------------------------------------------------------*\
| EVGAKeyboardController.h                                  |
|                                                           |
|   Driver for EVGA keyboard                                |
|                                                           |
|   Chris M (Dr_No)                             25 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "LogManager.h"
#include "RGBController.h"

#define NA                                          0xFFFFFFFF
#define HID_MAX_STR                                 255

#define EVGA_KEYBOARD_CONTROLLER_ID_3_SIZE          1597
#define EVGA_KEYBOARD_CONTROLLER_ID_4_SIZE          17
#define EVGA_KEYBOARD_CONTROLLER_ID_6_SIZE          792
#define EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE          136
#define EVGA_KEYBOARD_CONTROLLER_INTERRUPT_TIMEOUT  250

#define EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MIN     0
#define EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX     255
#define EVGA_KEYBOARD_FULL_SIZE_KEYCOUNT            108
#define EVGA_KEYBOARD_Z20_EXTRA_KEYS                24
#define EVGA_KEYBOARD_Z20_EXTRA_ZONES               3

static const uint8_t direction_map[8][4] =
{
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 1, 0, 0, 0 }, //Spiral - Left = Anti Clockwise, Right = Clockwise
    { 3, 2, 0, 1 }, //Rainbow
    { 0, 0, 0, 0 },
    { 0, 1, 2, 0 }, //Trigger - Right = Typing, Left = Single, Up = 3x3
};

enum EVGA_Keyboard_Controller_Modes
{
    EVGA_KEYBOARD_CONTROLLER_MODE_OFF           = 0x00, //Turn off - All leds off
    EVGA_KEYBOARD_CONTROLLER_MODE_STATIC        = 0x01, //Static Mode - Set entire zone to a single color.
    EVGA_KEYBOARD_CONTROLLER_MODE_BREATHING     = 0x02, //Breathing Mode - Fades between fully off and fully on.
    EVGA_KEYBOARD_CONTROLLER_MODE_PULSE         = 0x03, //Flashing Mode - Abruptly changing between fully off and fully on.
    EVGA_KEYBOARD_CONTROLLER_MODE_SPIRAL        = 0x04, //Spiral Mode - All keys light in a colourful spiral
    EVGA_KEYBOARD_CONTROLLER_MODE_RAINBOW       = 0x05, //Rainbow Wave Mode - Cycle thru the color spectrum as a wave across all LEDs
    EVGA_KEYBOARD_CONTROLLER_MODE_STAR          = 0x06, //Starry Night effect
    EVGA_KEYBOARD_CONTROLLER_MODE_TRIGGER       = 0x07, //Key reactive
    EVGA_KEYBOARD_CONTROLLER_MODE_DIRECT        = 0xFF, //Direct Led Control - Independently set LEDs in zone
};

enum EVGA_Keyboard_Controller_Zones
{
    EVGA_KEYBOARD_CONTROLLER_ZONE_NUMPAD_KEYS   = (1 << 0),
    EVGA_KEYBOARD_CONTROLLER_ZONE_FUNCTION_KEYS = (1 << 1),
    EVGA_KEYBOARD_CONTROLLER_ZONE_NUMBER_KEYS   = (1 << 2),
    EVGA_KEYBOARD_CONTROLLER_ZONE_ARROW_KEYS    = (1 << 3),
    EVGA_KEYBOARD_CONTROLLER_ZONE_WASD_KEYS     = (1 << 4),
    EVGA_KEYBOARD_CONTROLLER_ZONE_ALL_KEYS      = (1 << 7),
};

enum EVGA_Keyboard_Controller_Byte_Map
{
    EVGA_KB_REPORT_BYTE         = 0,
    EVGA_KB_COMMAND_BYTE        = 1,
    EVGA_KB_FUNCTION_BYTE       = 2,
    EVGA_KB_MODE_BYTE           = 3,
    EVGA_KB_CRC_BYTE            = 7,
    EVGA_KB_ZONE_BYTE           = 8,
    EVGA_KB_DIR_BYTE            = 9,
    EVGA_KB_SPEED_LSB           = 24,
    EVGA_KB_SPEED_MSB           = 25,
    EVGA_KB_COLORS_SZ           = 26,
};

enum EVGA_Keyboard_Controller_Speed
{
    EVGA_KEYBOARD_CONTROLLER_SPEED_SLOWEST       = 0x64, // Slowest speed
    EVGA_KEYBOARD_CONTROLLER_SPEED_SLOWISH       = 0x3E, // Slowish speed
    EVGA_KEYBOARD_CONTROLLER_SPEED_NORMAL        = 0x32, // Normal speed
    EVGA_KEYBOARD_CONTROLLER_SPEED_FASTEST       = 0x05, // Fastest speed
};

class EVGAKeyboardController
{
public:
    EVGAKeyboardController(hid_device* dev_handle, const char* path, uint16_t kb_pid, std::string dev_name);
    ~EVGAKeyboardController();

    std::string     GetName();
    std::string     GetSerial();
    std::string     GetLocation();

    void            SaveMode();
    void            SetHWModes();
    void            SetLedsDirect(std::vector<RGBColor> colors);
    void            SetMode(uint8_t mode, uint16_t speed, uint8_t brightness,
                            uint8_t direction, std::vector<RGBColor> colors);
    void            SetSleepTime();
    void            GetStatus(mode *mode);
    uint8_t         GetMode();
    uint16_t        GetPid();
private:
    std::string     name;
    std::string     location;
    hid_device*     dev;
    uint16_t        pid;

    void            NFIPacket();
    void            FillColours(uint8_t * buffer, uint8_t brightness, std::vector<RGBColor> colors);
    void            SendMode(uint8_t mode, uint8_t direction);
    void            SendColour(uint8_t mode, uint16_t speed, uint8_t brightness, uint8_t direction, std::vector<RGBColor> colors);
    uint8_t         GetChecksum(uint8_t * data, size_t count);

    uint8_t         FindDirection(uint8_t mode, uint8_t direction);
    uint8_t         FindColours(uint8_t * data, uint8_t count, std::vector<RGBColor> &colors);
};
