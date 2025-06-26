/*---------------------------------------------------------*\
| KasaSmartSettingsEntry.h                                  |
|                                                           |
|   User interface for OpenRGB Kasa Smart settings entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

namespace Ui
{
    class KasaSmartSettingsEntry;
}

class KasaSmartSettingsEntry : public BaseManualDeviceEntry
{
    Q_OBJECT

public:
    explicit KasaSmartSettingsEntry(QWidget *parent = nullptr);
    ~KasaSmartSettingsEntry();
    void loadFromSettings(const json& data);
    void setName(QString name);

    json saveSettings() override;
    bool isDataValid() override;

private:
    Ui::KasaSmartSettingsEntry *ui;

private slots:
    void changeEvent(QEvent *event) override;
};
