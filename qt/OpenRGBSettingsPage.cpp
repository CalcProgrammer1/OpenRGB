#include "AutoStart.h"
#include "OpenRGBSettingsPage.h"
#include "ui_OpenRGBSettingsPage.h"
#include "ResourceManager.h"
#include <QUrl>
#include <QDesktopServices>

using namespace Ui;

OpenRGBSettingsPage::OpenRGBSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBSettingsPageUi)
{
    ui->setupUi(this);

    /*---------------------------------------------------------*\
    | Load theme settings (Windows only)                        |
    \*---------------------------------------------------------*/
#ifdef _WIN32
    ui->ComboBoxTheme->addItems({"auto", "light", "dark"});

    json theme_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Theme");

    if(theme_settings.contains("theme"))
    {
        std::string theme = theme_settings["theme"];
        ui->ComboBoxTheme->setCurrentText(QString::fromStdString(theme));
    }
    else
    {
        ui->ComboBoxTheme->setCurrentText(QString::fromStdString(("light")));
    }

    theme_initialized = true;
#else
    ui->ComboBoxTheme->hide();
    ui->ThemeLabel->hide();
#endif

    /*---------------------------------------------------------*\
    | Load user interface settings                              |
    \*---------------------------------------------------------*/
    json ui_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("UserInterface");

    if(ui_settings.contains("minimize_on_close"))
    {
        ui->CheckboxMinimizeOnClose->setChecked(ui_settings["minimize_on_close"]);
    }

    if(ui_settings.contains("geometry"))
    {
        if(ui_settings["geometry"].contains("load_geometry"))
        {
            ui->CheckboxLoadGeometry->setChecked(ui_settings["geometry"]["load_geometry"]);
        }

        if(ui_settings["geometry"].contains("save_on_exit"))
        {
            ui->CheckboxSaveGeometry->setChecked(ui_settings["geometry"]["save_on_exit"]);
        }
    }

    if(ui_settings.contains("RunZoneChecks"))
    {
        ui->CheckboxRunZoneChecks->setChecked(ui_settings["RunZoneChecks"]);
    }
    else
    {   // default value
        ui->CheckboxRunZoneChecks->setChecked(true);
    }

    /*---------------------------------------------------------*\
    | Load LogManager settings                                  |
    \*---------------------------------------------------------*/
    json log_manager_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LogManager");

    /*---------------------------------------------------------*\
    | Checkboxes                                                |
    \*---------------------------------------------------------*/
    if(log_manager_settings.contains("log_console"))
    {
        ui->CheckboxLogConsole->setChecked(log_manager_settings["log_console"]);
    }

    /*---------------------------------------------------------*\
    | Load AutoStart settings                                   |
    \*---------------------------------------------------------*/
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    /*---------------------------------------------------------*\
    | Load profiles into combo box                              |
    \*---------------------------------------------------------*/
    if(profile_manager != NULL)
    {
        ui->ComboBoxAutoStartProfile->clear();

        for(std::size_t profile_index = 0; profile_index < profile_manager->profile_list.size(); profile_index++)
        {
            ui->ComboBoxAutoStartProfile->addItem(profile_manager->profile_list[profile_index].c_str());
        }
    }

    /*---------------------------------------------------------*\
    | Make sure autostart settings exist                        |
    \*---------------------------------------------------------*/
    CreateAutoStartSettings();

    /*---------------------------------------------------------*\
    | Initialise UI to current settings or defaults             |
    \*---------------------------------------------------------*/
    json autostart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("AutoStart");

    RemediateAutoStartProfile(autostart_settings);

    /*---------------------------------------------------------*\
    | Text boxes                                                |
    \*---------------------------------------------------------*/
    ui->TextServerPort->setText(QString::fromStdString(autostart_settings["port"]));
    ui->TextServerPort->setEnabled(autostart_settings["setserverport"]);

    ui->TextClientHost->setText(QString::fromStdString(autostart_settings["client"]));
    ui->TextClientHost->setEnabled(autostart_settings["setclient"]);

    ui->TextCustomArgs->setText(QString::fromStdString(autostart_settings["custom"]));
    ui->TextCustomArgs->setEnabled(autostart_settings["setcustom"]);

    /*---------------------------------------------------------*\
    | Checkboxes                                                |
    \*---------------------------------------------------------*/
    ui->CheckboxAutoStart->setChecked(autostart_settings["enabled"]);
    SetAutoStartVisibility(autostart_settings["enabled"]);

    ui->CheckboxAutoStartMinimized->setChecked(autostart_settings["setminimized"]);
    ui->CheckboxAutoStartClient->setChecked(autostart_settings["setclient"]);
    ui->CheckboxAutoStartServer->setChecked(autostart_settings["setserver"]);
    ui->CheckboxAutoStartSetServerPort->setChecked(autostart_settings["setserverport"]);
    ui->CheckboxAutoStartCustom->setChecked(autostart_settings["setcustom"]);

    ui->AutoStartStatusLabel->hide();
    autostart_initialized = true;
}

