/*---------------------------------------------------------*\
| DeviceView.cpp                                            |
|                                                           |
|   OpenRGB Device view widget for Qt                       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>
#include <QtCore/qmath.h>
#include <QDebug>
#include <QMouseEvent>
#include "DeviceView.h"
#include "ResourceManager.h"
#include "RGBControllerKeyNames.h"
#include "RGBController.h"
#include "SettingsManager.h"

#define MAX_COLS    20
#define PAD_LED     0.1f
#define PAD_TEXT    0.1f
#define PAD_ZONE    1.0f
#define PAD_SEGMENT 0.9f
#define SIZE_TEXT   0.5f

DeviceView::DeviceView(QWidget *parent) :
    QWidget(parent),
    initSize(128,128),
    mouseDown(false)
{
    controller = NULL;
    numerical_labels = false;
    per_led = true;
    setMouseTracking(1);

    size = width();
}

DeviceView::~DeviceView()
{

}

struct led_label
{
    QString label_text;
    QString label_utf8;
};

static const std::map<std::string, led_label> led_label_lookup =
{
    { KEY_EN_A,                 { "A"     , "A",                }},
    { KEY_EN_B,                 { "B"     , "B",                }},
    { KEY_EN_C,                 { "C"     , "C",                }},
    { KEY_EN_D,                 { "D"     , "D",                }},
    { KEY_EN_E,                 { "E"     , "E",                }},
    { KEY_EN_F,                 { "F"     , "F",                }},
    { KEY_EN_G,                 { "G"     , "G",                }},
    { KEY_EN_H,                 { "H"     , "H",                }},
    { KEY_EN_I,                 { "I"     , "I",                }},
    { KEY_EN_J,                 { "J"     , "J",                }},
    { KEY_EN_K,                 { "K"     , "K",                }},
    { KEY_EN_L,                 { "L"     , "L",                }},
    { KEY_EN_M,                 { "M"     , "M",                }},
    { KEY_EN_N,                 { "N"     , "N",                }},
    { KEY_EN_O,                 { "O"     , "O",                }},
    { KEY_EN_P,                 { "P"     , "P",                }},
    { KEY_EN_Q,                 { "Q"     , "Q",                }},
    { KEY_EN_R,                 { "R"     , "R",                }},
    { KEY_EN_S,                 { "S"     , "S",                }},
    { KEY_EN_T,                 { "T"     , "T",                }},
    { KEY_EN_U,                 { "U"     , "U",                }},
    { KEY_EN_V,                 { "V"     , "V",                }},
    { KEY_EN_W,                 { "W"     , "W",                }},
    { KEY_EN_X,                 { "X"     , "X",                }},
    { KEY_EN_Y,                 { "Y"     , "Y",                }},
    { KEY_EN_Z,                 { "Z"     , "Z",                }},
    { KEY_EN_0,                 { "0"     , "0",                }},
    { KEY_EN_1,                 { "1"     , "1",                }},
    { KEY_EN_2,                 { "2"     , "2",                }},
    { KEY_EN_3,                 { "3"     , "3",                }},
    { KEY_EN_4,                 { "4"     , "4",                }},
    { KEY_EN_5,                 { "5"     , "5",                }},
    { KEY_EN_6,                 { "6"     , "6",                }},
    { KEY_EN_7,                 { "7"     , "7",                }},
    { KEY_EN_8,                 { "8"     , "8",                }},
    { KEY_EN_9,                 { "9"     , "9",                }},
    { KEY_EN_F1,                { "F1"    , "F1",               }},
    { KEY_EN_F2,                { "F2"    , "F2",               }},
    { KEY_EN_F3,                { "F3"    , "F3",               }},
    { KEY_EN_F4,                { "F4"    , "F4",               }},
    { KEY_EN_F5,                { "F5"    , "F5",               }},
    { KEY_EN_F6,                { "F6"    , "F6",               }},
    { KEY_EN_F7,                { "F7"    , "F7",               }},
    { KEY_EN_F8,                { "F8"    , "F8",               }},
    { KEY_EN_F9,                { "F9"    , "F9",               }},
    { KEY_EN_F10,               { "F10"   , "F10",              }},
    { KEY_EN_F11,               { "F11"   , "F11",              }},
    { KEY_EN_F12,               { "F12"   , "F12",              }},
    { KEY_EN_BACK_TICK,         { "`"     , "`",                }},
    { KEY_EN_MINUS,             { "-"     , "-",                }},
    { KEY_EN_PLUS,              { "+"     , "+",                }},
    { KEY_EN_EQUALS,            { "="     , "=",                }},
    { KEY_EN_LEFT_BRACKET,      { "["     , "[",                }},
    { KEY_EN_RIGHT_BRACKET,     { "]"     , "]",                }},
    { KEY_EN_BACK_SLASH,        { "\\"    , "\\",               }},
    { KEY_EN_ANSI_BACK_SLASH,   { "\\"    , "\\",               }},
    { KEY_EN_ISO_BACK_SLASH,    { "\\"    , "\\",               }},
    { KEY_EN_SEMICOLON,         { ";"     , ";",                }},
    { KEY_EN_QUOTE,             { "'"     , "'",                }},
    { KEY_EN_POUND,             { "#"     , "#",                }},
    { KEY_EN_COMMA,             { ","     , ",",                }},
    { KEY_EN_PERIOD,            { "."     , ".",                }},
    { KEY_EN_FORWARD_SLASH,     { "/"     , "/",                }},
    { KEY_EN_ESCAPE,            { "Esc"   , "Esc",              }},
    { KEY_EN_PRINT_SCREEN,      { "Prt"   , "Prt",              }},
    { KEY_EN_SCROLL_LOCK,       { "Scr"   , "Scr",              }},
    { KEY_EN_PAUSE_BREAK,       { "Brk"   , "Brk",              }},
    { KEY_EN_BACKSPACE,         { "Bks"   , "\xE2\x8C\xAB"      }}, // ⌫
    { KEY_EN_INSERT,            { "Ins"   , "Ins",              }},
    { KEY_EN_HOME,              { "Hom"   , "Hom",              }},
    { KEY_EN_PAGE_UP,           { "PUp"   , "PUp",              }},
    { KEY_EN_TAB,               { "Tab"   , "\xE2\x86\xb9"      }}, // ⭾ ↹ ⇄ ⇥
    { KEY_EN_DELETE,            { "Del"   , "Del",              }},
    { KEY_EN_END,               { "End"   , "End",              }},
    { KEY_EN_PAGE_DOWN,         { "PDn"   , "PDn",              }},
    { KEY_EN_CAPS_LOCK,         { "Cap"   , "Cap",              }},
    { KEY_EN_ANSI_ENTER,        { "Ent"   , "\xE2\x86\xB5"      }}, // ↵ ⏎
    { KEY_EN_ISO_ENTER,         { "Ent"   , "\xE2\x86\xB5"      }}, // ↵ ⏎
    { KEY_EN_LEFT_SHIFT,        { "Sft"   , "\xE2\x87\xA7"      }}, // ⇧
    { KEY_EN_RIGHT_SHIFT,       { "Sft"   , "\xE2\x87\xA7"      }}, // ⇧
    { KEY_EN_UP_ARROW,          { "Up"    , "\xF0\x9F\xA1\xB9"  }}, // ↑ 🡹
    { KEY_EN_LEFT_CONTROL,      { "Ctl"   , "Ctl",              }},
    { KEY_EN_LEFT_WINDOWS,      { "Win"   , "\xe2\x9d\x96"      }}, // ❖
    { KEY_EN_LEFT_FUNCTION,     { "Fn"    , "Fn",               }},
    { KEY_EN_LEFT_ALT,          { "Alt"   , "Alt",              }},
    { KEY_EN_SPACE,             { "Spc"   , "Spc",              }},
    { KEY_EN_RIGHT_ALT,         { "Alt"   , "Alt",              }},
    { KEY_EN_RIGHT_FUNCTION,    { "Fn"    , "Fn",               }},
    { KEY_EN_RIGHT_WINDOWS,     { "Win"   , "\xe2\x9d\x96"      }}, // ❖
    { KEY_EN_MENU,              { "Mnu"   , "\xE2\x98\xB0"      }}, // ▤ ☰ 𝌆 🗏
    { KEY_EN_RIGHT_CONTROL,     { "Ctl"   , "Ctl",              }},
    { KEY_EN_LEFT_ARROW,        { "Lft"   , "\xF0\x9F\xA1\xB8"  }}, // ↑ 🡹
    { KEY_EN_DOWN_ARROW,        { "Dn"    , "\xF0\x9F\xA1\xBB"  }}, // ↑ 🡹
    { KEY_EN_RIGHT_ARROW,       { "Rgt"   , "\xF0\x9F\xA1\xBA"  }}, // ↑ 🡹
    { KEY_EN_NUMPAD_LOCK,       { "Num"   , "Num",              }},
    { KEY_EN_NUMPAD_DIVIDE,     { "/"     , "/",                }},
    { KEY_EN_NUMPAD_TIMES,      { "*"     , "*",                }},
    { KEY_EN_NUMPAD_MINUS,      { "-"     , "-",                }},
    { KEY_EN_NUMPAD_PLUS,       { "+"     , "+",                }},
    { KEY_EN_NUMPAD_PERIOD,     { "."     , ".",                }},
    { KEY_EN_NUMPAD_ENTER,      { "Ent"   , "\xE2\x86\xB5"      }}, // ↵ ⏎
    { KEY_EN_NUMPAD_0,          { "0"     , "0",                }},
    { KEY_EN_NUMPAD_1,          { "1"     , "1",                }},
    { KEY_EN_NUMPAD_2,          { "2"     , "2",                }},
    { KEY_EN_NUMPAD_3,          { "3"     , "3",                }},
    { KEY_EN_NUMPAD_4,          { "4"     , "4",                }},
    { KEY_EN_NUMPAD_5,          { "5"     , "5",                }},
    { KEY_EN_NUMPAD_6,          { "6"     , "6",                }},
    { KEY_EN_NUMPAD_7,          { "7"     , "7",                }},
    { KEY_EN_NUMPAD_8,          { "8"     , "8",                }},
    { KEY_EN_NUMPAD_9,          { "9"     , "9",                }},
    { KEY_EN_MEDIA_PLAY_PAUSE,  { "Ply"   , "\xE2\x8F\xAF",     }}, // ⏯
    { KEY_EN_MEDIA_PREVIOUS,    { "Prv"   , "\xE2\x8F\xAE",     }}, // ⏮
    { KEY_EN_MEDIA_NEXT,        { "Nxt"   , "\xE2\x8F\xAD",     }}, // ⏭
    { KEY_EN_MEDIA_STOP,        { "Stp"   , "\xE2\x8F\xB9",     }}, // ⏹
    { KEY_EN_MEDIA_MUTE,        { "Mte"   , "\xF0\x9F\x94\x87", }}, // 🔇
    { KEY_EN_MEDIA_VOLUME_DOWN, { "Vl-"   , "\xF0\x9F\x94\x88", }}, // 🔉
    { KEY_EN_MEDIA_VOLUME_UP,   { "Vl+"   , "\xF0\x9F\x94\x89", }}, // 🔊
    { KEY_EN_POWER,             { "Pwr"   , "\xE2\x8F\xBB",     }}, // ⏻
    { KEY_JP_RO,                { "_"     , "_",                }},
    { KEY_JP_EJ,                { "E/J"   , "E/J",              }},
    { KEY_JP_ZENKAKU,           { "Zen"   , "\xE5\x8D\x8A\xE8"
                                            "\xA7\x92\x2F\xE5"
                                            "\x85\xA8\xE8\xA7"
                                            "\x92",             }}, // 半角/全角
    { KEY_JP_KANA,              { "Kana"  , "\xE3\x81\x8B\xE3"
                                            "\x81\xAA",         }}, // かな
    { KEY_JP_HENKAN,            { "Hnkn"  , "\xE5\xA4\x89\xE6"
                                            "\x8F\x9B",         }}, // 変換
    { KEY_JP_MUHENKAN,          { "MuKn"  , "\xE7\x84\xA1\xE5"
                                            "\xA4\x89\xE6\x8F"
                                            "\x9B",             }}, // 無変換
    { KEY_JP_YEN,               { "Yen"   , "\xC2\xA5",         }}, // ¥
    { KEY_JP_AT,                { "@"     , "@",                }},
    { KEY_JP_CHEVRON,           { "^"     , "^",                }},
    { KEY_JP_COLON,             { ":"     , ":",                }},
    { KEY_KR_HAN,               { "Hayg"  , "\xED\x95\x9C\x2F"
                                            "\xEC\x98\x81",     }}, // 한/영
    { KEY_KR_HANJA,             { "Haja"  , "\xED\x95\x9C\xEC"
                                            "\x9E\x90",         }}, // 한자
    { KEY_NORD_AAL,             { "Å"     , "\xC3\x85",         }}, // Å
    { KEY_NORD_A_OE,            { "Ä Ø"   , "\xC3\x84\x20\xC3"
                                            "\x98"              }}, // Ä Ø
    { KEY_NORD_O_AE,            { "Ö Æ"   , "\xC3\x96\x20\xC3"
                                            "\x86"              }}, // Ö Æ
    { KEY_NORD_HALF,            { "§ ½"   , "\xC2\xA7\x20\xC2"
                                            "\xBD"              }}, // § ½
    { KEY_NORD_HYPHEN,          { "- _"   , "- _"               }},
    { KEY_NORD_PLUS_QUESTION,   { "+ ?"   , "+ ?",              }},
    { KEY_NORD_ACUTE_GRAVE,     { "´ `"   , "\xC2\xB4\x20\x60", }}, // ´ `
    { KEY_NORD_DOTS_CARET,      { "¨ ^"   , "\xC2\xA8\x20\x5E", }}, // ¨ ^
    { KEY_NORD_QUOTE,           { "' *"   , "' *",              }},
    { KEY_NORD_ANGLE_BRACKET,   { "< >"   , "< >"               }},
    { KEY_DE_ESZETT,            { "ß"     , "\xc3\x9F",         }},
    { KEY_DE_DIAERESIS_A,       { "Ä"     , "\xC3\x84",         }},
    { KEY_DE_DIAERESIS_O,       { "Ö"     , "\xC3\x96",         }},
    { KEY_DE_DIAERESIS_U,       { "Ü"     , "\xC3\x9C"          }},
    { KEY_FR_SUPER_2,           { "²"     , "\xc2\xb2"          }},
    { KEY_FR_AMPERSAND,         { "&"     , "&"                 }},
    { KEY_FR_ACUTE_E,           { "é"     , "\xc3\xa9"          }},
    { KEY_FR_DOUBLEQUOTE,       { "\""    , "\""                }},
    { KEY_FR_LEFT_PARENTHESIS,  { "("     , "("                 }},
    { KEY_FR_GRAVE_E,           { "è"     , "\xc3\xa8"          }},
    { KEY_FR_UNDERSCORE,        { "_"     , "_"                 }},
    { KEY_FR_CEDILLA_C,         { "ç"     , "\xc3\xa7"          }},
    { KEY_FR_GRAVE_A,           { "à"     , "\xc3\xa0"          }},
    { KEY_FR_RIGHT_PARENTHESIS, { ")"     , ")"                 }},
    { KEY_FR_DOLLAR,            { "$"     , "$"                 }},
    { KEY_FR_GRAVE_U,           { "ù"     , "\xc3\xb9"          }},
    { KEY_FR_ASTERIX,           { "*"     , "*"                 }},
    { KEY_FR_EXCLAIMATION,      { "!"     , "!"                 }},
    { KEY_ES_OPEN_QUESTION_MARK,{ "¿"     , "¡"                 }},
    { KEY_ES_TILDE,             { "´"     , "¨"                 }},
    { KEY_ES_ENIE,              { "ñ"     , "Ñ"                 }},
    { KEY_BR_TILDE,             { "~"     , "~"                 }}
};

void DeviceView::setController(RGBController * controller_ptr)
{
    /*-----------------------------------------------------*\
    | Store the controller pointer                          |
    \*-----------------------------------------------------*/
    controller = controller_ptr;
}

