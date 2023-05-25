#include "OpenRGBSettingsManagerPage.h"
#include "ui_OpenRGBSettingsManagerPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBSettingsManagerPage::OpenRGBSettingsManagerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBSettingsManagerPageUi)
{
    ui->setupUi(this);

    json settings_prototype = ResourceManager::get()->GetSettingsManager()->GetSettingsPrototype();

    for(json::const_iterator it = settings_prototype.begin(); it != settings_prototype.end(); it++)
    {
        OpenRGBSettingsManagerEntry* new_entry = new OpenRGBSettingsManagerEntry(it.key(), settings_prototype[it.key()]);

        entries.push_back(new_entry);

        QListWidgetItem* item = new QListWidgetItem;

        item->setSizeHint(new_entry->sizeHint());

        ui->SettingsEntryList->addItem(item);
        ui->SettingsEntryList->setItemWidget(item, new_entry);
        ui->SettingsEntryList->show();
    }
}

OpenRGBSettingsManagerPage::~OpenRGBSettingsManagerPage()
{
    delete ui;
}

void OpenRGBSettingsManagerPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
