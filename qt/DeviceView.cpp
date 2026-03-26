/*---------------------------------------------------------*\
| DeviceView.cpp                                            |
|                                                           |
|   OpenRGB Device view widget for Qt                       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

DeviceView::DeviceView(QWidget *parent) :
    QWidget(parent),
    init_size(128,128)
{
    /*-----------------------------------------------------*\
    | Initialize variables                                  |
    \*-----------------------------------------------------*/
    changed             = false;
    controller          = NULL;
    mouse_down          = false;
    numerical_labels    = false;
    per_led             = true;
    size                = width();

    /*-----------------------------------------------------*\
    | Enable mouse tracking                                 |
    \*-----------------------------------------------------*/
    setMouseTracking(1);
}

DeviceView::~DeviceView()
{

}

/*---------------------------------------------------------*\
| Qt size hints                                             |
\*---------------------------------------------------------*/
QSize DeviceView::minimumSizeHint() const
{
    return(init_size);
}

QSize DeviceView::sizeHint() const
{
    return(QSize(height() - 1, height() - 1));
}

/*---------------------------------------------------------*\
| Selection functions                                       |
\*---------------------------------------------------------*/
bool DeviceView::SelectLED(std::size_t led_idx)
{
    /*-----------------------------------------------------*\
    | Check validity                                        |
    \*-----------------------------------------------------*/
    if(led_idx >= controller->GetLEDCount())
    {
        return(false);
    }

    /*-----------------------------------------------------*\
    | Set selection                                         |
    \*-----------------------------------------------------*/
    selected_leds.resize(1);
    selected_leds[0] = led_idx;

    selection_flags.clear();
    selection_flags.resize(controller->GetLEDCount());
    selection_flags[led_idx] = 1;

    /*-----------------------------------------------------*\
    | Update UI                                             |
    \*-----------------------------------------------------*/
    update();

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(-1, -1, selected_leds);

    return(true);
}

bool DeviceView::SelectLEDs(std::vector<std::size_t> leds)
{
    /*-----------------------------------------------------*\
    | Check validity                                        |
    \*-----------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        if(leds[led_idx] >= controller->GetLEDCount())
        {
            return(false);
        }
    }

    /*-----------------------------------------------------*\
    | Set selection                                         |
    \*-----------------------------------------------------*/
    selection_flags.clear();
    selection_flags.resize(controller->GetLEDCount());

    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        selection_flags[led_idx] = 1;
    }

    /*-----------------------------------------------------*\
    | Filter out duplicate items                            |
    \*-----------------------------------------------------*/
    selected_leds.clear();

    for(std::size_t led_idx = 0; led_idx < selection_flags.size(); led_idx++)
    {
        if(selection_flags[led_idx])
        {
            selected_leds.push_back(led_idx);
        }
    }

    /*-----------------------------------------------------*\
    | Update UI                                             |
    \*-----------------------------------------------------*/
    update();

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(-1, -1, selected_leds);

    return(true);
}

bool DeviceView::SelectSegment(std::size_t zone_idx, std::size_t segment_idx, bool add)
{
    /*-----------------------------------------------------*\
    | Check validity                                        |
    \*-----------------------------------------------------*/
    if(zone_idx >= controller->GetZoneCount())
    {
        return(false);
    }

    if(segment_idx >= controller->GetZoneSegmentCount(zone_idx))
    {
        return(false);
    }

    /*-----------------------------------------------------*\
    | If not adding to the current selection, clear the     |
    | existing selection                                    |
    \*-----------------------------------------------------*/
    if(!add)
    {
        selected_leds.clear();
        selection_flags.clear();
        selection_flags.resize(controller->GetLEDCount());
    }

    /*-----------------------------------------------------*\
    | Add segment LEDs to selection                         |
    \*-----------------------------------------------------*/
    unsigned int zone_start     = controller->GetZoneStartIndex(zone_idx);
    unsigned int segment_start  = controller->GetZoneSegmentStartIndex(zone_idx, segment_idx);

    for(unsigned int led_idx = 0; led_idx < controller->GetZoneSegmentLEDsCount(zone_idx, segment_idx); led_idx++)
    {
        if(!selection_flags[zone_start + segment_start + led_idx])
        {
            selected_leds.push_back(zone_start + segment_start + led_idx);
            selection_flags[zone_start + segment_start + led_idx] = 1;
        }
    }

    /*-----------------------------------------------------*\
    | Update UI                                             |
    \*-----------------------------------------------------*/
    update();

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    if(!add)
    {
        emit selectionChanged(zone_idx, segment_idx, selected_leds);
    }
    else
    {
        emit selectionChanged(-1, -1, selected_leds);
    }

    return(true);
}

