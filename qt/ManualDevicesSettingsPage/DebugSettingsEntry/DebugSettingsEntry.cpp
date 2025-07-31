/*---------------------------------------------------------*\
| DebugSettingsEntry.cpp                                    |
|                                                           |
|   User interface for OpenRGB Debug settings entry         |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      30 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "DebugSettingsEntry.h"
#include "ui_DebugSettingsEntry.h"
#include "ManualDevicesTypeManager.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

#define NUM_TYPES 6

const std::string types[] =
{
    "motherboard",
    "dram",
    "gpu",
    "keyboard",
    "mouse",
    "argb"
};

DebugSettingsEntry::DebugSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::DebugSettingsEntry)
{
    ui->setupUi(this);

    ui->TypeComboBox->addItem("motherboard");
    ui->TypeComboBox->addItem("dram");
    ui->TypeComboBox->addItem("gpu");
    ui->TypeComboBox->addItem("keyboard");
    ui->TypeComboBox->addItem("mouse");
    ui->TypeComboBox->addItem("argb");

    ui->LayoutComboBox->addItem("Default");
    ui->LayoutComboBox->addItem("ANSI QWERTY");
    ui->LayoutComboBox->addItem("ISO QWERTY");
    ui->LayoutComboBox->addItem("ISO QWERTZ");
    ui->LayoutComboBox->addItem("ISO AZERTY");
    ui->LayoutComboBox->addItem("JIS");
}

DebugSettingsEntry::~DebugSettingsEntry()
{
    delete ui;
}

void DebugSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void DebugSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("name"))
    {
        ui->NameEdit->setText(QString::fromStdString(data["name"]));
    }

    if(data.contains("type"))
    {
        for(unsigned int type_idx = 0; type_idx < NUM_TYPES; type_idx++)
        {
            if(data["type"] == types[type_idx])
            {
                ui->TypeComboBox->setCurrentIndex(type_idx);
                break;
            }
        }
    }

    if(data.contains("layout"))
    {
        ui->LayoutComboBox->setCurrentIndex(data["layout"]);
    }

    if(data.contains("single"))
    {
        ui->SingleCheckBox->setChecked(data["single"]);
    }

    if(data.contains("linear"))
    {
        ui->LinearCheckBox->setChecked(data["linear"]);
    }

    if(data.contains("resizable"))
    {
        ui->ResizableCheckBox->setChecked(data["resizable"]);
    }

    if(data.contains("keyboard"))
    {
        ui->KeyboardCheckBox->setChecked(data["keyboard"]);
    }

    if(data.contains("underglow"))
    {
        ui->UnderglowCheckBox->setChecked(data["underglow"]);
    }
}

json DebugSettingsEntry::saveSettings()
{
    json result;

    result["name"]      = ui->NameEdit->text().toStdString();
    result["type"]      = types[ui->TypeComboBox->currentIndex()];
    result["layout"]    = ui->LayoutComboBox->currentIndex();
    result["single"]    = ui->SingleCheckBox->isChecked();
    result["linear"]    = ui->LinearCheckBox->isChecked();
    result["resizable"] = ui->ResizableCheckBox->isChecked();
    result["keyboard"]  = ui->KeyboardCheckBox->isChecked();
    result["underglow"] = ui->UnderglowCheckBox->isChecked();

    return result;
}

bool DebugSettingsEntry::isDataValid()
{
    return true;
}

static BaseManualDeviceEntry* SpawnDebugEntry(const json& data)
{
    DebugSettingsEntry* entry = new DebugSettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

static const char* DebugDeviceName = QT_TRANSLATE_NOOP("ManualDevice", "Debug Device");

REGISTER_MANUAL_DEVICE_TYPE(DebugDeviceName, "DebugDevices", SpawnDebugEntry);
