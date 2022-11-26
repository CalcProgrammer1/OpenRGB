#ifndef OPENRGBYEELIGHTSETTINGSPAGE_H
#define OPENRGBYEELIGHTSETTINGSPAGE_H

#include "ui_OpenRGBYeelightSettingsPage.h"
#include <QWidget>

#include "OpenRGBYeelightSettingsEntry.h"

namespace Ui {
class OpenRGBYeelightSettingsPage;
}

class Ui::OpenRGBYeelightSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBYeelightSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBYeelightSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddYeelightDeviceButton_clicked();

    void on_RemoveYeelightDeviceButton_clicked();

    void on_SaveYeelightConfigurationButton_clicked();

private:
    Ui::OpenRGBYeelightSettingsPageUi *ui;
    std::vector<OpenRGBYeelightSettingsEntry*> entries;

};

#endif // OPENRGBYEELIGHTSETTINGSPAGE_H
