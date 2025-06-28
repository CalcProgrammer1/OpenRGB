/*---------------------------------------------------------*\
| E131SettingsEntry.cpp                                     |
|                                                           |
|   User interface for OpenRGB E1.31 settings entry         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "E131SettingsEntry.h"
#include "ui_E131SettingsEntry.h"

E131SettingsEntry::E131SettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::E131SettingsEntry)
{
    ui->setupUi(this);

    ui->TypeComboBox->addItem(tr("Single"));
    ui->TypeComboBox->addItem(tr("Linear"));
    ui->TypeComboBox->addItem(tr("Matrix"));

    ui->MatrixOrderComboBox->addItem(tr("Horizontal Top Left"));
    ui->MatrixOrderComboBox->addItem(tr("Horizontal Top Right"));
    ui->MatrixOrderComboBox->addItem(tr("Horizontal Bottom Left"));
    ui->MatrixOrderComboBox->addItem(tr("Horizontal Bottom Right"));
    ui->MatrixOrderComboBox->addItem(tr("Vertical Top Left"));
    ui->MatrixOrderComboBox->addItem(tr("Vertical Top Right"));
    ui->MatrixOrderComboBox->addItem(tr("Vertical Bottom Left"));
    ui->MatrixOrderComboBox->addItem(tr("Vertical Bottom Right"));

    ui->RGBOrderComboBox->addItem("RGB");
    ui->RGBOrderComboBox->addItem("RBG");
    ui->RGBOrderComboBox->addItem("GRB");
    ui->RGBOrderComboBox->addItem("GBR");
    ui->RGBOrderComboBox->addItem("BRG");
    ui->RGBOrderComboBox->addItem("BGR");

    HideMatrixSettings();
}

E131SettingsEntry::~E131SettingsEntry()
{
    delete ui;
}

void E131SettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void E131SettingsEntry::HideMatrixSettings()
{
    ui->MatrixWidthLabel->setDisabled(true);
    ui->MatrixWidthEdit->setDisabled(true);

    ui->MatrixHeightLabel->setDisabled(true);
    ui->MatrixHeightEdit->setDisabled(true);

    ui->MatrixOrderLabel->setDisabled(true);
    ui->MatrixOrderComboBox->setDisabled(true);
}

void E131SettingsEntry::ShowMatrixSettings()
{
    ui->MatrixWidthLabel->setDisabled(false);
    ui->MatrixWidthEdit->setDisabled(false);

    ui->MatrixHeightLabel->setDisabled(false);
    ui->MatrixHeightEdit->setDisabled(false);

    ui->MatrixOrderLabel->setDisabled(false);
    ui->MatrixOrderComboBox->setDisabled(false);
}

void E131SettingsEntry::on_TypeComboBox_currentIndexChanged(int index)
{
    if(index == 2)
    {
        ShowMatrixSettings();
    }
    else
    {
        HideMatrixSettings();
    }
}

void E131SettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("name"))
    {
        ui->NameEdit->setText(QString::fromStdString(data["name"]));
    }

    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }

    if(data.contains("start_universe"))
    {
        ui->StartUniverseEdit->setText(QString::number((int)data["start_universe"]));
    }

    if(data.contains("start_channel"))
    {
        ui->StartChannelEdit->setText(QString::number((int)data["start_channel"]));
    }

    if(data.contains("num_leds"))
    {
        ui->NumLEDsEdit->setText(QString::number((int)data["num_leds"]));
    }

    if(data.contains("type"))
    {
        if(data["type"].is_string())
        {
            std::string type_val = data["type"];

            if(type_val == "SINGLE")
            {
                ui->TypeComboBox->setCurrentIndex(0);
            }
            else if(type_val == "LINEAR")
            {
                ui->TypeComboBox->setCurrentIndex(1);
            }
            else if(type_val == "MATRIX")
            {
                ui->TypeComboBox->setCurrentIndex(2);
            }
        }
        else
        {
            ui->TypeComboBox->setCurrentIndex(data["type"]);
        }
    }

    if(data.contains("rgb_order"))
    {
        if(data["rgb_order"].is_string())
        {
            std::string rgb_order_val = data["rgb_order"];

            if(rgb_order_val == "RGB")
            {
                ui->RGBOrderComboBox->setCurrentIndex(0);
            }
            else if(rgb_order_val == "RBG")
            {
                ui->RGBOrderComboBox->setCurrentIndex(1);
            }
            else if(rgb_order_val == "GRB")
            {
                ui->RGBOrderComboBox->setCurrentIndex(2);
            }
            else if(rgb_order_val == "GBR")
            {
                ui->RGBOrderComboBox->setCurrentIndex(3);
            }
            else if(rgb_order_val == "BRG")
            {
                ui->RGBOrderComboBox->setCurrentIndex(4);
            }
            else if(rgb_order_val == "BGR")
            {
                ui->RGBOrderComboBox->setCurrentIndex(5);
            }
        }
        else
        {
            ui->RGBOrderComboBox->setCurrentIndex(data["rgb_order"]);
        }
    }

    if(data.contains("matrix_width"))
    {
        ui->MatrixWidthEdit->setText(QString::number((int)data["matrix_width"]));
    }

    if(data.contains("matrix_height"))
    {
        ui->MatrixHeightEdit->setText(QString::number((int)data["matrix_height"]));
    }

    if(data.contains("matrix_order"))
    {
        if(data["matrix_order"].is_string())
        {
            std::string matrix_order_val = data["matrix_order"];

            if(matrix_order_val == "HORIZONTAL_TOP_LEFT")
            {
                ui->MatrixOrderComboBox->setCurrentIndex(0);
            }
            else if(matrix_order_val == "HORIZONTAL_TOP_RIGHT")
            {
                ui->MatrixOrderComboBox->setCurrentIndex(1);
            }
            else if(matrix_order_val == "HORIZONTAL_BOTTOM_LEFT")
            {
                ui->MatrixOrderComboBox->setCurrentIndex(2);
            }
            else if(matrix_order_val == "HORIZONTAL_BOTTOM_RIGHT")
            {
                ui->MatrixOrderComboBox->setCurrentIndex(3);
            }
            else if(matrix_order_val == "VERTICAL_TOP_LEFT")
            {
                ui->MatrixOrderComboBox->setCurrentIndex(4);
            }
            else if(matrix_order_val == "VERTICAL_TOP_RIGHT")
            {
                ui->MatrixOrderComboBox->setCurrentIndex(5);
            }
            else if(matrix_order_val == "VERTICAL_BOTTOM_LEFT")
            {
                ui->MatrixOrderComboBox->setCurrentIndex(6);
            }
            else if(matrix_order_val == "VERTICAL_BOTTOM_RIGHT")
            {
                ui->MatrixOrderComboBox->setCurrentIndex(7);
            }
        }
        else
        {
            ui->MatrixOrderComboBox->setCurrentIndex(data["matrix_order"]);
        }
    }

    if(data.contains("universe_size"))
    {
        ui->UniverseSizeEdit->setText(QString::number((int)data["universe_size"]));
    }

    if(data.contains("keepalive_time"))
    {
        ui->KeepaliveTimeEdit->setText(QString::number((int)data["keepalive_time"]));
    }
}

json E131SettingsEntry::saveSettings()
{
    json result;
    /*-------------------------------------------------*\
    | Required parameters                               |
    \*-------------------------------------------------*/
    result["name"]                = ui->NameEdit->text().toStdString();
    result["start_universe"]      = ui->StartUniverseEdit->text().toUInt();
    result["start_channel"]       = ui->StartChannelEdit->text().toUInt();
    result["num_leds"]            = ui->NumLEDsEdit->text().toUInt();
    result["type"]                = ui->TypeComboBox->currentIndex();
    result["rgb_order"]           = ui->RGBOrderComboBox->currentIndex();

    /*-------------------------------------------------*\
    | Optional parameters                               |
    \*-------------------------------------------------*/
    if(ui->IPEdit->text() != "")
    {
        result["ip"]              = ui->IPEdit->text().toStdString();
    }

    if(result["type"] == 2)
    {
        result["matrix_width"]    = ui->MatrixWidthEdit->text().toUInt();
        result["matrix_height"]   = ui->MatrixHeightEdit->text().toUInt();
        result["matrix_order"]    = ui->MatrixOrderComboBox->currentIndex();
    }

    if(ui->UniverseSizeEdit->text() != "")
    {
        result["universe_size"]   = ui->UniverseSizeEdit->text().toUInt();
    }

    if(ui->KeepaliveTimeEdit->text() != "")
    {
        result["keepalive_time"]  = ui->KeepaliveTimeEdit->text().toUInt();
    }

    return result;
}

bool E131SettingsEntry::isDataValid()
{
    // stub
    return true;
}

static BaseManualDeviceEntry* SpawnE131Entry(const json& data)
{
    E131SettingsEntry* entry = new E131SettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

static const char* E131DeviceName = QT_TRANSLATE_NOOP("ManualDevice", "E1.31 (including WLED)");

REGISTER_MANUAL_DEVICE_TYPE(E131DeviceName, "E131Devices", SpawnE131Entry);
