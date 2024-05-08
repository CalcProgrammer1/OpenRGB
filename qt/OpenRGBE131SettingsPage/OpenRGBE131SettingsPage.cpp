/*---------------------------------------------------------*\
| OpenRGBE131SettingsPage.cpp                               |
|                                                           |
|   User interface for OpenRGB E1.31 settings page          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBE131SettingsPage.h"
#include "ui_OpenRGBE131SettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBE131SettingsPage::OpenRGBE131SettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBE131SettingsPageUi)
{
    ui->setupUi(this);

    json                e131_settings;

    /*-------------------------------------------------*\
    | Get E1.31 settings from settings manager          |
    \*-------------------------------------------------*/
    e131_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("E131Devices");

    /*-------------------------------------------------*\
    | If the E1.31 settings contains devices, process   |
    \*-------------------------------------------------*/
    if(e131_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < e131_settings["devices"].size(); device_idx++)
        {
            OpenRGBE131SettingsEntry* entry = new OpenRGBE131SettingsEntry;

            if(e131_settings["devices"][device_idx].contains("name"))
            {
                entry->ui->NameEdit->setText(QString::fromStdString(e131_settings["devices"][device_idx]["name"]));
            }

            if(e131_settings["devices"][device_idx].contains("ip"))
            {
                entry->ui->IPEdit->setText(QString::fromStdString(e131_settings["devices"][device_idx]["ip"]));
            }

            if(e131_settings["devices"][device_idx].contains("start_universe"))
            {
                entry->ui->StartUniverseEdit->setText(QString::number((int)e131_settings["devices"][device_idx]["start_universe"]));
            }

            if(e131_settings["devices"][device_idx].contains("start_channel"))
            {
                entry->ui->StartChannelEdit->setText(QString::number((int)e131_settings["devices"][device_idx]["start_channel"]));
            }

            if(e131_settings["devices"][device_idx].contains("num_leds"))
            {
                entry->ui->NumLEDsEdit->setText(QString::number((int)e131_settings["devices"][device_idx]["num_leds"]));
            }

            if(e131_settings["devices"][device_idx].contains("type"))
            {
                if(e131_settings["devices"][device_idx]["type"].is_string())
                {
                    std::string type_val = e131_settings["devices"][device_idx]["type"];

                    if(type_val == "SINGLE")
                    {
                        entry->ui->TypeComboBox->setCurrentIndex(0);
                    }
                    else if(type_val == "LINEAR")
                    {
                        entry->ui->TypeComboBox->setCurrentIndex(1);
                    }
                    else if(type_val == "MATRIX")
                    {
                        entry->ui->TypeComboBox->setCurrentIndex(2);
                    }
                }
                else
                {
                    entry->ui->TypeComboBox->setCurrentIndex(e131_settings["devices"][device_idx]["type"]);
                }
            }

            if(e131_settings["devices"][device_idx].contains("rgb_order"))
            {
                if(e131_settings["devices"][device_idx]["rgb_order"].is_string())
                {
                    std::string rgb_order_val = e131_settings["devices"][device_idx]["rgb_order"];

                    if(rgb_order_val == "RGB")
                    {
                        entry->ui->RGBOrderComboBox->setCurrentIndex(0);
                    }
                    else if(rgb_order_val == "RBG")
                    {
                        entry->ui->RGBOrderComboBox->setCurrentIndex(1);
                    }
                    else if(rgb_order_val == "GRB")
                    {
                        entry->ui->RGBOrderComboBox->setCurrentIndex(2);
                    }
                    else if(rgb_order_val == "GBR")
                    {
                        entry->ui->RGBOrderComboBox->setCurrentIndex(3);
                    }
                    else if(rgb_order_val == "BRG")
                    {
                        entry->ui->RGBOrderComboBox->setCurrentIndex(4);
                    }
                    else if(rgb_order_val == "BGR")
                    {
                        entry->ui->RGBOrderComboBox->setCurrentIndex(5);
                    }
                }
                else
                {
                    entry->ui->RGBOrderComboBox->setCurrentIndex(e131_settings["devices"][device_idx]["rgb_order"]);
                }
            }

            if(e131_settings["devices"][device_idx].contains("matrix_width"))
            {
                entry->ui->MatrixWidthEdit->setText(QString::number((int)e131_settings["devices"][device_idx]["matrix_width"]));
            }

            if(e131_settings["devices"][device_idx].contains("matrix_height"))
            {
                entry->ui->MatrixHeightEdit->setText(QString::number((int)e131_settings["devices"][device_idx]["matrix_height"]));
            }

            if(e131_settings["devices"][device_idx].contains("matrix_order"))
            {
                if(e131_settings["devices"][device_idx]["matrix_order"].is_string())
                {
                    std::string matrix_order_val = e131_settings["devices"][device_idx]["matrix_order"];

                    if(matrix_order_val == "HORIZONTAL_TOP_LEFT")
                    {
                        entry->ui->MatrixOrderComboBox->setCurrentIndex(0);
                    }
                    else if(matrix_order_val == "HORIZONTAL_TOP_RIGHT")
                    {
                        entry->ui->MatrixOrderComboBox->setCurrentIndex(1);
                    }
                    else if(matrix_order_val == "HORIZONTAL_BOTTOM_LEFT")
                    {
                        entry->ui->MatrixOrderComboBox->setCurrentIndex(2);
                    }
                    else if(matrix_order_val == "HORIZONTAL_BOTTOM_RIGHT")
                    {
                        entry->ui->MatrixOrderComboBox->setCurrentIndex(3);
                    }
                    else if(matrix_order_val == "VERTICAL_TOP_LEFT")
                    {
                        entry->ui->MatrixOrderComboBox->setCurrentIndex(4);
                    }
                    else if(matrix_order_val == "VERTICAL_TOP_RIGHT")
                    {
                        entry->ui->MatrixOrderComboBox->setCurrentIndex(5);
                    }
                    else if(matrix_order_val == "VERTICAL_BOTTOM_LEFT")
                    {
                        entry->ui->MatrixOrderComboBox->setCurrentIndex(6);
                    }
                    else if(matrix_order_val == "VERTICAL_BOTTOM_RIGHT")
                    {
                        entry->ui->MatrixOrderComboBox->setCurrentIndex(7);
                    }
                }
                else
                {
                    entry->ui->MatrixOrderComboBox->setCurrentIndex(e131_settings["devices"][device_idx]["matrix_order"]);
                }
            }

            if(e131_settings["devices"][device_idx].contains("universe_size"))
            {
                entry->ui->UniverseSizeEdit->setText(QString::number((int)e131_settings["devices"][device_idx]["universe_size"]));
            }

            if(e131_settings["devices"][device_idx].contains("keepalive_time"))
            {
                entry->ui->KeepaliveTimeEdit->setText(QString::number((int)e131_settings["devices"][device_idx]["keepalive_time"]));
            }

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->E131DeviceList->addItem(item);
            ui->E131DeviceList->setItemWidget(item, entry);
            ui->E131DeviceList->show();
        }
    }
}

