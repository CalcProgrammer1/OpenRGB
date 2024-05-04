/*---------------------------------------------------------*\
| OpenRGBFont.cpp                                           |
|                                                           |
|   Functionality for OpenRGB custom font icons             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

QString OpenRGBFont::icon(int glyph)
{
    return QChar(glyph);
}

QFont OpenRGBFont::GetFont()
{
    return Get()->font;
}

