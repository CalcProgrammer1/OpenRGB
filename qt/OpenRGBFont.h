/*---------------------------------------------------------*\
| OpenRGBFont.h                                             |
|                                                           |
|   Functionality for OpenRGB custom font icons             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QFont>
#include <QString>

class OpenRGBFont
{
public:
    static OpenRGBFont* Get();

    enum Glyph
    {
        bulb                = 0xF001,
        controller          = 0xF002,
        cooler              = 0xF003,
        data                = 0xF004,
        dram                = 0xF005,
        drive               = 0xF006,
        extension           = 0xF007,
        gamepad             = 0xF008,
        gpu                 = 0xF009,
        headset             = 0xF00A,
        headsetstand        = 0xF00B,
        info                = 0xF00C,
        keyboard            = 0xF00D,
        keypad              = 0xF00E,
        ledstrip            = 0xF00F,
        mainboard           = 0xF010,
        mic                 = 0xF011,
        mouse               = 0xF012,
        mousemat            = 0xF013,
        music_speaker       = 0xF014,
        options             = 0xF015,
        pc_case             = 0xF016,
        serial              = 0xF017,
        terminal            = 0xF018,
        toolbox             = 0xF019,
        unknown             = 0xF01A,
        virtual_controller  = 0xF01B,
        usb                 = 0xF01C,
        laptop              = 0xF01D,
        monitor             = 0xF01E
    };

    static QString icon(int);
    static QFont GetFont();

private:
    OpenRGBFont();

    static OpenRGBFont* instance;
    int fontId = -1;
    QFont font;
};
