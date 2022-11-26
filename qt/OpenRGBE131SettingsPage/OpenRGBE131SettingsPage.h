#ifndef OPENRGBE131SETTINGSPAGE_H
#define OPENRGBE131SETTINGSPAGE_H

#include "ui_OpenRGBE131SettingsPage.h"
#include <QWidget>

#include "OpenRGBE131SettingsEntry.h"

namespace Ui {
class OpenRGBE131SettingsPage;
}

class Ui::OpenRGBE131SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBE131SettingsPage(QWidget *parent = nullptr);
    ~OpenRGBE131SettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddE131DeviceButton_clicked();

    void on_RemoveE131DeviceButton_clicked();

    void on_SaveE131ConfigurationButton_clicked();

private:
    Ui::OpenRGBE131SettingsPageUi *ui;
    std::vector<OpenRGBE131SettingsEntry*> entries;

};

#endif // OPENRGBE131SETTINGSPAGE_H
