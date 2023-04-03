/*---------------------------------------------------------------------*\
|  KeyboardLayoutManager.h                                              |
|                                                                       |
|  Helper library to produce keyboard layouts                           |
|                                                                       |
|  Chris M (Dr_No)          04 Feb 2023                                 |
|                                                                       |
\*---------------------------------------------------------------------*/
#pragma once

#include <map>
#include <string>
#include <vector>
#include "RGBControllerKeyNames.h"

extern const char* KLM_CLASS_NAME;
extern const char* KEYBOARD_NAME_FULL;
extern const char* KEYBOARD_NAME_TKL;
extern const char* KEYBOARD_NAME_SIXTY;
extern const char* LOG_MSG_UNUSED_KEY;

enum KEYBOARD_ZONE_BITS
{
    KEYBOARD_ZONE_MAIN      = ( 1 << 0 ),
    KEYBOARD_ZONE_FN_ROW    = ( 1 << 1 ),
    KEYBOARD_ZONE_EXTRA     = ( 1 << 2 ),
    KEYBOARD_ZONE_NUMPAD    = ( 1 << 3 ),
};

enum KEYBOARD_SIZE
{
    KEYBOARD_SIZE_EMPTY = 0,
    KEYBOARD_SIZE_FULL  = ( KEYBOARD_ZONE_MAIN | KEYBOARD_ZONE_FN_ROW | KEYBOARD_ZONE_EXTRA | KEYBOARD_ZONE_NUMPAD ),
    KEYBOARD_SIZE_TKL   = ( KEYBOARD_ZONE_MAIN | KEYBOARD_ZONE_FN_ROW | KEYBOARD_ZONE_EXTRA ),
    KEYBOARD_SIZE_SIXTY = ( KEYBOARD_ZONE_MAIN ),
};

enum KEYBOARD_LAYOUT
{
    KEYBOARD_LAYOUT_ANSI_QWERTY = 0,
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

typedef struct
{
    uint8_t                                 zone;
    uint8_t                                 row;
    uint8_t                                 col;
    unsigned int                            value;
    const char*                             name;
}   keyboard_led;

typedef
    std::vector<keyboard_led>               key_set;

typedef struct
{
    std::vector<unsigned int>               ansi;
    std::map<KEYBOARD_LAYOUT, key_set>      regional_overlay;
}   layout_values;

typedef struct
{
    KEYBOARD_SIZE                           base_size;
    key_set                                 insert;
    key_set                                 swap;
    key_set                                 remove;
}   keyboard_keymap_overlay;

typedef struct
{
    KEYBOARD_SIZE                           base_size;
    layout_values                           key_values;
    key_set                                 insert;
    key_set                                 swap;
    key_set                                 remove;
}   keyboard_keymap_overlay_values;

static const std::vector<keyboard_led> empty_basemap =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,     Index,      Key                         |
    \*---------------------------------------------------------------------*/
};

class KeyboardLayoutManager
{
public:
    KeyboardLayoutManager(KEYBOARD_LAYOUT, KEYBOARD_SIZE);
    KeyboardLayoutManager(KEYBOARD_LAYOUT, KEYBOARD_SIZE, layout_values values);
    ~KeyboardLayoutManager();

    void                        ChangeKeys(keyboard_keymap_overlay new_layout);
    void                        ChangeKeys(keyboard_keymap_overlay_values new_layout);
    void                        InsertKeys(std::vector<keyboard_led> keys);
    void                        SwapKeys(std::vector<keyboard_led> keys);
    void                        UpdateDimensions();

    std::string                 GetName();
    KEYBOARD_LAYOUT             GetLayout();
    KEYBOARD_SIZE               GetPhysicalSize();

    unsigned int                GetKeyCount();
    std::string                 GetKeyNameAt(unsigned int key_idx);
    std::string                 GetKeyNameAt(unsigned int row, unsigned int col);

    unsigned int                GetKeyValueAt(unsigned int key_idx);
    unsigned int                GetKeyValueAt(unsigned int row, unsigned int col);

    unsigned int                GetRowCount();
    unsigned int                GetColumnCount();

    void                        GetKeyMap(unsigned int* map_ptr);
    void                        GetKeyMap(unsigned int* map_ptr, KEYBOARD_MAP_FILL_TYPE fill_type);
    void                        GetKeyMap(unsigned int* map_ptr, KEYBOARD_MAP_FILL_TYPE fill_type,
                                          uint8_t height, uint8_t width);

private:
    void                        RemoveKeys(std::vector<keyboard_led> keys);

    KEYBOARD_LAYOUT             layout;
    KEYBOARD_SIZE               physical_size;
    std::string                 name            = KLM_CLASS_NAME;
    uint8_t                     rows            = 0;
    uint8_t                     cols            = 0;
    std::vector<keyboard_led>   keymap          = empty_basemap;
};

