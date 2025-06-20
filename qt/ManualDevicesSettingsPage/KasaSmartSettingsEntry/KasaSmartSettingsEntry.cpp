/*---------------------------------------------------------*\
| KasaSmartSettingsEntry.cpp                                |
|                                                           |
|   User interface for OpenRGB Kasa Smart settings entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "KasaSmartSettingsEntry.h"
#include "ui_KasaSmartSettingsEntry.h"

KasaSmartSettingsEntry::KasaSmartSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::KasaSmartSettingsEntry)
{
    ui->setupUi(this);
}

KasaSmartSettingsEntry::~KasaSmartSettingsEntry()
{
    delete ui;
}

void KasaSmartSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void KasaSmartSettingsEntry::loadFromSettings(const json& data)
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

json KasaSmartSettingsEntry::saveSettings()
{
    json result;
    result["ip"] = ui->IPEdit->text().toStdString();
    result["name"] = ui->NameEdit->text().toStdString();
    return result;
}

const char* KasaSmartSettingsEntry::settingsSection()
{
    return "KasaSmartDevices";
}

void KasaSmartSettingsEntry::setName(QString name)
{
    ui->NameEdit->setText(name);
}
