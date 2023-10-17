#include "OpenRGBSettingsManagerEntry.h"
#include "ui_OpenRGBSettingsManagerEntry.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include <QLabel>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QCheckBox>

using namespace Ui;

class SettingsManagerEntryPointer : public QObject
{
public:
    SettingsManagerEntryPointer(json & proto_ref, json & data_ref) : proto(proto_ref), data(data_ref)
    {
        QObject();
    }
    std::string     key;
    json &          proto;
    json &          data;
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
    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(QObject *)), this, SLOT(onSettingChanged(QObject *)));

    /*-----------------------------------------------------*\
    | Loop through all settings in this key                 |
    \*-----------------------------------------------------*/
    for(json::const_iterator it = proto.begin(); it != proto.end(); it++)
    {
        /*-------------------------------------------------*\
        | Create widget pointer for this setting            |
        \*-------------------------------------------------*/
        QWidget* item;

        item = CreateWidget(it.key(), proto, data);


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

QWidget * OpenRGBSettingsManagerEntry::CreateWidget(std::string key, json & proto, json & data)
{
    /*-------------------------------------------------*\
    | Gather information about this setting             |
    \*-------------------------------------------------*/
    std::string name        = proto[key]["name"];
    std::string type        = proto[key]["type"];

    /*-------------------------------------------------*\
    | Create widget pointer for this setting            |
    \*-------------------------------------------------*/
    QWidget* item;

    /*-------------------------------------------------*\
    | Create settings manager entry pointer to map      |
    | settings widget events to their keys              |
    \*-------------------------------------------------*/
    SettingsManagerEntryPointer * new_arg = new SettingsManagerEntryPointer(proto, data);
    new_arg->key            = key;

    /*-------------------------------------------------*\
    | Create widgets for group settings                 |
    | This is a special case that recursively calls     |
    | CreateWidget to create nested groups of settings  |
    | under a nested JSON object                        |
    \*-------------------------------------------------*/
    if(type == "group")
    {
        /*---------------------------------------------*\
        | Group settings use a groupbox widget          |
        \*---------------------------------------------*/
        item                = new QGroupBox;

        ((QGroupBox*)item)->setTitle(QString::fromStdString(name));

        /*---------------------------------------------*\
        | Create a vertical layout to hold the settings |
        | controls                                      |
        \*---------------------------------------------*/
        QVBoxLayout* layout = new QVBoxLayout;
        ((QGroupBox*)item)->setLayout(layout);

        /*---------------------------------------------*\
        | Loop through all settings in this key         |
        \*---------------------------------------------*/
        for(json::const_iterator it = proto[key].begin(); it != proto[key].end(); it++)
        {
            /*-----------------------------------------*\
            | Skip reserved keys                        |
            \*-----------------------------------------*/
            if(it.key() == "name" || it.key() == "type")
            {
                continue;
            }

            /*-----------------------------------------*\
            | Create widget pointer for this setting    |
            \*-----------------------------------------*/
            QWidget * sub_item;

            sub_item = CreateWidget(it.key(), proto[key], data[key]);

            /*-----------------------------------------*\
            | Add widget to the vertical layout         |
            \*-----------------------------------------*/
            layout->addWidget(sub_item);
        }
    }
    /*-------------------------------------------------*\
    | Create widgets for boolean settings               |
    \*-------------------------------------------------*/
    else if(type == "boolean")
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
        if(data.contains(key))
        {
            ((QCheckBox*)item)->setChecked(data[key]);
        }

        /*---------------------------------------------*\
        | Connect widget to signal mapper to handle     |
        | changes to this setting                       |
        \*---------------------------------------------*/
        connect(item, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(item, new_arg);
    }
    /*-------------------------------------------------*\
    | Create widgets for integer settings               |
    \*-------------------------------------------------*/
    else if(type == "integer")
    {
        /*---------------------------------------------*\
        | Create a widget to hold the layout            |
        \*---------------------------------------------*/
        item                = new QWidget;

        /*---------------------------------------------*\
        | Create a horizontal layout to hold the        |
        | settings controls                             |
        \*---------------------------------------------*/
        QHBoxLayout * layout = new QHBoxLayout;

        /*---------------------------------------------*\
        | Integer settings use both a label widget and  |
        | a line edit widget in a horizontal layout     |
        \*---------------------------------------------*/
        QLabel *    label   = new QLabel;
        QLineEdit * edit    = new QLineEdit;
        new_arg->widget     = edit;

        /*---------------------------------------------*\
        | Set the line edit label to the setting name   |
        \*---------------------------------------------*/
        ((QLabel*)label)->setText(QString::fromStdString(name));

        /*---------------------------------------------*\
        | Set the checkbox state based on the current   |
        | value of the setting                          |
        \*---------------------------------------------*/
        if(data.contains(key))
        {
            int value = data[key];
            ((QLineEdit*)edit)->setText(QString::number(value));
        }

        /*---------------------------------------------*\
        | Add widgets to the horizontal layout          |
        \*---------------------------------------------*/
        layout->addWidget(label);
        layout->addWidget(edit);

        /*---------------------------------------------*\
        | Add layout to top level widget                |
        \*---------------------------------------------*/
        item->setLayout(layout);

        /*---------------------------------------------*\
        | Connect widget to signal mapper to handle     |
        | changes to this setting                       |
        \*---------------------------------------------*/
        connect(edit, SIGNAL(editingFinished()), mapper, SLOT(map()));
        mapper->setMapping(edit, new_arg);
    }
    /*-------------------------------------------------*\
    | Create widgets for string settings                |
    \*-------------------------------------------------*/
    else if(type == "string")
    {
        /*---------------------------------------------*\
        | Create a widget to hold the layout            |
        \*---------------------------------------------*/
        item                = new QWidget;

        /*---------------------------------------------*\
        | Create a horizontal layout to hold the        |
        | settings controls                             |
        \*---------------------------------------------*/
        QHBoxLayout * layout = new QHBoxLayout;

        /*---------------------------------------------*\
        | String settings use both a label widget and   |
        | a line edit widget in a horizontal layout     |
        \*---------------------------------------------*/
        QLabel *    label   = new QLabel;
        QLineEdit * edit    = new QLineEdit;
        new_arg->widget     = edit;

        /*---------------------------------------------*\
        | Set the line edit label to the setting name   |
        \*---------------------------------------------*/
        ((QLabel*)label)->setText(QString::fromStdString(name));

        /*---------------------------------------------*\
        | Set the checkbox state based on the current   |
        | value of the setting                          |
        \*---------------------------------------------*/
        if(data.contains(key))
        {
            ((QLineEdit*)edit)->setText(QString::fromStdString(data[key]));
        }

        /*---------------------------------------------*\
        | Add widgets to the horizontal layout          |
        \*---------------------------------------------*/
        layout->addWidget(label);
        layout->addWidget(edit);

        /*---------------------------------------------*\
        | Add layout to top level widget                |
        \*---------------------------------------------*/
        item->setLayout(layout);

        /*---------------------------------------------*\
        | Connect widget to signal mapper to handle     |
        | changes to this setting                       |
        \*---------------------------------------------*/
        connect(edit, SIGNAL(editingFinished()), mapper, SLOT(map()));
        mapper->setMapping(edit, new_arg);
    }

    return(item);
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
    std::string type                        = setting->proto[item_key]["type"];

    printf("updating key %s\r\n", item_key.c_str());

    /*-----------------------------------------------------*\
    | Handle boolean settings changes                       |
    \*-----------------------------------------------------*/
    if(type == "boolean")
    {
        bool value              = ((QCheckBox *)setting->widget)->isChecked();

        setting->data[item_key] = value;
    }
    else if(type == "integer")
    {
        int value               = ((QLineEdit *)setting->widget)->text().toInt();

        setting->data[item_key] = value;
    }
    else if(type == "string")
    {
        std::string value       = ((QLineEdit *)setting->widget)->text().toStdString();

        setting->data[item_key] = value;
    }

    /*-----------------------------------------------------*\
    | Set and save the updated settings                     |
    \*-----------------------------------------------------*/
    ResourceManager::get()->GetSettingsManager()->SetSettings(key, data);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
