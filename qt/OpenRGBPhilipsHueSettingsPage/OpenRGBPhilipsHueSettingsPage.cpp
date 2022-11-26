#include "OpenRGBPhilipsHueSettingsPage.h"
#include "ui_OpenRGBPhilipsHueSettingsPage.h"
#include "ResourceManager.h"

using namespace Ui;

OpenRGBPhilipsHueSettingsPage::OpenRGBPhilipsHueSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPhilipsHueSettingsPageUi)
{
    ui->setupUi(this);

    json                hue_settings;

    /*-------------------------------------------------*\
    | Get Philips Hue settings                          |
    \*-------------------------------------------------*/
    hue_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("PhilipsHueDevices");

    /*-------------------------------------------------*\
    | If the Hue settings contains bridges, process     |
    \*-------------------------------------------------*/
    if(hue_settings.contains("bridges"))
    {
        for(unsigned int device_idx = 0; device_idx < hue_settings["bridges"].size(); device_idx++)
        {
            OpenRGBPhilipsHueSettingsEntry* entry = new OpenRGBPhilipsHueSettingsEntry;

            if(hue_settings["bridges"][device_idx].contains("ip"))
            {
                entry->ui->IPEdit->setText(QString::fromStdString(hue_settings["bridges"][device_idx]["ip"]));
            }

            if(hue_settings["bridges"][device_idx].contains("mac"))
            {
                entry->ui->MACEdit->setText(QString::fromStdString(hue_settings["bridges"][device_idx]["mac"]));
            }

            if(hue_settings["bridges"][device_idx].contains("entertainment"))
            {
                entry->ui->EntertainmentCheckBox->setChecked(hue_settings["bridges"][device_idx]["entertainment"]);
            }

            if(hue_settings["bridges"][device_idx].contains("autoconnect"))
            {
                entry->ui->AutoConnectCheckBox->setChecked(hue_settings["bridges"][device_idx]["autoconnect"]);
            }

            if(hue_settings["bridges"][device_idx].contains("username"))
            {
                entry->ui->UsernameValue->setText(QString::fromStdString(hue_settings["bridges"][device_idx]["username"]));
            }

            if(hue_settings["bridges"][device_idx].contains("clientkey"))
            {
                entry->ui->ClientKeyValue->setText(QString::fromStdString(hue_settings["bridges"][device_idx]["clientkey"]));
            }

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->PhilipsHueDeviceList->addItem(item);
            ui->PhilipsHueDeviceList->setItemWidget(item, entry);
            ui->PhilipsHueDeviceList->show();
        }
    }
}

OpenRGBPhilipsHueSettingsPage::~OpenRGBPhilipsHueSettingsPage()
{
    delete ui;
}

void OpenRGBPhilipsHueSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBPhilipsHueSettingsPage::on_AddPhilipsHueDeviceButton_clicked()
{
    OpenRGBPhilipsHueSettingsEntry* entry = new OpenRGBPhilipsHueSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->PhilipsHueDeviceList->addItem(item);
    ui->PhilipsHueDeviceList->setItemWidget(item, entry);
    ui->PhilipsHueDeviceList->show();
}

void Ui::OpenRGBPhilipsHueSettingsPage::on_RemovePhilipsHueDeviceButton_clicked()
{
    int cur_row = ui->PhilipsHueDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->PhilipsHueDeviceList->takeItem(cur_row);

    ui->PhilipsHueDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBPhilipsHueSettingsPage::on_SavePhilipsHueConfigurationButton_clicked()
{
    json                hue_settings;

    /*-------------------------------------------------*\
    | Get Philips Hue settings                          |
    \*-------------------------------------------------*/
    hue_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("PhilipsHueDevices");

    hue_settings["bridges"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        hue_settings["bridges"][device_idx]["ip"]               = entries[device_idx]->ui->IPEdit->text().toStdString();
        hue_settings["bridges"][device_idx]["mac"]              = entries[device_idx]->ui->MACEdit->text().toStdString();
        hue_settings["bridges"][device_idx]["entertainment"]    = entries[device_idx]->ui->EntertainmentCheckBox->isChecked();
        hue_settings["bridges"][device_idx]["autoconnect"]      = entries[device_idx]->ui->AutoConnectCheckBox->isChecked();

        if(entries[device_idx]->ui->UsernameValue->text() != "")
        {
            hue_settings["bridges"][device_idx]["username"]     = entries[device_idx]->ui->UsernameValue->text().toStdString();
        }

        if(entries[device_idx]->ui->ClientKeyValue->text() != "")
        {
            hue_settings["bridges"][device_idx]["clientkey"]    = entries[device_idx]->ui->ClientKeyValue->text().toStdString();
        }
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("PhilipsHueDevices", hue_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