OpenRGBSettingsPage::~OpenRGBSettingsPage()
{
    delete ui;
}

void OpenRGBSettingsPage::on_ComboBoxTheme_currentTextChanged(const QString theme)
{
    if(theme_initialized)
    {
        json theme_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Theme");
        theme_settings["theme"] = theme.toStdString();
        ResourceManager::get()->GetSettingsManager()->SetSettings("Theme", theme_settings);
        SaveSettings();
    }
}

void OpenRGBSettingsPage::on_CheckboxMinimizeOnClose_clicked()
{
    json ui_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("UserInterface");
    ui_settings["minimize_on_close"] = ui->CheckboxMinimizeOnClose->isChecked();
    ResourceManager::get()->GetSettingsManager()->SetSettings("UserInterface", ui_settings);
    SaveSettings();
}

void Ui::OpenRGBSettingsPage::on_CheckboxLoadGeometry_clicked()
{
    json ui_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("UserInterface");
    ui_settings["geometry"]["load_geometry"] = ui->CheckboxLoadGeometry->isChecked();
    ResourceManager::get()->GetSettingsManager()->SetSettings("UserInterface", ui_settings);
    SaveSettings();
}

void Ui::OpenRGBSettingsPage::on_CheckboxSaveGeometry_clicked()
{
    json ui_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("UserInterface");
    ui_settings["geometry"]["save_on_exit"] = ui->CheckboxSaveGeometry->isChecked();
    ResourceManager::get()->GetSettingsManager()->SetSettings("UserInterface", ui_settings);
    SaveSettings();
}

void Ui::OpenRGBSettingsPage::on_CheckboxRunZoneChecks_clicked()
{
    json ui_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("UserInterface");
    ui_settings["RunZoneChecks"] = ui->CheckboxRunZoneChecks->isChecked();
    ResourceManager::get()->GetSettingsManager()->SetSettings("UserInterface", ui_settings);
    SaveSettings();
}

void Ui::OpenRGBSettingsPage::on_CheckboxAutoStart_clicked()
{
    if(autostart_initialized)
    {
        json autostart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("AutoStart");
        autostart_settings["enabled"] = ui->CheckboxAutoStart->isChecked();
        
        if(autostart_settings["enabled"])
        {
            RemediateAutoStartProfile(autostart_settings);
        }

        ResourceManager::get()->GetSettingsManager()->SetSettings("AutoStart", autostart_settings);
        SaveSettings();

        SetAutoStartVisibility(autostart_settings["enabled"]);

        ConfigureAutoStart();
    }
}

void Ui::OpenRGBSettingsPage::on_CheckboxAutoStartMinimized_clicked()
{
    SaveAutoStartSetting("setminimized", ui->CheckboxAutoStartMinimized->isChecked());
}

void Ui::OpenRGBSettingsPage::on_CheckboxAutoStartServer_clicked()
{
    SaveAutoStartSetting("setserver", ui->CheckboxAutoStartServer->isChecked());
}

void Ui::OpenRGBSettingsPage::on_CheckboxAutoStartSetServerPort_clicked()
{
    SaveAutoStartSetting("setserverport", ui->CheckboxAutoStartSetServerPort->isChecked());
    ui->TextServerPort->setEnabled(ui->CheckboxAutoStartSetServerPort->isChecked());
}

void Ui::OpenRGBSettingsPage::on_CheckboxAutoStartClient_clicked()
{
    SaveAutoStartSetting("setclient", ui->CheckboxAutoStartClient->isChecked());
    ui->TextClientHost->setEnabled(ui->CheckboxAutoStartClient->isChecked());
}

void Ui::OpenRGBSettingsPage::on_CheckboxAutoStartProfile_clicked()
{
    SaveAutoStartSetting("setprofile", ui->CheckboxAutoStartProfile->isChecked());
    ui->ComboBoxAutoStartProfile->setEnabled(ui->CheckboxAutoStartProfile->isChecked());
}

void Ui::OpenRGBSettingsPage::on_CheckboxAutoStartCustom_clicked()
{
    SaveAutoStartSetting("setcustom", ui->CheckboxAutoStartCustom->isChecked());
    ui->TextCustomArgs->setEnabled(ui->CheckboxAutoStartCustom->isChecked());
}

