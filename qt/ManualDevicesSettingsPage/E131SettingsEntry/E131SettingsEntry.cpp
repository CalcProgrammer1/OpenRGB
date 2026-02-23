/*---------------------------------------------------------*\
| E131SettingsEntry.cpp                                     |
|                                                           |
|   User interface for OpenRGB E1.31 settings entry         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "E131SettingsEntry.h"
#include "ui_E131SettingsEntry.h"

E131SettingsEntry::E131SettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::E131SettingsEntry)
{
    ui->setupUi(this);
}

E131SettingsEntry::~E131SettingsEntry()
{
    delete ui;
}

void E131SettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void E131SettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("name"))
    {
        ui->NameEdit->setText(QString::fromStdString(data["name"]));
    }

    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }

    if(data.contains("start_universe"))
    {
        ui->StartUniverseEdit->setText(QString::number((int)data["start_universe"]));
    }

    if(data.contains("start_channel"))
    {
        ui->StartChannelEdit->setText(QString::number((int)data["start_channel"]));
    }

    if(data.contains("num_leds"))
    {
        ui->NumLEDsEdit->setText(QString::number((int)data["num_leds"]));
    }

    if(data.contains("universe_size"))
    {
        ui->UniverseSizeEdit->setText(QString::number((int)data["universe_size"]));
    }

    if(data.contains("keepalive_time"))
    {
        ui->KeepaliveTimeEdit->setText(QString::number((int)data["keepalive_time"]));
    }
}

json E131SettingsEntry::saveSettings()
{
    json result;
    /*-------------------------------------------------*\
    | Required parameters                               |
    \*-------------------------------------------------*/
    result["name"]                = ui->NameEdit->text().toStdString();
    result["start_universe"]      = ui->StartUniverseEdit->text().toUInt();
    result["start_channel"]       = ui->StartChannelEdit->text().toUInt();
    result["num_leds"]            = ui->NumLEDsEdit->text().toUInt();

    /*-------------------------------------------------*\
    | Optional parameters                               |
    \*-------------------------------------------------*/
    if(ui->IPEdit->text() != "")
    {
        result["ip"]              = ui->IPEdit->text().toStdString();
    }

    if(ui->UniverseSizeEdit->text() != "")
    {
        result["universe_size"]   = ui->UniverseSizeEdit->text().toUInt();
    }

    if(ui->KeepaliveTimeEdit->text() != "")
    {
        result["keepalive_time"]  = ui->KeepaliveTimeEdit->text().toUInt();
    }

    return result;
}

bool E131SettingsEntry::isDataValid()
{
    // stub
    return true;
}

static BaseManualDeviceEntry* SpawnE131Entry(const json& data)
{
    E131SettingsEntry* entry = new E131SettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

static const char* E131DeviceName = QT_TRANSLATE_NOOP("ManualDevice", "E1.31");

REGISTER_MANUAL_DEVICE_TYPE(E131DeviceName, "E131Devices", SpawnE131Entry);
