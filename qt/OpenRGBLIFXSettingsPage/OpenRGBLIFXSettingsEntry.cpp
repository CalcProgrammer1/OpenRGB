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

using namespace Ui;

OpenRGBLIFXSettingsEntry::OpenRGBLIFXSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::OpenRGBLIFXSettingsEntryUi)
{
    ui->setupUi(this);

    connect(ui->MultizoneCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onMultizoneCheckStateChanged(int)));
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
    if(lifx_device_settings.contains("multizone") && lifx_device_settings["multizone"].is_boolean())
    {
        ui->MultizoneCheckBox->setCheckState(lifx_device_settings["multizone"] == true ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    }
    if(lifx_device_settings.contains("extended_multizone") && lifx_device_settings["extended_multizone"].is_boolean())
    {
        ui->ExtendedMultizoneCheckBox->setCheckState(lifx_device_settings["extended_multizone"] == true ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    }
}

json OpenRGBLIFXSettingsEntry::saveSettings()
{
    json result;
    result["ip"]                 = ui->IPEdit->text().toStdString();
    result["name"]               = ui->NameEdit->text().toStdString();
    result["multizone"]          = entries[device_idx]->ui->MultizoneCheckBox->checkState() == Qt::Checked;
    result["extended_multizone"] = entries[device_idx]->ui->ExtendedMultizoneCheckBox->checkState() == Qt::Checked;
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

void Ui::OpenRGBLIFXSettingsEntry::on_MultizoneCheckBox_stateChanged(int checkState)
{
    if (checkState == Qt::Checked) {
        ui->ExtendedMultizoneCheckBox->setEnabled(true);
    }
    else {
        ui->ExtendedMultizoneCheckBox->setEnabled(false);
        ui->ExtendedMultizoneCheckBox->setCheckState(Qt::Unchecked);
    }
}
