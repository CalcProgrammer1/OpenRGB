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
    | Store device flags                                    |
    \*-----------------------------------------------------*/
    edit_flags = dev->GetFlags();

    /*-----------------------------------------------------*\
    | Append device name to window title                    |
    \*-----------------------------------------------------*/
    QString currentTitle = windowTitle();

    QString newTitle = currentTitle + " - " + QString::fromStdString(edit_dev->GetDisplayName());

    setWindowTitle(newTitle);

    /*-----------------------------------------------------*\
    | Initialize device name                                |
    \*-----------------------------------------------------*/
    ui->LineEditDeviceName->blockSignals(true);
    ui->LineEditDeviceName->setText(QString::fromStdString(dev->GetDisplayName()));
    ui->LineEditDeviceName->blockSignals(false);

    if((edit_flags & CONTROLLER_FLAG_MANUALLY_CONFIGURABLE_NAME) == 0)
    {
        ui->LineEditDeviceName->setEnabled(false);
    }
    else if(edit_flags & CONTROLLER_FLAG_MANUALLY_CONFIGURED_NAME)
    {
        ui->LabelDeviceName->setText("Device Name (*):");
    }

    if(edit_flags & CONTROLLER_FLAG_MANUALLY_CONFIGURABLE_DEVICE_SPECIFIC)
    {
        /*-------------------------------------------------*\
        | Initialize configuration schema and data          |
        \*-------------------------------------------------*/
        nlohmann::json configuration_schema = edit_dev->GetDeviceSpecificConfigurationSchema();
        nlohmann::json configuration_value  = edit_dev->GetDeviceSpecificConfiguration();

        /*-------------------------------------------------*\
        | Loop through the schema and create an entry for   |
        | each setting                                      |
        \*-------------------------------------------------*/
        for(nlohmann::json::iterator json_iterator = configuration_schema.begin(); json_iterator != configuration_schema.end(); json_iterator++)
        {
            nlohmann::json                  schema_entry    = json_iterator.value();
            OpenRGBDynamicSettingsWidget*   item_widget     = new OpenRGBDynamicSettingsWidget(json_iterator.key(), schema_entry, configuration_value);

            item_widget->SetCallback(Callback, this);
            ui->ScrollAreaDeviceConfigurationLayout->addWidget(item_widget);
        }

        /*-------------------------------------------------*\
        | Add a spacer at the end to prevent expanding      |
        \*-------------------------------------------------*/
        QSpacerItem*    spacer          = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        ui->ScrollAreaDeviceConfigurationLayout->addItem(spacer);
    }
    else
    {
        ui->GroupBoxDeviceSpecificConfiguration->setHidden(true);
    }
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
        | Read the device name line edit                    |
        \*-------------------------------------------------*/
        std::string new_name = ui->LineEditDeviceName->text().toStdString();

        /*-------------------------------------------------*\
        | Configure the device                              |
        \*-------------------------------------------------*/
        edit_dev->ConfigureDevice(edit_flags, new_name);

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

void OpenRGBDeviceEditorDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBDeviceEditorDialog::on_ButtonResetDeviceConfiguration_clicked()
{
    edit_flags &= ~CONTROLLER_FLAGS_MANUALLY_CONFIGURED;

    /*-------------------------------------------------*\
    | Configure the device                              |
    \*-------------------------------------------------*/
    edit_dev->ConfigureDevice(edit_flags, "");

    /*-------------------------------------------------*\
    | Save the configuration                            |
    \*-------------------------------------------------*/
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        profile_manager->SaveConfiguration();
    }

    done(0);
}

void OpenRGBDeviceEditorDialog::on_LineEditDeviceName_textChanged(const QString& /*arg1*/)
{
    if((edit_flags & CONTROLLER_FLAG_MANUALLY_CONFIGURED_NAME) == 0)
    {
        edit_flags |= CONTROLLER_FLAG_MANUALLY_CONFIGURED_NAME;
        ui->LabelDeviceName->setText("Device Name (*):");
    }
}

void OpenRGBDeviceEditorDialog::OnSettingChanged(std::string /*key*/, nlohmann::json settings)
{
    edit_flags |= CONTROLLER_FLAG_MANUALLY_CONFIGURED_DEVICE_SPECIFIC;
    device_configuration.update(settings, true);
}
