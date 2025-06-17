/*---------------------------------------------------------*\
| OpenRGBPhilipsWizSettingsEntry.cpp                        |
|                                                           |
|   User interface for OpenRGB Philips Wiz settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBPhilipsWizSettingsEntry.h"
#include "ui_OpenRGBPhilipsWizSettingsEntry.h"

OpenRGBPhilipsWizSettingsEntry::OpenRGBPhilipsWizSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::OpenRGBPhilipsWizSettingsEntry)
{
    ui->setupUi(this);

    ui->WhiteStrategyComboBox->addItem(tr("Average"));
    ui->WhiteStrategyComboBox->addItem(tr("Minimum"));
}

OpenRGBPhilipsWizSettingsEntry::~OpenRGBPhilipsWizSettingsEntry()
{
    delete ui;
}

void OpenRGBPhilipsWizSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBPhilipsWizSettingsEntry::loadFromSettings(const json& data)
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

json OpenRGBPhilipsWizSettingsEntry::saveSettings()
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

const char* OpenRGBPhilipsWizSettingsEntry::settingsSection()
{
    return "PhilipsWizDevices";
}
