#include "OpenRGBQMKORGBSettingsPage.h"
#include "ui_OpenRGBQMKORGBSettingsPage.h"
#include "ResourceManager.h"

using namespace Ui;

OpenRGBQMKORGBSettingsPage::OpenRGBQMKORGBSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBQMKORGBSettingsPageUi)
{
    ui->setupUi(this);

    json                qmk_settings;

    /*-------------------------------------------------*\
    | Get QMKOpenRGB settings                           |
    \*-------------------------------------------------*/
    qmk_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("QMKOpenRGBDevices");

    /*-------------------------------------------------*\
    | If the LEDStrip settings contains devices, process|
    \*-------------------------------------------------*/
    if(qmk_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < qmk_settings["devices"].size(); device_idx++)
        {
            OpenRGBQMKORGBSettingsEntry* entry = new OpenRGBQMKORGBSettingsEntry;

            if(qmk_settings["devices"][device_idx].contains("name"))
            {
                entry->ui->NameEdit->setText(QString::fromStdString(qmk_settings["devices"][device_idx]["name"]));
            }

            if(qmk_settings["devices"][device_idx].contains("usb_vid"))
            {
                entry->ui->USBVIDEdit->setText(QString::fromStdString(qmk_settings["devices"][device_idx]["usb_vid"]));
            }

            if(qmk_settings["devices"][device_idx].contains("usb_pid"))
            {
                entry->ui->USBPIDEdit->setText(QString::fromStdString(qmk_settings["devices"][device_idx]["usb_pid"]));
            }

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->QMKORGBDeviceList->addItem(item);
            ui->QMKORGBDeviceList->setItemWidget(item, entry);
            ui->QMKORGBDeviceList->show();
        }
    }
}

OpenRGBQMKORGBSettingsPage::~OpenRGBQMKORGBSettingsPage()
{
    delete ui;
}

void OpenRGBQMKORGBSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBQMKORGBSettingsPage::on_AddQMKORGBDeviceButton_clicked()
{
    OpenRGBQMKORGBSettingsEntry* entry = new OpenRGBQMKORGBSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->QMKORGBDeviceList->addItem(item);
    ui->QMKORGBDeviceList->setItemWidget(item, entry);
    ui->QMKORGBDeviceList->show();
}

void Ui::OpenRGBQMKORGBSettingsPage::on_RemoveQMKORGBDeviceButton_clicked()
{
    int cur_row = ui->QMKORGBDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->QMKORGBDeviceList->takeItem(cur_row);

    ui->QMKORGBDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBQMKORGBSettingsPage::on_SaveQMKORGBConfigurationButton_clicked()
{
    json                qmk_settings;

    /*-------------------------------------------------*\
    | Get QMKOpenRGB settings                           |
    \*-------------------------------------------------*/
    qmk_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("QMKOpenRGBDevices");

    qmk_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        qmk_settings["devices"][device_idx]["name"]     = entries[device_idx]->ui->NameEdit->text().toStdString();
        qmk_settings["devices"][device_idx]["usb_vid"]  = entries[device_idx]->ui->USBVIDEdit->text().toStdString();
        qmk_settings["devices"][device_idx]["usb_pid"]  = entries[device_idx]->ui->USBPIDEdit->text().toStdString();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("QMKOpenRGBDevices", qmk_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
