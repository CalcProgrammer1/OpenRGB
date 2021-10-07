#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "ui_OpenRGBSettingsPage.h"
#include <QWidget>
#include "ResourceManager.h"

namespace Ui {
class OpenRGBSettingsPage;
}

class Ui::OpenRGBSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBSettingsPage();

private:
    Ui::OpenRGBSettingsPageUi *ui;
    void SaveSettings();

    void CreateAutoStartSettings();
    void ConfigureAutoStart();
    void RemediateAutoStartProfile(json &autostart_settings);
    void SetAutoStartVisibility(bool visible);
    void SaveAutoStartSetting(std::string name, QString value);
    void SaveAutoStartSetting(std::string name, bool value);

    bool theme_initialized = false;
    bool autostart_initialized = false;

private slots:
    void on_ComboBoxTheme_currentTextChanged(const QString);
    void on_CheckboxMinimizeOnClose_clicked();
    void on_CheckboxLoadGeometry_clicked();
    void on_CheckboxSaveGeometry_clicked();
    void on_CheckboxAutoStart_clicked();
    void on_CheckboxAutoStartMinimized_clicked();
    void on_CheckboxAutoStartServer_clicked();
    void on_CheckboxAutoStartClient_clicked();
    void on_CheckboxAutoStartProfile_clicked();
    void on_TextServerPort_textChanged(const QString);
    void on_TextClientHost_textChanged(const QString);
    void on_TextCustomArgs_textChanged(const QString);
    void on_ComboBoxAutoStartProfile_currentTextChanged(const QString);
    void on_CheckboxAutoStartSetServerPort_clicked();
    void on_CheckboxAutoStartCustom_clicked();
    void on_CheckboxRunZoneChecks_clicked();
    void on_OpenSettingsFolderButton_clicked();
    void on_CheckboxLogConsole_clicked();
};

#endif // OPENRGBSETTINGSPAGE_H
