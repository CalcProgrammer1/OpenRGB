/*---------------------------------------------------------*\
| OpenRGBSettingsPage.h                                     |
|                                                           |
|   User interface for general settings page                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QCoreApplication>
#include <QDirIterator>
#include <QTranslator>
#include <QWidget>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Ui
{
    class OpenRGBSettingsPage;
}

class OpenRGBSettingsPage : public QWidget
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
    Ui::OpenRGBSettingsPage *ui;
    void SaveSettings();

    void CreateAutoStartSettings();
    void ConfigureAutoStart();
    void RemediateAutoStartProfile(json &autostart_settings);
    void SetAutoStartVisibility(bool visible);
    void SaveAutoStartSetting(std::string name, QString value);
    void SaveAutoStartSetting(std::string name, bool value);

    bool theme_initialized = false;
    bool autostart_initialized = false;
    bool hex_format_initialized = false;
    QTranslator translator;

private slots:
    void changeEvent(QEvent *event);
    void on_ComboBoxLanguage_currentTextChanged(const QString);
    void on_ComboBoxTheme_currentTextChanged(const QString);
    void on_ComboBoxHexFormat_currentTextChanged(const QString);
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
    void on_TextServerPort_valueChanged(int);
    void on_TextClientHost_textChanged(const QString);
    void on_TextCustomArgs_textChanged(const QString);
    void on_ComboBoxAutoStartProfile_currentTextChanged(const QString);
    void on_CheckboxAutoStartSetServerHost_clicked();
    void on_CheckboxAutoStartSetServerPort_clicked();
    void on_CheckboxAutoStartCustom_clicked();
    void on_CheckboxRunZoneChecks_clicked();
    void on_OpenSettingsFolderButton_clicked();
    void on_CheckboxLogConsole_clicked();
    void on_CheckboxLogFile_clicked();
    void on_CheckboxAMDSMBusReduceCPU_clicked();
    void on_CheckboxSharedSMBusAccess_clicked();

    void on_CheckboxSetOnExit_clicked(bool checked);
    void on_ComboBoxExitProfile_currentTextChanged(const QString exit_profile_name);
    void on_CheckboxSetOnResume_clicked(bool checked);
    void on_ComboBoxResumeProfile_currentTextChanged(const QString resume_profile_name);
    void on_CheckboxSetOnSuspend_clicked(bool checked);
    void on_ComboBoxSuspendProfile_currentTextChanged(const QString suspend_profile_name);
    void on_CheckboxDisableKeyExpansion_clicked();
    void on_CheckboxShowLEDView_clicked();
    void on_CheckboxAllDevices_clicked(bool checked);
    void on_CheckboxLegacyWorkaround_clicked(bool checked);
};