OpenRGBE131SettingsPage::~OpenRGBE131SettingsPage()
{
    delete ui;
}

void OpenRGBE131SettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBE131SettingsPage::on_AddE131DeviceButton_clicked()
{
    OpenRGBE131SettingsEntry* entry = new OpenRGBE131SettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->E131DeviceList->addItem(item);
    ui->E131DeviceList->setItemWidget(item, entry);
    ui->E131DeviceList->show();
}

void Ui::OpenRGBE131SettingsPage::on_RemoveE131DeviceButton_clicked()
{
    int cur_row = ui->E131DeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->E131DeviceList->takeItem(cur_row);

    ui->E131DeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBE131SettingsPage::on_SaveE131ConfigurationButton_clicked()
{
    json                e131_settings;

    /*-------------------------------------------------*\
    | Get E1.31 settings from settings manager          |
    \*-------------------------------------------------*/
    e131_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("E131Devices");

    e131_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        e131_settings["devices"][device_idx]["name"]                = entries[device_idx]->ui->NameEdit->text().toStdString();
        e131_settings["devices"][device_idx]["start_universe"]      = entries[device_idx]->ui->StartUniverseEdit->text().toUInt();
        e131_settings["devices"][device_idx]["start_channel"]       = entries[device_idx]->ui->StartChannelEdit->text().toUInt();
        e131_settings["devices"][device_idx]["num_leds"]            = entries[device_idx]->ui->NumLEDsEdit->text().toUInt();
        e131_settings["devices"][device_idx]["type"]                = entries[device_idx]->ui->TypeComboBox->currentIndex();
        e131_settings["devices"][device_idx]["rgb_order"]           = entries[device_idx]->ui->RGBOrderComboBox->currentIndex();

        /*-------------------------------------------------*\
        | Optional parameters                               |
        \*-------------------------------------------------*/
        if(entries[device_idx]->ui->IPEdit->text() != "")
        {
            e131_settings["devices"][device_idx]["ip"]              = entries[device_idx]->ui->IPEdit->text().toStdString();
        }

        if(e131_settings["devices"][device_idx]["type"] == 2)
        {
            e131_settings["devices"][device_idx]["matrix_width"]    = entries[device_idx]->ui->MatrixWidthEdit->text().toUInt();
            e131_settings["devices"][device_idx]["matrix_height"]   = entries[device_idx]->ui->MatrixHeightEdit->text().toUInt();
            e131_settings["devices"][device_idx]["matrix_order"]    = entries[device_idx]->ui->MatrixOrderComboBox->currentIndex();
        }

        if(entries[device_idx]->ui->UniverseSizeEdit->text() != "")
        {
            e131_settings["devices"][device_idx]["universe_size"]   = entries[device_idx]->ui->UniverseSizeEdit->text().toUInt();
        }

        if(entries[device_idx]->ui->KeepaliveTimeEdit->text() != "")
        {
            e131_settings["devices"][device_idx]["keepalive_time"]  = entries[device_idx]->ui->KeepaliveTimeEdit->text().toUInt();
        }
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("E131Devices", e131_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
