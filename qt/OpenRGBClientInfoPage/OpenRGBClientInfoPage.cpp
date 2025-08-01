/*---------------------------------------------------------*\
| OpenRGBClientInfoPage.cpp                                 |
|                                                           |
|   User interface for OpenRGB client information page      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iostream>
#include <QSignalMapper>
#include <QCheckBox>
#include "OpenRGBClientInfoPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "ui_OpenRGBClientInfoPage.h"

static void UpdateInfoCallback(void * this_ptr)
{
    OpenRGBClientInfoPage * this_obj = (OpenRGBClientInfoPage *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "UpdateInfo", Qt::QueuedConnection);
}

class NetworkClientPointer : public QObject
{
public:
    NetworkClient * net_client;
    QWidget *       widget;
};

OpenRGBClientInfoPage::OpenRGBClientInfoPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBClientInfoPage)
{
    /*-----------------------------------------------------*\
    | Set initial values for GUI fields                     |
    \*-----------------------------------------------------*/
    ui->setupUi(this);
    ui->ClientIPValue->setText("127.0.0.1");
    ui->ClientPortValue->setText(QString::number(OPENRGB_SDK_PORT));

    /*-----------------------------------------------------*\
    | Register callbacks with resource manager              |
    \*-----------------------------------------------------*/
    ResourceManager::get()->RegisterClientInfoChangeCallback(UpdateInfoCallback, this);

    /*-----------------------------------------------------*\
    | Update the information view                           |
    \*-----------------------------------------------------*/
    UpdateInfo();
}

OpenRGBClientInfoPage::~OpenRGBClientInfoPage()
{

}

void OpenRGBClientInfoPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBClientInfoPage::UpdateInfo()
{
    /*-----------------------------------------------------*\
    | Clear the tree view before recreating its contents    |
    \*-----------------------------------------------------*/
    ui->ClientTree->clear();

    /*-----------------------------------------------------*\
    | Set up the tree view header                           |
    \*-----------------------------------------------------*/
    ui->ClientTree->setColumnCount(5);
    ui->ClientTree->header()->setStretchLastSection(false);
    ui->ClientTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->ClientTree->setColumnWidth(1, 140);
    ui->ClientTree->setColumnWidth(2, 140);
    ui->ClientTree->setColumnWidth(3, 140);
    ui->ClientTree->setColumnWidth(4, 140);

    /*-----------------------------------------------------*\
    | Set up signal mappers to handle buttons               |
    \*-----------------------------------------------------*/
    QSignalMapper* signalMapperDisconnect   = new QSignalMapper(this);
    QSignalMapper* signalMapperSave         = new QSignalMapper(this);
    QSignalMapper* signalMapperRescan       = new QSignalMapper(this);

    connect(signalMapperDisconnect, SIGNAL(mapped(QObject *)), this, SLOT(onClientDisconnectButton_clicked(QObject *)));
    connect(signalMapperSave,       SIGNAL(mapped(QObject *)), this, SLOT(onClientSaveCheckBox_clicked(QObject *)));
    connect(signalMapperRescan,     SIGNAL(mapped(QObject *)), this, SLOT(onClientRescanButton_clicked(QObject *)));

    /*-------------------------------------------------*\
    | Get Client settings                               |
    \*-------------------------------------------------*/
    json                client_settings;

    client_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Client");

    /*-----------------------------------------------------*\
    | Loop through all clients in list and display them     |
    \*-----------------------------------------------------*/
    for(std::size_t client_idx = 0; client_idx < ResourceManager::get()->GetClients().size(); client_idx++)
    {
        /*-----------------------------------------------------*\
        | Check to see if this client is in the saved clients   |
        | list                                                  |
        \*-----------------------------------------------------*/
        bool found = false;
        if(client_settings.contains("clients"))
        {
            for(unsigned int saved_client_idx = 0; saved_client_idx < client_settings["clients"].size(); saved_client_idx++)
            {
                if(client_settings["clients"][saved_client_idx].contains("ip") && client_settings["clients"][saved_client_idx].contains("port"))
                {
                    std::string     saved_ip    = client_settings["clients"][saved_client_idx]["ip"];
                    unsigned short  saved_port  = client_settings["clients"][saved_client_idx]["port"];
                    std::string     client_ip   = ResourceManager::get()->GetClients()[client_idx]->GetIP();
                    unsigned short  client_port = ResourceManager::get()->GetClients()[client_idx]->GetPort();

                    if((client_ip == saved_ip) && (client_port == saved_port))
                    {
                        found = true;
                        break;
                    }
                }
            }
        }

        /*-----------------------------------------------------*\
        | Create the top level tree widget items and display the|
        | client IP addresses and protocol versions in them     |
        \*-----------------------------------------------------*/
        QTreeWidgetItem* new_top_item = new QTreeWidgetItem(ui->ClientTree);
        new_top_item->setText(0, QString::fromStdString(ResourceManager::get()->GetClients()[client_idx]->GetIP()));
        new_top_item->setText(1, QString::number(ResourceManager::get()->GetClients()[client_idx]->GetProtocolVersion()));

        /*-----------------------------------------------------*\
        | Create the save checkbox                              |
        \*-----------------------------------------------------*/
        QCheckBox* checkbox_save                = new QCheckBox( "" );
        ui->ClientTree->setItemWidget(new_top_item, 2, checkbox_save);
        checkbox_save->setChecked(found);

        connect(checkbox_save, SIGNAL(clicked()), signalMapperSave, SLOT(map()));

        NetworkClientPointer * arg_save         = new NetworkClientPointer();
        arg_save->net_client                    = ResourceManager::get()->GetClients()[client_idx];
        arg_save->widget                        = checkbox_save;

        signalMapperSave->setMapping(checkbox_save, arg_save);

        /*-----------------------------------------------------*\
        | Create the rescan button if protocol version is 5 or  |
        | greater                                               |
        \*-----------------------------------------------------*/
        if(ResourceManager::get()->GetClients()[client_idx]->GetProtocolVersion() >= 5)
        {
            QPushButton* button_rescan              = new QPushButton(tr("Rescan Devices"));
            ui->ClientTree->setItemWidget(new_top_item, 3, button_rescan);

            connect(button_rescan, SIGNAL(clicked()), signalMapperRescan, SLOT(map()));

            NetworkClientPointer * arg_rescan       = new NetworkClientPointer();
            arg_rescan->net_client                  = ResourceManager::get()->GetClients()[client_idx];
            arg_rescan->widget                      = button_rescan;

            signalMapperRescan->setMapping(button_rescan, arg_rescan);
        }

        /*-----------------------------------------------------*\
        | Create the disconnect button                          |
        \*-----------------------------------------------------*/
        QPushButton* button_disconnect          = new QPushButton(tr("Disconnect"));
        ui->ClientTree->setItemWidget(new_top_item, 4, button_disconnect);

        connect(button_disconnect, SIGNAL(clicked()), signalMapperDisconnect, SLOT(map()));

        NetworkClientPointer * arg_disconnect   = new NetworkClientPointer();
        arg_disconnect->net_client              = ResourceManager::get()->GetClients()[client_idx];
        arg_disconnect->widget                  = button_disconnect;

        signalMapperDisconnect->setMapping(button_disconnect, arg_disconnect);

        /*-----------------------------------------------------*\
        | Add child items for each device in the client         |
        \*-----------------------------------------------------*/
        for(std::size_t dev_idx = 0; dev_idx < ResourceManager::get()->GetClients()[client_idx]->server_controllers.size(); dev_idx++)
        {
            /*-----------------------------------------------------*\
            | Create child tree widget items and display the device |
            | names in them                                         |
            \*-----------------------------------------------------*/
            QTreeWidgetItem* new_item = new QTreeWidgetItem(new_top_item);
            new_item->setText(0, QString::fromStdString(ResourceManager::get()->GetClients()[client_idx]->server_controllers[dev_idx]->GetName()));

            /*-----------------------------------------------------*\
            | Add child items for each zone in the device           |
            \*-----------------------------------------------------*/
            for(std::size_t zone_idx = 0; zone_idx < ResourceManager::get()->GetClients()[client_idx]->server_controllers[dev_idx]->zones.size(); zone_idx++)
            {
                /*-----------------------------------------------------*\
                | Create child tree widget items and display the zone   |
                | names, number of LEDs, and types in them              |
                \*-----------------------------------------------------*/
                QTreeWidgetItem* new_child = new QTreeWidgetItem();

                std::string zone_str = ResourceManager::get()->GetClients()[client_idx]->server_controllers[dev_idx]->zones[zone_idx].name + ", ";
                zone_str.append(std::to_string(ResourceManager::get()->GetClients()[client_idx]->server_controllers[dev_idx]->zones[zone_idx].leds_count));
                zone_str.append(" LEDs, ");
                // TODO : translate
                switch(ResourceManager::get()->GetClients()[client_idx]->server_controllers[dev_idx]->zones[zone_idx].type)
                {
                    case ZONE_TYPE_SINGLE:
                        zone_str.append("Single");
                    break;

                    case ZONE_TYPE_LINEAR:
                        zone_str.append("Linear");
                        break;

                    case ZONE_TYPE_MATRIX:
                        zone_str.append("Matrix");
                        break;
                }

                new_child->setText(0, QString::fromStdString(zone_str));

                new_item->addChild(new_child);
            }
        }
    }
}

