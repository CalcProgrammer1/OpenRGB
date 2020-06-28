#include <QSignalMapper>
#include "OpenRGBClientInfoPage.h"

using namespace Ui;

static void UpdateInfoCallback(void * this_ptr)
{
    OpenRGBClientInfoPage * this_obj = (OpenRGBClientInfoPage *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "UpdateInfo", Qt::QueuedConnection);
}

class NetworkClientPointer : public QObject
{
public:
    NetworkClient * net_client;
};

OpenRGBClientInfoPage::OpenRGBClientInfoPage(std::vector<RGBController *>& control, QWidget *parent) :
    QFrame(parent),
    controllers(control),
    ui(new Ui::OpenRGBClientInfoPageUi)
{
    /*-----------------------------------------------------*\
    | Set initial values for GUI fields                     |
    \*-----------------------------------------------------*/
    ui->setupUi(this);
    ui->ClientIPValue->setText("127.0.0.1");
    ui->ClientPortValue->setText(QString::number(OPENRGB_SDK_PORT));

    /*-----------------------------------------------------*\
    | Update the information view                           |
    \*-----------------------------------------------------*/
    UpdateInfo();
}

OpenRGBClientInfoPage::~OpenRGBClientInfoPage()
{

}

void OpenRGBClientInfoPage::AddClient(NetworkClient* new_client)
{
    /*-----------------------------------------------------*\
    | Add a new client to the list, register the callback,  |
    | and update the information view if the pointer is     |
    | valid                                                 |
    \*-----------------------------------------------------*/
    if(new_client != NULL)
    {
        rgb_clients.push_back(new_client);
        new_client->RegisterClientInfoChangeCallback(UpdateInfoCallback, this);

        UpdateInfo();
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
    ui->ClientTree->setColumnCount(2);
    ui->ClientTree->header()->setStretchLastSection(false);
    ui->ClientTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->ClientTree->setColumnWidth(1, 100);
    ui->ClientTree->setHeaderLabels(QStringList() << "Connected Clients" << "");

    /*-----------------------------------------------------*\
    | Set up a signal mapper to handle disconnect buttons   |
    \*-----------------------------------------------------*/
    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(QObject *)), this, SLOT(on_ClientDisconnectButton_clicked(QObject *)));

    /*-----------------------------------------------------*\
    | Loop through all clients in list and display them     |
    \*-----------------------------------------------------*/
    for(int client_idx = 0; client_idx < rgb_clients.size(); client_idx++)
    {
        /*-----------------------------------------------------*\
        | Create the top level tree widget items and display the|
        | client IP addresses in them                           |
        \*-----------------------------------------------------*/
        QTreeWidgetItem* new_top_item = new QTreeWidgetItem(ui->ClientTree);
        new_top_item->setText(0, QString::fromStdString(rgb_clients[client_idx]->GetIP()));

        /*-----------------------------------------------------*\
        | Create the disconnect buttons and connect them to the |
        | signal mapper                                         |
        \*-----------------------------------------------------*/
        QPushButton* new_button = new QPushButton( "Disconnect" );
        ui->ClientTree->setItemWidget(new_top_item, 1, new_button);

        connect(new_button, SIGNAL(clicked()), signalMapper, SLOT(map()));

        NetworkClientPointer * new_arg = new NetworkClientPointer();
        new_arg->net_client = rgb_clients[client_idx];

        signalMapper->setMapping(new_button, new_arg);

        /*-----------------------------------------------------*\
        | Add child items for each device in the client         |
        \*-----------------------------------------------------*/
        for(int dev_idx = 0; dev_idx < rgb_clients[client_idx]->server_controllers.size(); dev_idx++)
        {
            /*-----------------------------------------------------*\
            | Create child tree widget items and display the device |
            | names in them                                         |
            \*-----------------------------------------------------*/
            QTreeWidgetItem* new_item = new QTreeWidgetItem(new_top_item);
            new_item->setText(0, QString::fromStdString(rgb_clients[client_idx]->server_controllers[dev_idx]->name));

            /*-----------------------------------------------------*\
            | Add child items for each zone in the device           |
            \*-----------------------------------------------------*/
            for(int zone_idx = 0; zone_idx < rgb_clients[client_idx]->server_controllers[dev_idx]->zones.size(); zone_idx++)
            {
                /*-----------------------------------------------------*\
                | Create child tree widget items and display the zone   |
                | names, number of LEDs, and types in them              |
                \*-----------------------------------------------------*/
                QTreeWidgetItem* new_child = new QTreeWidgetItem();

                std::string zone_str = rgb_clients[client_idx]->server_controllers[dev_idx]->zones[zone_idx].name + ", ";
                zone_str.append(std::to_string(rgb_clients[client_idx]->server_controllers[dev_idx]->zones[zone_idx].leds_count));
                zone_str.append(" LEDs, ");

                switch(rgb_clients[client_idx]->server_controllers[dev_idx]->zones[zone_idx].type)
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

    /*-----------------------------------------------------*\
    | Emit client information updated signal                |
    \*-----------------------------------------------------*/
    emit ClientListUpdated();
}

void Ui::OpenRGBClientInfoPage::on_ClientConnectButton_clicked()
{
    /*-----------------------------------------------------*\
    | Read the new client IP and Port values from the UI    |
    \*-----------------------------------------------------*/
    unsigned short  port = std::stoi(ui->ClientPortValue->text().toStdString());
    std::string     ip   = ui->ClientIPValue->text().toStdString();

    /*-----------------------------------------------------*\
    | Create a new client and set name, IP, and port values |
    \*-----------------------------------------------------*/
    NetworkClient * rgb_client = new NetworkClient(controllers);

    std::string titleString = "OpenRGB ";
    titleString.append(VERSION_STRING);

    rgb_client->SetIP(ip.c_str());
    rgb_client->SetName(titleString.c_str());
    rgb_client->SetPort(port);

    rgb_client->StartClient();

    /*-----------------------------------------------------*\
    | Add new client to list and register update callback   |
    \*-----------------------------------------------------*/
    rgb_clients.push_back(rgb_client);

    rgb_client->RegisterClientInfoChangeCallback(UpdateInfoCallback, this);
}

void Ui::OpenRGBClientInfoPage::on_ClientDisconnectButton_clicked(QObject * arg)
{
    /*-----------------------------------------------------*\
    | Get the pointer to the disconnecting client from args |
    \*-----------------------------------------------------*/
    NetworkClient * disconnect_client = ((NetworkClientPointer *)arg)->net_client;

    /*-----------------------------------------------------*\
    | Stop the disconnecting client                         |
    \*-----------------------------------------------------*/
    disconnect_client->StopClient();

    /*-----------------------------------------------------*\
    | Remove disconnecting client from list                 |
    \*-----------------------------------------------------*/
    for(unsigned int client_idx = 0; client_idx < rgb_clients.size(); client_idx++)
    {
        if(disconnect_client == rgb_clients[client_idx])
        {
            rgb_clients.erase(rgb_clients.begin() + client_idx);
            break;
        }
    }

    /*-----------------------------------------------------*\
    | Delete the disconnecting client                       |
    \*-----------------------------------------------------*/
    delete disconnect_client;
}
