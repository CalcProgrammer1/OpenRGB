
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QToolTip>
#include <QVBoxLayout>
#include "OpenRGBDynamicSettingsWidget.h"
#include "ProfileManager.h"
#include "ResourceManager.h"

#define JSON_INTEGER(val, key, dft)     (((val.contains(key)) &&                                                                \
                                        ((val[key].type() == nlohmann::json::value_t::number_integer)  ||                       \
                                         (val[key].type() == nlohmann::json::value_t::number_unsigned) ||                       \
                                         (val[key].type() == nlohmann::json::value_t::number_float)))  ? (int)val[key] : dft)
#define JSON_BOOL(val, key)             (((val.contains(key)) && (val[key].type() == nlohmann::json::value_t::boolean)) ? (bool)val[key] : false)
#define JSON_STRING(val, key)           (((val.contains(key)) && (val[key].type() == nlohmann::json::value_t::string)) ? (std::string)val[key] : "")

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
    this->key       = key;
    type            = "";
    is_enum         = false;
    callback        = nullptr;

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
        std::string         title           = JSON_STRING(schema, "title");
        type                                = JSON_STRING(schema, "type");

        if(schema.contains("enum"))
        {
            is_enum                         = true;
        }

        if(schema.contains("description"))
        {
            std::string     description     = JSON_STRING(schema, "description");

            setToolTip(QString::fromStdString(description));
        }

        /*-------------------------------------------------*\
        | If type is object, create a nested group          |
        \*-------------------------------------------------*/
        if(type == "object" && schema.contains("properties"))
        {
            /*---------------------------------------------*\
            | Create the groupbox and layout                |
            \*---------------------------------------------*/
            nlohmann::json&     properties_json = schema["properties"];
            nlohmann::json&     settings_json   = settings[key];
            QGroupBox*          groupbox        = new QGroupBox(QString::fromStdString(title));
            QVBoxLayout*        groupbox_layout = new QVBoxLayout();

            groupbox->setLayout(groupbox_layout);

            for(nlohmann::json::iterator json_iterator = properties_json.begin(); json_iterator != properties_json.end(); json_iterator++)
            {
                nlohmann::json&                 property    = json_iterator.value();
                OpenRGBDynamicSettingsWidget*   item_widget = new OpenRGBDynamicSettingsWidget(json_iterator.key(), property, settings_json);

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
                    enabled_value                           = JSON_BOOL(settings[key], "enabled");
                    name_value                              = JSON_STRING(settings[key], "name");
                }

                left_widget                                 = (QWidget*)new QCheckBox(QString::fromStdString(title));
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
                left_widget                                 = (QWidget*)new QLabel(QString::fromStdString(title));

                /*-----------------------------------------*\
                | Add a LineEdit for non-enum string        |
                | settings                                  |
                \*-----------------------------------------*/
                if(type == "string" && !is_enum)
                {
                    std::string     value                   = JSON_STRING(schema, "default");

                    if(settings.contains(key))
                    {
                        value                               = JSON_STRING(settings, key);
                    }

                    right_widget                            = (QWidget*)new QLineEdit(QString::fromStdString(value));

                    QObject::connect((QLineEdit*)right_widget, &QLineEdit::textChanged, this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
                }
                /*-----------------------------------------*\
                | Add a ComboBox for enum string settings   |
                \*-----------------------------------------*/
                else if(type == "string" && is_enum)
                {
                    std::string     value                   = JSON_STRING(schema, "default");

                    if(settings.contains(key))
                    {
                        value                               = JSON_STRING(settings, key);
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
                    int             maximum                 = JSON_INTEGER(schema, "maximum", 2147483647);
                    int             minimum                 = JSON_INTEGER(schema, "minimum", -2147483648);
                    int             value                   = JSON_INTEGER(schema, "default", 0);

                    if(settings.contains(key))
                    {
                        value                               = JSON_INTEGER(settings, key, 0);
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
                    int             value                   = JSON_INTEGER(schema, "default", 0);;

                    if(settings.contains(key))
                    {
                        value                               = JSON_INTEGER(settings, key, 0);
                    }

                    right_widget                            = (QWidget*)new QComboBox();

                    for(std::size_t enum_idx = 0; enum_idx < schema["enum"].size(); enum_idx++)
                    {
                        int         enum_int                = schema["enum"][enum_idx];
                        ((QComboBox*)right_widget)->addItem(QString::number(enum_int));
                    }

                    ((QComboBox*)right_widget)->setCurrentText(QString::number(value));

                    QObject::connect((QComboBox*)right_widget, qOverload<int>(&QComboBox::currentIndexChanged), this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
                }
                /*-----------------------------------------*\
                | Add a CheckBox for bool settings          |
                \*-----------------------------------------*/
                else if(type == "bool")
                {
                    bool            value                   = JSON_BOOL(schema, "default");

                    if(settings.contains(key))
                    {
                        value                               = JSON_BOOL(settings, key);
                    }

                    right_widget                            = (QWidget*)new QCheckBox();

                    ((QCheckBox*)right_widget)->setChecked(value);

                    QObject::connect((QCheckBox*)right_widget, &QCheckBox::stateChanged, this, &OpenRGBDynamicSettingsWidget::OnSettingChanged);
                }
            }

            layout->addWidget(left_widget, 1);
            layout->addWidget(right_widget, 1);

            adjustSize();
        }
    }
}

OpenRGBDynamicSettingsWidget::~OpenRGBDynamicSettingsWidget()
{
    ResourceManager::get()->GetProfileManager()->UnregisterProfileManagerCallback(OpenRGBDynamicSettingsWidgetProfileManagerCallback, this);
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
    if(type == "integer" && !is_enum)
    {
        int             value           = ((QSpinBox*)right_widget)->value();
        settings_change[key]            = value;
    }
    else if(type == "integer" && is_enum)
    {
        int             value           = ((QComboBox*)right_widget)->currentText().toInt();
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
