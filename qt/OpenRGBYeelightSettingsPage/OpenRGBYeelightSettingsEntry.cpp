/*---------------------------------------------------------*\
| OpenRGBYeelightSettingsEntry.cpp                          |
|                                                           |
|   User interface for Yeelight settings entry              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QInputDialog>
#include "OpenRGBYeelightSettingsEntry.h"
#include "ui_OpenRGBYeelightSettingsEntry.h"
#include "net_port.h"

using namespace Ui;

OpenRGBYeelightSettingsEntry::OpenRGBYeelightSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::OpenRGBYeelightSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBYeelightSettingsEntry::~OpenRGBYeelightSettingsEntry()
{
    delete ui;
}

void OpenRGBYeelightSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBYeelightSettingsEntry::on_HostIPChooserButton_clicked()
{
    char hostname[256];
    gethostname(hostname, 256);

    char **in_addrs = gethostbyname(hostname)->h_addr_list;

    QStringList in_addr_list;

    while (*in_addrs != NULL)
    {
        in_addr_list << inet_ntoa(*((struct in_addr*) *in_addrs));
        in_addrs++;
    }

    QInputDialog inp;

    inp.setOptions(QInputDialog::UseListViewForComboBoxItems);
    inp.setComboBoxItems(in_addr_list);
    inp.setWindowTitle(tr("Choose an IP..."));
    inp.setLabelText(tr("Choose the correct IP for the host"));

    if(!inp.exec())
    {
        return;
    }

    ui->HostIPEdit->setText(inp.textValue());
}

void OpenRGBYeelightSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }

    if(data.contains("host_ip"))
    {
        ui->HostIPEdit->setText(QString::fromStdString(data["host_ip"]));
    }

    if(data.contains("music_mode"))
    {
        ui->MusicModeCheckBox->setChecked(data["music_mode"]);
    }
}

json OpenRGBYeelightSettingsEntry::saveSettings()
{
    json result;
    /*-------------------------------------------------*\
    | Required parameters                               |
    \*-------------------------------------------------*/
    result["ip"]          = ui->IPEdit->text().toStdString();
    result["host_ip"]     = ui->HostIPEdit->text().toStdString();
    result["music_mode"]  = ui->MusicModeCheckBox->isChecked();

    return result;
}

const char* OpenRGBYeelightSettingsEntry::settingsSection()
{
    return "YeelightDevices";
}
