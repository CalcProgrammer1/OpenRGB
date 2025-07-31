/*---------------------------------------------------------*\
| ManualDevicesSettingsPage.cpp                             |
|                                                           |
|   User interface for OpenRGB Manually Added Devices       |
|   settings page                                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "ManualDevicesSettingsPage.h"
#include "ui_ManualDevicesSettingsPage.h"

#include "NanoleafSettingsEntry.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

#include <QLineEdit>

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

    addDeviceMenu = new QMenu(this);
    ui->addDeviceButton->setMenu(addDeviceMenu);
    connect(addDeviceMenu, &QMenu::triggered, this, &ManualDevicesSettingsPage::onAddDeviceItemSelected);

    QMenu* saveButtonMenu = new QMenu(this);
    saveButtonMenu->addAction(ui->ActionSaveAndRescan);
    saveButtonMenu->addAction(ui->ActionSaveNoRescan);
    ui->saveConfigurationButton->setMenu(saveButtonMenu);
    ui->saveConfigurationButton->setDefaultAction(ui->ActionSaveAndRescan);

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
        reloadMenu();
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

    setUnsavedChanges(true);
}

void ManualDevicesSettingsPage::saveSettings()
{
    SettingsManager* sm = ResourceManager::get()->GetSettingsManager();

    json result;

    /*-----------------------------------------------------------------------------*\
    | First, cache the data that will be stored as JSON                             |
    | We wrap data into arrays by type (except Nanoleaf)                            |
    | Nanoleaf stores it's data as an object with "location" as key for some reason |
    \*-----------------------------------------------------------------------------*/
    for(std::size_t idx = 0; idx < entries.size(); idx++)
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
    | Use ALL possible settings entry names, so that those with |
    | all entries deleted are cleared properly                  |
    \*---------------------------------------------------------*/
    std::vector<ManualDeviceTypeBlock> blocks = ManualDevicesTypeManager::get()->getRegisteredTypes();
    for(std::size_t i = 0; i < blocks.size(); i++)
    {
        sm->SetSettings(blocks[i].settingsSection, result[blocks[i].settingsSection]);
    }
    sm->SaveSettings();

    setUnsavedChanges(false);
}

void ManualDevicesSettingsPage::reloadMenu()
{
    std::vector<std::string> names = ManualDevicesTypeManager::get()->getRegisteredTypeNames();

    addDeviceMenu->clear();
    for(std::size_t i = 0; i < names.size(); i++)
    {
        QAction* action = addDeviceMenu->addAction(qApp->translate("ManualDevice", names[i].c_str()));
        action->setData(QString::fromStdString(names[i]));
    }
}

void ManualDevicesSettingsPage::reloadList()
{
    clearList();
    addDeviceMenu->clear();

    std::vector<ManualDeviceTypeBlock> blocks = ManualDevicesTypeManager::get()->getRegisteredTypes();
    for(std::size_t i = 0; i < blocks.size(); i++)
    {
        addEntries(blocks[i]);

        /*------------------------------------------------------------*\
        | While we have all the data at hand, load in the menu as well |
        \*------------------------------------------------------------*/
        QAction* action = addDeviceMenu->addAction(qApp->translate("ManualDevice", blocks[i].name.c_str()));
        action->setData(QString::fromStdString(blocks[i].name));
    }

    /*---------------------------------------------------------*\
    | Refresh button state                                      |
    \*---------------------------------------------------------*/
    setUnsavedChanges(false);
    on_deviceList_itemSelectionChanged();
}

void ManualDevicesSettingsPage::onTextEditChanged()
{
    setUnsavedChanges(true);
}

void ManualDevicesSettingsPage::clearList()
{
    std::vector<BaseManualDeviceEntry*> entries_copy;
    entries_copy.swap(entries);
    ui->deviceList->clear();

    for(std::size_t i = 0; i < entries_copy.size(); i++)
    {
        delete entries_copy[i];
    }

    entries_copy.clear();
    setUnsavedChanges(true);
}

