/*---------------------------------------------------------*\
| OpenRGBLIFXSettingsEntry.cpp                              |
|                                                           |
|   User interface for OpenRGB LIFX settings entry          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBLIFXSettingsEntry.h"
#include "ui_OpenRGBLIFXSettingsEntry.h"

OpenRGBLIFXSettingsEntry::OpenRGBLIFXSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::OpenRGBLIFXSettingsEntry)
{
    ui->setupUi(this);
}

OpenRGBLIFXSettingsEntry::~OpenRGBLIFXSettingsEntry()
{
    delete ui;
}

void OpenRGBLIFXSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBLIFXSettingsEntry::loadFromSettings(const json& data)
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

json OpenRGBLIFXSettingsEntry::saveSettings()
{
    json result;
    result["ip"] = ui->IPEdit->text().toStdString();
    result["name"] = ui->NameEdit->text().toStdString();
    return result;
}

const char* OpenRGBLIFXSettingsEntry::settingsSection()
{
    return "LIFXDevices";
}

void OpenRGBLIFXSettingsEntry::setName(QString name)
{
    ui->NameEdit->setText(name);
}
