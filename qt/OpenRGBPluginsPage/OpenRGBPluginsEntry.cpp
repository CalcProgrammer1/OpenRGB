/*---------------------------------------------------------*\
| OpenRGBPluginsEntry.cpp                                   |
|                                                           |
|   User interface entry for OpenRGB plugin entry widget    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBPluginsEntry.h"
#include "ui_OpenRGBPluginsEntry.h"

using namespace Ui;

OpenRGBPluginsEntry::OpenRGBPluginsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPluginsEntryUi)
{
    ui->setupUi(this);

    EnableClickCallbackVal  = nullptr;
    EnableClickCallbackArg  = nullptr;
}

OpenRGBPluginsEntry::~OpenRGBPluginsEntry()
{
    delete ui;
}

void OpenRGBPluginsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBPluginsEntry::RegisterEnableClickCallback(EnableClickCallback new_callback, void * new_callback_arg)
{
    EnableClickCallbackVal  = new_callback;
    EnableClickCallbackArg  = new_callback_arg;
}

void Ui::OpenRGBPluginsEntry::on_EnabledCheckBox_stateChanged(int /*checked*/)
{
    /*-------------------------------------------------*\
    | Call the callbacks                                |
    \*-------------------------------------------------*/
    if(EnableClickCallbackVal != nullptr)
    {
        EnableClickCallbackVal(EnableClickCallbackArg, this);
    }
}
