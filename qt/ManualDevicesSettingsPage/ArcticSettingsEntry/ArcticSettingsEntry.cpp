/*-------------------------------------------------------------*\
| ArcticSettingsEntry.cpp                                       |
|                                                               |
|   User interface for OpenRGB Arctic controller settings entry |
|                                                               |
|   This file is part of the OpenRGB project                    |
|   SPDX-License-Identifier: GPL-2.0-or-later                   |
\*-------------------------------------------------------------*/

#include "ArcticSettingsEntry.h"
#include "ui_ArcticSettingsEntry.h"

#include "serial_port.h"
#include <QStandardItemModel>

ArcticSettingsEntry::ArcticSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::ArcticSettingsEntry)
{
    ui->setupUi(this);

    std::vector<std::string> serialPorts = serial_port::getSerialPorts();
    for(size_t i = 0; i < serialPorts.size(); ++i)
    {
        ui->PortComboBox->addItem(QString::fromStdString(serialPorts[i]));
    }
    if(serialPorts.empty())
    {
        /*---------------------------------------------------*\
        | When no ports were found, add an unselectable entry |
        | denoting this fact istead                           |
        \*---------------------------------------------------*/
        QStandardItemModel* comboBoxModel = qobject_cast<QStandardItemModel *>(ui->PortComboBox->model());
        if(comboBoxModel != nullptr)
        {
            ui->PortComboBox->addItem(tr("No serial ports found"));
            QStandardItem *item = comboBoxModel->item(0);
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        }
    }
    ui->PortComboBox->clearEditText();
}

ArcticSettingsEntry::~ArcticSettingsEntry()
{
    delete ui;
}

void ArcticSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void ArcticSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("port"))
    {
        ui->PortComboBox->setCurrentText(QString::fromStdString(data["port"]));
    }
}

json ArcticSettingsEntry::saveSettings()
{
    json result;

    result["port"] = ui->PortComboBox->currentText().toStdString();

    return result;
}

bool ArcticSettingsEntry::isDataValid()
{
    // stub
    return true;
}

static BaseManualDeviceEntry* SpawnArcticEntry(const json& data)
{
    ArcticSettingsEntry* entry = new ArcticSettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

REGISTER_MANUAL_DEVICE_TYPE("Arctic", "ArcticDevices", SpawnArcticEntry);
