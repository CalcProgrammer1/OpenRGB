/*-------------------------------------------------------------*\
| ArcticSettingsEntry.h                                         |
|                                                               |
|   User interface for OpenRGB Arctic controller settings entry |
|                                                               |
|   This file is part of the OpenRGB project                    |
|   SPDX-License-Identifier: GPL-2.0-or-later                   |
\*-------------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class ArcticSettingsEntry;
}

class ArcticSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit ArcticSettingsEntry(QWidget *parent = nullptr);
    ~ArcticSettingsEntry();
    void loadFromSettings(const json& data);
    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::ArcticSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
};
