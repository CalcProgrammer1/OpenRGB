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

#include "NanoleafSettingsEntry.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

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

    addDeviceMenu = new QMenu();
    ui->addDeviceButton->setMenu(addDeviceMenu);
    connect(addDeviceMenu, &QMenu::triggered, this, &ManualDevicesSettingsPage::onAddDeviceItemSelected);

    reloadList();
    on_deviceList_itemSelectionChanged(); // Refresh button state
}

ManualDevicesSettingsPage::~ManualDevicesSettingsPage()
{
    ResourceManager::get()->UnregisterDetectionEndCallback(&ManualDevicesPageReloadCallback, this);
    clearList();
    delete ui;
    delete addDeviceMenu;
}

void ManualDevicesSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
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
        std::string section = entries[idx]->getSettingsSection();
        if(section == "NanoleafDevices")
        {
            NanoleafSettingsEntry* entry = dynamic_cast<NanoleafSettingsEntry*>(entries[idx]);
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

void ManualDevicesSettingsPage::reloadMenu()
{
    std::vector<std::string> names = ManualDevicesTypeManager::get()->getRegisteredTypeNames();

    addDeviceMenu->clear();
    for(int i = 0; i < names.size(); ++i)
    {
        QAction* action = addDeviceMenu->addAction(QString::fromStdString(names[i]));
        action->setData(QString::fromStdString(names[i]));
    }
}

void ManualDevicesSettingsPage::reloadList()
{
    clearList();
    addDeviceMenu->clear();

    std::vector<ManualDeviceTypeBlock> blocks = ManualDevicesTypeManager::get()->getRegisteredTypes();
    for(int i = 0; i < blocks.size(); ++i)
    {
        QAction* action = addDeviceMenu->addAction(QString::fromStdString(blocks[i].name));
        action->setData(QString::fromStdString(blocks[i].name));

        addEntries(blocks[i]);
    }
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

void ManualDevicesSettingsPage::addEntry(BaseManualDeviceEntry* entry)
{
	if(!entry)
	{
		return;
	}

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->deviceList->addItem(item);
    ui->deviceList->setItemWidget(item, entry);
    ui->deviceList->show();

    entries.push_back(entry);
}

void ManualDevicesSettingsPage::addEntries(const ManualDeviceTypeBlock& block)
{
    json settings = ResourceManager::get()->GetSettingsManager()->GetSettings(block.settingsSection);
    const char* array_name = "devices";
    if(block.settingsSection == "PhilipsHueDevices")
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
        // For uniformity, use iterators, as if it's always an object
        for(json::const_iterator iter = array_ref.begin(); iter != array_ref.end(); ++iter)
        {
            BaseManualDeviceEntry* entry = block.spawn(iter.value());
            if(entry)
            {
                addEntry(entry);
            }
        }
    }
}

void ManualDevicesSettingsPage::onAddDeviceItemSelected(QAction* action)
{
    std::string entryName = action->data().toString().toStdString();
    BaseManualDeviceEntry* entry = ManualDevicesTypeManager::get()->spawnByTypeName(entryName, json());
    if(entry)
    {
        addEntry(entry);
    }
}

void ManualDevicesSettingsPage::on_deviceList_itemSelectionChanged()
{
    int cur_row = ui->deviceList->currentRow();

    bool anySelected = (cur_row >= 0);
    ui->removeDeviceButton->setEnabled(anySelected);
}
