#include "OpenRGBDMXSettingsPage.h"
#include "ui_OpenRGBDMXSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBDMXSettingsPage::OpenRGBDMXSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBDMXSettingsPageUi)
{
    ui->setupUi(this);

    json                dmx_settings;

    /*-------------------------------------------------*\
    | Get DMX settings from settings manager            |
    \*-------------------------------------------------*/
    dmx_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("DMXDevices");

    /*-------------------------------------------------*\
    | If the DMX settings contains devices, process     |
    \*-------------------------------------------------*/
    if(dmx_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < dmx_settings["devices"].size(); device_idx++)
        {
            OpenRGBDMXSettingsEntry* entry = new OpenRGBDMXSettingsEntry;

            if(dmx_settings["devices"][device_idx].contains("name"))
            {
                entry->ui->NameEdit->setText(QString::fromStdString(dmx_settings["devices"][device_idx]["name"]));
            }

            if(dmx_settings["devices"][device_idx].contains("port"))
            {
                entry->ui->PortEdit->setText(QString::fromStdString(dmx_settings["devices"][device_idx]["port"]));
            }

            if(dmx_settings["devices"][device_idx].contains("red_channel"))
            {
                entry->ui->RedEdit->setText(QString::number((int)dmx_settings["devices"][device_idx]["red_channel"]));
            }

            if(dmx_settings["devices"][device_idx].contains("green_channel"))
            {
                entry->ui->GreenEdit->setText(QString::number((int)dmx_settings["devices"][device_idx]["green_channel"]));
            }

            if(dmx_settings["devices"][device_idx].contains("blue_channel"))
            {
                entry->ui->BlueEdit->setText(QString::number((int)dmx_settings["devices"][device_idx]["blue_channel"]));
            }

            if(dmx_settings["devices"][device_idx].contains("brightness_channel"))
            {
                entry->ui->BrightnessEdit->setText(QString::number((int)dmx_settings["devices"][device_idx]["brightness_channel"]));
            }

            if(dmx_settings["devices"][device_idx].contains("keepalive_time"))
            {
                entry->ui->KeepaliveTimeEdit->setText(QString::number((int)dmx_settings["devices"][device_idx]["keepalive_time"]));
            }

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->DMXDeviceList->addItem(item);
            ui->DMXDeviceList->setItemWidget(item, entry);
            ui->DMXDeviceList->show();
        }
    }
}

OpenRGBDMXSettingsPage::~OpenRGBDMXSettingsPage()
{
    delete ui;
}

void OpenRGBDMXSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBDMXSettingsPage::on_AddDMXDeviceButton_clicked()
{
    OpenRGBDMXSettingsEntry* entry = new OpenRGBDMXSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->DMXDeviceList->addItem(item);
    ui->DMXDeviceList->setItemWidget(item, entry);
    ui->DMXDeviceList->show();
}

void Ui::OpenRGBDMXSettingsPage::on_RemoveDMXDeviceButton_clicked()
{
    int cur_row = ui->DMXDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->DMXDeviceList->takeItem(cur_row);

    ui->DMXDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBDMXSettingsPage::on_SaveDMXConfigurationButton_clicked()
{
    json                dmx_settings;

    /*-------------------------------------------------*\
    | Get DMX settings from settings manager            |
    \*-------------------------------------------------*/
    dmx_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("DMXDevices");

    dmx_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        dmx_settings["devices"][device_idx]["name"]                     = entries[device_idx]->ui->NameEdit->text().toStdString();
        dmx_settings["devices"][device_idx]["port"]                     = entries[device_idx]->ui->PortEdit->text().toStdString();
        dmx_settings["devices"][device_idx]["red_channel"]              = entries[device_idx]->ui->RedEdit->text().toUInt();
        dmx_settings["devices"][device_idx]["green_channel"]            = entries[device_idx]->ui->GreenEdit->text().toUInt();
        dmx_settings["devices"][device_idx]["blue_channel"]             = entries[device_idx]->ui->BlueEdit->text().toUInt();

        /*-------------------------------------------------*\
        | Optional parameters                               |
        \*-------------------------------------------------*/
        if(entries[device_idx]->ui->BrightnessEdit->text() != "")
        {
            dmx_settings["devices"][device_idx]["brightness_channel"]   = entries[device_idx]->ui->BrightnessEdit->text().toUInt();
        }

        if(entries[device_idx]->ui->KeepaliveTimeEdit->text() != "")
        {
            dmx_settings["devices"][device_idx]["keepalive_time"]       = entries[device_idx]->ui->KeepaliveTimeEdit->text().toUInt();
        }
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("DMXDevices", dmx_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
