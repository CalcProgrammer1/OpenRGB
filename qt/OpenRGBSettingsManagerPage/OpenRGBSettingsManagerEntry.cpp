#include "OpenRGBSettingsManagerEntry.h"
#include "ui_OpenRGBSettingsManagerEntry.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include <QLabel>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QListWidget>
#include <QCheckBox>

using namespace Ui;

class SettingsManagerEntryPointer : public QObject
{
public:
    std::string     key;
    QWidget *       widget;
};

OpenRGBSettingsManagerEntry::OpenRGBSettingsManagerEntry(std::string settings_key, json settings_proto, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBSettingsManagerEntryUi)
{
    /*-----------------------------------------------------*\
    | Store settings key, prototype, and data               |
    \*-----------------------------------------------------*/
    key     = settings_key;
    proto   = settings_proto;
    data    = ResourceManager::get()->GetSettingsManager()->GetSettings(key);

    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Set the title of the group to the settings key        |
    \*-----------------------------------------------------*/
    ui->SettingsGroupBox->setTitle(QString::fromStdString(key));

    /*-----------------------------------------------------*\
    | Create a vertical layout to hold the settings controls|
    \*-----------------------------------------------------*/
    QVBoxLayout* layout = new QVBoxLayout;
    ui->SettingsGroupBox->setLayout(layout);

    /*-----------------------------------------------------*\
    | Set up a signal mapper to handle settings changes     |
    \*-----------------------------------------------------*/
    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(QObject *)), this, SLOT(onSettingChanged(QObject *)));

    /*-----------------------------------------------------*\
    | Loop through all settings in this key                 |
    \*-----------------------------------------------------*/
    for(json::const_iterator it = proto.begin(); it != proto.end(); it++)
    {
        /*-------------------------------------------------*\
        | Gather information about this setting             |
        \*-------------------------------------------------*/
        std::string item_key    = it.key();
        std::string name        = proto[it.key()]["name"];
        std::string type        = proto[it.key()]["type"];

        /*-------------------------------------------------*\
        | Create widget pointer for this setting            |
        \*-------------------------------------------------*/
        QWidget* item;

        /*-------------------------------------------------*\
        | Create settings manager entry pointer to map      |
        | settings widget events to their keys              |
        \*-------------------------------------------------*/
        SettingsManagerEntryPointer * new_arg = new SettingsManagerEntryPointer();
        new_arg->key            = it.key();

        /*-------------------------------------------------*\
        | Create widgets for boolean settings               |
        \*-------------------------------------------------*/
        if(type == "boolean")
        {
            /*---------------------------------------------*\
            | Boolean settings use a checkbox widget        |
            \*---------------------------------------------*/
            item                = new QCheckBox;
            new_arg->widget     = item;

            /*---------------------------------------------*\
            | Set the checkbox label to the setting name    |
            \*---------------------------------------------*/
            ((QCheckBox*)item)->setText(QString::fromStdString(name));

            /*---------------------------------------------*\
            | Set the checkbox state based on the current   |
            | value of the setting                          |
            \*---------------------------------------------*/
            if(data.contains(item_key))
            {
                ((QCheckBox*)item)->setChecked(data[item_key]);
            }

            /*---------------------------------------------*\
            | Connect widget to signal mapper to handle     |
            | changes to this setting                       |
            \*---------------------------------------------*/
            connect(item, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(item, new_arg);
        }

        /*-------------------------------------------------*\
        | Add widget to the vertical layout                 |
        \*-------------------------------------------------*/
        layout->addWidget(item);
    }
}

OpenRGBSettingsManagerEntry::~OpenRGBSettingsManagerEntry()
{
    delete ui;
}

void OpenRGBSettingsManagerEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBSettingsManagerEntry::onSettingChanged(QObject * arg)
{
    /*-----------------------------------------------------*\
    | Read information from argument                        |
    \*-----------------------------------------------------*/
    SettingsManagerEntryPointer * setting   = (SettingsManagerEntryPointer *)arg;
    std::string item_key                    = setting->key;
    std::string type                        = proto[item_key]["type"];

    /*-----------------------------------------------------*\
    | Handle boolean settings changes                       |
    \*-----------------------------------------------------*/
    if(type == "boolean")
    {
        bool value      = ((QCheckBox *)setting->widget)->isChecked();

        data[item_key]  = value;
    }

    /*-----------------------------------------------------*\
    | Set and save the updated settings                     |
    \*-----------------------------------------------------*/
    ResourceManager::get()->GetSettingsManager()->SetSettings(key, data);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
