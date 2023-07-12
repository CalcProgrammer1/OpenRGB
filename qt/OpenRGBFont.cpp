#include "OpenRGBFont.h"
#include <QStringList>
#include <QFontDatabase>

OpenRGBFont* OpenRGBFont::instance;

OpenRGBFont::OpenRGBFont(){}

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

