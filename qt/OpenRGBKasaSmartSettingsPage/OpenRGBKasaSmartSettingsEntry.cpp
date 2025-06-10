/*---------------------------------------------------------*\
| OpenRGBKasaSmartSettingsEntry.cpp                         |
|                                                           |
|   User interface for OpenRGB Kasa Smart settings entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBKasaSmartSettingsEntry.h"
#include "ui_OpenRGBKasaSmartSettingsEntry.h"

using namespace Ui;

OpenRGBKasaSmartSettingsEntry::OpenRGBKasaSmartSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::OpenRGBKasaSmartSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBKasaSmartSettingsEntry::~OpenRGBKasaSmartSettingsEntry()
{
    delete ui;
}

void OpenRGBKasaSmartSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBKasaSmartSettingsEntry::loadFromSettings(const json& data)
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

json OpenRGBKasaSmartSettingsEntry::saveSettings()
{
    json result;
    result["ip"] = ui->IPEdit->text().toStdString();
    result["name"] = ui->NameEdit->text().toStdString();
    return result;
}

const char* OpenRGBKasaSmartSettingsEntry::settingsSection()
{
    return "KasaSmartDevices";
}

void OpenRGBKasaSmartSettingsEntry::setName(QString name)
{
    ui->NameEdit->setText(name);
}