void Ui::OpenRGBSettingsPage::on_TextServerPort_textChanged(QString port)
{
    SaveAutoStartSetting("port", port);
}

void Ui::OpenRGBSettingsPage::on_TextClientHost_textChanged(QString client)
{
    SaveAutoStartSetting("client", client);
}

void Ui::OpenRGBSettingsPage::on_TextCustomArgs_textChanged(QString custom)
{
    SaveAutoStartSetting("custom", custom);
}

void Ui::OpenRGBSettingsPage::on_ComboBoxAutoStartProfile_currentTextChanged(const QString profile)
{
    SaveAutoStartSetting("profile", profile);
}

void OpenRGBSettingsPage::SaveAutoStartSetting(std::string name, QString value)
{
    if(autostart_initialized)
    {
        json autostart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("AutoStart");
        autostart_settings[name] = value.toStdString();
        ResourceManager::get()->GetSettingsManager()->SetSettings("AutoStart", autostart_settings);
        SaveSettings();

        ConfigureAutoStart();
    }
}

void OpenRGBSettingsPage::SaveAutoStartSetting(std::string name, bool value)
{
    if(autostart_initialized)
    {
        json autostart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("AutoStart");
        autostart_settings[name] = value;
        ResourceManager::get()->GetSettingsManager()->SetSettings("AutoStart", autostart_settings);
        SaveSettings();

        ConfigureAutoStart();
    }
}

void OpenRGBSettingsPage::SetAutoStartVisibility(bool visible)
{
    if (!visible)
    {
        ui->ComboBoxAutoStartProfile->hide();
        ui->CheckboxAutoStartClient->hide();
        ui->CheckboxAutoStartCustom->hide();
        ui->CheckboxAutoStartMinimized->hide();
        ui->CheckboxAutoStartProfile->hide();
        ui->CheckboxAutoStartServer->hide();
        ui->CheckboxAutoStartSetServerPort->hide();
        ui->TextCustomArgs->hide();
        ui->TextClientHost->hide();
        ui->TextServerPort->hide();
        ui->AutoStartStatusLabel->hide();
    }
    else
    {
        ui->ComboBoxAutoStartProfile->show();
        ui->CheckboxAutoStartClient->show();
        ui->CheckboxAutoStartCustom->show();
        ui->CheckboxAutoStartMinimized->show();
        ui->CheckboxAutoStartProfile->show();
        ui->CheckboxAutoStartServer->show();
        ui->CheckboxAutoStartSetServerPort->show();
        ui->TextCustomArgs->show();
        ui->TextClientHost->show();
        ui->TextServerPort->show();
        ui->AutoStartStatusLabel->show();
    }
}

void OpenRGBSettingsPage::ConfigureAutoStart()
{
    std::map<std::string, std::tuple<std::string, std::string, bool>> autostart_map = {
        {"setminimized", {"--startminimized","",false}},
        {"setserver", {"--server","",false}},
        {"setserverport", {"--server-port","port",false}},
        {"setclient", {"--client","client",false}},
        {"setprofile", {"--profile","profile",true}},
        {"setcustom", {"","custom",false}},
    };

    json autostart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("AutoStart");

    AutoStart auto_start("OpenRGB");

    if (!autostart_settings["enabled"])
    {
        if (!auto_start.DisableAutoStart())
        {
            ui->AutoStartStatusLabel->setText("A problem occurred disabling Start At Login.");
            ui->AutoStartStatusLabel->show();
            SetAutoStartVisibility(true);
        }
        else
        {
            ui->AutoStartStatusLabel->hide();
        }
    }
    else
    {
        std::string desc = "OpenRGB ";
        desc += VERSION_STRING;
        desc += ", for controlling RGB lighting.";

        std::string arguments = "";

        for(std::pair<const std::string, std::tuple<std::string, std::string, bool>>& x: autostart_map)
        {
            std::string argumentsetting = x.first;
            std::string argument = std::get<0>(x.second);
            std::string argumentvaluename = std::get<1>(x.second);
            bool argumentquoted = std::get<2>(x.second);

            if (autostart_settings[x.first])
            {
                if (argument != "")
                {
                    if (arguments != "")
                    {
                        arguments += " ";
                    }
                    arguments += argument;
                }
                if (argumentvaluename != "")
                {
                    std::string argumentvalue = autostart_settings[argumentvaluename];
                    if (argumentvalue != "")
                    {
                        if (arguments != "")
                        {
                            arguments += " ";
                        }
                        if (argumentquoted)
                        {
                            arguments += "\"" + argumentvalue + "\"";
                        }
                        else
                        {
                            arguments += argumentvalue;
                        }
                    }
                }
            }
        }

        AutoStartInfo auto_start_info;

        auto_start_info.args        = arguments;
        auto_start_info.category    = "Utility;";
        auto_start_info.desc        = desc;
        auto_start_info.icon        = "OpenRGB";
        auto_start_info.path        = auto_start.GetExePath();

        if (!auto_start.EnableAutoStart(auto_start_info))
        {
            ui->AutoStartStatusLabel->setText("A problem occurred enabling Start At Login.");
            ui->AutoStartStatusLabel->show();
            SetAutoStartVisibility(true);
        }
        else
        {
            ui->AutoStartStatusLabel->hide();
        }
    }
}

