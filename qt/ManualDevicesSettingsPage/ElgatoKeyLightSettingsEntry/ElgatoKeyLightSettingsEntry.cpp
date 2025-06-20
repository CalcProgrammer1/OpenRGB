/*---------------------------------------------------------*\
| ElgatoKeyLightSettingsEntry.cpp                           |
|                                                           |
|   User interface for OpenRGB Elgato Key Light entry       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "ElgatoKeyLightSettingsEntry.h"
#include "ui_ElgatoKeyLightSettingsEntry.h"

ElgatoKeyLightSettingsEntry::ElgatoKeyLightSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::ElgatoKeyLightSettingsEntry)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

ElgatoKeyLightSettingsEntry::~ElgatoKeyLightSettingsEntry()
{
    delete ui;
}

void ElgatoKeyLightSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void ElgatoKeyLightSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }
}

json ElgatoKeyLightSettingsEntry::saveSettings()
{
    json result;
    result["ip"] = ui->IPEdit->text().toStdString();
    return result;
}

const char* ElgatoKeyLightSettingsEntry::settingsSection()
{
    return "ElgatoKeyLightDevices";
}
