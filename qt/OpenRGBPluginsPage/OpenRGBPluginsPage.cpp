#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "filesystem.h"
#include "OpenRGBPluginsPage.h"
#include "ui_OpenRGBPluginsPage.h"

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
    QString     install_file    = QFileDialog::getOpenFileName(this, "Install OpenRGB Plugin", "", "DLL Files (*.dll; *.dylib; *.so; *.so.*)");

    std::string from_path       = install_file.toStdString();
    std::string to_path         = ResourceManager::get()->GetConfigurationDirectory() + "plugins/";
    std::string to_file         = to_path + filesystem::path(from_path).filename().string();
    bool        match           = false;

    for(unsigned int plugin_idx = 0; plugin_idx < plugin_manager->ActivePlugins.size(); plugin_idx++)
    {
        if(to_file == plugin_manager->ActivePlugins[plugin_idx].path)
        {
            match = true;
            break;
        }
    }

    if(match == true)
    {
        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(this, "Replace Plugin", "A plugin with this filename is already installed.  Are you sure you want to replace this plugin?", QMessageBox::Yes | QMessageBox::No);

        if(reply != QMessageBox::Yes)
        {
            return;
        }
    }

    try
    {
        filesystem::copy(from_path, to_path, filesystem::copy_options::update_existing);

        //TODO: Unregister the old plugin and load the new one if matched
        // For now, don't load the new plugin

        if(match == false)
        {
            plugin_manager->LoadPlugin(to_path + "/" + filesystem::path(from_path).filename().string());

            RefreshList();
        }
    }
    catch(std::exception& e)
    {

    }
}

void Ui::OpenRGBPluginsPage::on_RemovePluginButton_clicked()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Remove Plugin", "Are you sure you want to remove this plugin?", QMessageBox::Yes | QMessageBox::No);

    if(reply != QMessageBox::Yes)
    {
        return;
    }

    int cur_row = ui->PluginsList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->PluginsList->takeItem(cur_row);

    ui->PluginsList->removeItemWidget(item);
    delete item;

    //TODO: Unregister the plugin from the plugin manager

    filesystem::remove(entries[cur_row]->ui->PathValue->text().toStdString());

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

