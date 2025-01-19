/*---------------------------------------------------------*\
| OpenRGBThemeManager.cpp                                   |
|                                                           |
|   Functionality for managing dark theme mode              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QApplication>
#include <QWidget>
#include <QStyle>
#include <QPalette>
#include <QStyleFactory>

#ifdef _WIN32
#include <QSettings>
#endif

#include "OpenRGBThemeManager.h"
#include "ResourceManager.h"
#include "PluginManager.h"
#include "SettingsManager.h"

void OpenRGBThemeManager::Init()
{

#ifdef __APPLE__
    /*-------------------------------------------------*\
    | Apply Qt Fusion theme on MacOS, as the MacOS      |
    | default theme does not handle vertical tabs well  |
    \*-------------------------------------------------*/
    QApplication::setStyle(QStyleFactory::create("Fusion"));
#endif
    /*-------------------------------------------------*\
    | Apply dark theme if configured                    |
    \*-------------------------------------------------*/
    if(IsDarkTheme())
    {
        SetDarkTheme();
    }
}

void OpenRGBThemeManager::SetDarkTheme()
{
    QPalette pal;

    pal.setColor(QPalette::WindowText,      Qt::white);
    pal.setColor(QPalette::Link,            QColor(0,127,220));
    pal.setColor(QPalette::LinkVisited,     QColor(64,196,220));
    pal.setColor(QPalette::Window,          QColor(53,53,53));
    pal.setColor(QPalette::Base,            QColor(53,53,53));
    pal.setColor(QPalette::AlternateBase,   QColor(66,66,66));
    pal.setColor(QPalette::ToolTipBase,     Qt::white);
    pal.setColor(QPalette::ToolTipText,     Qt::black);
    pal.setColor(QPalette::Text,            Qt::white);
    pal.setColor(QPalette::Dark,            QColor(35,35,35));
    pal.setColor(QPalette::Shadow,          QColor(20,20,20));
    pal.setColor(QPalette::Button,          QColor(53,53,53));
    pal.setColor(QPalette::ButtonText,      Qt::white);
    pal.setColor(QPalette::BrightText,      Qt::red);
    pal.setColor(QPalette::Highlight,       QColor(42,130,218));
    pal.setColor(QPalette::HighlightedText, Qt::white);

    pal.setColor(QPalette::Disabled, QPalette::Text,             QColor(127,127,127));
    pal.setColor(QPalette::Disabled, QPalette::WindowText,       QColor(127,127,127));
    pal.setColor(QPalette::Disabled, QPalette::Highlight,        QColor(80,80,80)   );
    pal.setColor(QPalette::Disabled, QPalette::ButtonText,       QColor(127,127,127));
    pal.setColor(QPalette::Disabled, QPalette::HighlightedText,  QColor(127,127,127));
    pal.setColor(QPalette::Disabled, QPalette::Text,             QColor(127,127,127));
    pal.setColor(QPalette::Disabled, QPalette::ButtonText,       QColor(127,127,127));

#ifdef _WIN32
    QApplication::setStyle(QStyleFactory::create("Fusion"));
#endif

    QApplication::setPalette(pal);
}

bool OpenRGBThemeManager::IsDarkTheme()
{

    /*-------------------------------------------------*\
    | Dark theme settings                               |
    \*-------------------------------------------------*/
    json            theme_settings;

    /*-------------------------------------------------*\
    | Get prefered theme from settings manager          |
    \*-------------------------------------------------*/
    theme_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Theme");

    /*-------------------------------------------------*\
    | Read the theme key and adjust accordingly         |
    \*-------------------------------------------------*/
    std::string current_theme = "Light";

    if(theme_settings.contains("theme"))
    {
        current_theme = theme_settings["theme"];
    }

    if(current_theme == "Dark")
    {
        return true;
    }
#ifdef _WIN32
    else if(current_theme == "Auto")
    {
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);

        if(settings.value("AppsUseLightTheme") != 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
#endif

    return false;
}
