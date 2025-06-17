/*---------------------------------------------------------*\
| OpenRGBPluginsPage.cpp                                    |
|                                                           |
|   User interface entry for OpenRGB plugin settings        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "filesystem.h"
#include "LogManager.h"
#include "SettingsManager.h"
#include "OpenRGBPluginsPage.h"
#include "ui_OpenRGBPluginsPage.h"
#include "ResourceManager.h"

void EnableClickCallbackFunction(void* this_ptr, void* entry_ptr)
{
    OpenRGBPluginsPage* this_page = (OpenRGBPluginsPage*)this_ptr;

    this_page->on_EnableButton_clicked((OpenRGBPluginsEntry*)entry_ptr);
}

OpenRGBPluginsPage::OpenRGBPluginsPage(PluginManager* plugin_manager_ptr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPluginsPage)
{
    plugin_manager = plugin_manager_ptr;
    ui->setupUi(this);

    RefreshList();
}

OpenRGBPluginsPage::~OpenRGBPluginsPage()
{
    delete ui;
}

void OpenRGBPluginsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBPluginsPage::RefreshList()
{
    ui->PluginsList->clear();
    entries.clear();

    for(const OpenRGBPluginEntry& plugin: plugin_manager->ActivePlugins)
    {
        OpenRGBPluginsEntry* entry = new OpenRGBPluginsEntry();

        /*---------------------------------------------------------*\
        | Fill in plugin information fields                         |
        \*---------------------------------------------------------*/
        entry->fillFrom(&plugin);

        entry->RegisterEnableClickCallback(EnableClickCallbackFunction, this);

        /*---------------------------------------------------------*\
        | Add the entry to the plugin list                          |
        \*---------------------------------------------------------*/
        QListWidgetItem* item = new QListWidgetItem;

        item->setSizeHint(entry->sizeHint());

        ui->PluginsList->addItem(item);
        ui->PluginsList->setItemWidget(item, entry);

        entries.push_back(entry);
    }
}

void OpenRGBPluginsPage::on_InstallPluginButton_clicked()
{
    /*-----------------------------------------------------*\
    | Open a file selection prompt to choose the plugin file|
    \*-----------------------------------------------------*/
    QString     install_file    = QFileDialog::getOpenFileName(this, tr("Install OpenRGB Plugin"), "", tr("Plugin files (*.dll *.dylib *.so *.so.*)"));

    bool installed = InstallPlugin(install_file.toStdString());

    if(installed)
    {
        RefreshList();
    }
}

bool OpenRGBPluginsPage::InstallPlugin(std::string install_file)
{
    filesystem::path from_path = filesystem::u8path(install_file);
    filesystem::path to_path   = ResourceManager::get()->GetConfigurationDirectory() / "plugins" / from_path.filename();
    bool        match           = false;

    LOG_TRACE("[OpenRGBPluginsPage] Installing plugin %s", install_file.c_str());

    /*-----------------------------------------------------*\
    | Check if a plugin with this path already exists       |
    \*-----------------------------------------------------*/
    for(unsigned int plugin_idx = 0; plugin_idx < plugin_manager->ActivePlugins.size(); plugin_idx++)
    {
        if(to_path == plugin_manager->ActivePlugins[plugin_idx].path)
        {
            match = true;
            break;
        }
    }

    /*-----------------------------------------------------*\
    | If this plugin already exists, prompt to replace it   |
    \*-----------------------------------------------------*/
    if(match == true)
    {
        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(this, tr("Replace Plugin"), tr("A plugin with this filename is already installed.  Are you sure you want to replace this plugin?"), QMessageBox::Yes | QMessageBox::No);

        if(reply != QMessageBox::Yes)
        {
            return false;
        }
    }

    /*-----------------------------------------------------*\
    | When replacing, remove the existing plugin before     |
    | copying the file and adding the new one               |
    \*-----------------------------------------------------*/
    try
    {
        plugin_manager->RemovePlugin(to_path);

        LOG_TRACE("[OpenRGBPluginsPage] Copying from %s to %s", from_path.c_str(), to_path.c_str());
        filesystem::copy(from_path, to_path, filesystem::copy_options::overwrite_existing);

        plugin_manager->AddPlugin(to_path, false);

        return true;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR("[OpenRGBPluginsPage] Failed to install plugin: %s", e.what());
    }

    return false;
}

