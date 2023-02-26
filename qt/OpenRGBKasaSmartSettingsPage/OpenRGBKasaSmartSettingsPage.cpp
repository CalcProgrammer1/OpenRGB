#include "OpenRGBKasaSmartSettingsPage.h"
#include "ui_OpenRGBKasaSmartSettingsPage.h"
#include "ResourceManager.h"

using namespace Ui;

OpenRGBKasaSmartSettingsPage::OpenRGBKasaSmartSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBKasaSmartSettingsPageUi)
{
    ui->setupUi(this);

    json                KasaSmart_settings;

    /*-------------------------------------------------*\
    | Get KasaSmart settings                                 |
    \*-------------------------------------------------*/
    KasaSmart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("KasaSmartDevices");

    /*-------------------------------------------------*\
    | If the Wiz settings contains devices, process     |
    \*-------------------------------------------------*/
    if(KasaSmart_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < KasaSmart_settings["devices"].size(); device_idx++)
        {
            OpenRGBKasaSmartSettingsEntry* entry = new OpenRGBKasaSmartSettingsEntry;

            if(KasaSmart_settings["devices"][device_idx].contains("ip"))
            {
                entry->ui->IPEdit->setText(QString::fromStdString(KasaSmart_settings["devices"][device_idx]["ip"]));
            }

            if(KasaSmart_settings["devices"][device_idx].contains("name"))
            {
                entry->ui->IPEdit->setText(QString::fromStdString(KasaSmart_settings["devices"][device_idx]["name"]));
            }

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->KasaSmartDeviceList->addItem(item);
            ui->KasaSmartDeviceList->setItemWidget(item, entry);
            ui->KasaSmartDeviceList->show();
        }
    }
}

OpenRGBKasaSmartSettingsPage::~OpenRGBKasaSmartSettingsPage()
{
    delete ui;
}

void OpenRGBKasaSmartSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBKasaSmartSettingsPage::on_AddKasaSmartDeviceButton_clicked()
{
    OpenRGBKasaSmartSettingsEntry* entry = new OpenRGBKasaSmartSettingsEntry;
    entry->ui->NameEdit->setText(QString("KasaSmart%1").arg(entries.size()));
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->KasaSmartDeviceList->addItem(item);
    ui->KasaSmartDeviceList->setItemWidget(item, entry);
    ui->KasaSmartDeviceList->show();
}

void Ui::OpenRGBKasaSmartSettingsPage::on_RemoveKasaSmartDeviceButton_clicked()
{
    int cur_row = ui->KasaSmartDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->KasaSmartDeviceList->takeItem(cur_row);

    ui->KasaSmartDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBKasaSmartSettingsPage::on_SaveKasaSmartConfigurationButton_clicked()
{
    json                KasaSmart_settings;

    /*-------------------------------------------------*\
    | Get KasaSmart settings                                 |
    \*-------------------------------------------------*/
    KasaSmart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("KasaSmartDevices");

    KasaSmart_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        KasaSmart_settings["devices"][device_idx]["ip"]      = entries[device_idx]->ui->IPEdit->text().toStdString();
        KasaSmart_settings["devices"][device_idx]["name"]    = entries[device_idx]->ui->NameEdit->text().toStdString();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("KasaSmartDevices", KasaSmart_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
