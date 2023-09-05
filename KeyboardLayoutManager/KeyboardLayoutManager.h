/*---------------------------------------------------------*\
| KeyboardLayoutManager.h                                   |
|                                                           |
|   Helper library to produce keyboard layouts              |
|                                                           |
|   Chris M (Dr_No)                             04 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include "RGBControllerKeyNames.h"

extern const char* KLM_CLASS_NAME;
extern const char* KEYBOARD_NAME_FULL;
extern const char* KEYBOARD_NAME_TKL;
extern const char* KEYBOARD_NAME_SIXTY;
extern const char* KEYBOARD_NAME_SEVENTY_FIVE;
extern const char* LOG_MSG_UNUSED_KEY;

enum KEYBOARD_ZONE_BITS
{
    KEYBOARD_ZONE_MAIN                  = ( 1 << 0 ),
    KEYBOARD_ZONE_FN_ROW                = ( 1 << 1 ),
    KEYBOARD_ZONE_EXTRA                 = ( 1 << 2 ),
    KEYBOARD_ZONE_NUMPAD                = ( 1 << 3 ),
};

enum KEYBOARD_SIZE
{
    KEYBOARD_SIZE_EMPTY                 = 0,
    KEYBOARD_SIZE_FULL                  = ( KEYBOARD_ZONE_MAIN  | KEYBOARD_ZONE_FN_ROW |
                                            KEYBOARD_ZONE_EXTRA | KEYBOARD_ZONE_NUMPAD ),
    KEYBOARD_SIZE_TKL                   = ( KEYBOARD_ZONE_MAIN  | KEYBOARD_ZONE_FN_ROW | KEYBOARD_ZONE_EXTRA ),
    KEYBOARD_SIZE_SEVENTY_FIVE          = ( KEYBOARD_ZONE_MAIN  | KEYBOARD_ZONE_FN_ROW ),
    KEYBOARD_SIZE_SIXTY                 = ( KEYBOARD_ZONE_MAIN  ),
};

enum KEYBOARD_LAYOUT
{
    KEYBOARD_LAYOUT_DEFAULT             = 0,
    KEYBOARD_LAYOUT_ANSI_QWERTY,
    KEYBOARD_LAYOUT_ISO_QWERTY,
    KEYBOARD_LAYOUT_ISO_QWERTZ,
    KEYBOARD_LAYOUT_ISO_AZERTY,
    KEYBOARD_LAYOUT_JIS,
};

enum KEYBOARD_MAP_FILL_TYPE
{
    KEYBOARD_MAP_FILL_TYPE_COUNT,
    KEYBOARD_MAP_FILL_TYPE_INDEX,
    KEYBOARD_MAP_FILL_TYPE_VALUE,
};

enum KEYBOARD_OPCODE
{
    KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  = 0,
    KEYBOARD_OPCODE_SWAP_ONLY           = 1,
    KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   = 2,
    KEYBOARD_OPCODE_INS_SHFT_ADJACENT   = 3,
    KEYBOARD_OPCODE_INSERT_ROW          = 4,
    KEYBOARD_OPCODE_REMOVE_ROW          = 5,
    KEYBOARD_OPCODE_ADD_ALT_NAME        = 6,
};

typedef struct
{
    std::uint8_t                            zone;
    std::uint8_t                            row;
    std::uint8_t                            col;
    unsigned int                            value;
    const char*                             name;
    const char*                             alt_name;
    KEYBOARD_OPCODE                         opcode;
}   keyboard_led;

typedef
    std::vector<keyboard_led>               key_set;

typedef struct
{
    std::vector<unsigned int>               default_values;
    std::map<KEYBOARD_LAYOUT, key_set>      regional_overlay;
}   layout_values;

typedef struct
{
    KEYBOARD_SIZE                           base_size;
    key_set                                 edit_keys;
}   keyboard_keymap_overlay;

typedef struct
{
    KEYBOARD_SIZE                           base_size;
    layout_values                           key_values;
    key_set                                 edit_keys;
}   keyboard_keymap_overlay_values;

class KeyboardLayoutManager
{
public:
    KeyboardLayoutManager(KEYBOARD_LAYOUT, KEYBOARD_SIZE);
    KeyboardLayoutManager(KEYBOARD_LAYOUT, KEYBOARD_SIZE, layout_values values);
    ~KeyboardLayoutManager();

    void                        ChangeKeys(key_set edit_keys);
    void                        ChangeKeys(keyboard_keymap_overlay new_layout);
    void                        ChangeKeys(keyboard_keymap_overlay_values new_layout);
    void                        UpdateDimensions();

    std::string                 GetName();
    KEYBOARD_LAYOUT             GetLayout();
    KEYBOARD_SIZE               GetPhysicalSize();

    unsigned int                GetKeyCount();
    std::string                 GetKeyNameAt(unsigned int key_idx);
    std::string                 GetKeyNameAt(unsigned int row, unsigned int col);
    std::string                 GetKeyAltNameAt(unsigned int key_idx);
    std::string                 GetKeyAltNameAt(unsigned int row, unsigned int col);

    unsigned int                GetKeyValueAt(unsigned int key_idx);
    unsigned int                GetKeyValueAt(unsigned int row, unsigned int col);

    unsigned int                GetRowCount();
    unsigned int                GetColumnCount();

    void                        GetKeyMap(unsigned int* map_ptr);
    void                        GetKeyMap(unsigned int* map_ptr, KEYBOARD_MAP_FILL_TYPE fill_type);
    void                        GetKeyMap(unsigned int* map_ptr, KEYBOARD_MAP_FILL_TYPE fill_type,
                                          std::uint8_t height, std::uint8_t width);

private:
    void                        OpCodeSwitch(key_set change_keys);
    void                        InsertKey(keyboard_led key);
    void                        InsertKeys(std::vector<keyboard_led> keys);
    bool                        InsertRow(std::uint8_t row);
    void                        SwapKey(keyboard_led keys);
    void                        SwapKeys(std::vector<keyboard_led> keys);
    void                        RemoveKey(keyboard_led keys);
    void                        RemoveRow(std::uint8_t row);
    void                        AddAltName(keyboard_led key);

    KEYBOARD_LAYOUT             layout;
    KEYBOARD_SIZE               physical_size;
    std::string                 name            = KLM_CLASS_NAME;
    std::uint8_t                rows            = 0;
    std::uint8_t                cols            = 0;
    std::vector<keyboard_led>   keymap;
};

