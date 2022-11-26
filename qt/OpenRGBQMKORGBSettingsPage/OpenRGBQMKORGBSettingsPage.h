#ifndef OPENRGBQMKORGBSETTINGSPAGE_H
#define OPENRGBQMKORGBSETTINGSPAGE_H

#include "ui_OpenRGBQMKORGBSettingsPage.h"
#include <QWidget>

#include "OpenRGBQMKORGBSettingsEntry.h"

namespace Ui {
class OpenRGBQMKORGBSettingsPage;
}

class Ui::OpenRGBQMKORGBSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBQMKORGBSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBQMKORGBSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddQMKORGBDeviceButton_clicked();

    void on_RemoveQMKORGBDeviceButton_clicked();

    void on_SaveQMKORGBConfigurationButton_clicked();

private:
    Ui::OpenRGBQMKORGBSettingsPageUi *ui;
    std::vector<OpenRGBQMKORGBSettingsEntry*> entries;

};

#endif // OPENRGBQMKORGBSETTINGSPAGE_H