bool DeviceView::SelectZone(std::size_t zone_idx, bool add)
{
    /*-----------------------------------------------------*\
    | Check validity                                        |
    \*-----------------------------------------------------*/
    if(zone_idx >= controller->GetZoneCount())
    {
        return(false);
    }

    /*-----------------------------------------------------*\
    | If not adding to the current selection, clear the     |
    | existing selection                                    |
    \*-----------------------------------------------------*/
    if(!add)
    {
        selected_leds.clear();
        selection_flags.clear();
        selection_flags.resize(controller->GetLEDCount());
    }

    /*-----------------------------------------------------*\
    | Add zone LEDs to selection                            |
    \*-----------------------------------------------------*/
    unsigned int zone_start     = controller->GetZoneStartIndex(zone_idx);

    for(unsigned int led_idx = 0; led_idx < controller->GetLEDsInZone(zone_idx); led_idx++)
    {
        if(!selection_flags[zone_start + led_idx])
        {
            selected_leds.push_back(zone_start + led_idx);
            selection_flags[zone_start + led_idx] = 1;
        }
    }

    /*-----------------------------------------------------*\
    | Update UI                                             |
    \*-----------------------------------------------------*/
    update();

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    if(!add)
    {
        emit(selectionChanged(zone_idx, -1, selected_leds));
    }
    else
    {
        emit selectionChanged(-1, -1, selected_leds);
    }

    return(true);
}

/*---------------------------------------------------------*\
| Setter functions                                          |
\*---------------------------------------------------------*/
void DeviceView::ClearSelection()
{
    /*-----------------------------------------------------*\
    | Same as selecting the entire device                   |
    \*-----------------------------------------------------*/
    selected_leds.clear();
    selection_flags.clear();
    selection_flags.resize(controller->GetLEDCount());
}

void DeviceView::SetChanged()
{
    /*-----------------------------------------------------*\
    | Set the changed flag                                  |
    \*-----------------------------------------------------*/
    changed = true;
}

void DeviceView::SetController(RGBController * controller_ptr)
{
    /*-----------------------------------------------------*\
    | Store the controller pointer                          |
    \*-----------------------------------------------------*/
    controller = controller_ptr;
}

void DeviceView::SetNumericalLabels(bool enable)
{
    /*-----------------------------------------------------*\
    | Store the numerical labels flag                       |
    \*-----------------------------------------------------*/
    numerical_labels = enable;
}

void DeviceView::SetPerLED(bool per_led_mode)
{
    /*-----------------------------------------------------*\
    | Store the Per LED flag                                |
    \*-----------------------------------------------------*/
    per_led = per_led_mode;

    /*-----------------------------------------------------*\
    | Update UI                                             |
    \*-----------------------------------------------------*/
    update();
}

void DeviceView::SetSelectionColor(RGBColor color)
{
    /*-----------------------------------------------------*\
    | If no LEDs are selected, set entire device using the  |
    | SetAllColors function                                 |
    \*-----------------------------------------------------*/
    if(selected_leds.empty())
    {
        controller->SetAllColors(color);
    }
    /*-----------------------------------------------------*\
    | Otherwise, set the individual selected LEDs using the |
    | SetColor function                                     |
    \*-----------------------------------------------------*/
    else
    {
        for(std::size_t led_idx = 0; led_idx < selected_leds.size(); led_idx++)
        {
            controller->SetColor(selected_leds[led_idx], color);
        }
    }

    /*-----------------------------------------------------*\
    | Update the device to apply the new colors             |
    \*-----------------------------------------------------*/
    controller->UpdateLEDs();

    /*-----------------------------------------------------*\
    | Update UI                                             |
    \*-----------------------------------------------------*/
    update();
}

/*---------------------------------------------------------*\
| Qt events                                                 |
\*---------------------------------------------------------*/
void DeviceView::mousePressEvent(QMouseEvent *event)
{
    /*-----------------------------------------------------*\
    | Only handle events when in per-LED mode               |
    \*-----------------------------------------------------*/
    if(per_led)
    {
        /*-------------------------------------------------*\
        | Set flags                                         |
        \*-------------------------------------------------*/
        ctrl_down                = event->modifiers().testFlag(Qt::ControlModifier);
        mouse_down               = true;
        mouse_moved              = false;

        /*-------------------------------------------------*\
        | If CTRL is held, store the previous selection     |
        \*-------------------------------------------------*/
        if(ctrl_down)
        {
            previous_flags      = selection_flags;
            previous_selection  = selected_leds;
        }

        /*-------------------------------------------------*\
        | Initialize the selection rect to the mouse press  |
        | position.  It's okay if the size becomes negative |
        \*-------------------------------------------------*/
        #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            selection_rect.setLeft(event->position().x());
            selection_rect.setTop(event->position().y());
            selection_rect.setRight(event->position().x());
            selection_rect.setBottom(event->position().y());
        #else
            selection_rect.setLeft(event->x());
            selection_rect.setTop(event->y());
            selection_rect.setRight(event->x());
            selection_rect.setBottom(event->y());
        #endif

        /*-------------------------------------------------*\
        | Update selection                                  |
        \*-------------------------------------------------*/
        UpdateSelection();

        /*-------------------------------------------------*\
        | Update UI                                         |
        \*-------------------------------------------------*/
        update();
    }
}

