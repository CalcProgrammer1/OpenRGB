#include <QSignalMapper>
#include "OpenRGBClientInfoPage.h"
#include "ResourceManager.h"

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

OpenRGBClientInfoPage::OpenRGBClientInfoPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBClientInfoPageUi)
{
    /*-----------------------------------------------------*\
    | Set initial values for GUI fields                     |
    \*-----------------------------------------------------*/
    ui->setupUi(this);
    ui->ClientIPValue->setText("127.0.0.1");
    ui->ClientPortValue->setText(QString::number(OPENRGB_SDK_PORT));

    /*-----------------------------------------------------*\
    | Register callbacks for existing clients               |
    \*-----------------------------------------------------*/
    for(unsigned int client_idx = 0; client_idx < ResourceManager::get()->GetClients().size(); client_idx++)
    {
        ResourceManager::get()->GetClients()[client_idx]->RegisterClientInfoChangeCallback(UpdateInfoCallback, this);
    }

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
        ResourceManager::get()->GetClients().push_back(new_client);
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
    ui->ClientTree->setColumnCount(3);
    ui->ClientTree->header()->setStretchLastSection(false);
    ui->ClientTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->ClientTree->setColumnWidth(1, 100);
    ui->ClientTree->setColumnWidth(2, 100);
    ui->ClientTree->setHeaderLabels(QStringList() << "Connected Clients" << "Protocol Version" << "");

    /*-----------------------------------------------------*\
    | Set up a signal mapper to handle disconnect buttons   |
    \*-----------------------------------------------------*/
    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(QObject *)), this, SLOT(onClientDisconnectButton_clicked(QObject *)));

    /*-----------------------------------------------------*\
    | Loop through all clients in list and display them     |
    \*-----------------------------------------------------*/
    for(std::size_t client_idx = 0; client_idx < ResourceManager::get()->GetClients().size(); client_idx++)
    {
        /*-----------------------------------------------------*\
        | Create the top level tree widget items and display the|
        | client IP addresses and protocol versions in them     |
        \*-----------------------------------------------------*/
        QTreeWidgetItem* new_top_item = new QTreeWidgetItem(ui->ClientTree);
        new_top_item->setText(0, QString::fromStdString(ResourceManager::get()->GetClients()[client_idx]->GetIP()));
        new_top_item->setText(1, QString::number(ResourceManager::get()->GetClients()[client_idx]->GetProtocolVersion()));

        /*-----------------------------------------------------*\
        | Create the disconnect buttons and connect them to the |
        | signal mapper                                         |
        \*-----------------------------------------------------*/
        QPushButton* new_button = new QPushButton( "Disconnect" );
        ui->ClientTree->setItemWidget(new_top_item, 2, new_button);

        connect(new_button, SIGNAL(clicked()), signalMapper, SLOT(map()));

        NetworkClientPointer * new_arg = new NetworkClientPointer();
        new_arg->net_client = ResourceManager::get()->GetClients()[client_idx];

        signalMapper->setMapping(new_button, new_arg);

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
            new_item->setText(0, QString::fromStdString(ResourceManager::get()->GetClients()[client_idx]->server_controllers[dev_idx]->name));

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

void Ui::OpenRGBClientInfoPage::onClientDisconnectButton_clicked(QObject * arg)
{
    /*-----------------------------------------------------*\
    | Get the pointer to the disconnecting client from args |
    \*-----------------------------------------------------*/
    NetworkClient * disconnect_client = ((NetworkClientPointer *)arg)->net_client;

    /*-----------------------------------------------------*\
    | Remove the client from the resource manager, which    |
    | deletes the client                                    |
    \*-----------------------------------------------------*/
    ResourceManager::get()->UnregisterNetworkClient(disconnect_client);
}
