#include "OpenRGBSettingsPage.h"
#include "ui_OpenRGBSettingsPage.h"
#include "ResourceManager.h"

using namespace Ui;

OpenRGBSettingsPage::OpenRGBSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBSettingsPageUi)
{
    ui->setupUi(this);


#ifdef _WIN32
    ui->ComboBoxTheme->addItems({"auto", "light", "dark"});

    json theme_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Theme");

    if(theme_settings.contains("theme"))
    {
        std::string theme = theme_settings["theme"];
        ui->ComboBoxTheme->setCurrentText(QString::fromStdString(theme));
    }
#else
    ui->ComboBoxTheme->hide();
    ui->ThemeLabel->hide();
#endif

    json ui_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("UserInterface");
    ui->CheckboxMinimizeOnClose->setChecked(ui_settings["minimize_on_close"]);
}

OpenRGBSettingsPage::~OpenRGBSettingsPage()
{
    delete ui;
}

void OpenRGBSettingsPage::on_ComboBoxTheme_currentTextChanged(const QString theme)
{
    json theme_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Theme");
    theme_settings["theme"] = theme.toStdString();
    ResourceManager::get()->GetSettingsManager()->SetSettings("Theme", theme_settings);
    SaveSettings();
}

void OpenRGBSettingsPage::on_CheckboxMinimizeOnClose_clicked()
{
    json ui_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("UserInterface");
    ui_settings["minimize_on_close"] = ui->CheckboxMinimizeOnClose->isChecked();
    ResourceManager::get()->GetSettingsManager()->SetSettings("UserInterface", ui_settings);
    SaveSettings();
}

void OpenRGBSettingsPage::SaveSettings()
{
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}


