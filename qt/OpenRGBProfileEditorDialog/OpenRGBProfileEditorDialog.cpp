/*---------------------------------------------------------*\
| OpenRGBProfileEditorDialog.cpp                            |
|                                                           |
|   User interface entry for OpenRGB profile editor dialog  |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      16 Feb 2026 |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <QTreeWidgetItem>
#include "OpenRGBFont.h"
#include "OpenRGBProfileEditorDialog.h"
#include "PluginManager.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

OpenRGBProfileEditorDialog::OpenRGBProfileEditorDialog(std::string name, QWidget* parent) : QDialog(parent), ui(new Ui::OpenRGBProfileEditorDialog)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Save profile name                                     |
    \*-----------------------------------------------------*/
    profile_name = name;

    /*-----------------------------------------------------*\
    | Get the existing profile JSON data                    |
    \*-----------------------------------------------------*/
    nlohmann::json profile_json = ResourceManager::get()->GetProfileManager()->ReadProfileJSON(profile_name);

    /*-----------------------------------------------------*\
    | Append profile name to window title                   |
    \*-----------------------------------------------------*/
    QString currentTitle = windowTitle();

    QString newTitle = currentTitle + " - " + QString::fromStdString(profile_name);

    setWindowTitle(newTitle);

    /*-----------------------------------------------------*\
    | Get the UserInterface settings hex format settings    |
    \*-----------------------------------------------------*/
    SettingsManager*    settings_manager    = ResourceManager::get()->GetSettingsManager();
    std::string         ui_string           = "UserInterface";
    json                ui_settings;

    ui_settings = settings_manager->GetSettings(ui_string);

    if(ui_settings.contains("hex_format"))
    {
        if(ui_settings["hex_format"] == "RGB")
        {
            HexFormatRGB = true;
        }
        else if(ui_settings["hex_format"] == "BGR")
        {
            HexFormatRGB = false;
        }
    }

    /*-----------------------------------------------------*\
    | Fill in the base color if it exists                   |
    \*-----------------------------------------------------*/
    RGBColor    base_color          = 0;
    bool        base_color_enabled  = false;

    if(profile_json.contains("base_color"))
    {
        base_color          = profile_json["base_color"];
        base_color_enabled  = true;
    }

    /*-----------------------------------------------------*\
    | If the hex format is BGR, swap R and B before         |
    | displaying as hex                                     |
    \*-----------------------------------------------------*/
    if(HexFormatRGB)
    {
        base_color = RGBGetRValue(base_color) << 16
                   | RGBGetGValue(base_color) << 8
                   | RGBGetBValue(base_color);
    }

    ui->CheckBoxBaseColor->setChecked(base_color_enabled);
    ui->ColorWheelBaseColor->setColor(base_color);

    ui->LineEditBaseColorHex->blockSignals(true);
    ui->LineEditBaseColorHex->setText(QString().asprintf("%06X", base_color));
    ui->LineEditBaseColorHex->blockSignals(false);

    /*-----------------------------------------------------*\
    | Create 3 controller lists:                            |
    |   * Updated controllers                               |
    |       Present in both the active controllers list and |
    |       in the existing profile.                        |
    |   * New controllers                                   |
    |       Present in the active controllers list but not  |
    |       in the existing profile.                        |
    |   * Old controllers                                   |
    |       Not present in the active controllersl list but |
    |       present in the existing profile.                |
    \*-----------------------------------------------------*/
    old_controllers = ResourceManager::get()->GetProfileManager()->GetControllerListFromProfileJson(profile_json);
    new_controllers = ResourceManager::get()->GetRGBControllers();


    Qt::CheckState state = Qt::Checked;

    if(old_controllers.size() == 0)
    {
        state = Qt::Unchecked;
    }

    /*-----------------------------------------------------*\
    | Search new and old lists for matches, move these      |
    | matches to the updated list                           |
    \*-----------------------------------------------------*/
    for(std::size_t new_controller_idx = 0; new_controller_idx < new_controllers.size(); new_controller_idx++)
    {
        for(std::size_t old_controller_idx = 0; old_controller_idx < old_controllers.size(); old_controller_idx++)
        {
            if(ProfileManager::CompareControllers(new_controllers[new_controller_idx], old_controllers[old_controller_idx]))
            {
                /*-----------------------------------------*\
                | Controllers match, copy from new list to  |
                | updated list, then remove from new and    |
                | old lists                                 |
                \*-----------------------------------------*/
                updated_controllers.push_back(new_controllers[new_controller_idx]);

                new_controllers.erase(new_controllers.begin() + new_controller_idx);
                old_controllers.erase(old_controllers.begin() + old_controller_idx);

                /*-----------------------------------------*\
                | Decrement new controller index as to      |
                | account for the entry being removed       |
                \*-----------------------------------------*/
                new_controller_idx--;

                break;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Set up the controller states tree view                |
    \*-----------------------------------------------------*/
    ui->TreeWidgetControllerStates->setColumnCount(4);
    ui->TreeWidgetControllerStates->setRootIsDecorated(false);
    ui->TreeWidgetControllerStates->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TreeWidgetControllerStates->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->TreeWidgetControllerStates->header()->setStretchLastSection(false);

    QStringList controller_header_labels;
    controller_header_labels << "" << "Controller Name" << "Action" << "Include In Profile";
    ui->TreeWidgetControllerStates->setHeaderLabels(controller_header_labels);

    /*-----------------------------------------------------*\
    | Add updated controllers to controller states list     |
    \*-----------------------------------------------------*/
    for(std::size_t controller_idx = 0; controller_idx < updated_controllers.size(); controller_idx++)
    {
        QTreeWidgetItem*    new_item    = new QTreeWidgetItem();
        QFont               font        = OpenRGBFont::GetFont();

        Qt::ItemFlags flags = new_item->flags();
        flags &= ~Qt::ItemIsSelectable;
        new_item->setFlags(flags);

        font.setPointSize(18);

        new_item->setFont(0, font);
        new_item->setText(0, OpenRGBFont::icon(OpenRGBFont::GetIconIDFromDeviceType(updated_controllers[controller_idx]->GetDeviceType())));
        new_item->setText(1, QString::fromStdString(updated_controllers[controller_idx]->GetName()));
        new_item->setText(2, "Update");
        new_item->setCheckState(3, state);

        ui->TreeWidgetControllerStates->addTopLevelItem(new_item);
    }

    /*-----------------------------------------------------*\
    | Add new controllers to controller states list         |
    \*-----------------------------------------------------*/
    for(std::size_t controller_idx = 0; controller_idx < new_controllers.size(); controller_idx++)
    {
        QTreeWidgetItem*    new_item    = new QTreeWidgetItem();
        QFont               font        = OpenRGBFont::GetFont();

        Qt::ItemFlags flags = new_item->flags();
        flags &= ~Qt::ItemIsSelectable;
        new_item->setFlags(flags);

        font.setPointSize(18);

        new_item->setFont(0, font);
        new_item->setText(0, OpenRGBFont::icon(OpenRGBFont::GetIconIDFromDeviceType(new_controllers[controller_idx]->GetDeviceType())));
        new_item->setText(1, QString::fromStdString(new_controllers[controller_idx]->GetName()));
        new_item->setText(2, "Add");
        new_item->setCheckState(3, state);

        ui->TreeWidgetControllerStates->addTopLevelItem(new_item);
    }

    /*-----------------------------------------------------*\
    | Add old controllers to controller states list         |
    \*-----------------------------------------------------*/
    for(std::size_t controller_idx = 0; controller_idx < old_controllers.size(); controller_idx++)
    {
        QTreeWidgetItem*    new_item    = new QTreeWidgetItem();
        QFont               font        = OpenRGBFont::GetFont();

        Qt::ItemFlags flags = new_item->flags();
        flags &= ~Qt::ItemIsSelectable;
        new_item->setFlags(flags);

        font.setPointSize(18);

        new_item->setFont(0, font);
        new_item->setText(0, OpenRGBFont::icon(OpenRGBFont::GetIconIDFromDeviceType(old_controllers[controller_idx]->GetDeviceType())));
        new_item->setText(1, QString::fromStdString(old_controllers[controller_idx]->GetName()));
        new_item->setText(2, "Keep");
        new_item->setCheckState(3, state);

        ui->TreeWidgetControllerStates->addTopLevelItem(new_item);
    }

    /*-----------------------------------------------------*\
    | Add plugins to plugins list                           |
    \*-----------------------------------------------------*/
    PluginManager* plugin_manager = (PluginManager*)ResourceManager::get()->GetPluginManager();

    ui->TreeWidgetPlugins->setColumnCount(3);
    ui->TreeWidgetPlugins->setRootIsDecorated(false);
    ui->TreeWidgetPlugins->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TreeWidgetPlugins->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->TreeWidgetPlugins->header()->setStretchLastSection(false);

    QStringList plugin_header_labels;
    plugin_header_labels << "" << "Plugin Name" << "Include In Profile";
    ui->TreeWidgetPlugins->setHeaderLabels(plugin_header_labels);

    if(plugin_manager)
    {
        for(std::size_t plugin_idx = 0; plugin_idx < plugin_manager->GetPluginCount(); plugin_idx++)
        {
            QTreeWidgetItem*    new_item        = new QTreeWidgetItem();

            Qt::ItemFlags flags = new_item->flags();
            flags &= ~Qt::ItemIsSelectable;
            new_item->setFlags(flags);

            state = Qt::Unchecked;

            if(profile_json.contains("plugins"))
            {
                if(profile_json["plugins"].contains(plugin_manager->GetPluginName(plugin_idx)))
                {
                    state = Qt::Checked;
                }
            }

            new_item->setIcon(0, QIcon(QPixmap::fromImage(plugin_manager->ActivePlugins[plugin_idx].info.Icon)));
            new_item->setText(1, QString::fromStdString(plugin_manager->GetPluginName(plugin_idx)));
            new_item->setCheckState(2, state);

            ui->TreeWidgetPlugins->addTopLevelItem(new_item);
        }
    }
}

OpenRGBProfileEditorDialog::~OpenRGBProfileEditorDialog()
{

}

bool OpenRGBProfileEditorDialog::show()
{
    int result = this->exec();

    if(result == QDialog::Rejected)
    {
        return(false);
    }
    else
    {
        bool                            base_color_enabled  = ui->CheckBoxBaseColor->isChecked();
        std::vector<std::string>        enabled_plugins;
        std::vector<RGBController *>    rgb_controllers;
        QTreeWidgetItem*                ControllersRootItem = ui->TreeWidgetControllerStates->invisibleRootItem();
        QTreeWidgetItem*                PluginsRootItem     = ui->TreeWidgetPlugins->invisibleRootItem();

        for(int item_idx = 0; item_idx < ControllersRootItem->childCount(); item_idx++)
        {
            QTreeWidgetItem*            item                = ControllersRootItem->child(item_idx);

            if(item->checkState(3) == Qt::Checked)
            {
                if(item_idx < (int)updated_controllers.size())
                {
                    rgb_controllers.push_back(updated_controllers[item_idx]);
                }
                else if(item_idx < (int)(updated_controllers.size() + new_controllers.size()))
                {
                    rgb_controllers.push_back(new_controllers[item_idx - updated_controllers.size()]);
                }
                else if(item_idx < (int)(updated_controllers.size() + new_controllers.size() + old_controllers.size()))
                {
                    rgb_controllers.push_back(old_controllers[item_idx - updated_controllers.size() - new_controllers.size()]);
                }
            }
        }

        for(int item_idx = 0; item_idx < PluginsRootItem->childCount(); item_idx++)
        {
            QTreeWidgetItem*            item                = PluginsRootItem->child(item_idx);

            if(item->checkState(2) == Qt::Checked)
            {
                enabled_plugins.push_back(item->text(1).toStdString());
            }
        }

        RGBColor rgb_color = (0x00FFFFFF & ui->ColorWheelBaseColor->color().rgb());

        /*-------------------------------------------------*\
        | If the hex format is BGR, swap R and B before     |
        | displaying as hex                                 |
        \*-------------------------------------------------*/
        if(HexFormatRGB)
        {
            rgb_color = RGBGetRValue(rgb_color) << 16
                      | RGBGetGValue(rgb_color) << 8
                      | RGBGetBValue(rgb_color);
        }


        ResourceManager::get()->GetProfileManager()->SaveProfileCustom(profile_name, rgb_controllers, rgb_color, base_color_enabled, enabled_plugins);
        return(true);
    }
}

void OpenRGBProfileEditorDialog::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBProfileEditorDialog::on_ColorWheelBaseColor_colorChanged(const QColor& color)
{
    RGBColor rgb_color = (0x00FFFFFF & color.rgb());

    /*-----------------------------------------------------*\
    | If the hex format is BGR, swap R and B before         |
    | displaying as hex                                     |
    \*-----------------------------------------------------*/
    if(!HexFormatRGB)
    {
        rgb_color = RGBGetRValue(rgb_color) << 16
                  | RGBGetGValue(rgb_color) << 8
                  | RGBGetBValue(rgb_color);
    }

    ui->LineEditBaseColorHex->blockSignals(true);
    ui->LineEditBaseColorHex->setText(QString().asprintf("%06X", rgb_color));
    ui->LineEditBaseColorHex->blockSignals(false);
}

void OpenRGBProfileEditorDialog::on_LineEditBaseColorHex_textChanged(const QString& text)
{
    /*-----------------------------------------------------*\
    | Make an editable copy of the string                   |
    \*-----------------------------------------------------*/
    QString temp = text;

    /*-----------------------------------------------------*\
    | Remove # character so that #XXXXXX color codes are    |
    | acceptable.  0xXXXXXX codes are already accepted by   |
    | toInt().  Convert into an RGBColor.  Mask off the     |
    | unused bits.                                          |
    \*-----------------------------------------------------*/
    QColor      color;
    RGBColor    rgb_color = (RGBColor)(0x00FFFFFF & temp.replace("#", "").toInt(NULL, 16));

    /*-----------------------------------------------------*\
    | Store new color into the current color QColor         |
    | Because RGBColor stores color in BGR format, we have  |
    | to reverse the R and B channels if the hex format is  |
    | RGB.                                                  |
    \*-----------------------------------------------------*/
    if(HexFormatRGB)
    {
        color.setRed(RGBGetBValue(rgb_color));
        color.setGreen(RGBGetGValue(rgb_color));
        color.setBlue(RGBGetRValue(rgb_color));
    }
    else
    {
        color.setRed(RGBGetRValue(rgb_color));
        color.setGreen(RGBGetGValue(rgb_color));
        color.setBlue(RGBGetBValue(rgb_color));
    }

    /*-----------------------------------------------------*\
    | Update the color wheel                                |
    \*-----------------------------------------------------*/
    ui->LineEditBaseColorHex->blockSignals(true);
    ui->ColorWheelBaseColor->setColor(color);
    ui->LineEditBaseColorHex->blockSignals(false);
}

void OpenRGBProfileEditorDialog::on_ButtonSelectAllControllerStates_clicked()
{
    QTreeWidgetItem* ControllersRootItem = ui->TreeWidgetControllerStates->invisibleRootItem();

    for(int item_idx = 0; item_idx < ControllersRootItem->childCount(); item_idx++)
    {
        ControllersRootItem->child(item_idx)->setCheckState(3, Qt::Checked);
    }
}

void OpenRGBProfileEditorDialog::on_ButtonSelectNoneControllerStates_clicked()
{
    QTreeWidgetItem* ControllersRootItem = ui->TreeWidgetControllerStates->invisibleRootItem();

    for(int item_idx = 0; item_idx < ControllersRootItem->childCount(); item_idx++)
    {
        ControllersRootItem->child(item_idx)->setCheckState(3, Qt::Unchecked);
    }
}

void OpenRGBProfileEditorDialog::on_ButtonSelectAllPlugins_clicked()
{
    QTreeWidgetItem* PluginsRootItem = ui->TreeWidgetPlugins->invisibleRootItem();

    for(int item_idx = 0; item_idx < PluginsRootItem->childCount(); item_idx++)
    {
        PluginsRootItem->child(item_idx)->setCheckState(2, Qt::Checked);
    }
}

void OpenRGBProfileEditorDialog::on_ButtonSelectNonePlugins_clicked()
{
    QTreeWidgetItem* PluginsRootItem = ui->TreeWidgetPlugins->invisibleRootItem();

    for(int item_idx = 0; item_idx < PluginsRootItem->childCount(); item_idx++)
    {
        PluginsRootItem->child(item_idx)->setCheckState(2, Qt::Unchecked);
    }
}