void DeviceView::mouseMoveEvent(QMouseEvent *event)
{
    /*-----------------------------------------------------*\
    | Only handle events when in per-LED mode               |
    \*-----------------------------------------------------*/
    if(per_led)
    {
        /*-------------------------------------------------*\
        | Set last mouse point                              |
        \*-------------------------------------------------*/
        last_mouse_point        = event->pos();

        /*-------------------------------------------------*\
        | Update the selection rect to the current mouse    |
        | position                                          |
        \*-------------------------------------------------*/
        #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            selection_rect.setRight(event->position().x());
            selection_rect.setBottom(event->position().y());
        #else
            selection_rect.setRight(event->x());
            selection_rect.setBottom(event->y());
        #endif

        /*-------------------------------------------------*\
        | If mouse is held, update selection                |
        \*-------------------------------------------------*/
        if(mouse_down)
        {
            /*---------------------------------------------*\
            | Set flags                                     |
            \*---------------------------------------------*/
            ctrl_down           = event->modifiers().testFlag(Qt::ControlModifier);
            mouse_moved         = true;

            /*---------------------------------------------*\
            | If CTRL is released, clear the previous       |
            | selection.                                    |
            \*---------------------------------------------*/
            if(!ctrl_down)
            {
                previous_selection.clear();
                previous_flags.clear();
                previous_flags.resize(controller->GetLEDCount());
            }

            /*---------------------------------------------*\
            | Update selection                              |
            \*---------------------------------------------*/
            UpdateSelection();
        }

        /*-------------------------------------------------*\
        | Update UI                                         |
        \*-------------------------------------------------*/
        update();
    }
}

void DeviceView::mouseReleaseEvent(QMouseEvent* event)
{
    /*-----------------------------------------------------*\
    | Only handle events when in per-LED mode               |
    \*-----------------------------------------------------*/
    if(per_led)
    {
        /*-------------------------------------------------*\
        | Clear the mouse down flag and normalize the       |
        | selection rect                                    |
        \*-------------------------------------------------*/
        mouse_down                      = false;
        selection_rect                  = selection_rect.normalized();

        /*-------------------------------------------------*\
        | Check if the user clicked a zone or segment name  |
        \*-------------------------------------------------*/
        if(!mouse_moved)
        {
            /*---------------------------------------------*\
            | Set size and X offset based on width          |
            \*---------------------------------------------*/
            int size                    = width();
            int offset_x                = 0;

            /*---------------------------------------------*\
            | If height is less than width, set size and X  |
            | offset based on height                        |
            \*---------------------------------------------*/
            if(height() < (size * matrix_h))
            {
                size                    = (height() / matrix_h);
                offset_x                = ((width() - size) / 2);
            }

            /*---------------------------------------------*\
            | Loop through each zone and check if the click |
            | position is within the rect for the zone name |
            | label                                         |
            \*---------------------------------------------*/
            unsigned int segment_count  = 0;

            for(unsigned int zone_idx = 0; zone_idx < controller->GetZoneCount(); zone_idx++)
            {
                int posx                = zone_pos[zone_idx].matrix_x * size + offset_x + 12;
                int posy                = zone_pos[zone_idx].matrix_y * size;
                int posw                = zone_pos[zone_idx].matrix_w * size;
                int posh                = zone_pos[zone_idx].matrix_h * size;

                QRect rect              = {posx, posy, posw, posh};

                if(rect.contains(event->pos()))
                {
                    SelectZone(zone_idx, ctrl_down);
                }

                /*-----------------------------------------*\
                | Loop through each segment and check if    |
                | the click position is within the rect for |
                | the segment name label                    |
                \*-----------------------------------------*/
                for(unsigned int segment_idx = 0; segment_idx < controller->GetZoneSegmentCount(zone_idx); segment_idx++)
                {
                    posx                = segment_pos[segment_count].matrix_x * size + offset_x + 12;
                    posy                = segment_pos[segment_count].matrix_y * size;
                    posw                = segment_pos[segment_count].matrix_w * size;
                    posh                = segment_pos[segment_count].matrix_h * size;

                    segment_count++;

                    rect                = {posx, posy, posw, posh};

                    if(rect.contains(event->pos()))
                    {
                        SelectSegment(zone_idx, segment_idx, ctrl_down);
                    }
                }
            }
        }

        /*-------------------------------------------------*\
        | Update UI                                         |
        \*-------------------------------------------------*/
        update();
    }
}

