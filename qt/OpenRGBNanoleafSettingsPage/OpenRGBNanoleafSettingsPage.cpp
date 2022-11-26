#include "OpenRGBNanoleafSettingsPage.h"
#include "ui_OpenRGBNanoleafSettingsPage.h"
#include "ResourceManager.h"
#include "json.hpp"
using json = nlohmann::json;

using namespace Ui;

OpenRGBNanoleafSettingsPage::OpenRGBNanoleafSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBNanoleafSettingsPageUi)
{
    ui->setupUi(this);

    json nanoleaf_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("NanoleafDevices");

    if(nanoleaf_settings.contains("devices"))
    {
        for(json::const_iterator it = nanoleaf_settings["devices"].begin(); it != nanoleaf_settings["devices"].end(); ++it)
        {
            const json& device = it.value();
            const std::string& location = it.key();

            if(device.contains("ip") && device.contains("port"))
            {
                OpenRGBNanoleafSettingsEntry* entry = new OpenRGBNanoleafSettingsEntry(QString::fromStdString(device["ip"]), device["port"]);

                entries[location] = entry;

                QListWidgetItem* item = new QListWidgetItem;

                item->setSizeHint(entry->sizeHint());

                ui->NanoleafDeviceList->addItem(item);
                ui->NanoleafDeviceList->setItemWidget(item, entry);
                ui->NanoleafDeviceList->show();
            }
        }
    }
}

OpenRGBNanoleafSettingsPage::~OpenRGBNanoleafSettingsPage()
{
    delete ui;
}

void OpenRGBNanoleafSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBNanoleafSettingsPage::on_ScanForNanoleafDevicesButton_clicked()
{
    /*-----------------------------------------------------*\
    | Create a worker thread for the mDNS query and hookup  |
    | callbacks for when it finds devices                   |
    \*-----------------------------------------------------*/
    OpenRGBNanoleafScanningThread *scanThread = new OpenRGBNanoleafScanningThread;

    connect(scanThread, SIGNAL(DeviceFound(QString, int)),
                        SLOT(on_DeviceFound(QString, int)));

    connect(scanThread, SIGNAL(finished()),
            scanThread, SLOT(deleteLater()));

    scanThread->start();
}

void Ui::OpenRGBNanoleafSettingsPage::on_DeviceFound(QString address, int port)
{
    std::string location = address.toStdString()+":"+std::to_string(port);

    if(entries.find(location) == entries.end())
    {
        OpenRGBNanoleafSettingsEntry* entry = new OpenRGBNanoleafSettingsEntry(address, port);

        entries[location] = entry;

        QListWidgetItem* item = new QListWidgetItem;

        item->setSizeHint(entry->sizeHint());

        ui->NanoleafDeviceList->addItem(item);
        ui->NanoleafDeviceList->setItemWidget(item, entry);
        ui->NanoleafDeviceList->show();
    }
}
