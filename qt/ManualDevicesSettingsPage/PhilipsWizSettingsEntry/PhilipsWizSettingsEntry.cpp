/*---------------------------------------------------------*\
| PhilipsWizSettingsEntry.cpp                               |
|                                                           |
|   User interface for OpenRGB Philips Wiz settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "PhilipsWizSettingsEntry.h"
#include "ui_PhilipsWizSettingsEntry.h"

PhilipsWizSettingsEntry::PhilipsWizSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::PhilipsWizSettingsEntry)
{
    ui->setupUi(this);

    ui->WhiteStrategyComboBox->addItem(tr("Average"));
    ui->WhiteStrategyComboBox->addItem(tr("Minimum"));
}

PhilipsWizSettingsEntry::~PhilipsWizSettingsEntry()
{
    delete ui;
}

void PhilipsWizSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void PhilipsWizSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }

    if(data.contains("use_cool_white"))
    {
        ui->UseCoolWhiteCheckBox->setChecked(data["use_cool_white"]);
    }

    if(data.contains("use_warm_white"))
    {
        ui->UseWarmWhiteCheckBox->setChecked(data["use_warm_white"]);
    }

    if(data.contains("selected_white_strategy"))
    {
        ui->WhiteStrategyComboBox->setCurrentText(QString::fromStdString(data["selected_white_strategy"]));
    }
}

json PhilipsWizSettingsEntry::saveSettings()
{
    json result;
    /*-------------------------------------------------*\
    | Required parameters                               |
    \*-------------------------------------------------*/
    result["ip"]               = ui->IPEdit->text().toStdString();
    result["use_cool_white"]   = ui->UseCoolWhiteCheckBox->isChecked();
    result["use_warm_white"]   = ui->UseWarmWhiteCheckBox->isChecked();
    result["selected_white_strategy"]   = ui->WhiteStrategyComboBox->currentText().toStdString();

    return result;
}

bool PhilipsWizSettingsEntry::isDataValid()
{
    // stub
    return true;
}

static BaseManualDeviceEntry* SpawnPhilipsWizSettingsEntry(const json& data)
{
    PhilipsWizSettingsEntry* entry = new PhilipsWizSettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

REGISTER_MANUAL_DEVICE_TYPE("Philips Wiz", "PhilipsWizDevices", SpawnPhilipsWizSettingsEntry);
