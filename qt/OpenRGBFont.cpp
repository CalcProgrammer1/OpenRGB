/*---------------------------------------------------------*\
| OpenRGBFont.cpp                                           |
|                                                           |
|   Functionality for OpenRGB custom font icons             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <QStringList>
#include <QFontDatabase>
#include "OpenRGBFont.h"

OpenRGBFont* OpenRGBFont::instance;

OpenRGBFont::OpenRGBFont()
{
}

OpenRGBFont *OpenRGBFont::Get()
{
    if(!instance)
    {
        instance = new OpenRGBFont();
        instance->fontId = QFontDatabase::addApplicationFont(":/fonts/OpenRGB.ttf");

        if(instance->fontId == -1)
        {
            printf("Cannot load requested font.\n");
        }
        else
        {
            QString family = QFontDatabase::applicationFontFamilies(instance->fontId).at(0);
            instance->font = QFont(family);
            instance->font.setStyleStrategy(QFont::PreferAntialias);
        }
    }

    return instance;
}

int OpenRGBFont::GetIconIDFromDeviceType(device_type type)
{
    /*-----------------------------------------------------*\
    | Return the icon int value for the given device        |
    | type value                                            |
    \*-----------------------------------------------------*/
    int icon;

    switch(type)
    {
    case DEVICE_TYPE_ACCESSORY:
        icon = OpenRGBFont::usb;
        break;
    case DEVICE_TYPE_MOTHERBOARD:
        icon = OpenRGBFont::mainboard;
        break;
    case DEVICE_TYPE_DRAM:
        icon = OpenRGBFont::dram;
        break;
    case DEVICE_TYPE_GPU:
        icon = OpenRGBFont::gpu;
        break;
    case DEVICE_TYPE_COOLER:
        icon = OpenRGBFont::cooler;
        break;
    case DEVICE_TYPE_LEDSTRIP:
        icon = OpenRGBFont::ledstrip;
        break;
    case DEVICE_TYPE_KEYBOARD:
        icon = OpenRGBFont::keyboard;
        break;
    case DEVICE_TYPE_MICROPHONE:
        icon = OpenRGBFont::mic;
        break;
    case DEVICE_TYPE_MOUSE:
        icon = OpenRGBFont::mouse;
        break;
    case DEVICE_TYPE_MOUSEMAT:
        icon = OpenRGBFont::mousemat;
        break;
    case DEVICE_TYPE_HEADSET:
        icon = OpenRGBFont::headset;
        break;
    case DEVICE_TYPE_HEADSET_STAND:
        icon = OpenRGBFont::headsetstand;
        break;
    case DEVICE_TYPE_GAMEPAD:
        icon = OpenRGBFont::gamepad;
        break;
    case DEVICE_TYPE_LIGHT:
        icon = OpenRGBFont::bulb;
        break;
    case DEVICE_TYPE_SPEAKER:
        icon = OpenRGBFont::music_speaker;
        break;
    case DEVICE_TYPE_VIRTUAL:
        icon = OpenRGBFont::virtual_controller;
        break;
    case DEVICE_TYPE_STORAGE:
        icon = OpenRGBFont::drive;
        break;
    case DEVICE_TYPE_CASE:
        icon = OpenRGBFont::pc_case;
        break;
    case DEVICE_TYPE_KEYPAD:
        icon = OpenRGBFont::keypad;
        break;
    case DEVICE_TYPE_LAPTOP:
        icon = OpenRGBFont::laptop;
        break;
    case DEVICE_TYPE_MONITOR:
        icon = OpenRGBFont::monitor;
        break;
    default:
        icon = OpenRGBFont::unknown;
        break;
    }

    return icon;
}

QString OpenRGBFont::icon(int glyph)
{
    return QChar(glyph);
}

QFont OpenRGBFont::GetFont()
{
    return Get()->font;
}

