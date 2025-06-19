/*-----------------------------------------------------------------*\
| ManualDevicesSettingsPage.cpp                                     |
|                                                                   |
|   User interface for OpenRGB Manually Added Devices settings page |
|                                                                   |
|   This file is part of the OpenRGB project                        |
|   SPDX-License-Identifier: GPL-2.0-only                           |
\*-----------------------------------------------------------------*/

#include "ManualDevicesSettingsPage.h"
#include "ui_ManualDevicesSettingsPage.h"
#include "OpenRGBDMXSettingsEntry.h"
#include "OpenRGBE131SettingsEntry.h"
#include "OpenRGBElgatoKeyLightSettingsEntry.h"
#include "OpenRGBElgatoLightStripSettingsEntry.h"
#include "OpenRGBKasaSmartSettingsEntry.h"
#include "OpenRGBLIFXSettingsEntry.h"
#include "OpenRGBNanoleafSettingsEntry.h"
#include "OpenRGBPhilipsHueSettingsEntry.h"
#include "OpenRGBPhilipsWizSettingsEntry.h"
#include "OpenRGBQMKORGBSettingsEntry.h"
#include "OpenRGBSerialSettingsEntry.h"
#include "OpenRGBYeelightSettingsEntry.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "AddDeviceDialog.h"

static void ManualDevicesPageReloadCallback(void* this_ptr)
{
    ManualDevicesSettingsPage * this_obj = (ManualDevicesSettingsPage *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "reloadList", Qt::QueuedConnection);
}

ManualDevicesSettingsPage::ManualDevicesSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManualDevicesSettingsPage)
{
    ui->setupUi(this);
    ResourceManager::get()->RegisterDetectionEndCallback(&ManualDevicesPageReloadCallback, this);

    reloadList();
}

ManualDevicesSettingsPage::~ManualDevicesSettingsPage()
{
    ResourceManager::get()->UnregisterDetectionEndCallback(&ManualDevicesPageReloadCallback, this);
    clearList();
    delete ui;
}

void ManualDevicesSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void ManualDevicesSettingsPage::on_addDeviceButton_clicked()
{
    AddDeviceDialog dialog(this);
    dialog.exec();
}

void ManualDevicesSettingsPage::on_removeDeviceButton_clicked()
{
    int cur_row = ui->deviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->deviceList->takeItem(cur_row);

    ui->deviceList->removeItemWidget(item);
    delete item;

    BaseManualDeviceEntry* entry = entries[cur_row];
    entries.erase(entries.begin() + cur_row);
    delete entry;
}

void ManualDevicesSettingsPage::on_saveConfigurationButton_clicked()
{
    SettingsManager* sm = ResourceManager::get()->GetSettingsManager();

    json result;

    /*-----------------------------------------------------------------------------*\
    | First, cache the data that will be stored as JSON                             |
    | We wrap data into arrays by type (except Nanoleaf)                            |
    | Nanoleaf stores it's data as an object with "location" as key for some reason |
    \*-----------------------------------------------------------------------------*/
    for(size_t idx = 0; idx < entries.size(); ++idx)
    {
        std::string section = entries[idx]->settingsSection();
        if(section == "NanoleafDevices")
        {
            OpenRGBNanoleafSettingsEntry* entry = dynamic_cast<OpenRGBNanoleafSettingsEntry*>(entries[idx]);
            result[section]["devices"][entry->getLocation()] = entries[idx]->saveSettings();
        }
        else if(section == "PhilipsHueDevices")
        {
            result[section]["bridges"].push_back(entries[idx]->saveSettings());
        }
        else
        {
            result[section]["devices"].push_back(entries[idx]->saveSettings());
        }
    }

    /*---------------------------------------------------------*\
    | Transfer data from the cached object into config sections |
    \*---------------------------------------------------------*/
    json::iterator it;
    for(it = result.begin(); it != result.end(); ++it)
    {
        sm->SetSettings(it.key(), it.value());
    }
    sm->SaveSettings();
}

void ManualDevicesSettingsPage::reloadList()
{
    clearList();

    addEntries("DMXDevices",              [](){return new OpenRGBDMXSettingsEntry();});
    addEntries("E131Devices",             [](){return new OpenRGBE131SettingsEntry();});
    addEntries("ElgatoKeyLightDevices",   [](){return new OpenRGBElgatoKeyLightSettingsEntry();});
    addEntries("ElgatoLightStripDevices", [](){return new OpenRGBElgatoLightStripSettingsEntry();});
    addEntries("KasaSmartDevices",        [](){return new OpenRGBKasaSmartSettingsEntry();});
    addEntries("LIFXDevices",             [](){return new OpenRGBLIFXSettingsEntry();});
    addEntries("NanoleafDevices",         [](){return new OpenRGBNanoleafSettingsEntry();});
    addEntries("PhilipsHueDevices",       [](){return new OpenRGBPhilipsHueSettingsEntry();});
    addEntries("PhilipsWizDevices",       [](){return new OpenRGBPhilipsWizSettingsEntry();});
    addEntries("QMKOpenRGBDevices",       [](){return new OpenRGBQMKORGBSettingsEntry();});
    addEntries("LEDStripDevices",         [](){return new OpenRGBSerialSettingsEntry();});
    addEntries("YeelightDevices",         [](){return new OpenRGBYeelightSettingsEntry();});
}

void ManualDevicesSettingsPage::clearList()
{
    std::vector<BaseManualDeviceEntry*> entries_copy;
    entries_copy.swap(entries);
    ui->deviceList->clear();

    for(int i = 0; i < entries_copy.size(); ++i)
    {
        delete entries_copy[i];
    }
    entries_copy.clear();
}

void ManualDevicesSettingsPage::addEntry(const json& source, EntrySpawnFunction spawn)
{
    QListWidgetItem* item = new QListWidgetItem;
    BaseManualDeviceEntry* entry = spawn();
    entry->loadFromSettings(source);

    item->setSizeHint(entry->sizeHint());

    ui->deviceList->addItem(item);
    ui->deviceList->setItemWidget(item, entry);
    ui->deviceList->show();

    entries.push_back(entry);
}

void ManualDevicesSettingsPage::addEntries(const std::string& settingsName, EntrySpawnFunction spawn)
{
    json settings = ResourceManager::get()->GetSettingsManager()->GetSettings(settingsName);
    const char* array_name = "devices";
    if(settingsName == "PhilipsHueDevices")
    {
        array_name = "bridges";
    }

    if(settings.contains(array_name))
    {
        json& array_ref = settings[array_name];

        if(!array_ref.is_array() && !array_ref.is_object())
        {
            return;
        }
        // Nanoleaf stores it's data as objects with location field as "key", everything else is arrays
        for(json::const_iterator iter = array_ref.begin(); iter != array_ref.end(); ++iter)
        {
            addEntry(iter.value(), spawn);
        }
    }
}
