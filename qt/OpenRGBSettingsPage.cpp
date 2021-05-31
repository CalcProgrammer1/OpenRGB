#include "OpenRGBSettingsPage.h"
#include "ui_OpenRGBSettingsPage.h"
#include "ResourceManager.h"

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
    | Load user interface settings (Windows only)               |
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

void OpenRGBSettingsPage::SaveSettings()
{
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
