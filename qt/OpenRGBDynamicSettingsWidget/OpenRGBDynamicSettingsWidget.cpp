
#include <QApplication>
#include <QCheckBox>
#include <QDirIterator>
#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QToolTip>
#include <QTranslator>
#include <QVBoxLayout>
#include "JsonUtils.h"
#include "OpenRGBDynamicSettingsWidget.h"
#include "ProfileManager.h"
#include "ResourceManager.h"



static void OpenRGBDynamicSettingsWidgetProfileManagerCallback(void * this_ptr, unsigned int update_reason)
{
    OpenRGBDynamicSettingsWidget * this_obj = (OpenRGBDynamicSettingsWidget *)this_ptr;

    switch(update_reason)
    {
        case PROFILEMANAGER_UPDATE_REASON_PROFILE_LIST_UPDATED:
            this_obj->ProfileListUpdated();
            break;
    }
}

void OpenRGBDynamicSettingsWidget::NestedCallback(void* this_ptr, std::string /*key*/, nlohmann::json settings)
{
    nlohmann::json nested_settings;

    nested_settings[((OpenRGBDynamicSettingsWidget*)this_ptr)->key] = settings;

    if(((OpenRGBDynamicSettingsWidget*)this_ptr)->callback)
    {
        ((OpenRGBDynamicSettingsWidget*)this_ptr)->callback(((OpenRGBDynamicSettingsWidget*)this_ptr)->callback_arg, ((OpenRGBDynamicSettingsWidget*)this_ptr)->key, nested_settings);
    }
}

