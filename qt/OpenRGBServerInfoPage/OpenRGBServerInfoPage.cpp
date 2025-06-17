/*---------------------------------------------------------*\
| OpenRGBServerInfoPage.cpp                                 |
|                                                           |
|   User interface for server information page              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBServerInfoPage.h"
#include "ui_OpenRGBServerInfoPage.h"

static void UpdateInfoCallback(void * this_ptr)
{
    OpenRGBServerInfoPage * this_obj = (OpenRGBServerInfoPage *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "UpdateInfo", Qt::QueuedConnection);
}

OpenRGBServerInfoPage::OpenRGBServerInfoPage(NetworkServer * server, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBServerInfoPage)
{
    network_server = server;

    ui->setupUi(this);

    UpdateInfo();

    network_server->RegisterClientInfoChangeCallback(UpdateInfoCallback, this);
    network_server->RegisterServerListeningChangeCallback(UpdateInfoCallback, this);
}

OpenRGBServerInfoPage::~OpenRGBServerInfoPage()
{
    delete ui;
}

void OpenRGBServerInfoPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBServerInfoPage::UpdateInfo()
{
    ui->ServerHostValue->setText(network_server->GetHost().c_str());
    ui->ServerPortValue->setText(std::to_string(network_server->GetPort()).c_str());

    if(network_server->GetListening() && !network_server->GetOnline())
    {
        ui->ServerStatusValue->setText(tr("Stopping..."));
        ui->ServerStartButton->setEnabled(false);
        ui->ServerStopButton->setEnabled(false);
        ui->ServerHostValue->setEnabled(false);
        ui->ServerPortValue->setEnabled(false);
    }
    else if(network_server->GetListening())
    {
        ui->ServerStatusValue->setText(tr("Online"));
        ui->ServerStartButton->setEnabled(false);
        ui->ServerStopButton->setEnabled(true);
        ui->ServerHostValue->setEnabled(false);
        ui->ServerPortValue->setEnabled(false);
    }
    else
    {
        ui->ServerStatusValue->setText(tr("Offline"));
        ui->ServerStartButton->setEnabled(true);
        ui->ServerStopButton->setEnabled(false);
        ui->ServerHostValue->setEnabled(true);
        ui->ServerPortValue->setEnabled(true);
    }

    ui->ServerClientTree->clear();
    for(unsigned int client_idx = 0; client_idx < network_server->GetNumClients(); client_idx++)
    {
        QTreeWidgetItem * new_item = new QTreeWidgetItem();

        new_item->setText(0, network_server->GetClientIP(client_idx));
        new_item->setText(1, QString::number(network_server->GetClientProtocolVersion(client_idx)));
        new_item->setText(2, network_server->GetClientString(client_idx));

        ui->ServerClientTree->addTopLevelItem(new_item);
    }
}

void OpenRGBServerInfoPage::on_ServerStartButton_clicked()
{
    if(network_server->GetOnline() == false)
    {
        network_server->SetHost(ui->ServerHostValue->text().toStdString());
        network_server->SetPort(ui->ServerPortValue->text().toInt());
        network_server->StartServer();

        UpdateInfo();
    }
}

void OpenRGBServerInfoPage::on_ServerStopButton_clicked()
{
    if(network_server->GetOnline() == true)
    {
        network_server->StopServer();

        UpdateInfo();
    }
}