void ManualDevicesSettingsPage::setUnsavedChanges(bool v)
{
    unsavedChanges = v;

    if(v)
    {
        ui->saveConfigurationButton->setStyleSheet("font: bold");
    }
    else
    {
        ui->saveConfigurationButton->setStyleSheet("");
    }
}

bool ManualDevicesSettingsPage::checkValidToSave()
{
    for(std::size_t i = 0; i < entries.size(); i++)
    {
        if(!entries[i]->isDataValid())
        {
            return false;
        }
    }

    return true;
}

QListWidgetItem* ManualDevicesSettingsPage::addEntry(BaseManualDeviceEntry* entry)
{
	if(!entry)
	{
        return nullptr;
	}

    /*---------------------------------------------------------*\
    | Find EVERY QLineEdit in the entry and get notified if ANY |
    | text in them changes - for validation                     |
    | Validation mostly affects the "Save" button state         |
    \*---------------------------------------------------------*/
    QList<QLineEdit*> textEditList = entry->findChildren<QLineEdit*>(QString(), Qt::FindChildrenRecursively);
    for(qsizetype i = 0; i < textEditList.size(); i++)
    {
        connect(textEditList[i], &QLineEdit::textChanged, this, &ManualDevicesSettingsPage::onTextEditChanged);
    }

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->deviceList->addItem(item);
    ui->deviceList->setItemWidget(item, entry);
    ui->deviceList->show();

    entries.push_back(entry);

    /*---------------------------------------------------------*\
    | New entries generally indicate unsaved changes            |
    \*---------------------------------------------------------*/
    setUnsavedChanges(true);

    return item;
}

void ManualDevicesSettingsPage::addEntries(const ManualDeviceTypeBlock& block)
{
    /*---------------------------------------------------------*\
    | Spawn list entries for all config entries of one type     |
    \*---------------------------------------------------------*/
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
        /*-----------------------------------------------------------------*\
        | Nanoleaf stores it's data as objects with location field as "key" |
        | everything else is arrays                                         |
        | For uniformity, use iterators, as if it's always an object        |
        \*-----------------------------------------------------------------*/
        for(json::const_iterator iter = array_ref.begin(); iter != array_ref.end(); ++iter)
        {
            if(!iter.value().empty())
            {
                BaseManualDeviceEntry* entry = block.spawn(iter.value());

                /*---------------------------------------------------*\
                | Note: spawn functions are allowed to return nullptr |
                \*---------------------------------------------------*/
                if(entry)
                {
                    addEntry(entry);
                }
            }
        }
    }
}

void ManualDevicesSettingsPage::onAddDeviceItemSelected(QAction* action)
{
    /*---------------------------------------------------------*\
    | Spawn new entry based on name                             |
    \*---------------------------------------------------------*/
    std::string             entryName   = action->data().toString().toStdString();
    BaseManualDeviceEntry*  entry       = ManualDevicesTypeManager::get()->spawnByTypeName(entryName, json());

    /*---------------------------------------------------------*\
    | Note: spawn functions are allowed to return nullptr       |
    \*---------------------------------------------------------*/
    if(entry)
    {
        QListWidgetItem* item = addEntry(entry);

        /*-----------------------------------------------------*\
        | Scroll to the newly added entry (last one in the list |
        \*-----------------------------------------------------*/
        if(item)
        {
            ui->deviceList->scrollToItem(item);
        }
    }
}

void ManualDevicesSettingsPage::on_deviceList_itemSelectionChanged()
{
    /*---------------------------------------------------------*\
    | Enable Remove button if any entry is selected             |
    \*---------------------------------------------------------*/
    int     cur_row     = ui->deviceList->currentRow();
    bool    anySelected = (cur_row >= 0);

    ui->removeDeviceButton->setEnabled(anySelected);
}

void ManualDevicesSettingsPage::on_ActionSaveNoRescan_triggered()
{
    saveSettings();
}

void ManualDevicesSettingsPage::on_ActionSaveAndRescan_triggered()
{
    saveSettings();

    /*---------------------------------------------------------*\
    | Trigger rescan                                            |
    \*---------------------------------------------------------*/
    ResourceManager::get()->DetectDevices();
}