void OpenRGBClientInfoPage::on_ClientConnectButton_clicked()
{
    /*-----------------------------------------------------*\
    | Read the new client IP and Port values from the UI    |
    \*-----------------------------------------------------*/
    unsigned short  port = std::stoi(ui->ClientPortValue->text().toStdString());
    std::string     ip   = ui->ClientIPValue->text().toStdString();

    /*-----------------------------------------------------*\
    | Create a new client and set name, IP, and port values |
    \*-----------------------------------------------------*/
    NetworkClient * rgb_client = new NetworkClient(ResourceManager::get()->GetRGBControllers());

    std::string titleString = "OpenRGB ";
    titleString.append(VERSION_STRING);

    rgb_client->SetIP(ip.c_str());
    rgb_client->SetName(titleString.c_str());
    rgb_client->SetPort(port);

    rgb_client->StartClient();

    /*-----------------------------------------------------*\
    | Add new client to list and register update callback   |
    \*-----------------------------------------------------*/
    ResourceManager::get()->RegisterNetworkClient(rgb_client);

    rgb_client->RegisterClientInfoChangeCallback(UpdateInfoCallback, this);
}

void OpenRGBClientInfoPage::onClientDisconnectButton_clicked(QObject * arg)
{
    /*-----------------------------------------------------*\
    | Get the pointer to the client from args               |
    \*-----------------------------------------------------*/
    NetworkClient * disconnect_client = ((NetworkClientPointer *)arg)->net_client;

    /*-----------------------------------------------------*\
    | Remove the client from the resource manager, which    |
    | deletes the client                                    |
    \*-----------------------------------------------------*/
    ResourceManager::get()->UnregisterNetworkClient(disconnect_client);
}

void OpenRGBClientInfoPage::onClientRescanButton_clicked(QObject * arg)
{
    /*-----------------------------------------------------*\
    | Get the pointer to the client from args               |
    \*-----------------------------------------------------*/
    NetworkClient * rescan_client = ((NetworkClientPointer *)arg)->net_client;

    /*-----------------------------------------------------*\
    | Send a rescan request to the client                   |
    \*-----------------------------------------------------*/
    rescan_client->SendRequest_RescanDevices();
}

void OpenRGBClientInfoPage::onClientSaveCheckBox_clicked(QObject * arg)
{
    /*-----------------------------------------------------*\
    | Get the pointer to the client from args               |
    \*-----------------------------------------------------*/
    NetworkClient * save_client     = ((NetworkClientPointer *)arg)->net_client;
    QCheckBox *     save_checkbox   = (QCheckBox *)((NetworkClientPointer *)arg)->widget;

    json            client_settings;

    /*-------------------------------------------------*\
    | Get Client settings                               |
    \*-------------------------------------------------*/
    client_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Client");

    if(save_checkbox->isChecked())
    {
        bool found = false;
        for(unsigned int client_idx = 0; client_idx < client_settings["clients"].size(); client_idx++)
        {
            if(client_settings["clients"][client_idx].contains("ip") && client_settings["clients"][client_idx].contains("port"))
            {
                std::string     client_ip   = client_settings["clients"][client_idx]["ip"];
                unsigned short  client_port = client_settings["clients"][client_idx]["port"];
                std::string     save_ip     = save_client->GetIP();
                unsigned short  save_port   = save_client->GetPort();

                if((client_ip == save_ip) && (client_port == save_port))
                {
                    found = true;
                    break;
                }
            }
        }

        if(!found)
        {
            json new_client;

            new_client["ip"]    = save_client->GetIP();
            new_client["port"]  = save_client->GetPort();

            client_settings["clients"].push_back(new_client);
        }
    }
    else
    {
        if(client_settings.contains("clients"))
        {
            for(unsigned int client_idx = 0; client_idx < client_settings["clients"].size(); client_idx++)
            {
                if(client_settings["clients"][client_idx].contains("ip") && client_settings["clients"][client_idx].contains("port"))
                {
                    std::string     client_ip   = client_settings["clients"][client_idx]["ip"];
                    unsigned short  client_port = client_settings["clients"][client_idx]["port"];
                    std::string     save_ip     = save_client->GetIP();
                    unsigned short  save_port   = save_client->GetPort();

                    if((client_ip == save_ip) && (client_port == save_port))
                    {
                        client_settings["clients"].erase(client_settings["clients"].begin() + client_idx);
                        break;
                    }
                }
            }
        }
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("Client", client_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
