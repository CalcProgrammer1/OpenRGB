#include "OpenRGBPhilipsWizSettingsPage.h"
#include "ui_OpenRGBPhilipsWizSettingsPage.h"
#include "ResourceManager.h"

using namespace Ui;

OpenRGBPhilipsWizSettingsPage::OpenRGBPhilipsWizSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPhilipsWizSettingsPageUi)
{
    ui->setupUi(this);

    json                wiz_settings;

    /*-------------------------------------------------*\
    | Get Philips Wiz settings                          |
    \*-------------------------------------------------*/
    wiz_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("PhilipsWizDevices");

    /*-------------------------------------------------*\
    | If the Wiz settings contains devices, process     |
    \*-------------------------------------------------*/
    if(wiz_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < wiz_settings["devices"].size(); device_idx++)
        {
            OpenRGBPhilipsWizSettingsEntry* entry = new OpenRGBPhilipsWizSettingsEntry;

            if(wiz_settings["devices"][device_idx].contains("ip"))
            {
                entry->ui->IPEdit->setText(QString::fromStdString(wiz_settings["devices"][device_idx]["ip"]));
            }

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->PhilipsWizDeviceList->addItem(item);
            ui->PhilipsWizDeviceList->setItemWidget(item, entry);
            ui->PhilipsWizDeviceList->show();
        }
    }
}

OpenRGBPhilipsWizSettingsPage::~OpenRGBPhilipsWizSettingsPage()
{
    delete ui;
}

void OpenRGBPhilipsWizSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBPhilipsWizSettingsPage::on_AddPhilipsWizDeviceButton_clicked()
{
    OpenRGBPhilipsWizSettingsEntry* entry = new OpenRGBPhilipsWizSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->PhilipsWizDeviceList->addItem(item);
    ui->PhilipsWizDeviceList->setItemWidget(item, entry);
    ui->PhilipsWizDeviceList->show();
}

void Ui::OpenRGBPhilipsWizSettingsPage::on_RemovePhilipsWizDeviceButton_clicked()
{
    int cur_row = ui->PhilipsWizDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->PhilipsWizDeviceList->takeItem(cur_row);

    ui->PhilipsWizDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBPhilipsWizSettingsPage::on_SavePhilipsWizConfigurationButton_clicked()
{
    json                wiz_settings;

    /*-------------------------------------------------*\
    | Get Philips Wiz settings                          |
    \*-------------------------------------------------*/
    wiz_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("PhilipsWizDevices");

    wiz_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        wiz_settings["devices"][device_idx]["ip"]       = entries[device_idx]->ui->IPEdit->text().toStdString();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("PhilipsWizDevices", wiz_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
