
/*---------------------------------------------------------*\
| OpenRGBNanoleafSettingsEntry.cpp                          |
|                                                           |
|   User interface for OpenRGB Nanoleaf settings entry      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBNanoleafSettingsEntry.h"
#include "ui_OpenRGBNanoleafSettingsEntry.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "NanoleafController.h"

OpenRGBNanoleafSettingsEntry::OpenRGBNanoleafSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::OpenRGBNanoleafSettingsEntry),
    paired(false)
{
    ui->setupUi(this);
}

OpenRGBNanoleafSettingsEntry::OpenRGBNanoleafSettingsEntry(QString a_address, int a_port) :
    OpenRGBNanoleafSettingsEntry(nullptr)
{
    address = a_address;
    port = a_port;
    const std::string location = getLocation();

    ui->IPValue->setText(address);
    ui->PortValue->setText(QString::fromStdString(std::to_string(a_port)));

    json nanoleaf_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("NanoleafDevices");

    if(nanoleaf_settings["devices"].contains(location) &&
       nanoleaf_settings["devices"][location].contains("auth_token") &&
       nanoleaf_settings["devices"][location]["auth_token"].size())
    {
        paired = true;
        auth_token = nanoleaf_settings["devices"][location]["auth_token"];
        ui->AuthKeyValue->setText(QString::fromStdString(auth_token));
        ui->PairButton->hide();
    }
    else
    {
        ui->UnpairButton->hide();
    }
}

OpenRGBNanoleafSettingsEntry::~OpenRGBNanoleafSettingsEntry()
{
    delete ui;
}

void OpenRGBNanoleafSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBNanoleafSettingsEntry::on_PairButton_clicked()
{
    try
    {
        auth_token = NanoleafController::Pair(address.toStdString(), port);

        // Save auth token.
        std::string location = getLocation();
        json nanoleaf_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("NanoleafDevices");
        nanoleaf_settings["devices"][location]["ip"] = address.toStdString();
        nanoleaf_settings["devices"][location]["port"] = port;
        nanoleaf_settings["devices"][location]["auth_token"] = auth_token;
        ResourceManager::get()->GetSettingsManager()->SetSettings("NanoleafDevices", nanoleaf_settings);
        ResourceManager::get()->GetSettingsManager()->SaveSettings();

        // Update UI.
        paired = true;
        ui->AuthKeyValue->setText(QString::fromStdString(auth_token));
        ui->PairButton->hide();
        ui->UnpairButton->show();
    }
    catch(const std::exception& /*e*/)
    {
        paired = false;
        ui->AuthKeyValue->setText("PAIRING FAILED");
    }
}

void OpenRGBNanoleafSettingsEntry::on_UnpairButton_clicked()
{
    NanoleafController::Unpair(address.toStdString(), port, auth_token);

    std::string location = getLocation();
    json nanoleaf_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("NanoleafDevices");
    nanoleaf_settings["devices"].erase(location);
    ResourceManager::get()->GetSettingsManager()->SetSettings("NanoleafDevices", nanoleaf_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();

    paired = false;
    ui->AuthKeyValue->setText("");
    ui->PairButton->show();
    ui->UnpairButton->hide();
}

void OpenRGBNanoleafSettingsEntry::loadFromSettings(const json& data)
{
    address = QString::fromStdString(data["ip"]);
    port = data["port"];

    ui->IPValue->setText(address);
    ui->PortValue->setText(QString::fromStdString(std::to_string(port)));

    if(data.contains("auth_token") && data["auth_token"].size())
    {
        auth_token = data["auth_token"];
        ui->AuthKeyValue->setText(QString::fromStdString(auth_token));
        ui->PairButton->hide();
    }
    else
    {
        auth_token.clear();
        ui->UnpairButton->hide();
    }
}

json OpenRGBNanoleafSettingsEntry::saveSettings()
{
    json result;

    /*-------------------------------------------------*\
    | Required parameters                               |
    \*-------------------------------------------------*/
    result["ip"] = address.toStdString();
    result["port"] = port;
    if(!auth_token.empty())
    {
        result["auth_token"] = auth_token;
    }

    return result;
}

const char* OpenRGBNanoleafSettingsEntry::settingsSection()
{
    return "NanoleafDevices";
}

std::string OpenRGBNanoleafSettingsEntry::getLocation()
{
    return (address.toStdString() + ":" + std::to_string(port));
}