OpenRGBDynamicSettingsWidget::OpenRGBDynamicSettingsWidget(std::string key, nlohmann::json& schema, nlohmann::json& settings, QWidget* parent) :
    QWidget(parent)
{
    /*-----------------------------------------------------*\
    | Type to track ordering                                |
    \*-----------------------------------------------------*/
    typedef struct
    {
        int                                     order;
        std::string                             key;
        std::reference_wrapper<nlohmann::json>  value;
    } ordered_settings_t;

    this->key       = key;
    type            = "";
    is_enum         = false;
    callback        = nullptr;
    left_widget     = nullptr;
    right_widget    = nullptr;

    /*-----------------------------------------------------*\
    | Ensure each schema entry has a title and type         |
    \*-----------------------------------------------------*/
    if(schema.contains("title") && schema.contains("type"))
    {
        /*-------------------------------------------------*\
        | Create main layout                                |
        \*-------------------------------------------------*/
        layout                              = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        /*-------------------------------------------------*\
        | Get schema title and type, check if it is an enum |
        \*-------------------------------------------------*/
        description                         = JsonUtils::JsonGetString(schema, "description");
        title                               = JsonUtils::JsonGetString(schema, "title");
        type                                = JsonUtils::JsonGetString(schema, "type");

        if(schema.contains("enum"))
        {
            is_enum                         = true;
        }

        /*-------------------------------------------------*\
        | If type is object, create a nested group          |
        \*-------------------------------------------------*/
        if(type == "object" && schema.contains("properties"))
        {
            /*---------------------------------------------*\
            | Create the groupbox and layout                |
            \*---------------------------------------------*/
            QGroupBox*                      groupbox            = new QGroupBox(QString::fromStdString(title));
            QVBoxLayout*                    groupbox_layout     = new QVBoxLayout();
            nlohmann::json&                 properties_json     = schema["properties"];
            std::vector<ordered_settings_t> setting_entries;
            nlohmann::json&                 settings_json       = settings[key];

            groupbox->setLayout(groupbox_layout);

            /*---------------------------------------------*\
            | Loop through the schema and build a vector    |
            | containing the entries in desired order.      |
            | Entries with negative order (default -1) are  |
            | put at the end of the list.                   |
            \*---------------------------------------------*/
            for(nlohmann::json::iterator json_iterator = properties_json.begin(); json_iterator != properties_json.end(); json_iterator++)
            {
                int                         order               = -1;

                if(json_iterator.value().contains("order"))
                {
                    order                                       = json_iterator.value()["order"];
                }

                ordered_settings_t          setting             = {order, json_iterator.key(), json_iterator.value()};

                if((order < 0) || (setting_entries.size() == 0))
                {
                    setting_entries.push_back(setting);
                }
                else
                {
                    bool                    added               = false;

                    for(std::size_t setting_idx = 0; setting_idx < setting_entries.size(); setting_idx++)
                    {
                        if((setting_entries[setting_idx].order > order) || (setting_entries[setting_idx].order < 0))
                        {
                            setting_entries.insert(setting_entries.begin() + setting_idx, setting);
                            added = true;
                            break;
                        }
                    }

                    if(!added)
                    {
                        setting_entries.push_back(setting);
                    }
                }
            }

            /*---------------------------------------------*\
            | Create UI elements for each setting           |
            \*---------------------------------------------*/
            for(std::size_t setting_idx = 0; setting_idx < setting_entries.size(); setting_idx++)
            {
                OpenRGBDynamicSettingsWidget*   item_widget = new OpenRGBDynamicSettingsWidget(setting_entries[setting_idx].key, setting_entries[setting_idx].value, settings_json);

                item_widget->SetCallback(&OpenRGBDynamicSettingsWidget::NestedCallback, this);
                groupbox_layout->addWidget(item_widget);
            }

            layout->addWidget(groupbox);
        }
        /*-------------------------------------------------*\
        | Otherwise, create a single settings entry         |
        \*-------------------------------------------------*/
        else
        {
            if(type == "profile")
            {
                /*-----------------------------------------*\
                | Add a CheckBox and ComboBox for profile   |
                | settings                                  |
                \*-----------------------------------------*/
                bool                        enabled_value   = false;
                std::string                 name_value      = "";
                std::vector<std::string>    profile_list    =  ResourceManager::get()->GetProfileManager()->GetProfileList();

                ResourceManager::get()->GetProfileManager()->RegisterProfileManagerCallback(OpenRGBDynamicSettingsWidgetProfileManagerCallback, this);

                if(settings.contains(key))
                {
                    enabled_value                           = JsonUtils::JsonGetBool(settings[key], "enabled");
                    name_value                              = JsonUtils::JsonGetString(settings[key], "name");
                }

                left_widget                                 = (QWidget*)new QCheckBox();
                right_widget                                = (QWidget*)new QComboBox();

                for(std::size_t profile_idx = 0; profile_idx < profile_list.size(); profile_idx++)
                {
                    std::string             profile_str     = profile_list[profile_idx];
                    ((QComboBox*)right_widget)->addItem(QString::fromStdString(profile_str));
                }

                ((QCheckBox*)left_widget)->setChecked(enabled_value);
                ((QComboBox*)right_widget)->setEnabled(enabled_value);
                ((QComboBox*)right_widget)->setCurrentText(QString::fromStdString(name_value));

                QObject::connect((QCheckBox*)left_widget, qOverload<int>(&QCheckBox::stateChanged), this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
                QObject::connect((QComboBox*)right_widget, qOverload<int>(&QComboBox::currentIndexChanged), this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
            }
            else
            {
                /*-----------------------------------------*\
                | The left widget for these setting types   |
                | is a label                                |
                \*-----------------------------------------*/
                left_widget                                 = (QWidget*)new QLabel();

                /*-----------------------------------------*\
                | Add a LineEdit for non-enum string        |
                | settings                                  |
                \*-----------------------------------------*/
                if(type == "string" && !is_enum)
                {
                    std::string     value                   = JsonUtils::JsonGetString(schema, "default");

                    if(settings.contains(key))
                    {
                        value                               = JsonUtils::JsonGetString(settings, key);
                    }

                    right_widget                            = (QWidget*)new QLineEdit(QString::fromStdString(value));

                    QObject::connect((QLineEdit*)right_widget, &QLineEdit::textChanged, this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
                }
                /*-----------------------------------------*\
                | Add a ComboBox for enum string settings   |
                \*-----------------------------------------*/
                else if(type == "string" && is_enum)
                {
                    std::string     value                   = JsonUtils::JsonGetString(schema, "default");

                    if(settings.contains(key))
                    {
                        value                               = JsonUtils::JsonGetString(settings, key);
                    }

                    right_widget                            = (QWidget*)new QComboBox();

                    for(std::size_t enum_idx = 0; enum_idx < schema["enum"].size(); enum_idx++)
                    {
                        std::string enum_str                = schema["enum"][enum_idx];
                        ((QComboBox*)right_widget)->addItem(QString::fromStdString(enum_str));
                    }

                    ((QComboBox*)right_widget)->setCurrentText(QString::fromStdString(value));

                    QObject::connect((QComboBox*)right_widget, qOverload<int>(&QComboBox::currentIndexChanged), this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
                }
                /*-----------------------------------------*\
                | Add a LineEdit for non-enum integer       |
                | settings                                  |
                \*-----------------------------------------*/
                if(type == "integer" && !is_enum)
                {
                    int             maximum                 = JsonUtils::JsonGetInt(schema, "maximum", 2147483647);
                    int             minimum                 = JsonUtils::JsonGetInt(schema, "minimum", -2147483648);
                    int             value                   = JsonUtils::JsonGetInt(schema, "default");

                    if(settings.contains(key))
                    {
                        value                               = JsonUtils::JsonGetInt(settings, key);
                    }

                    right_widget                            = (QWidget*)new QSpinBox();
                    ((QSpinBox*)right_widget)->setRange(minimum, maximum);
                    ((QSpinBox*)right_widget)->setValue(value);

                    QObject::connect((QSpinBox*)right_widget, qOverload<int>(&QSpinBox::valueChanged), this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
                }
                /*-----------------------------------------*\
                | Add a ComboBox for enum integer settings  |
                \*-----------------------------------------*/
                else if(type == "integer" && is_enum)
                {
                    int             value                   = JsonUtils::JsonGetInt(schema, "default");

                    if(settings.contains(key))
                    {
                        value                               = JsonUtils::JsonGetInt(settings, key);
                    }

                    right_widget                            = (QWidget*)new QComboBox();

                    for(std::size_t enum_idx = 0; enum_idx < schema["enum"].size(); enum_idx++)
                    {
                        int         enum_int                = schema["enum"][enum_idx];
                        std::string label                   = std::to_string(enum_int);

                        if(schema.contains("enumNames") && (enum_idx < schema["enumNames"].size()))
                        {
                            label                           = schema["enumNames"][enum_idx];
                        }

                        ((QComboBox*)right_widget)->addItem(QString::fromStdString(label), enum_int);
                    }

                    int index = ((QComboBox*)right_widget)->findData(value);
                    if(index != -1)
                    {
                        ((QComboBox*)right_widget)->setCurrentIndex(index);
                    }

                    QObject::connect((QComboBox*)right_widget, qOverload<int>(&QComboBox::currentIndexChanged), this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
                }
                /*-----------------------------------------*\
                | Add a CheckBox for bool settings          |
                \*-----------------------------------------*/
                else if(type == "bool")
                {
                    bool            value                   = JsonUtils::JsonGetBool(schema, "default");

                    if(settings.contains(key))
                    {
                        value                               = JsonUtils::JsonGetBool(settings, key);
                    }

                    right_widget                            = (QWidget*)new QCheckBox();

                    ((QCheckBox*)right_widget)->setChecked(value);

                    QObject::connect((QCheckBox*)right_widget, &QCheckBox::stateChanged, this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
                }
                else if(type == "language")
                {
                    std::string     value                   = JsonUtils::JsonGetString(schema, "default");

                    if(settings.contains(key))
                    {
                        value                               = JsonUtils::JsonGetString(settings, key);
                    }

                    right_widget                            = (QWidget*)new QComboBox();

                    /*-------------------------------------*\
                    | Load available languages              |
                    | Technically the QString is unused but |
                    | declared here to show up in the       |
                    | translation file.                     |
                    \*-------------------------------------*/
                    QTranslator             translator;
                    QMap<QString, QString>  language_map;
                    QString                 language        = tr("English - US");
                    QDirIterator            file(":/i18n/", QDirIterator::Subdirectories);

                    while(file.hasNext())
                    {
                        if(translator.load(file.next()))
                        {
                            QString         language_locale = file.fileName().replace("OpenRGB_", "").replace(".qm", "");
                            language_map.insert(translator.translate("Settings", "English - US"), language_locale);
                        }
                    }

                    ((QComboBox*)right_widget)->addItem(tr("System Default"), "default");

                    QMapIterator<QString, QString> language_map_iterator(language_map);

                    while(language_map_iterator.hasNext())
                    {
                        language_map_iterator.next();
                        ((QComboBox*)right_widget)->addItem(language_map_iterator.key(), language_map_iterator.value());
                    }

                    int index = ((QComboBox*)right_widget)->findData(QVariant::fromValue(QString::fromStdString(value)));
                    if(index != -1)
                    {
                        ((QComboBox*)right_widget)->setCurrentIndex(index);
                    }

                    QObject::connect((QComboBox*)right_widget, qOverload<int>(&QComboBox::currentIndexChanged), this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
                }
            }

            layout->addWidget(left_widget, 1);
            layout->addWidget(right_widget, 1);

            UpdateLabels();

            adjustSize();
        }
    }
}

OpenRGBDynamicSettingsWidget::~OpenRGBDynamicSettingsWidget()
{
    ResourceManager::get()->GetProfileManager()->UnregisterProfileManagerCallback(OpenRGBDynamicSettingsWidgetProfileManagerCallback, this);
}

void OpenRGBDynamicSettingsWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        UpdateLabels();
    }
}

void OpenRGBDynamicSettingsWidget::ProfileListUpdated()
{
    if(type == "profile")
    {
        std::string                 name_value      = ((QComboBox*)right_widget)->currentText().toStdString();
        std::vector<std::string>    profile_list    = ResourceManager::get()->GetProfileManager()->GetProfileList();

        ((QComboBox*)right_widget)->clear();

        for(std::size_t profile_idx = 0; profile_idx < profile_list.size(); profile_idx++)
        {
            std::string             profile_str     = profile_list[profile_idx];
            ((QComboBox*)right_widget)->addItem(QString::fromStdString(profile_str));
        }

        ((QComboBox*)right_widget)->setCurrentText(QString::fromStdString(name_value));
    }
}

void OpenRGBDynamicSettingsWidget::OnSettingChanged()
{
    nlohmann::json          settings_change;

    if(type == "string" && !is_enum)
    {
        std::string     value           = ((QLineEdit*)right_widget)->text().toStdString();
        settings_change[key]            = value;
    }
    else if(type == "string" && is_enum)
    {
        std::string     value           = ((QComboBox*)right_widget)->currentText().toStdString();
        settings_change[key]            = value;
    }
    else if(type == "integer" && !is_enum)
    {
        int             value           = ((QSpinBox*)right_widget)->value();
        settings_change[key]            = value;
    }
    else if(type == "integer" && is_enum)
    {
        int             value           = ((QComboBox*)right_widget)->currentData().toInt();
        settings_change[key]            = value;
    }
    else if(type == "bool")
    {
        bool            value           = ((QCheckBox*)right_widget)->isChecked();
        settings_change[key]            = value;
    }
    else if(type == "profile")
    {
        bool            enabled_value   = ((QCheckBox*)left_widget)->isChecked();
        std::string     name_value      = ((QComboBox*)right_widget)->currentText().toStdString();
        ((QComboBox*)right_widget)->setEnabled(enabled_value);
        settings_change[key]["enabled"] = enabled_value;
        settings_change[key]["name"]    = name_value;
    }
    else if(type == "language")
    {
        std::string     value           = ((QComboBox*)right_widget)->currentData().toString().toStdString();
        settings_change[key]            = value;
    }

    if(callback)
    {
        callback(callback_arg, key, settings_change);
    }
}

void OpenRGBDynamicSettingsWidget::SetCallback(OpenRGBDynamicSettingsCallback callback, void* callback_arg)
{
    this->callback      = callback;
    this->callback_arg  = callback_arg;
}

void OpenRGBDynamicSettingsWidget::UpdateLabels()
{
    QApplication* app = static_cast<QApplication *>(QApplication::instance());

    if(type == "profile")
    {
        if(left_widget)
        {
            ((QCheckBox*)left_widget)->setText(app->translate("Settings", title.c_str()) + ":");
        }
        setToolTip(app->translate("Settings", description.c_str()));
    }
    else if(type != "object")
    {
        if(left_widget)
        {
            ((QLabel*)left_widget)->setText(app->translate("Settings", title.c_str()) + ":");
        }

        setToolTip(app->translate("Settings", description.c_str()));
    }
}