void DeviceView::resizeEvent(QResizeEvent* /*event*/)
{
    /*-----------------------------------------------------*\
    | Set size and X offset based on width                  |
    \*-----------------------------------------------------*/
    size                        = width();
    offset_x                    = 0;

    /*-----------------------------------------------------*\
    | If height is less than width, set size and X offset   |
    | based on height                                       |
    \*-----------------------------------------------------*/
    if(height() < (size * matrix_h))
    {
        size                    = (height() / matrix_h);
        offset_x                = ((width() - size) / 2);
    }

    /*-----------------------------------------------------*\
    | Update UI                                             |
    \*-----------------------------------------------------*/
    update();
}

void DeviceView::paintEvent(QPaintEvent* /* event */)
{
    /*-----------------------------------------------------*\
    | If Device View is hidden, don't paint                 |
    \*-----------------------------------------------------*/
    if(isHidden() || !per_led)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Create painter and font                               |
    \*-----------------------------------------------------*/
    QPainter    painter(this);
    QFont       font            = painter.font();

    /*-----------------------------------------------------*\
    | If controller has resized, reinitialize local data    |
    \*-----------------------------------------------------*/
    if(changed || (controller->GetZoneCount() != zone_pos.size()) || (controller->GetLEDCount() != led_pos.size()))
    {
        InitDeviceView();
        changed = false;
    }

    /*-----------------------------------------------------*\
    | If segments have resized, reinitialize local data     |
    \*-----------------------------------------------------*/
    unsigned int segments = 0;

    for(std::size_t zone_idx = 0; zone_idx < controller->GetZoneCount(); zone_idx++)
    {
        for(std::size_t segment_idx = 0; segment_idx < controller->GetZoneSegmentCount(zone_idx); segment_idx++)
        {
            segments++;
        }
    }

    if(segments != segment_pos.size())
    {
        InitDeviceView();
    }

    /*-----------------------------------------------------*\
    | Paint LED rectangles                                  |
    \*-----------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        /*-------------------------------------------------*\
        | Determine position and size                       |
        \*-------------------------------------------------*/
        int posx                = led_pos[led_idx].matrix_x * size + offset_x;
        int posy                = led_pos[led_idx].matrix_y * size;
        int posw                = led_pos[led_idx].matrix_w * size;
        int posh                = led_pos[led_idx].matrix_h * size;

        /*-------------------------------------------------*\
        | Create rect                                       |
        \*-------------------------------------------------*/
        QRect rect              = {posx, posy, posw, posh};

        /*-------------------------------------------------*\
        | Set Fill color                                    |
        \*-------------------------------------------------*/
        QColor currentColor     = QColor::fromRgb(RGBGetRValue(controller->GetColor(led_idx)),
                                                  RGBGetGValue(controller->GetColor(led_idx)),
                                                  RGBGetBValue(controller->GetColor(led_idx)));
        painter.setBrush(currentColor);

        /*-------------------------------------------------*\
        | Set Border color                                  |
        \*-------------------------------------------------*/
        if(selection_flags[led_idx])
        {
            painter.setPen(palette().highlight().color());
        }
        else
        {
            painter.setPen(palette().dark().color());
        }

        /*-------------------------------------------------*\
        | Draw LED rectangle                                |
        \*-------------------------------------------------*/
        painter.drawRect(rect);

        /*-------------------------------------------------*\
        | LED Label                                         |
        | Set the font color so that the text is visible    |
        \*-------------------------------------------------*/
        font.setPixelSize(std::max<int>(1, posh / 2));
        painter.setFont(font);

        /*-------------------------------------------------*\
        | Determine whether label should be drawn in white  |
        | or black based on luma of the LED's color         |
        \*-------------------------------------------------*/
        unsigned int luma       = (unsigned int)(0.2126f * currentColor.red() + 0.7152f * currentColor.green() + 0.0722f * currentColor.blue());

        if(luma > 127)
        {
            painter.setPen(Qt::black);
        }
        else
        {
            painter.setPen(Qt::white);
        }

        /*-------------------------------------------------*\
        | Draw LED label on LED rectangle                   |
        \*-------------------------------------------------*/
        painter.drawText(rect, Qt::AlignVCenter | Qt::AlignHCenter, QString(led_labels[led_idx]));
    }

    /*-----------------------------------------------------*\
    | Change font size for drawing zone and segment names   |
    \*-----------------------------------------------------*/
    font.setPixelSize(12);
    painter.setFont(font);

    /*-----------------------------------------------------*\
    | Paint Zone and Segment names                          |
    \*-----------------------------------------------------*/
    unsigned int segment_count  = 0;

    for(std::size_t zone_idx = 0; zone_idx < controller->GetZoneCount(); zone_idx++)
    {
        /*-------------------------------------------------*\
        | Determine position and size for zone name         |
        \*-------------------------------------------------*/
        int posx                = zone_pos[zone_idx].matrix_x * size + offset_x;
        int posy                = zone_pos[zone_idx].matrix_y * size;
        int posw                = zone_pos[zone_idx].matrix_w * size;
        int posh                = zone_pos[zone_idx].matrix_h * size;

        /*-------------------------------------------------*\
        | Create rect                                       |
        \*-------------------------------------------------*/
        QRect rect              = {posx, posy, posw, posh};

        /*-------------------------------------------------*\
        | If the mouse is hovering inside of this rect,     |
        | draw the name in the highlight color instead of   |
        | the window text color                             |
        \*-------------------------------------------------*/
        if(rect.contains(last_mouse_point) && (!mouse_down || !mouse_moved))
        {
            painter.setPen(palette().highlight().color());
        }
        else
        {
            painter.setPen(palette().windowText().color());
        }

        /*-------------------------------------------------*\
        | Draw zone name                                    |
        \*-------------------------------------------------*/
        painter.drawText(posx, posy + posh, QString(controller->GetZoneName((unsigned int)zone_idx).c_str()));

        for(std::size_t segment_idx = 0; segment_idx < controller->GetZoneSegmentCount(zone_idx); segment_idx++)
        {
            /*---------------------------------------------*\
            | Determine position and size for segment name  |
            \*---------------------------------------------*/
            posx                = segment_pos[segment_count].matrix_x * size + offset_x;
            posy                = segment_pos[segment_count].matrix_y * size;
            posw                = segment_pos[segment_count].matrix_w * size;
            posh                = segment_pos[segment_count].matrix_h * size;

            segment_count++;

            /*---------------------------------------------*\
            | Create rect                                   |
            \*---------------------------------------------*/
            rect                = {posx, posy, posw, posh};

            /*---------------------------------------------*\
            | If the mouse is hovering inside of this rect, |
            | draw the name in the highlight color instead  |
            | of the window text color                      |
            \*---------------------------------------------*/
            if(rect.contains(last_mouse_point) && (!mouse_down || !mouse_moved))
            {
                painter.setPen(palette().highlight().color());
            }
            else
            {
                painter.setPen(palette().windowText().color());
            }

            /*---------------------------------------------*\
            | Draw segment name                             |
            \*---------------------------------------------*/
            painter.drawText(posx, posy + posh, QString(controller->GetZoneSegmentName(zone_idx, segment_idx).c_str()));
        }
    }

    /*-----------------------------------------------------*\
    | Draw selection area                                   |
    \*-----------------------------------------------------*/
    if(mouse_down)
    {
        /*-------------------------------------------------*\
        | Create rect based on normalized selection         |
        \*-------------------------------------------------*/
        QRect rect              = selection_rect.normalized();

        /*-------------------------------------------------*\
        | Use highlight color                               |
        \*-------------------------------------------------*/
        QColor color            = palette().highlight().color();

        /*-------------------------------------------------*\
        | Draw fill with transparent highlight color        |
        \*-------------------------------------------------*/
        color.setAlpha(63);
        painter.fillRect(rect, color);

        /*-------------------------------------------------*\
        | Draw border with more opaque highlight color      |
        \*-------------------------------------------------*/
        color.setAlpha(127);
        painter.setBrush(color);

        /*-------------------------------------------------*\
        | Draw the rect                                     |
        \*-------------------------------------------------*/
        painter.drawRect(rect);
    }
}

