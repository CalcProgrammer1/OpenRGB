/*---------------------------------------------------------*\
| LIFXSettingsEntry.h                                       |
|                                                           |
|   User interface for OpenRGB LIFX settings entry          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class LIFXSettingsEntry;
}

class LIFXSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit LIFXSettingsEntry(QWidget *parent = nullptr);
    ~LIFXSettingsEntry();
    void loadFromSettings(const json& data);
    void setName(QString name);

    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::LIFXSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
    void on_MultizoneCheckBox_stateChanged(int arg1);
};
