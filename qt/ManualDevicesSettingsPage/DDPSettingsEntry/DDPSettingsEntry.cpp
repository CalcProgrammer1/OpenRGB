/*---------------------------------------------------------*\
| DDPSettingsEntry.cpp                                      |
|                                                           |
|   User interface for OpenRGB DDP settings entry          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "DDPSettingsEntry.h"
#include "ui_DDPSettingsEntry.h"
#include "ManualDevicesTypeManager.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

DDPSettingsEntry::DDPSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::DDPSettingsEntry)
{
    ui->setupUi(this);
}

DDPSettingsEntry::~DDPSettingsEntry()
{
    delete ui;
}

void DDPSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void DDPSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("name"))
    {
        ui->NameEdit->setText(QString::fromStdString(data["name"]));
    }
    
    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }
    
    if(data.contains("port"))
    {
        ui->PortSpinBox->setValue(data["port"]);
    }
    else
    {
        ui->PortSpinBox->setValue(4048);
    }
    
    if(data.contains("num_leds"))
    {
        ui->NumLedsSpinBox->setValue(data["num_leds"]);
    }
    
    if(data.contains("keepalive_time"))
    {
        ui->KeepaliveTimeSpinBox->setValue(data["keepalive_time"]);
    }
}

json DDPSettingsEntry::saveSettings()
{
    json result;
    
    result["name"] = ui->NameEdit->text().toStdString();
    result["ip"] = ui->IPEdit->text().toStdString();
    result["port"] = ui->PortSpinBox->value();
    result["num_leds"] = ui->NumLedsSpinBox->value();
    
    if(ui->KeepaliveTimeSpinBox->value() > 0)
    {
        result["keepalive_time"] = ui->KeepaliveTimeSpinBox->value();
    }
    
    return result;
}

bool DDPSettingsEntry::isDataValid()
{
    return !ui->IPEdit->text().isEmpty() && ui->NumLedsSpinBox->value() > 0;
}

static BaseManualDeviceEntry* SpawnDDPEntry(const json& data)
{
    DDPSettingsEntry* entry = new DDPSettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

static const char* DDPDeviceName = QT_TRANSLATE_NOOP("ManualDevice", "DDP (Distributed Display Protocol)");

REGISTER_MANUAL_DEVICE_TYPE(DDPDeviceName, "DDPDevices", SpawnDDPEntry);
