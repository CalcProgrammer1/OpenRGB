/*---------------------------------------------------------*\
| OpenRGBDeviceEditorDialog.cpp                             |
|                                                           |
|   User interface for OpenRGB device editor dialog         |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      15 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "OpenRGBDynamicSettingsWidget.h"
#include "OpenRGBDeviceEditorDialog.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "ui_OpenRGBDeviceEditorDialog.h"

static void Callback(void* this_ptr, std::string key, nlohmann::json settings)
{
    ((OpenRGBDeviceEditorDialog*)this_ptr)->OnSettingChanged(key, settings);
}

OpenRGBDeviceEditorDialog::OpenRGBDeviceEditorDialog(RGBController *dev, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBDeviceEditorDialog)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Store device pointer                                  |
    \*-----------------------------------------------------*/
    edit_dev = dev;

    /*-----------------------------------------------------*\
    | Append zone name to window title                      |
    \*-----------------------------------------------------*/
    QString currentTitle = windowTitle();

    QString newTitle = currentTitle + " - " + QString::fromStdString(edit_dev->GetName());

    setWindowTitle(newTitle);

    /*-----------------------------------------------------*\
    | Initialize configuration schema and data              |
    \*-----------------------------------------------------*/
    nlohmann::json configuration_schema = edit_dev->GetDeviceSpecificConfigurationSchema();
    nlohmann::json configuration_value  = edit_dev->GetDeviceSpecificConfiguration();

    /*-----------------------------------------------------*\
    | Loop through the schema and create an entry for each  |
    | setting                                               |
    \*-----------------------------------------------------*/
    for(nlohmann::json::iterator json_iterator = configuration_schema.begin(); json_iterator != configuration_schema.end(); json_iterator++)
    {
        nlohmann::json                  schema_entry    = json_iterator.value();
        OpenRGBDynamicSettingsWidget*   item_widget     = new OpenRGBDynamicSettingsWidget(json_iterator.key(), schema_entry, configuration_value);

        item_widget->SetCallback(Callback, this);
        ui->ScrollAreaDeviceConfigurationLayout->addWidget(item_widget);
    }

    /*-----------------------------------------------------*\
    | Add a spacer at the end to prevent expanding          |
    \*-----------------------------------------------------*/
    QSpacerItem*    spacer          = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->ScrollAreaDeviceConfigurationLayout->addItem(spacer);
}

OpenRGBDeviceEditorDialog::~OpenRGBDeviceEditorDialog()
{

}

int OpenRGBDeviceEditorDialog::show()
{
    /*-----------------------------------------------------*\
    | Execute this dialog                                   |
    \*-----------------------------------------------------*/
    int ret_val                 = 0;
    int result                  = this->exec();

    /*-----------------------------------------------------*\
    | Return -1 if cancelled or edit device is invalid      |
    \*-----------------------------------------------------*/
    if(result == QDialog::Rejected || edit_dev == NULL)
    {
        ret_val                 = -1;
    }
    else
    {
        /*-------------------------------------------------*\
        | Apply the configuration                           |
        \*-------------------------------------------------*/
        edit_dev->SetDeviceSpecificConfiguration(device_configuration);

        /*-------------------------------------------------*\
        | Save the configuration                            |
        \*-------------------------------------------------*/
        ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

        if(profile_manager != NULL)
        {
            profile_manager->SaveConfiguration();
        }
    }

    return(ret_val);
}

void OpenRGBDeviceEditorDialog::OnSettingChanged(std::string /*key*/, nlohmann::json settings)
{
    device_configuration.update(settings, true);
}