void DeviceView::InitDeviceView()
{
    /*-----------------------------------------------------*\
    | Set the size of the selection flags vector            |
    \*-----------------------------------------------------*/
    selectionFlags.resize((int)controller->leds.size());

    /*-----------------------------------------------------*\
    | Set the size of the zone and LED position vectors     |
    \*-----------------------------------------------------*/
    zone_pos.resize(controller->zones.size());
    led_pos.resize(controller->leds.size());
    led_labels.resize(controller->leds.size());

    /*-----------------------------------------------------*\
    | Process position and size for zones                   |
    \*-----------------------------------------------------*/
    unsigned int maxWidth       = 0;
    unsigned int segment_count  = 0;
    float        totalHeight    = 0.0f;

    /*-----------------------------------------------------*\
    | Get device view settings                              |
    \*-----------------------------------------------------*/
    SettingsManager*    settings_manager    = ResourceManager::get()->GetSettingsManager();
    std::string         ui_string           = "UserInterface";
    json                ui_settings;

    bool                disable_expansion   = false;

    ui_settings = settings_manager->GetSettings(ui_string);

    if(ui_settings.contains("disable_key_expansion"))
    {
        disable_expansion       = ui_settings["disable_key_expansion"];
    }

    /*-----------------------------------------------------*\
    | Determine the total height (in LEDs) of all zones     |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
    {
        /*-----------------------------------------------------*\
        | For matrix zones, use matrix height from the map      |
        \*-----------------------------------------------------*/
        if((controller->zones[zone_idx].type == ZONE_TYPE_MATRIX) && (controller->zones[zone_idx].matrix_map))
        {
            totalHeight                += controller->zones[zone_idx].matrix_map->height;
            zone_pos[zone_idx].matrix_w = controller->zones[zone_idx].matrix_map->width;
        }
        /*-----------------------------------------------------*\
        | For all other zones, compute the height including     |
        | wrap-around                                           |
        \*-----------------------------------------------------*/
        else if(controller->zones[zone_idx].segments.size() > 0)
        {
            for(std::size_t segment_idx = 0; segment_idx < controller->zones[zone_idx].segments.size(); segment_idx++)
            {
                unsigned int count          = controller->zones[zone_idx].segments[segment_idx].leds_count;
                zone_pos[zone_idx].matrix_w = std::min(count, (unsigned int)MAX_COLS);
                totalHeight                += (count / MAX_COLS) + ((count % MAX_COLS) > 0);

                segment_count++;
            }
        }
        else
        {
            unsigned int count          = controller->GetLEDsInZone((unsigned int)zone_idx);
            zone_pos[zone_idx].matrix_w = std::min(count, (unsigned int)MAX_COLS);
            totalHeight                += (count / MAX_COLS) + ((count % MAX_COLS) > 0);
        }

        /*-----------------------------------------------------*\
        | Determine the maximum width (in LEDs) in the view     |
        \*-----------------------------------------------------*/
        if(zone_pos[zone_idx].matrix_w > maxWidth)
        {
            maxWidth = zone_pos[zone_idx].matrix_w;
        }
    }

    segment_pos.resize(segment_count);

    /*-----------------------------------------------------*\
    | Add some space for zone names and padding             |
    \*-----------------------------------------------------*/
    totalHeight    += controller->zones.size() * PAD_ZONE;
    totalHeight    += segment_count * PAD_SEGMENT;

    float current_y = 0;                    // We will be descending, placing each zone one unit below the previous one
    matrix_h        = totalHeight;
    segment_count   = 0;

    for(std::size_t zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
    {
        /*-----------------------------------------------------*\
        | Calculate zone label position and size                |
        \*-----------------------------------------------------*/
        zone_pos[zone_idx].matrix_x = (maxWidth - zone_pos[zone_idx].matrix_w) / 2.0f;
        zone_pos[zone_idx].matrix_y = current_y + SIZE_TEXT;
        zone_pos[zone_idx].matrix_h = SIZE_TEXT - PAD_TEXT;
        current_y                  += PAD_ZONE;

        /*-----------------------------------------------------*\
        | Calculate LEDs position and size for zone             |
        \*-----------------------------------------------------*/
        if((controller->zones[zone_idx].type == ZONE_TYPE_MATRIX) && (controller->zones[zone_idx].matrix_map))
        {
            matrix_map_type * map = controller->zones[zone_idx].matrix_map;

            for(unsigned int led_x = 0; led_x < map->width; led_x++)
            {
                for(unsigned int led_y = 0; led_y < map->height; led_y++)
                {
                    unsigned int map_idx    = led_y * map->width + led_x;
                    unsigned int color_idx  = map->map[map_idx] + controller->zones[zone_idx].start_idx;

                    if(map->map[map_idx] != 0xFFFFFFFF && color_idx < led_pos.size())
                    {
                        led_pos[color_idx].matrix_x = (zone_pos[zone_idx].matrix_x + led_x + PAD_LED);
                        led_pos[color_idx].matrix_y = current_y + (led_y + PAD_LED);

                        /*-----------------------------------------------------*\
                        | LED is a 1x1 square, minus padding on all sides       |
                        \*-----------------------------------------------------*/
                        led_pos[color_idx].matrix_w = (1.0f - (2.0f * PAD_LED));
                        led_pos[color_idx].matrix_h = (1.0f - (2.0f * PAD_LED));

                        if(!disable_expansion)
                        {
                            /*-----------------------------------------------------*\
                            | Expand large keys to fill empty spaces in matrix, if  |
                            | possible.  Large keys can fill left, down, up, or wide|
                            | Fill Left:                                            |
                            |    Tab                                                |
                            |    Caps Lock                                          |
                            |    Left Shift                                         |
                            |    Right Shift                                        |
                            |    Backspace                                          |
                            |    Number Pad 0                                       |
                            |                                                       |
                            | Fill Up or Down:                                      |
                            |    Number Pad Enter                                   |
                            |    Number Pad +                                       |
                            |                                                       |
                            | Fill Wide:                                            |
                            |    Space                                              |
                            \*-----------------------------------------------------*/
                            if(led_x < map->width - 1 && map->map[map_idx + 1] == 0xFFFFFFFF)
                            {
                                if( ( controller->GetLEDName(color_idx) == KEY_EN_TAB        )
                                 || ( controller->GetLEDName(color_idx) == KEY_EN_CAPS_LOCK  )
                                 || ( controller->GetLEDName(color_idx) == KEY_EN_LEFT_SHIFT )
                                 || ( controller->GetLEDName(color_idx) == KEY_EN_RIGHT_SHIFT)
                                 || ( controller->GetLEDName(color_idx) == KEY_EN_BACKSPACE  )
                                 || ( controller->GetLEDName(color_idx) == KEY_EN_NUMPAD_0   ) )
                                {
                                    led_pos[color_idx].matrix_w += 1.0f;
                                }
                            }
                            if( ( controller->GetLEDName(color_idx) == KEY_EN_NUMPAD_ENTER   )
                             || ( controller->GetLEDName(color_idx) == KEY_EN_NUMPAD_PLUS    ) )
                            {
                                if(led_y < map->height - 1 && map->map[map_idx + map->width] == 0xFFFFFFFF)
                                {
                                    led_pos[color_idx].matrix_h += 1.0f;
                                }
                                /* TODO: check if there isn't another widened key above */
                                else if(led_y > 0 && map->map[map_idx - map->width] == 0xFFFFFFFF)
                                {
                                    led_pos[color_idx].matrix_y -= 1.0f;
                                    led_pos[color_idx].matrix_h += 1.0f;
                                }
                            }
                            else if(controller->GetLEDName(color_idx) == KEY_EN_SPACE)
                            {
                                for(unsigned int map_idx2 = map_idx - 1; map_idx2 > led_y * map->width && map->map[map_idx2] == 0xFFFFFFFF; map_idx2--)
                                {
                                    led_pos[color_idx].matrix_x -= 1.0f;
                                    led_pos[color_idx].matrix_w += 1.0f;
                                }
                                for(unsigned int map_idx2 = map_idx + 1; map_idx2 < (led_y + 1) * map->width && map->map[map_idx2] == 0xFFFFFFFF; map_idx2++)
                                {
                                    led_pos[color_idx].matrix_w += 1.0f;
                                }
                            }
                        }
                    }
                }
            }

            current_y += map->height;
        }
        else if(controller->zones[zone_idx].segments.size() > 0)
        {
            for(std::size_t segment_idx = 0; segment_idx < controller->zones[zone_idx].segments.size(); segment_idx++)
            {
                /*-----------------------------------------------------*\
                | Calculate segment label position and size             |
                \*-----------------------------------------------------*/
                segment_pos[segment_count].matrix_x = (maxWidth - zone_pos[zone_idx].matrix_w) / 2.0f;
                segment_pos[segment_count].matrix_y = current_y + SIZE_TEXT;
                segment_pos[segment_count].matrix_w = zone_pos[zone_idx].matrix_w;
                segment_pos[segment_count].matrix_h = SIZE_TEXT - PAD_TEXT;
                current_y                          += PAD_SEGMENT;

                segment_count++;

                /*-----------------------------------------------------*\
                | Calculate LED box positions for segmented zones       |
                \*-----------------------------------------------------*/
                unsigned int leds_count = controller->zones[zone_idx].segments[segment_idx].leds_count;

                for(unsigned int led_idx = 0; led_idx < leds_count; led_idx++)
                {
                    unsigned int led_pos_idx = controller->zones[zone_idx].start_idx + controller->zones[zone_idx].segments[segment_idx].start_idx + led_idx;

                    led_pos[led_pos_idx].matrix_x = zone_pos[zone_idx].matrix_x + ((led_idx % MAX_COLS) + PAD_LED);
                    led_pos[led_pos_idx].matrix_y = current_y + ((led_idx / MAX_COLS) + PAD_LED);

                    /*-----------------------------------------------------*\
                    | LED is a 1x1 square, minus padding on all sides       |
                    \*-----------------------------------------------------*/
                    led_pos[led_pos_idx].matrix_w = (1.0f - (2.0f * PAD_LED));
                    led_pos[led_pos_idx].matrix_h = (1.0f - (2.0f * PAD_LED));
                }

                current_y += (leds_count / MAX_COLS) + ((leds_count % MAX_COLS) > 0);
            }
        }
        else
        {
            /*-----------------------------------------------------*\
            | Calculate LED box positions for single/linear zones   |
            \*-----------------------------------------------------*/
            unsigned int leds_count = controller->GetLEDsInZone((unsigned int)zone_idx);

            for(unsigned int led_idx = 0; led_idx < leds_count; led_idx++)
            {
                unsigned int led_pos_idx = controller->zones[zone_idx].start_idx + led_idx;

                led_pos[led_pos_idx].matrix_x = zone_pos[zone_idx].matrix_x + ((led_idx % MAX_COLS) + PAD_LED);
                led_pos[led_pos_idx].matrix_y = current_y + ((led_idx / MAX_COLS) + PAD_LED);

                /*-----------------------------------------------------*\
                | LED is a 1x1 square, minus padding on all sides       |
                \*-----------------------------------------------------*/
                led_pos[led_pos_idx].matrix_w = (1.0f - (2.0f * PAD_LED));
                led_pos[led_pos_idx].matrix_h = (1.0f - (2.0f * PAD_LED));
            }

            current_y += (leds_count / MAX_COLS) + ((leds_count % MAX_COLS) > 0);
        }
    }

    /*-----------------------------------------------------*\
    | Update LED labels                                     |
    \*-----------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < controller->leds.size(); led_idx++)
    {
        std::map<std::string, led_label>::const_iterator it = led_label_lookup.find(controller->GetLEDName((unsigned int)led_idx));

        if(it != led_label_lookup.end())
        {
            led_labels[led_idx] = it->second.label_utf8;
        }
        else if(numerical_labels)
        {
            led_labels[led_idx] = QString::number(led_idx);
        }
    }

    /*-----------------------------------------------------*\
    | Scale the zones, segments, and LEDs                   |
    |                                                       |
    | Atom is the width of a single square; if the whole    |
    | thing becomes too tall, we ignore it and let the view |
    | widget take care of it                                |
    \*-----------------------------------------------------*/
    float atom = 1.0f / maxWidth;

    for(std::size_t zone_idx = 0; zone_idx < zone_pos.size(); zone_idx++)
    {
        zone_pos[zone_idx].matrix_x *= atom;
        zone_pos[zone_idx].matrix_y *= atom;
        zone_pos[zone_idx].matrix_w *= atom;
        zone_pos[zone_idx].matrix_h *= atom;
    }

    for(std::size_t segment_idx = 0; segment_idx < segment_pos.size(); segment_idx++)
    {
        segment_pos[segment_idx].matrix_x *= atom;
        segment_pos[segment_idx].matrix_y *= atom;
        segment_pos[segment_idx].matrix_w *= atom;
        segment_pos[segment_idx].matrix_h *= atom;
    }

    for(std::size_t led_idx = 0; led_idx < led_pos.size(); led_idx++)
    {
        led_pos[led_idx].matrix_x *= atom;
        led_pos[led_idx].matrix_y *= atom;
        led_pos[led_idx].matrix_w *= atom;
        led_pos[led_idx].matrix_h *= atom;
    }

    matrix_h *= atom;

    /*-----------------------------------------------------*\
    | Update cached size and offset                         |
    \*-----------------------------------------------------*/
    size     = width();
    offset_x = 0;

    if(height() < size * matrix_h)
    {
        size     = height() / matrix_h;
        offset_x = (width() - size) / 2;
    }
}

