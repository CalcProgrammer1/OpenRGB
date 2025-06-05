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
    if(data.contains("multizone") && data["multizone"].is_boolean())
    {
        ui->MultizoneCheckBox->setCheckState(data["multizone"] == true ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    }
    if(data.contains("extended_multizone") && data["extended_multizone"].is_boolean())
    {
        ui->ExtendedMultizoneCheckBox->setCheckState(data["extended_multizone"] == true ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    }
}

json OpenRGBLIFXSettingsEntry::saveSettings()
{
    json result;
    result["ip"]                 = ui->IPEdit->text().toStdString();
    result["name"]               = ui->NameEdit->text().toStdString();
    result["multizone"]          = ui->MultizoneCheckBox->checkState() == Qt::Checked;
    result["extended_multizone"] = ui->ExtendedMultizoneCheckBox->checkState() == Qt::Checked;
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
