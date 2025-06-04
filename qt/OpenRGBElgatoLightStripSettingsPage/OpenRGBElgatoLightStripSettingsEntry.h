/*---------------------------------------------------------*\
| OpenRGBElgatoLightStripSettingsEntry.h                    |
|                                                           |
|   User interface for OpenRGB Elgato Light Strips entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Ui
{
    class OpenRGBElgatoLightStripSettingsEntry;
    class OpenRGBElgatoLightStripSettingsEntryUi;
}

class Ui::OpenRGBElgatoLightStripSettingsEntry : public QDialog
{
    Q_OBJECT

    public:
        explicit OpenRGBElgatoLightStripSettingsEntry(QWidget *parent = nullptr);
        ~OpenRGBElgatoLightStripSettingsEntry();
        void loadFromSettings(const json& data);
        json saveSettings();
        const char* settingsSection();

    private:
        Ui::OpenRGBElgatoLightStripSettingsEntryUi *ui;

    private slots:
        void changeEvent(QEvent *event);
};
