#include "OpenRGBLIFXSettingsPage.h"
#include "ui_OpenRGBLIFXSettingsPage.h"
#include "ResourceManager.h"

using namespace Ui;

OpenRGBLIFXSettingsPage::OpenRGBLIFXSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBLIFXSettingsPageUi)
{
    ui->setupUi(this);

    json                lifx_settings;

    /*-------------------------------------------------*\
    | Get LIFX settings                                 |
    \*-------------------------------------------------*/
    lifx_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LIFXDevices");

    /*-------------------------------------------------*\
    | If the Wiz settings contains devices, process     |
    \*-------------------------------------------------*/
    if(lifx_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < lifx_settings["devices"].size(); device_idx++)
        {
            OpenRGBLIFXSettingsEntry* entry = new OpenRGBLIFXSettingsEntry;

            if(lifx_settings["devices"][device_idx].contains("ip"))
            {
                entry->ui->IPEdit->setText(QString::fromStdString(lifx_settings["devices"][device_idx]["ip"]));
            }

            if(lifx_settings["devices"][device_idx].contains("name"))
            {
                entry->ui->IPEdit->setText(QString::fromStdString(lifx_settings["devices"][device_idx]["name"]));
            }

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->LIFXDeviceList->addItem(item);
            ui->LIFXDeviceList->setItemWidget(item, entry);
            ui->LIFXDeviceList->show();
        }
    }
}

OpenRGBLIFXSettingsPage::~OpenRGBLIFXSettingsPage()
{
    delete ui;
}

void OpenRGBLIFXSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBLIFXSettingsPage::on_AddLIFXDeviceButton_clicked()
{
    OpenRGBLIFXSettingsEntry* entry = new OpenRGBLIFXSettingsEntry;
    entry->ui->NameEdit->setText(QString("LIFX%1").arg(entries.size()));
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->LIFXDeviceList->addItem(item);
    ui->LIFXDeviceList->setItemWidget(item, entry);
    ui->LIFXDeviceList->show();
}

void Ui::OpenRGBLIFXSettingsPage::on_RemoveLIFXDeviceButton_clicked()
{
    int cur_row = ui->LIFXDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->LIFXDeviceList->takeItem(cur_row);

    ui->LIFXDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBLIFXSettingsPage::on_SaveLIFXConfigurationButton_clicked()
{
    json                lifx_settings;

    /*-------------------------------------------------*\
    | Get LIFX settings                                 |
    \*-------------------------------------------------*/
    lifx_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LIFXDevices");

    lifx_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        lifx_settings["devices"][device_idx]["ip"]      = entries[device_idx]->ui->IPEdit->text().toStdString();
        lifx_settings["devices"][device_idx]["name"]    = entries[device_idx]->ui->NameEdit->text().toStdString();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("LIFXDevices", lifx_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
