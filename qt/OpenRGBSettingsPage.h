#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "ui_OpenRGBSettingsPage.h"
#include <QWidget>

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

    bool theme_initialized = false;

private slots:
    void on_ComboBoxTheme_currentTextChanged(const QString);
    void on_CheckboxMinimizeOnClose_clicked();
    void on_CheckboxLoadGeometry_clicked();
    void on_CheckboxSaveGeometry_clicked();
};

#endif // OPENRGBSETTINGSPAGE_H
