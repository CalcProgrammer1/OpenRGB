/*---------------------------------------------------------*\
| OpenRGBNanoleafSettingsPage.cpp                           |
|                                                           |
|   User interface for OpenRGB Nanoleaf settings page       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBNanoleafSettingsPage.h"
#include "ui_OpenRGBNanoleafSettingsPage.h"
#include "OpenRGBNanoleafNewDeviceDialog.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "LogManager.h"

using json = nlohmann::json;

OpenRGBNanoleafSettingsPage::OpenRGBNanoleafSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBNanoleafSettingsPage)
{
    ui->setupUi(this);

    json nanoleaf_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("NanoleafDevices");

    if(nanoleaf_settings.contains("devices"))
    {
        for(json::const_iterator it = nanoleaf_settings["devices"].begin(); it != nanoleaf_settings["devices"].end(); ++it)
        {
            const json& device = it.value();
            const std::string& location = it.key();

            if(device.contains("ip") && device.contains("port"))
            {
                OpenRGBNanoleafSettingsEntry* entry = new OpenRGBNanoleafSettingsEntry(QString::fromStdString(device["ip"]), device["port"]);

                entries[location] = entry;

                QListWidgetItem* item = new QListWidgetItem;

                item->setSizeHint(entry->sizeHint());

                ui->NanoleafDeviceList->addItem(item);
                ui->NanoleafDeviceList->setItemWidget(item, entry);
                ui->NanoleafDeviceList->show();
            }
        }
    }
}

OpenRGBNanoleafSettingsPage::~OpenRGBNanoleafSettingsPage()
{
    delete ui;
}

void OpenRGBNanoleafSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBNanoleafSettingsPage::on_AddNanoleafDeviceButton_clicked()
{
    /*-----------------------------------------------------*\
    | Open a popup to manually add a device by setting ip   |
    | and port                                              |
    \*-----------------------------------------------------*/
    OpenRGBNanoleafNewDeviceDialog dialog;
    NanoleafDevice device = dialog.show();
    if(!device.ip.empty())
    {
        LOG_TRACE("[%s] Add %s:%d", "Nanoleaf", device.ip.c_str(), device.port);
        std::string location = device.ip+":"+std::to_string(device.port);

        if(entries.find(location) == entries.end())
        {
            OpenRGBNanoleafSettingsEntry* entry = new OpenRGBNanoleafSettingsEntry(QString::fromUtf8(device.ip.c_str()), device.port);

            entries[location] = entry;

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->NanoleafDeviceList->addItem(item);
            ui->NanoleafDeviceList->setItemWidget(item, entry);
            ui->NanoleafDeviceList->show();

            json nanoleaf_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("NanoleafDevices");
            nanoleaf_settings["devices"][location]["ip"] = device.ip;
            nanoleaf_settings["devices"][location]["port"] = device.port;
            ResourceManager::get()->GetSettingsManager()->SetSettings("NanoleafDevices", nanoleaf_settings);
            ResourceManager::get()->GetSettingsManager()->SaveSettings();
        }
    }
}

void OpenRGBNanoleafSettingsPage::on_RemoveNanoleafDeviceButton_clicked()
{
    /*-------------------------------------------------*\
    | Remove the selected device                        |
    \*-------------------------------------------------*/
    int cur_row = ui->NanoleafDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->NanoleafDeviceList->item(cur_row);
    OpenRGBNanoleafSettingsEntry* entry = (OpenRGBNanoleafSettingsEntry*) ui->NanoleafDeviceList->itemWidget(item);
    LOG_TRACE("[%s] Remove %s:%d", "Nanoleaf", entry->address.toStdString().c_str(), entry->port);

    ui->NanoleafDeviceList->removeItemWidget(item);
    delete item;

    std::string location = entry->address.toStdString()+":"+std::to_string(entry->port);
    delete entries[location];
    entries.erase(location);

    json nanoleaf_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("NanoleafDevices");
    nanoleaf_settings["devices"].erase(location);
    ResourceManager::get()->GetSettingsManager()->SetSettings("NanoleafDevices", nanoleaf_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}

void OpenRGBNanoleafSettingsPage::on_ScanForNanoleafDevicesButton_clicked()
{
    /*-----------------------------------------------------*\
    | Create a worker thread for the mDNS query and hookup  |
    | callbacks for when it finds devices                   |
    \*-----------------------------------------------------*/
    OpenRGBNanoleafScanningThread *scanThread = new OpenRGBNanoleafScanningThread;

    connect(scanThread, SIGNAL(DeviceFound(QString, int)),
                        SLOT(on_DeviceFound(QString, int)));

    connect(scanThread, SIGNAL(finished()),
            scanThread, SLOT(deleteLater()));

    scanThread->start();
}

void OpenRGBNanoleafSettingsPage::on_DeviceFound(QString address, int port)
{
    std::string location = address.toStdString()+":"+std::to_string(port);

    if(entries.find(location) == entries.end())
    {
        OpenRGBNanoleafSettingsEntry* entry = new OpenRGBNanoleafSettingsEntry(address, port);

        entries[location] = entry;

        QListWidgetItem* item = new QListWidgetItem;

        item->setSizeHint(entry->sizeHint());

        ui->NanoleafDeviceList->addItem(item);
        ui->NanoleafDeviceList->setItemWidget(item, entry);
        ui->NanoleafDeviceList->show();
    }
}