void DeviceView::setNumericalLabels(bool enable)
{
    numerical_labels = enable;
}

void DeviceView::setPerLED(bool per_led_mode)
{
    per_led = per_led_mode;
    update();
}

QSize DeviceView::sizeHint () const
{
    return QSize(height() - 1, height() - 1);
}

QSize DeviceView::minimumSizeHint () const
{
    return initSize;
}

void DeviceView::mousePressEvent(QMouseEvent *event)
{
    if(per_led)
    {
        ctrlDown    = event->modifiers().testFlag(Qt::ControlModifier);
        mouseDown   = true;
        mouseMoved  = false;

        if(ctrlDown)
        {
            previousFlags = selectionFlags;
            previousSelection = selectedLeds;
        }

        /*-----------------------------------------------------*\
        | It's okay if the size becomes negative                |
        \*-----------------------------------------------------*/
        #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            selectionRect.setLeft(event->position().x());
            selectionRect.setTop(event->position().y());
            selectionRect.setRight(event->position().x());
            selectionRect.setBottom(event->position().y());
        #else
            selectionRect.setLeft(event->x());
            selectionRect.setTop(event->y());
            selectionRect.setRight(event->x());
            selectionRect.setBottom(event->y());
        #endif

        updateSelection();
        update();
    }
}

