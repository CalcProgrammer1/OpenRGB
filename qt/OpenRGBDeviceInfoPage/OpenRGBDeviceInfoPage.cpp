/*---------------------------------------------------------*\
| OpenRGBDeviceInfoPage.cpp                                 |
|                                                           |
|   User interface for OpenRGB device information page      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "OpenRGBDeviceInfoPage.h"
#include "ResourceManager.h"
#include "ui_OpenRGBDeviceInfoPage.h"

static void UpdateCallback(void * this_ptr, unsigned int update_reason, void * /*controller_ptr*/)
{
    OpenRGBDeviceInfoPage * this_obj = (OpenRGBDeviceInfoPage *)this_ptr;

    switch(update_reason)
    {
        case RGBCONTROLLER_UPDATE_REASON_CONFIGUREDEVICE:
            QMetaObject::invokeMethod(this_obj, "UpdateInterface", Qt::QueuedConnection);
            break;
    }
}

OpenRGBDeviceInfoPage::OpenRGBDeviceInfoPage(RGBController *dev, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBDeviceInfoPage)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Store device pointer                                  |
    \*-----------------------------------------------------*/
    controller = dev;

    /*-----------------------------------------------------*\
    | Register update callback with the device              |
    \*-----------------------------------------------------*/
    controller->RegisterUpdateCallback(UpdateCallback, this);

    UpdateInterface();
}

OpenRGBDeviceInfoPage::~OpenRGBDeviceInfoPage()
{
    /*-----------------------------------------------------*\
    | Unregister update callback from the controller if the |
    | controller still exists                               |
    \*-----------------------------------------------------*/
    for(unsigned int controller_idx = 0; controller_idx < ResourceManager::get()->GetRGBControllers().size(); controller_idx++)
    {
        if(ResourceManager::get()->GetRGBControllers()[controller_idx] == controller)
        {
            controller->UnregisterUpdateCallback(this);
            break;
        }
    }

    delete ui;
}

/*---------------------------------------------------------*\
| Callback handler                                          |
\*---------------------------------------------------------*/
void OpenRGBDeviceInfoPage::UpdateInterface()
{
    ui->TypeValue->setText(RGBController::DeviceTypeToString(controller->GetDeviceType()).c_str());

    ui->DisplayNameValue->setText(QString::fromStdString(controller->GetDisplayName()));
    ui->NameValue->setText(QString::fromStdString(controller->GetName()));
    ui->VendorValue->setText(QString::fromStdString(controller->GetVendor()));
    ui->DescriptionValue->setText(QString::fromStdString(controller->GetDescription()));
    ui->VersionValue->setText(QString::fromStdString(controller->GetVersion()));
    ui->LocationValue->setText(QString::fromStdString(controller->GetLocation()));
    ui->SerialValue->setText(QString::fromStdString(controller->GetSerial()));

    unsigned int    flags           = controller->GetFlags();
    std::string     flags_string    = "";
    bool            need_separator  = false;

    if(flags & CONTROLLER_FLAG_LOCAL)
    {
        flags_string   += "Local";
        need_separator  = true;
    }
    if(flags & CONTROLLER_FLAG_REMOTE)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Remote";
        need_separator  = true;
    }
    if(flags & CONTROLLER_FLAG_VIRTUAL)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Virtual";
        need_separator  = true;
    }
    if(flags & CONTROLLER_FLAG_HIDDEN)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Hidden";
        need_separator  = true;
    }
    if(flags & CONTROLLER_FLAG_RESET_BEFORE_UPDATE)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Reset Before Update";
        need_separator  = true;
    }
    if(flags & CONTROLLER_FLAG_MANUALLY_CONFIGURABLE_NAME)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Manually Configurable Name";
        need_separator  = true;
    }
    if(flags & CONTROLLER_FLAG_MANUALLY_CONFIGURABLE_DEVICE_SPECIFIC)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Manually Configurable Device Specific";
        need_separator  = true;
    }
    if(flags & CONTROLLER_FLAG_MANUALLY_CONFIGURED_NAME)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Manually Configured Name";
        need_separator  = true;
    }
    if(flags & CONTROLLER_FLAG_MANUALLY_CONFIGURED_DEVICE_SPECIFIC)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Manually Configured Device Specific";
        need_separator  = true;
    }

    ui->FlagsValue->setText(QString::fromStdString(flags_string));
}

/*---------------------------------------------------------*\
| Retranslate Event Slot                                    |
\*---------------------------------------------------------*/
void OpenRGBDeviceInfoPage::changeEvent(QEvent *event)
{
    /*-----------------------------------------------------*\
    | Retranslate the UI when a language change event occurs|
    \*-----------------------------------------------------*/
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

RGBController* OpenRGBDeviceInfoPage::GetController()
{
    return controller;
}
