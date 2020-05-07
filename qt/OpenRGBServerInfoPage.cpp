#include "OpenRGBServerInfoPage.h"

using namespace Ui;

OpenRGBServerInfoPage::OpenRGBServerInfoPage(NetworkServer * server, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBServerInfoPageUi)
{
    network_server = server;

    ui->setupUi(this);

    UpdateInfo();
}

OpenRGBServerInfoPage::~OpenRGBServerInfoPage()
{
    delete ui;
}

void OpenRGBServerInfoPage::UpdateInfo()
{
    ui->ServerPortValue->setText(std::to_string(network_server->GetPort()).c_str());

    if(network_server->GetOnline())
    {
        ui->ServerStatusValue->setText("Online");
        ui->ServerStartButton->setEnabled(false);
        ui->ServerStopButton->setEnabled(true);
        ui->ServerPortValue->setEnabled(false);
    }
    else
    {
        ui->ServerStatusValue->setText("Offline");
        ui->ServerStartButton->setEnabled(true);
        ui->ServerStopButton->setEnabled(false);
        ui->ServerPortValue->setEnabled(true);
    }

    ui->ServerClientTree->clear();
    ui->ServerClientTree->setColumnCount(2);
    for(unsigned int client_idx = 0; client_idx < network_server->GetNumClients(); client_idx++)
    {
        QTreeWidgetItem * new_item = new QTreeWidgetItem();

        new_item->setText(0, network_server->GetClientIP(client_idx));
        new_item->setText(1, network_server->GetClientString(client_idx));

        ui->ServerClientTree->addTopLevelItem(new_item);
    }
}

void Ui::OpenRGBServerInfoPage::on_ServerStartButton_clicked()
{
    if(network_server->GetOnline() == false)
    {
        network_server->SetPort(ui->ServerPortValue->text().toInt());
        network_server->StartServer();

        UpdateInfo();
    }
}

void Ui::OpenRGBServerInfoPage::on_ServerStopButton_clicked()
{
    if(network_server->GetOnline() == true)
    {
        network_server->StopServer();

        UpdateInfo();
    }
}