void DeviceView::mouseMoveEvent(QMouseEvent *event)
{
    if(per_led)
    {
        lastMousePos = event->pos();

        #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            selectionRect.setRight(event->position().x());
            selectionRect.setBottom(event->position().y());
        #else
            selectionRect.setRight(event->x());
            selectionRect.setBottom(event->y());
        #endif

        if(mouseDown)
        {
            mouseMoved  = true;
            ctrlDown    = event->modifiers().testFlag(Qt::ControlModifier);

            /*-----------------------------------------------------*\
            | Clear the previous selection in case ctrl is released |
            \*-----------------------------------------------------*/
            if(!ctrlDown)
            {
                previousSelection.clear();
                previousFlags.clear();
                previousFlags.resize((int)controller->leds.size());
            }
            updateSelection();
        }
        update();
    }
}

void DeviceView::mouseReleaseEvent(QMouseEvent* event)
{
    if(per_led)
    {
        selectionRect = selectionRect.normalized();
        mouseDown = false;

        /*-----------------------------------------------------*\
        | Check if the user clicked a zone name & select it     |
        \*-----------------------------------------------------*/
        if(!mouseMoved)
        {
            int size     = width();
            int offset_x = 0;

            if(height() < size * matrix_h)
            {
                size     = height() / matrix_h;
                offset_x = (width() - size) / 2;
            }

            unsigned int segment_count = 0;

            for(unsigned int zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
            {
                int posx = zone_pos[zone_idx].matrix_x * size + offset_x + 12;
                int posy = zone_pos[zone_idx].matrix_y * size;
                int posw = zone_pos[zone_idx].matrix_w * size;
                int posh = zone_pos[zone_idx].matrix_h * size;

                QRect rect = {posx, posy, posw, posh};

                if(rect.contains(event->pos()))
                {
                    selectZone(zone_idx, ctrlDown);
                }

                for(unsigned int segment_idx = 0; segment_idx < controller->zones[zone_idx].segments.size(); segment_idx++)
                {
                    posx = segment_pos[segment_count].matrix_x * size + offset_x + 12;
                    posy = segment_pos[segment_count].matrix_y * size;
                    posw = segment_pos[segment_count].matrix_w * size;
                    posh = segment_pos[segment_count].matrix_h * size;

                    segment_count++;

                    rect = {posx, posy, posw, posh};

                    if(rect.contains(event->pos()))
                    {
                        selectSegment(zone_idx, segment_idx, ctrlDown);
                    }
                }
            }
        }
        update();
    }
}

void DeviceView::resizeEvent(QResizeEvent* /*event*/)
{
    size     = width();
    offset_x = 0;

    if(height() < size * matrix_h)
    {
        size     = height() / matrix_h;
        offset_x = (width() - size) / 2;
    }
    update();
}

void DeviceView::paintEvent(QPaintEvent* /* event */)
{
    QPainter painter(this);
    QFont font = painter.font();

    /*-----------------------------------------------------*\
    | If Device View is hidden, don't paint                 |
    \*-----------------------------------------------------*/
    if(isHidden() || !per_led)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | If controller has resized, reinitialize local data    |
    \*-----------------------------------------------------*/
    if(controller->leds.size() != led_pos.size())
    {
        InitDeviceView();
    }

    /*-----------------------------------------------------*\
    | If segments have resized, reinitialize local data     |
    \*-----------------------------------------------------*/
    unsigned int segments = 0;

    for(std::size_t zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
    {
        for(std::size_t segment_idx = 0; segment_idx < controller->zones[zone_idx].segments.size(); segment_idx++)
        {
            segments++;
        }
    }

    if(segments != segment_pos.size())
    {
        InitDeviceView();
    }

    /*-----------------------------------------------------*\
    | LED rectangles                                        |
    \*-----------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < controller->leds.size(); led_idx++)
    {
        int posx = led_pos[led_idx].matrix_x * size + offset_x;
        int posy = led_pos[led_idx].matrix_y * size;
        int posw = led_pos[led_idx].matrix_w * size;
        int posh = led_pos[led_idx].matrix_h * size;

        QRect rect = {posx, posy, posw, posh};

        /*-----------------------------------------------------*\
        | Fill color                                            |
        \*-----------------------------------------------------*/
        QColor currentColor = QColor::fromRgb(
                    RGBGetRValue(controller->colors[led_idx]),
                    RGBGetGValue(controller->colors[led_idx]),
                    RGBGetBValue(controller->colors[led_idx]));
        painter.setBrush(currentColor);

        /*-----------------------------------------------------*\
        | Border color                                          |
        \*-----------------------------------------------------*/
        if(selectionFlags[led_idx])
        {
            painter.setPen(palette().highlight().color());
        }
        else
        {
            painter.setPen(palette().dark().color());
        }
        painter.drawRect(rect);

        /*-----------------------------------------------------*\
        | Label                                                 |
        | Set the font color so that the text is visible        |
        \*-----------------------------------------------------*/
        font.setPixelSize(std::max<int>(1, posh / 2));
        painter.setFont(font);

        unsigned int luma = (unsigned int)(0.2126f * currentColor.red() + 0.7152f * currentColor.green() + 0.0722f * currentColor.blue());

        if(luma > 127)
        {
            painter.setPen(Qt::black);
        }
        else
        {
            painter.setPen(Qt::white);
        }
        painter.drawText(rect, Qt::AlignVCenter | Qt::AlignHCenter, QString(led_labels[led_idx]));
    }

    font.setPixelSize(12);
    painter.setFont(font);

    /*-----------------------------------------------------*\
    | Zone and Segment names                                |
    \*-----------------------------------------------------*/
    unsigned int segment_count = 0;

    for(std::size_t zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
    {
        int posx = zone_pos[zone_idx].matrix_x * size + offset_x;
        int posy = zone_pos[zone_idx].matrix_y * size;
        int posw = zone_pos[zone_idx].matrix_w * size;
        int posh = zone_pos[zone_idx].matrix_h * size;

        QRect rect = {posx, posy, posw, posh};

        if(rect.contains(lastMousePos) && (!mouseDown || !mouseMoved))
        {
            painter.setPen(palette().highlight().color());
        }
        else
        {
            painter.setPen(palette().windowText().color());
        }
        painter.drawText(posx, posy + posh, QString(controller->GetZoneName((unsigned int)zone_idx).c_str()));

        for(std::size_t segment_idx = 0; segment_idx < controller->zones[zone_idx].segments.size(); segment_idx++)
        {
            posx = segment_pos[segment_count].matrix_x * size + offset_x;
            posy = segment_pos[segment_count].matrix_y * size;
            posw = segment_pos[segment_count].matrix_w * size;
            posh = segment_pos[segment_count].matrix_h * size;

            segment_count++;

            rect = {posx, posy, posw, posh};

            if(rect.contains(lastMousePos) && (!mouseDown || !mouseMoved))
            {
                painter.setPen(palette().highlight().color());
            }
            else
            {
                painter.setPen(palette().windowText().color());
            }
            painter.drawText(posx, posy + posh, QString(controller->zones[zone_idx].segments[segment_idx].name.c_str()));
        }
    }

    /*-----------------------------------------------------*\
    | Selection area                                        |
    \*-----------------------------------------------------*/
    if(mouseDown)
    {
        QRect rect = selectionRect.normalized();
        QColor color = palette().highlight().color();
        color.setAlpha(63);
        painter.fillRect(rect, color);
        color.setAlpha(127);
        painter.setBrush(color);
        painter.drawRect(rect);
    }
}

void DeviceView::updateSelection()
{
    selectedLeds.clear();
    selectionFlags.clear();
    selectionFlags.resize((int)controller->leds.size());

    QRect sel              = selectionRect.normalized();
    std::vector<led>& leds = controller->leds;

    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        /*-----------------------------------------------------*\
        | Check intersection                                    |
        \*-----------------------------------------------------*/
        int posx = led_pos[led_idx].matrix_x * size + offset_x;
        int posy = led_pos[led_idx].matrix_y * size;
        int posw = led_pos[led_idx].matrix_w * size;
        int posh = led_pos[led_idx].matrix_h * size;

        QRect rect = {posx, posy, posw, posh};

        selectionFlags[led_idx] = 0;

        if(sel.intersects(rect))
        {
            selectionFlags[led_idx] = 1;
        }
        if(ctrlDown)
        {
            selectionFlags[led_idx] ^= previousFlags[led_idx];
        }

        if(selectionFlags[led_idx])
        {
            selectedLeds.push_back(led_idx);
        }
    }

    update();

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(selectedLeds);
}

bool DeviceView::selectLed(int target)
{
    if(target < 0 || size_t(target) >= controller->leds.size())
    {
        return false;
    }

    selectedLeds.resize(1);
    selectedLeds[0] = target;
    selectionFlags.clear();
    selectionFlags.resize((int)controller->leds.size());
    selectionFlags[target] = 1;

    update();

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(selectedLeds);

    return true;
}

bool DeviceView::selectLeds(QVector<int> target)
{
    for(int item: target)
    {
        if(item < 0 || size_t(item) >= controller->leds.size())
        {
            return false;
        }
    }

    selectionFlags.clear();
    selectionFlags.resize((int)controller->leds.size());

    for(int item: target)
    {
        selectionFlags[item] = 1;
    }

    //selectedLeds = target;

    /*-----------------------------------------------------*\
    | Filter out duplicate items                            |
    \*-----------------------------------------------------*/
    selectedLeds.clear();

    for(int i = 0; i < selectionFlags.size(); ++i)
    {
        if(selectionFlags[i])
        {
            selectedLeds.push_back(i);
        }
    }

    update();

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(selectedLeds);

    return true;
}

bool DeviceView::selectSegment(int zone, int segment, bool add)
{
    if(zone < 0 || size_t(zone) >= controller->zones.size())
    {
        return false;
    }

    if(segment < 0 || size_t(segment) >= controller->zones[zone].segments.size())
    {
        return false;
    }

    if(!add)
    {
        selectedLeds.clear();
        selectionFlags.clear();
        selectionFlags.resize((int)controller->leds.size());
    }

    int zoneStart = controller->zones[zone].start_idx;
    int segStart = controller->zones[zone].segments[segment].start_idx;

    for(int led_idx = 0; led_idx < (int)controller->zones[zone].segments[segment].leds_count; led_idx++)
    {
        if(!selectionFlags[zoneStart + segStart + led_idx])
        {
            selectedLeds.push_back(zoneStart + segStart + led_idx);
            selectionFlags[zoneStart + segStart + led_idx] = 1;
        }
    }

    update();

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(selectedLeds);

    return true;
}

bool DeviceView::selectZone(int zone, bool add)
{
    if(zone < 0 || size_t(zone) >= controller->zones.size())
    {
        return false;
    }

    if(!add)
    {
        selectedLeds.clear();
        selectionFlags.clear();
        selectionFlags.resize((int)controller->leds.size());
    }

    int zoneStart = controller->zones[zone].start_idx;

    for(std::size_t led_idx = 0; led_idx < controller->GetLEDsInZone(zone); led_idx++)
    {
        if(!selectionFlags[zoneStart + (int)led_idx])
        {
            selectedLeds.push_back(zoneStart + (int)led_idx);
            selectionFlags[zoneStart + (int)led_idx] = 1;
        }
    }

    update();

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(selectedLeds);

    return true;
}

void DeviceView::clearSelection()
{
    /*-----------------------------------------------------*\
    | Same as selecting the entire device                   |
    \*-----------------------------------------------------*/
    selectedLeds.clear();
    selectionFlags.clear();
    selectionFlags.resize((int)controller->leds.size());
}

void DeviceView::setSelectionColor(RGBColor color)
{
    if(selectedLeds.isEmpty())
    {
        controller->SetAllLEDs(color);
    }
    else
    {
        for(int led_idx: selectedLeds)
        {
            controller->SetLED(led_idx, color);
        }
    }
    controller->UpdateLEDs();
    update();
}