void OpenRGBPluginsPage::on_RemovePluginButton_clicked()
{
    QMessageBox::StandardButton reply;

    /*-----------------------------------------------------*\
    | Confirm plugin removal with message box               |
    \*-----------------------------------------------------*/
    reply = QMessageBox::question(this, tr("Remove Plugin"), tr("Are you sure you want to remove this plugin?"), QMessageBox::Yes | QMessageBox::No);

    if(reply != QMessageBox::Yes)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Get index of selected plugin entry                    |
    \*-----------------------------------------------------*/
    int cur_row = ui->PluginsList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Don't allow removing system plugins                   |
    \*-----------------------------------------------------*/
    if(entries[cur_row]->isSystem())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Open plugin settings                                  |
    \*-----------------------------------------------------*/
    json plugin_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Plugins");

    /*-----------------------------------------------------*\
    | Find plugin's entry in settings and remove it         |
    \*-----------------------------------------------------*/
    if(plugin_settings.contains("plugins"))
    {
        for(unsigned int plugin_idx = 0; plugin_idx < plugin_settings["plugins"].size(); plugin_idx++)
        {
            if((plugin_settings["plugins"][plugin_idx].contains("name"))
             &&(plugin_settings["plugins"][plugin_idx].contains("description")))
            {
                if((plugin_settings["plugins"][plugin_idx]["name"] == entries[cur_row]->getName())
                 &&(plugin_settings["plugins"][plugin_idx]["description"] == entries[cur_row]->getDescription()))
                {
                    /*-------------------------------------*\
                    | Remove plugin from settings           |
                    \*-------------------------------------*/
                    plugin_settings["plugins"].erase(plugin_idx);
                }
            }
        }
    }

    /*-----------------------------------------------------*\
    | Mark plugin to be removed on next restart             |
    \*-----------------------------------------------------*/
    plugin_settings["plugins_remove"][plugin_settings["plugins_remove"].size()] = entries[cur_row]->getPath();

    ResourceManager::get()->GetSettingsManager()->SetSettings("Plugins", plugin_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();

    QMessageBox::information(this, tr("Restart Needed"), tr("The plugin will be fully removed after restarting OpenRGB."), QMessageBox::Ok);
}

void OpenRGBPluginsPage::on_EnableButton_clicked(OpenRGBPluginsEntry* entry)
{
    /*-----------------------------------------------------*\
    | Open plugin list and check if plugin is in the list   |
    \*-----------------------------------------------------*/
    json plugin_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Plugins");

    /*-----------------------------------------------------*\
    | Search the settings to find the correct index         |
    \*-----------------------------------------------------*/
    std::string     name        = "";
    std::string     description = "";
    bool            enabled     = entry->isPluginEnabled();
    bool            found       = false;
    unsigned int    plugin_ct   = 0;
    unsigned int    plugin_idx  = 0;

    std::string     entry_name  = entry->getName();
    std::string     entry_desc  = entry->getDescription();
    std::string     entry_path  = entry->getPath();

    if(plugin_settings.contains("plugins"))
    {
        plugin_ct = (unsigned int)plugin_settings["plugins"].size();

        for(plugin_idx = 0; plugin_idx < (unsigned int)plugin_settings["plugins"].size(); plugin_idx++)
        {
            if(plugin_settings["plugins"][plugin_idx].contains("name"))
            {
                name        = plugin_settings["plugins"][plugin_idx]["name"];
            }

            if(plugin_settings["plugins"][plugin_idx].contains("description"))
            {
                description = plugin_settings["plugins"][plugin_idx]["description"];
            }

            if((entry_name == name)
             &&(entry_desc == description))
            {
                found = true;
                break;
            }
        }
    }

    /*-----------------------------------------------------*\
    | If the plugin was not in the list, add it to the list |
    | and default it to enabled, then save the settings     |
    \*-----------------------------------------------------*/
    if(!found)
    {
        plugin_settings["plugins"][plugin_ct]["name"]           = entry_name;
        plugin_settings["plugins"][plugin_ct]["description"]    = entry_desc;
        plugin_settings["plugins"][plugin_ct]["enabled"]        = enabled;

        ResourceManager::get()->GetSettingsManager()->SetSettings("Plugins", plugin_settings);
        ResourceManager::get()->GetSettingsManager()->SaveSettings();
    }
    else
    {
        plugin_settings["plugins"][plugin_idx]["enabled"]       = enabled;
        ResourceManager::get()->GetSettingsManager()->SetSettings("Plugins", plugin_settings);
        ResourceManager::get()->GetSettingsManager()->SaveSettings();
    }

    if(enabled)
    {
        plugin_manager->EnablePlugin(entry_path);
    }
    else
    {
        plugin_manager->DisablePlugin(entry_path);
    }
}

void OpenRGBPluginsPage::on_PluginsList_itemSelectionChanged()
{
    /*-----------------------------------------------------*\
    | Get index of selected plugin entry                    |
    \*-----------------------------------------------------*/
    int cur_row = ui->PluginsList->currentRow();

    /*-----------------------------------------------------*\
    | Disable the remove button if no item selected         |
    \*-----------------------------------------------------*/
    if(cur_row == -1)
    {
        ui->RemovePluginButton->setEnabled(false);
        return;
    }

    /*-----------------------------------------------------*\
    | Enable the remove button when there's a selected item |
    | and the selected item is not a system plugin          |
    \*-----------------------------------------------------*/
    if(!entries[cur_row]->isSystem())
    {
        ui->RemovePluginButton->setEnabled(!ui->PluginsList->selectedItems().empty());
        ui->RemovePluginButton->setText("Remove Plugin");
    }
    else
    {
        ui->RemovePluginButton->setEnabled(false);
        ui->RemovePluginButton->setText("System Plugin - Cannot Remove");
    }
}

void OpenRGBPluginsPage::on_PluginsList_PluginsDropped(std::vector<std::string> path_list)
{
    bool installed = false;

    for(const std::string& file_path: path_list)
    {
        installed |= InstallPlugin(file_path);
    }

    if(installed)
    {
        RefreshList();
    }
}

