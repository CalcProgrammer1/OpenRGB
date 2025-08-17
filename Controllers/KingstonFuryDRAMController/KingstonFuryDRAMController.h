/*---------------------------------------------------------*\
|  KingstonFuryDRAMController.h                             |
|                                                           |
|   Driver for Kingston Fury DDR4/5 RAM modules             |
|                                                           |
|   Geofrey Mon (geofbot)                       14 Jul 2024 |
|   Milan Cermak (krysmanta)                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <map>
#include <vector>
#include "i2c_smbus.h"
#include "RGBController.h"

#define FURY_CONTROLLER_NAME "Kingston Fury DDR4/5 DRAM"
#define FURY_BASE_ADDR_DDR4 0x58
#define FURY_BASE_ADDR_DDR5 0x60
#define FURY_DELAY std::chrono::milliseconds(10)
#define FURY_LEDS_PER_DIMM_DDR4 10
#define FURY_LEDS_PER_DIMM_DDR5 12
#define FURY_MAX_MODE_COLORS 10
#define FURY_DEFAULT_BG_COLOR ToRGBColor(16,16,16)
#define FURY_ALT_DIRECTIONS {\
        FURY_DIR_BOTTOM_TO_TOP,\
        FURY_DIR_TOP_TO_BOTTOM,\
        FURY_DIR_BOTTOM_TO_TOP,\
        FURY_DIR_TOP_TO_BOTTOM,\
        FURY_DIR_BOTTOM_TO_TOP,\
        FURY_DIR_TOP_TO_BOTTOM,\
        FURY_DIR_BOTTOM_TO_TOP,\
        FURY_DIR_TOP_TO_BOTTOM}

enum
{
    FURY_MODEL_BEAST_DDR5             = 0x10,
    FURY_MODEL_RENEGADE_DDR5          = 0x11,
    FURY_MODEL_BEAST_RGB_WHITE_DDR5   = 0x12,
    FURY_MODEL_BEAST2_DDR5            = 0x15,
    FURY_MODEL_BEAST_WHITE_DDR4       = 0x21,
    FURY_MODEL_BEAST_DDR4             = 0x23,
};

enum
{
    FURY_REG_MODEL                    = 0x06,
    FURY_REG_APPLY                    = 0x08,
    FURY_REG_MODE                     = 0x09,
    FURY_REG_INDEX                    = 0x0B,
    FURY_REG_DIRECTION                = 0x0C,
    FURY_REG_DELAY                    = 0x0D,
    FURY_REG_SPEED                    = 0x0E,
    FURY_REG_DYNAMIC_HOLD_A           = 0x12,
    FURY_REG_DYNAMIC_HOLD_B           = 0x13,
    FURY_REG_DYNAMIC_FADE_A           = 0x14,
    FURY_REG_DYNAMIC_FADE_B           = 0x15,
    FURY_REG_BREATH_MIN_TO_MID        = 0x16,
    FURY_REG_BREATH_MID_TO_MAX        = 0x17,
    FURY_REG_BREATH_MAX_TO_MID        = 0x18,
    FURY_REG_BREATH_MID_TO_MIN        = 0x19,
    FURY_REG_BREATH_MIN_HOLD          = 0x1A,
    FURY_REG_BREATH_MAX_BRIGHTNESS    = 0x1B,
    FURY_REG_BREATH_MID_BRIGHTNESS    = 0x1C,
    FURY_REG_BREATH_MIN_BRIGHTNESS    = 0x1D,
    FURY_REG_BRIGHTNESS               = 0x20,
    FURY_REG_BG_RED                   = 0x23,
    FURY_REG_BG_GREEN                 = 0x24,
    FURY_REG_BG_BLUE                  = 0x25,
    FURY_REG_LENGTH                   = 0x26,
    FURY_REG_NUM_SLOTS                = 0x27,
    FURY_REG_NUM_COLORS               = 0x30,
    FURY_REG_MODE_BASE_RED            = 0x31,
    FURY_REG_MODE_BASE_GREEN          = 0x32,
    FURY_REG_MODE_BASE_BLUE           = 0x33,
    FURY_REG_BASE_RED                 = 0x50,
    FURY_REG_BASE_GREEN               = 0x51,
    FURY_REG_BASE_BLUE                = 0x52,
};

enum
{
    FURY_BEGIN_TRNSFER                = 0x53,
    FURY_END_TRNSFER                  = 0x44,
};

// Differentiate modes which use the same written value using the upper bytes.
// The lowest order byte is generally the value written to the mode register.
enum
{
    FURY_MODE_STATIC                  =  0x00,
    FURY_MODE_RAINBOW                 = 0x001,
    FURY_MODE_SPECTRUM                = 0x101,
    FURY_MODE_RHYTHM                  =  0x02,
    FURY_MODE_BREATH                  =  0x03,
    FURY_MODE_DYNAMIC                 =  0x04,
    FURY_MODE_SLIDE                   = 0x005,
    FURY_MODE_SLITHER                 = 0x105,
    FURY_MODE_TELEPORT                = 0x205,
    FURY_MODE_WIND                    = 0x305,
    FURY_MODE_COMET                   = 0x006,
    FURY_MODE_RAIN                    = 0x106,
    FURY_MODE_FIREWORK                = 0x206,
    FURY_MODE_VOLTAGE                 =  0x07,
    FURY_MODE_COUNTDOWN               =  0x08,
    FURY_MODE_FLAME                   =  0x09,
    FURY_MODE_TWILIGHT                =  0x0A,
    FURY_MODE_FURY                    =  0x0B,
    FURY_MODE_DIRECT                  =  0x10,
    FURY_MODE_PRISM                   =  0x11,
    FURY_MODE_BREATH_DIRECT           =  0x13,
};

enum
{
    FURY_DIR_BOTTOM_TO_TOP            = 0x01,
    FURY_DIR_TOP_TO_BOTTOM            = 0x02,
};

class KingstonFuryDRAMController
{
public:
    KingstonFuryDRAMController(i2c_smbus_interface* bus, unsigned char base_addr, std::vector<int> slots, std::string dev_name);

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    unsigned int    GetLEDCount();
    unsigned int    GetLEDPerDIMM();
    unsigned int    GetSlotCount();
    unsigned char   GetMode();

    void            SendPreamble(bool synchronize);
    void            SendBegin();
    void            SendApply();
    void            SetMode(unsigned char val);
    void            SetNumSlots();

    void            SetRegister(int reg, unsigned char val);
    void            SetRegister(int reg, std::vector<unsigned char> vals);
    void            SetModeColors(std::vector<RGBColor> colors);
    void            SetLEDColors(std::vector<RGBColor> colors);

private:
    bool            CachedRead(int slot_idx, unsigned char reg, unsigned char *val);
    bool            CachedWrite(int slot_idx, unsigned char reg, unsigned char val);
    bool            SmbusRead(int slot_idx, unsigned char reg, unsigned char *val);
    bool            SmbusWrite(int slot_idx, unsigned char reg, unsigned char val);

    i2c_smbus_interface*    bus;
    std::vector<int>        slots;
    unsigned char           base_addr;
    std::string             name;
    std::vector<std::map<unsigned char, unsigned char>> reg_cache;
};