/*---------------------------------------------------------*\
| Private functions                                         |
\*---------------------------------------------------------*/
void DeviceView::InitDeviceView()
{
    /*-----------------------------------------------------*\
    | Set the size of the selection flags vector            |
    \*-----------------------------------------------------*/
    selection_flags.resize((int)controller->GetLEDCount());

    /*-----------------------------------------------------*\
    | Set the size of the zone and LED position vectors     |
    \*-----------------------------------------------------*/
    zone_pos.resize(controller->GetZoneCount());
    led_pos.resize(controller->GetLEDCount());
    led_labels.resize(controller->GetLEDCount());

    /*-----------------------------------------------------*\
    | Process position and size for zones                   |
    \*-----------------------------------------------------*/
    unsigned int        max_width           = 0;
    unsigned int        segment_count       = 0;
    float               total_height        = 0.0f;

    /*-----------------------------------------------------*\
    | Get device view settings                              |
    \*-----------------------------------------------------*/
    SettingsManager*    settings_manager    = ResourceManager::get()->GetSettingsManager();
    std::string         ui_string           = "UserInterface";
    json                ui_settings         = settings_manager->GetSettings(ui_string);
    bool                disable_expansion   = false;

    if(ui_settings.contains("disable_key_expansion"))
    {
        disable_expansion                   = ui_settings["disable_key_expansion"];
    }

    /*-----------------------------------------------------*\
    | Determine the total height (in LEDs) of all zones     |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < controller->GetZoneCount(); zone_idx++)
    {
        /*-------------------------------------------------*\
        | For matrix zones, use matrix height from the map  |
        \*-------------------------------------------------*/
        if(controller->GetZoneType(zone_idx) == ZONE_TYPE_MATRIX)
        {
            total_height                   += controller->GetZoneMatrixMapHeight(zone_idx);
            zone_pos[zone_idx].matrix_w     = controller->GetZoneMatrixMapWidth(zone_idx);
        }
        /*-------------------------------------------------*\
        | For all other zones, compute the height including |
        | wrap-around                                       |
        \*-------------------------------------------------*/
        else if(controller->GetZoneSegmentCount(zone_idx) > 0)
        {
            for(std::size_t segment_idx = 0; segment_idx < controller->GetZoneSegmentCount(zone_idx); segment_idx++)
            {
                if(controller->GetZoneSegmentType(zone_idx, segment_idx) == ZONE_TYPE_MATRIX)
                {
                    total_height               += controller->GetZoneSegmentMatrixMapHeight(zone_idx, segment_idx);
                    zone_pos[zone_idx].matrix_w = controller->GetZoneSegmentMatrixMapWidth(zone_idx, segment_idx);
                }
                else
                {
                    unsigned int count          = controller->GetZoneSegmentLEDsCount(zone_idx, segment_idx);
                    zone_pos[zone_idx].matrix_w = std::min(count, (unsigned int)MAX_COLS);
                    total_height                += (count / MAX_COLS) + ((count % MAX_COLS) > 0);
                }
                segment_count++;
            }
        }
        else
        {
            unsigned int count              = controller->GetLEDsInZone((unsigned int)zone_idx);
            zone_pos[zone_idx].matrix_w     = std::min(count, (unsigned int)MAX_COLS);
            total_height                   += (count / MAX_COLS) + ((count % MAX_COLS) > 0);
        }

        /*-------------------------------------------------*\
        | Determine the maximum width (in LEDs) in the view |
        \*-------------------------------------------------*/
        if(zone_pos[zone_idx].matrix_w > max_width)
        {
            max_width                       = zone_pos[zone_idx].matrix_w;
        }
    }

    segment_pos.resize(segment_count);

    /*-----------------------------------------------------*\
    | Add some space for zone names and padding             |
    \*-----------------------------------------------------*/
    total_height                           += controller->GetZoneCount() * PAD_ZONE;
    total_height                           += segment_count * PAD_SEGMENT;

    /*-----------------------------------------------------*\
    | We will be descending, placing each zone one unit     |
    | below the previous one                                |
    \*-----------------------------------------------------*/
    float current_y                         = 0;
    matrix_h                                = total_height;
    segment_count                           = 0;

    for(std::size_t zone_idx = 0; zone_idx < controller->GetZoneCount(); zone_idx++)
    {
        /*-------------------------------------------------*\
        | Calculate zone label position and size            |
        \*-------------------------------------------------*/
        zone_pos[zone_idx].matrix_x         = (max_width - zone_pos[zone_idx].matrix_w) / 2.0f;
        zone_pos[zone_idx].matrix_y         = current_y + SIZE_TEXT;
        zone_pos[zone_idx].matrix_h         = SIZE_TEXT - PAD_TEXT;
        current_y                          += PAD_ZONE;

        /*-------------------------------------------------*\
        | Calculate LEDs position and size for zone         |
        \*-------------------------------------------------*/
        if(controller->GetZoneType(zone_idx) == ZONE_TYPE_MATRIX)
        {
            for(unsigned int led_x = 0; led_x < controller->GetZoneMatrixMapWidth(zone_idx); led_x++)
            {
                for(unsigned int led_y = 0; led_y < controller->GetZoneMatrixMapHeight(zone_idx); led_y++)
                {
                    unsigned int map_idx    = led_y * controller->GetZoneMatrixMapWidth(zone_idx) + led_x;
                    unsigned int color_idx  = controller->GetZoneMatrixMapData(zone_idx)[map_idx] + controller->GetZoneStartIndex(zone_idx);

                    if(controller->GetZoneMatrixMapData(zone_idx)[map_idx] != 0xFFFFFFFF && color_idx < led_pos.size())
                    {
                        led_pos[color_idx].matrix_x = (zone_pos[zone_idx].matrix_x + led_x + PAD_LED);
                        led_pos[color_idx].matrix_y = current_y + (led_y + PAD_LED);

                        /*---------------------------------*\
                        | LED is a 1x1 square, minus        |
                        | padding on all sides              |
                        \*---------------------------------*/
                        led_pos[color_idx].matrix_w = (1.0f - (2.0f * PAD_LED));
                        led_pos[color_idx].matrix_h = (1.0f - (2.0f * PAD_LED));

                        if(!disable_expansion)
                        {
                            /*-----------------------------*\
                            | Expand large keys to fill     |
                            | empty spaces in matrix, if    |
                            | possible.  Large keys can     |
                            | fill left, down, up, or wide  |
                            | Fill Left:                    |
                            |    Tab                        |
                            |    Caps Lock                  |
                            |    Left Shift                 |
                            |    Right Shift                |
                            |    Backspace                  |
                            |    Number Pad 0               |
                            |                               |
                            | Fill Up or Down:              |
                            |    Number Pad Enter           |
                            |    Number Pad +               |
                            |                               |
                            | Fill Wide:                    |
                            |    Space                      |
                            \*-----------------------------*/
                            if(led_x < controller->GetZoneMatrixMapWidth(zone_idx) - 1 && controller->GetZoneMatrixMapData(zone_idx)[map_idx + 1] == 0xFFFFFFFF)
                            {
                                if( ( controller->GetLEDDisplayName(color_idx) == KEY_EN_TAB        )
                                 || ( controller->GetLEDDisplayName(color_idx) == KEY_EN_CAPS_LOCK  )
                                 || ( controller->GetLEDDisplayName(color_idx) == KEY_EN_LEFT_SHIFT )
                                 || ( controller->GetLEDDisplayName(color_idx) == KEY_EN_RIGHT_SHIFT)
                                 || ( controller->GetLEDDisplayName(color_idx) == KEY_EN_BACKSPACE  )
                                 || ( controller->GetLEDDisplayName(color_idx) == KEY_EN_NUMPAD_0   ) )
                                {
                                    led_pos[color_idx].matrix_w += 1.0f;
                                }
                            }
                            if( ( controller->GetLEDDisplayName(color_idx) == KEY_EN_NUMPAD_ENTER   )
                             || ( controller->GetLEDDisplayName(color_idx) == KEY_EN_NUMPAD_PLUS    ) )
                            {
                                if(led_y < controller->GetZoneMatrixMapHeight(zone_idx) - 1 && controller->GetZoneMatrixMapData(zone_idx)[map_idx + controller->GetZoneMatrixMapWidth(zone_idx)] == 0xFFFFFFFF)
                                {
                                    led_pos[color_idx].matrix_h += 1.0f;
                                }
                                /* TODO: check if there isn't another widened key above */
                                else if(led_y > 0 && controller->GetZoneMatrixMapData(zone_idx)[map_idx - controller->GetZoneMatrixMapWidth(zone_idx)] == 0xFFFFFFFF)
                                {
                                    led_pos[color_idx].matrix_y -= 1.0f;
                                    led_pos[color_idx].matrix_h += 1.0f;
                                }
                            }
                            else if(controller->GetLEDDisplayName(color_idx) == KEY_EN_SPACE)
                            {
                                for(unsigned int map_idx2 = map_idx - 1; map_idx2 > led_y * controller->GetZoneMatrixMapWidth(zone_idx) && controller->GetZoneMatrixMapData(zone_idx)[map_idx2] == 0xFFFFFFFF; map_idx2--)
                                {
                                    led_pos[color_idx].matrix_x -= 1.0f;
                                    led_pos[color_idx].matrix_w += 1.0f;
                                }
                                for(unsigned int map_idx2 = map_idx + 1; map_idx2 < (led_y + 1) * controller->GetZoneMatrixMapWidth(zone_idx) && controller->GetZoneMatrixMapData(zone_idx)[map_idx2] == 0xFFFFFFFF; map_idx2++)
                                {
                                    led_pos[color_idx].matrix_w += 1.0f;
                                }
                            }
                        }
                    }
                }
            }

            current_y += controller->GetZoneMatrixMapHeight(zone_idx);
        }
        else if(controller->GetZoneSegmentCount(zone_idx) > 0)
        {
            for(std::size_t segment_idx = 0; segment_idx < controller->GetZoneSegmentCount(zone_idx); segment_idx++)
            {
                /*-----------------------------------------*\
                | Calculate segment label position and size |
                \*-----------------------------------------*/
                segment_pos[segment_count].matrix_x = (max_width - zone_pos[zone_idx].matrix_w) / 2.0f;
                segment_pos[segment_count].matrix_y = current_y + SIZE_TEXT;
                segment_pos[segment_count].matrix_w = zone_pos[zone_idx].matrix_w;
                segment_pos[segment_count].matrix_h = SIZE_TEXT - PAD_TEXT;
                current_y                          += PAD_SEGMENT;

                segment_count++;

                if(controller->GetZoneSegmentType(zone_idx, segment_idx) == ZONE_TYPE_MATRIX)
                {
                    for(unsigned int led_x = 0; led_x < controller->GetZoneSegmentMatrixMapWidth(zone_idx, segment_idx); led_x++)
                    {
                        for(unsigned int led_y = 0; led_y < controller->GetZoneSegmentMatrixMapHeight(zone_idx, segment_idx); led_y++)
                        {
                            unsigned int map_idx    = led_y * controller->GetZoneSegmentMatrixMapWidth(zone_idx, segment_idx) + led_x;
                            unsigned int color_idx  = controller->GetZoneSegmentMatrixMapData(zone_idx, segment_idx)[map_idx] + controller->GetZoneSegmentStartIndex(zone_idx, segment_idx);

                            if(controller->GetZoneSegmentMatrixMapData(zone_idx, segment_idx)[map_idx] != 0xFFFFFFFF && color_idx < led_pos.size())
                            {
                                led_pos[color_idx].matrix_x = (segment_pos[zone_idx].matrix_x + led_x + PAD_LED);
                                led_pos[color_idx].matrix_y = current_y + (led_y + PAD_LED);

                                /*-------------------------*\
                                | LED is a 1x1 square,      |
                                | minus padding on all      |
                                | sides                     |
                                \*-------------------------*/
                                led_pos[color_idx].matrix_w = (1.0f - (2.0f * PAD_LED));
                                led_pos[color_idx].matrix_h = (1.0f - (2.0f * PAD_LED));
                            }
                        }
                    }

                    current_y += controller->GetZoneSegmentMatrixMapHeight(zone_idx, segment_idx);
                }
                else
                {
                    /*-------------------------------------*\
                    | Calculate LED box positions for       |
                    | segmented zones                       |
                    \*-------------------------------------*/
                    unsigned int leds_count = controller->GetZoneSegmentLEDsCount(zone_idx, segment_idx);

                    for(unsigned int led_idx = 0; led_idx < leds_count; led_idx++)
                    {
                        unsigned int led_pos_idx = controller->GetZoneStartIndex(zone_idx) + controller->GetZoneSegmentStartIndex(zone_idx, segment_idx) + led_idx;

                        led_pos[led_pos_idx].matrix_x = zone_pos[zone_idx].matrix_x + ((led_idx % MAX_COLS) + PAD_LED);
                        led_pos[led_pos_idx].matrix_y = current_y + ((led_idx / MAX_COLS) + PAD_LED);

                        /*---------------------------------*\
                        | LED is a 1x1 square, minus        |
                        | padding on all sides              |
                        \*---------------------------------*/
                        led_pos[led_pos_idx].matrix_w = (1.0f - (2.0f * PAD_LED));
                        led_pos[led_pos_idx].matrix_h = (1.0f - (2.0f * PAD_LED));
                    }

                    current_y += (leds_count / MAX_COLS) + ((leds_count % MAX_COLS) > 0);
                }
            }
        }
        else
        {
            /*---------------------------------------------*\
            | Calculate LED box positions for single/linear |
            | zones                                         |
            \*---------------------------------------------*/
            unsigned int leds_count = controller->GetLEDsInZone((unsigned int)zone_idx);

            for(unsigned int led_idx = 0; led_idx < leds_count; led_idx++)
            {
                unsigned int led_pos_idx = controller->GetZoneStartIndex(zone_idx) + led_idx;

                led_pos[led_pos_idx].matrix_x = zone_pos[zone_idx].matrix_x + ((led_idx % MAX_COLS) + PAD_LED);
                led_pos[led_pos_idx].matrix_y = current_y + ((led_idx / MAX_COLS) + PAD_LED);

                /*-----------------------------------------*\
                | LED is a 1x1 square, minus padding on all |
                | sides                                     |
                \*-----------------------------------------*/
                led_pos[led_pos_idx].matrix_w = (1.0f - (2.0f * PAD_LED));
                led_pos[led_pos_idx].matrix_h = (1.0f - (2.0f * PAD_LED));
            }

            current_y += (leds_count / MAX_COLS) + ((leds_count % MAX_COLS) > 0);
        }
    }

    /*-----------------------------------------------------*\
    | Update LED labels                                     |
    \*-----------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        std::map<std::string, led_label>::const_iterator it = led_label_lookup.find(controller->GetLEDDisplayName((unsigned int)led_idx));

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
    float atom = 1.0f / max_width;

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

void DeviceView::UpdateSelection()
{
    /*-----------------------------------------------------*\
    | Clear existing selection                              |
    \*-----------------------------------------------------*/
    selected_leds.clear();
    selection_flags.clear();
    selection_flags.resize(controller->GetLEDCount());

    /*-----------------------------------------------------*\
    | Get normalized selection rect                         |
    \*-----------------------------------------------------*/
    QRect sel = selection_rect.normalized();

    for(unsigned int led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        /*-------------------------------------------------*\
        | Check intersection                                |
        \*-------------------------------------------------*/
        int posx = led_pos[led_idx].matrix_x * size + offset_x;
        int posy = led_pos[led_idx].matrix_y * size;
        int posw = led_pos[led_idx].matrix_w * size;
        int posh = led_pos[led_idx].matrix_h * size;

        QRect rect = {posx, posy, posw, posh};

        selection_flags[led_idx] = 0;

        if(sel.intersects(rect))
        {
            selection_flags[led_idx] = 1;
        }
        if(ctrl_down)
        {
            selection_flags[led_idx] = selection_flags[led_idx] ^ previous_flags[led_idx];
        }

        if(selection_flags[led_idx])
        {
            selected_leds.push_back(led_idx);
        }
    }

    /*-----------------------------------------------------*\
    | Update UI                                             |
    \*-----------------------------------------------------*/
    update();

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(-1, -1, selected_leds);
}
