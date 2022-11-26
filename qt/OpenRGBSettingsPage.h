#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "ui_OpenRGBSettingsPage.h"
#include <QCoreApplication>
#include <QDirIterator>
#include <QTranslator>
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

signals:
    void TrayIconChanged(bool tray_icon);

public slots:
    void UpdateProfiles();

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
    QTranslator translator;

private slots:
    void changeEvent(QEvent *event);
    void on_ComboBoxLanguage_currentTextChanged(const QString);
    void on_ComboBoxTheme_currentTextChanged(const QString);
    void on_CheckboxMinimizeOnClose_clicked();
    void on_CheckboxTrayIconGreyscale_clicked();
    void on_CheckboxLoadGeometry_clicked();
    void on_CheckboxSaveGeometry_clicked();
    void on_CheckboxAutoStart_clicked();
    void on_CheckboxAutoStartMinimized_clicked();
    void on_CheckboxAutoStartServer_clicked();
    void on_CheckboxAutoStartClient_clicked();
    void on_CheckboxAutoStartProfile_clicked();
    void on_TextServerHost_textChanged(const QString);
    void on_TextServerPort_textChanged(const QString);
    void on_TextClientHost_textChanged(const QString);
    void on_TextCustomArgs_textChanged(const QString);
    void on_ComboBoxAutoStartProfile_currentTextChanged(const QString);
    void on_CheckboxAutoStartSetServerHost_clicked();
    void on_CheckboxAutoStartSetServerPort_clicked();
    void on_CheckboxAutoStartCustom_clicked();
    void on_CheckboxRunZoneChecks_clicked();
    void on_OpenSettingsFolderButton_clicked();
    void on_CheckboxLogConsole_clicked();
    void on_CheckboxAMDSMBusReduceCPU_clicked();
    void on_CheckboxSharedSMBusAccess_clicked();

    void on_CheckboxSetOnExit_clicked(bool checked);
    void on_ComboBoxExitProfile_currentTextChanged(const QString exit_profile_name);
};

#endif // OPENRGBSETTINGSPAGE_H
