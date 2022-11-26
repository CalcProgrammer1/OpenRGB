#include "OpenRGBElgatoKeyLightSettingsPage.h"
#include "ui_OpenRGBElgatoKeyLightSettingsPage.h"
#include "ResourceManager.h"

using namespace Ui;

OpenRGBElgatoKeyLightSettingsPage::OpenRGBElgatoKeyLightSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBElgatoKeyLightSettingsPageUi)
{
    ui->setupUi(this);

    json                elgato_keylight_settings;

    elgato_keylight_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("ElgatoKeyLightDevices");

    /*---------------------------------------------------------------*\
    | If the Elgato Key Light settings contains devices, process     |
    \*---------------------------------------------------------------*/
    if(elgato_keylight_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < elgato_keylight_settings["devices"].size(); device_idx++)
        {
            OpenRGBElgatoKeyLightSettingsEntry* entry = new OpenRGBElgatoKeyLightSettingsEntry;

            if(elgato_keylight_settings["devices"][device_idx].contains("ip"))
            {
                entry->ui->IPEdit->setText(QString::fromStdString(elgato_keylight_settings["devices"][device_idx]["ip"]));
            }

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->ElgatoKeyLightDeviceList->addItem(item);
            ui->ElgatoKeyLightDeviceList->setItemWidget(item, entry);
            ui->ElgatoKeyLightDeviceList->show();
        }
    }
}

OpenRGBElgatoKeyLightSettingsPage::~OpenRGBElgatoKeyLightSettingsPage()
{
    delete ui;
}

void OpenRGBElgatoKeyLightSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBElgatoKeyLightSettingsPage::on_AddElgatoKeyLightDeviceButton_clicked()
{
    OpenRGBElgatoKeyLightSettingsEntry* entry = new OpenRGBElgatoKeyLightSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->ElgatoKeyLightDeviceList->addItem(item);
    ui->ElgatoKeyLightDeviceList->setItemWidget(item, entry);
    ui->ElgatoKeyLightDeviceList->show();
}

void Ui::OpenRGBElgatoKeyLightSettingsPage::on_RemoveElgatoKeyLightDeviceButton_clicked()
{
    int cur_row = ui->ElgatoKeyLightDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->ElgatoKeyLightDeviceList->takeItem(cur_row);

    ui->ElgatoKeyLightDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBElgatoKeyLightSettingsPage::on_SaveElgatoKeyLightConfigurationButton_clicked()
{
    json                elgato_keylight_settings;

    elgato_keylight_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("ElgatoKeyLightDevices");

    elgato_keylight_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        elgato_keylight_settings["devices"][device_idx]["ip"]          = entries[device_idx]->ui->IPEdit->text().toStdString();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("ElgatoKeyLightDevices", elgato_keylight_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}

