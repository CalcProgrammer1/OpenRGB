#include "OpenRGBPluginsEntry.h"
#include "ui_OpenRGBPluginsEntry.h"

using namespace Ui;

OpenRGBPluginsEntry::OpenRGBPluginsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPluginsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBPluginsEntry::~OpenRGBPluginsEntry()
{
    delete ui;
}

void OpenRGBPluginsEntry::RegisterEnableClickCallback(EnableClickCallback new_callback, void * new_callback_arg)
{
    EnableClickCallbacks.push_back(new_callback);
    EnableClickCallbackArgs.push_back(new_callback_arg);
}

void Ui::OpenRGBPluginsEntry::on_EnabledCheckBox_stateChanged(int /*checked*/)
{
    /*-------------------------------------------------*\
    | Call the callbacks                                |
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < EnableClickCallbacks.size(); callback_idx++)
    {
        EnableClickCallbacks[callback_idx](EnableClickCallbackArgs[callback_idx], this);
    }
}