void OpenRGBSettingsPage::CreateAutoStartSettings()
{
    std::map<std::string, std::string> autostart_default_map_string = {
        {"custom", ""},
        {"port", "6742"},
        {"client","localhost:6742"},
        {"profile",ui->ComboBoxAutoStartProfile->count() > 0 ? ui->ComboBoxAutoStartProfile->itemText(0).toStdString(): ""}
    };

    std::map<std::string, bool> autostart_default_map_bool = {
        {"enabled", false},
        {"setminimized", false},
        {"setclient", false},
        {"setserver", false},
        {"setserverport", false},
        {"setcustom", false},
        {"setprofile", false},
    };

    json autostart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("AutoStart");

    for(std::pair<const std::string, bool>& x: autostart_default_map_bool)
    {
        if(!autostart_settings.contains(x.first))
        {
            autostart_settings[x.first] = x.second;
        }
    }

    for(std::pair<const std::string, std::string>& x: autostart_default_map_string)
    {
        if(!autostart_settings.contains(x.first))
        {
            autostart_settings[x.first] = x.second;
        }
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("AutoStart", autostart_settings);
}

void OpenRGBSettingsPage::RemediateAutoStartProfile(json &autostart_settings)
{
    /*---------------------------------------------------------*\
    | If there are no profiles then we disable the UI for       |
    | profiles and if AutoStart is enabled then we force        |
    | disable setprofile                                        |
    \*---------------------------------------------------------*/
    if(ui->ComboBoxAutoStartProfile->count() == 0)
    {
        ui->CheckboxAutoStartProfile->setEnabled(false);
        ui->ComboBoxAutoStartProfile->setEnabled(false);

        autostart_settings["profile"] = "";

        if(autostart_settings["enabled"])
        {
            autostart_settings["setprofile"] = false;

            ResourceManager::get()->GetSettingsManager()->SetSettings("AutoStart", autostart_settings);

            ConfigureAutoStart();
            SaveSettings();
        }
    }

    /*---------------------------------------------------------*\
    | Else If the profile we want doesn't exist then we force   |
    | it to a profile which exists and if AutoStart is enabled  |
    | then we force disable setprofile                          |
    \*---------------------------------------------------------*/
    else if(autostart_settings["profile"] == "" ||
           (autostart_settings["profile"] != "" &&
            ui->ComboBoxAutoStartProfile->findText(QString::fromStdString(autostart_settings["profile"])) == -1))
    {
        autostart_settings["profile"] = ui->ComboBoxAutoStartProfile->itemText(0).toStdString();
        
        if(autostart_settings["enabled"])
        {
            autostart_settings["setprofile"] = false;
            
            ResourceManager::get()->GetSettingsManager()->SetSettings("AutoStart", autostart_settings);
            
            ConfigureAutoStart();
            SaveSettings();
        }
    }

    ui->ComboBoxAutoStartProfile->setCurrentText(QString::fromStdString(autostart_settings["profile"]));
    ui->ComboBoxAutoStartProfile->setEnabled(autostart_settings["setprofile"]);
    ui->CheckboxAutoStartProfile->setChecked(autostart_settings["setprofile"]);
}

void OpenRGBSettingsPage::SaveSettings()
{
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}

void Ui::OpenRGBSettingsPage::on_OpenSettingsFolderButton_clicked()
{
    std::string config_dir = ResourceManager::get()->GetConfigurationDirectory();
    QUrl url = QUrl::fromLocalFile(QString::fromStdString(config_dir));
    QDesktopServices::openUrl(url);
}


void Ui::OpenRGBSettingsPage::on_CheckboxLogConsole_clicked()
{
    json log_manager_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LogManager");
    log_manager_settings["log_console"] = ui->CheckboxLogConsole->isChecked();
    ResourceManager::get()->GetSettingsManager()->SetSettings("LogManager", log_manager_settings);
    SaveSettings();
}

