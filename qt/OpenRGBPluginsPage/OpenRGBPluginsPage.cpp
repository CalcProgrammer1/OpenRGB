#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "filesystem.h"
#include "LogManager.h"
#include "OpenRGBPluginsPage.h"
#include "ui_OpenRGBPluginsPage.h"

void EnableClickCallbackFunction(void* this_ptr, void* entry_ptr)
{
    Ui::OpenRGBPluginsPage* this_page = (Ui::OpenRGBPluginsPage*)this_ptr;

    this_page->on_EnableButton_clicked((Ui::OpenRGBPluginsEntry*)entry_ptr);
}

Ui::OpenRGBPluginsPage::OpenRGBPluginsPage(PluginManager* plugin_manager_ptr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPluginsPageUi)
{
    plugin_manager = plugin_manager_ptr;
    ui->setupUi(this);

    RefreshList();
}

Ui::OpenRGBPluginsPage::~OpenRGBPluginsPage()
{
    delete ui;
}

void Ui::OpenRGBPluginsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBPluginsPage::RefreshList()
{
    ui->PluginsList->clear();
    entries.clear();

    for(unsigned int plugin_idx = 0; plugin_idx < plugin_manager->ActivePlugins.size(); plugin_idx++)
    {
        OpenRGBPluginsEntry* entry = new OpenRGBPluginsEntry();

        /*---------------------------------------------------------*\
        | Fill in plugin information fields                         |
        \*---------------------------------------------------------*/
        entry->ui->NameValue->setText(QString::fromStdString(plugin_manager->ActivePlugins[plugin_idx].info.Name));
        entry->ui->DescriptionValue->setText(QString::fromStdString(plugin_manager->ActivePlugins[plugin_idx].info.Description));
        entry->ui->VersionValue->setText(QString::fromStdString(plugin_manager->ActivePlugins[plugin_idx].info.Version));
        entry->ui->CommitValue->setText(QString::fromStdString(plugin_manager->ActivePlugins[plugin_idx].info.Commit));
        entry->ui->URLValue->setText(QString::fromStdString(plugin_manager->ActivePlugins[plugin_idx].info.URL));

        /*---------------------------------------------------------*\
        | Fill in plugin icon                                       |
        \*---------------------------------------------------------*/
        QPixmap pixmap(QPixmap::fromImage(plugin_manager->ActivePlugins[plugin_idx].info.Icon));

        entry->ui->IconView->setPixmap(pixmap);
        entry->ui->IconView->setScaledContents(true);

        /*---------------------------------------------------------*\
        | Fill in plugin path                                       |
        \*---------------------------------------------------------*/
        entry->ui->PathValue->setText(QString::fromStdString(plugin_manager->ActivePlugins[plugin_idx].path));

        /*---------------------------------------------------------*\
        | Fill in plugin enabled status                             |
        \*---------------------------------------------------------*/
        entry->ui->EnabledCheckBox->setChecked((plugin_manager->ActivePlugins[plugin_idx].enabled));

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

void Ui::OpenRGBPluginsPage::on_InstallPluginButton_clicked()
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

bool Ui::OpenRGBPluginsPage::InstallPlugin(std::string install_file)
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

        plugin_manager->AddPlugin(to_path);

        return true;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR("[OpenRGBPluginsPage] Failed to install plugin: %s", e.what());
    }

    return false;
}

void Ui::OpenRGBPluginsPage::on_RemovePluginButton_clicked()
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
                if((plugin_settings["plugins"][plugin_idx]["name"] == entries[cur_row]->ui->NameValue->text().toStdString())
                 &&(plugin_settings["plugins"][plugin_idx]["description"] == entries[cur_row]->ui->DescriptionValue->text().toStdString()))
                {
                    plugin_settings["plugins"].erase(plugin_idx);

                    ResourceManager::get()->GetSettingsManager()->SetSettings("Plugins", plugin_settings);
                    ResourceManager::get()->GetSettingsManager()->SaveSettings();

                    break;
                }
            }
        }
    }

    /*-----------------------------------------------------*\
    | Remove plugin entry from GUI plugin entries list      |
    \*-----------------------------------------------------*/
    QListWidgetItem* item = ui->PluginsList->takeItem(cur_row);

    ui->PluginsList->removeItemWidget(item);
    delete item;

    /*-----------------------------------------------------*\
    | Command plugin manager to unload and remove the plugin|
    \*-----------------------------------------------------*/
    plugin_manager->RemovePlugin(entries[cur_row]->ui->PathValue->text().toStdString());

    /*-----------------------------------------------------*\
    | Delete the plugin file and refresh the GUI            |
    \*-----------------------------------------------------*/
    filesystem::remove(entries[cur_row]->ui->PathValue->text().toStdString());

    RefreshList();
}

void Ui::OpenRGBPluginsPage::on_EnableButton_clicked(OpenRGBPluginsEntry* entry)
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
    bool            enabled     = true;
    bool            found       = false;
    unsigned int    plugin_ct   = 0;
    unsigned int    plugin_idx  = 0;

    std::string     entry_name  = entry->ui->NameValue->text().toStdString();
    std::string     entry_desc  = entry->ui->DescriptionValue->text().toStdString();
    std::string     entry_path  = entry->ui->PathValue->text().toStdString();

    if(entry->ui->EnabledCheckBox->isChecked())
    {
        enabled                 = true;
    }
    else
    {
        enabled                 = false;
    }

    if(plugin_settings.contains("plugins"))
    {
        plugin_ct = plugin_settings["plugins"].size();

        for(plugin_idx = 0; plugin_idx < plugin_settings["plugins"].size(); plugin_idx++)
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
        plugin_manager->LoadPlugin(entry_path);
    }
    else
    {
        plugin_manager->UnloadPlugin(entry_path);
    }
}

void Ui::OpenRGBPluginsPage::on_PluginsList_itemSelectionChanged()
{
    /*-----------------------------------------------------*\
    | Enable the remove button when there's a selected item |
    \*-----------------------------------------------------*/
    ui->RemovePluginButton->setEnabled(!ui->PluginsList->selectedItems().empty());
}

void Ui::OpenRGBPluginsPage::on_PluginsList_PluginsDropped(std::vector<std::string> path_list)
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

