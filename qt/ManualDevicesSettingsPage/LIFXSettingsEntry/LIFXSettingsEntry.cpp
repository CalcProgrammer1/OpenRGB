/*---------------------------------------------------------*\
| LIFXSettingsEntry.cpp                                     |
|                                                           |
|   User interface for OpenRGB LIFX settings entry          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LIFXSettingsEntry.h"
#include "ui_LIFXSettingsEntry.h"

LIFXSettingsEntry::LIFXSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::LIFXSettingsEntry)
{
    ui->setupUi(this);
}

LIFXSettingsEntry::~LIFXSettingsEntry()
{
    delete ui;
}

void LIFXSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void LIFXSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }
    if(data.contains("name"))
    {
        ui->NameEdit->setText(QString::fromStdString(data["name"]));
    }
}

json LIFXSettingsEntry::saveSettings()
{
    json result;
    result["ip"] = ui->IPEdit->text().toStdString();
    result["name"] = ui->NameEdit->text().toStdString();
    return result;
}

const char* LIFXSettingsEntry::settingsSection()
{
    return "LIFXDevices";
}

void LIFXSettingsEntry::setName(QString name)
{
    ui->NameEdit->setText(name);
}
