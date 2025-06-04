/*---------------------------------------------------------*\
| OpenRGBGoveeSettingsEntry.cpp                             |
|                                                           |
|   User interface for OpenRGB Govee settings entry         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBGoveeSettingsEntry.h"
#include "ui_OpenRGBGoveeSettingsEntry.h"

using namespace Ui;

OpenRGBGoveeSettingsEntry::OpenRGBGoveeSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBGoveeSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBGoveeSettingsEntry::~OpenRGBGoveeSettingsEntry()
{
    delete ui;
}

void OpenRGBGoveeSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBGoveeSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }
}

json OpenRGBGoveeSettingsEntry::saveSettings()
{
    json result;
    result["ip"] = ui->IPEdit->text().toStdString();
    return result;
}

const char* OpenRGBGoveeSettingsEntry::settingsSection()
{
    return "GoveeDevices";
}
