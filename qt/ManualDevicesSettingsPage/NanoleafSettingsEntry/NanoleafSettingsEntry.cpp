/*---------------------------------------------------------*\
| NanoleafSettingsEntry.cpp                                 |
|                                                           |
|   User interface for OpenRGB Nanoleaf settings entry      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "NanoleafSettingsEntry.h"
#include "ui_NanoleafSettingsEntry.h"

#include "NanoleafScanDialog.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "NanoleafController.h"

NanoleafSettingsEntry::NanoleafSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::NanoleafSettingsEntry),
    paired(false)
{
    ui->setupUi(this);
}

NanoleafSettingsEntry::NanoleafSettingsEntry(QString a_address, int a_port) :
    NanoleafSettingsEntry(nullptr)
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

NanoleafSettingsEntry::~NanoleafSettingsEntry()
{
    delete ui;
}

void NanoleafSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void NanoleafSettingsEntry::on_PairButton_clicked()
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

void NanoleafSettingsEntry::on_UnpairButton_clicked()
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

void NanoleafSettingsEntry::loadFromSettings(const json& data)
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

json NanoleafSettingsEntry::saveSettings()
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

std::string NanoleafSettingsEntry::getLocation()
{
    return (address.toStdString() + ":" + std::to_string(port));
}

bool NanoleafSettingsEntry::isDataValid()
{
    // stub
    return true;
}

static BaseManualDeviceEntry* SpawnNanoleafSettingsEntry(const json& data)
{
    if(data.empty())
    {
        // A special case: we open a new scanning dialog instead of returning a new entry
        // The caller should be able to handle this
        NanoleafScanDialog scanPage;
        scanPage.exec();
        return nullptr;
    }
    else
    {
        NanoleafSettingsEntry* entry = new NanoleafSettingsEntry;
        entry->loadFromSettings(data);
        return entry;
    }
}

REGISTER_MANUAL_DEVICE_TYPE("Nanoleaf", "NanoleafDevices", SpawnNanoleafSettingsEntry);
