/*---------------------------------------------------------*\
| DDPSettingsEntry.cpp                                      |
|                                                           |
|   User interface for OpenRGB DDP settings entry          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "DDPSettingsEntry.h"
#include "ui_DDPSettingsEntry.h"
#include "ManualDevicesTypeManager.h"
#include "net_port.h"
#include "DDPController.h"
#include "nlohmann/json.hpp"
#include <QTimer>
#include <QMessageBox>

using json = nlohmann::json;

DDPSettingsEntry::DDPSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::DDPSettingsEntry)
{
    ui->setupUi(this);
    connect(ui->QueryButton, &QPushButton::clicked, this, &DDPSettingsEntry::queryDevice);
}

DDPSettingsEntry::~DDPSettingsEntry()
{
    delete ui;
}

void DDPSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void DDPSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("name"))
    {
        ui->NameEdit->setText(QString::fromStdString(data["name"]));
    }
    
    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }
    
    if(data.contains("port"))
    {
        ui->PortSpinBox->setValue(data["port"]);
    }
    else
    {
        ui->PortSpinBox->setValue(4048);
    }
    
    if(data.contains("num_leds"))
    {
        ui->NumLedsSpinBox->setValue(data["num_leds"]);
    }
}

json DDPSettingsEntry::saveSettings()
{
    json result;
    
    result["name"] = ui->NameEdit->text().toStdString();
    result["ip"] = ui->IPEdit->text().toStdString();
    result["port"] = ui->PortSpinBox->value();
    result["num_leds"] = ui->NumLedsSpinBox->value();
    
    return result;
}

bool DDPSettingsEntry::isDataValid()
{
    return !ui->IPEdit->text().isEmpty() && ui->NumLedsSpinBox->value() > 0;
}

void DDPSettingsEntry::queryDevice()
{
    QString ip = ui->IPEdit->text().trimmed();
    if(ip.isEmpty())
    {
        QMessageBox::warning(this, "Query Device", "Please enter an IP address first.");
        return;
    }
    
    ui->QueryButton->setEnabled(false);
    ui->QueryButton->setText("...");
    
    QTimer::singleShot(100, [this, ip]() {
        std::string ip_str = ip.toStdString();
        unsigned short port = ui->PortSpinBox->value();
        
        net_port* query_port = new net_port();
        char port_str[16];
        snprintf(port_str, 16, "%d", port);
        
        if(query_port->udp_client(ip_str.c_str(), port_str))
        {
            std::vector<unsigned char> packet(DDP_HEADER_SIZE);
            ddp_header* header = (ddp_header*)packet.data();
            
            header->flags = DDP_FLAG_VER_1 | DDP_FLAG_QUERY;
            header->sequence = 1;
            header->data_type = 0;
            header->dest_id = 251;
            header->data_offset = htonl(0);
            header->data_length = htons(0);
            
            query_port->udp_write((char*)packet.data(), packet.size());
            
            char response[1500];
            int bytes_received = query_port->udp_read(response, sizeof(response), 2000);
            
            if(bytes_received > DDP_HEADER_SIZE)
            {
                try
                {
                    std::string json_str(response + DDP_HEADER_SIZE, bytes_received - DDP_HEADER_SIZE);
                    json device_info = json::parse(json_str);
                    
                    if(device_info.contains("info") && device_info["info"].contains("name"))
                    {
                        ui->NameEdit->setText(QString::fromStdString(device_info["info"]["name"]));
                    }
                    
                    if(device_info.contains("info") && device_info["info"].contains("leds") && 
                       device_info["info"]["leds"].contains("count"))
                    {
                        ui->NumLedsSpinBox->setValue(device_info["info"]["leds"]["count"]);
                    }
                    
                    if(device_info.contains("status") && device_info["status"].contains("mod"))
                    {
                        ui->NameEdit->setText(QString::fromStdString(device_info["status"]["mod"]));
                    }
                }
                catch(...)
                {
                    QMessageBox::information(this, "Query Device", "Device responded but data format not recognized.");
                }
            }
            else
            {
                QMessageBox::warning(this, "Query Device", "No response from device.");
            }
        }
        else
        {
            QMessageBox::warning(this, "Query Device", "Could not connect to device.");
        }
        
        delete query_port;
        ui->QueryButton->setEnabled(true);
        ui->QueryButton->setText("Query");
    });
}

static BaseManualDeviceEntry* SpawnDDPEntry(const json& data)
{
    DDPSettingsEntry* entry = new DDPSettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

REGISTER_MANUAL_DEVICE_TYPE("DDP (Distributed Display Protocol)", "DDPDevices", SpawnDDPEntry);
