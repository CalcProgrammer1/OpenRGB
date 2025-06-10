/*---------------------------------------------------------*\
| OpenRGBLIFXSettingsEntry.h                                |
|                                                           |
|   User interface for OpenRGB LIFX settings entry          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class OpenRGBLIFXSettingsEntry;
    class OpenRGBLIFXSettingsEntryUi;
}

class Ui::OpenRGBLIFXSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit OpenRGBLIFXSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBLIFXSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings();
    const char* settingsSection();
    void setName(QString name);

private:
    Ui::OpenRGBLIFXSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
