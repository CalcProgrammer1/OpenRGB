#ifndef OPENRGBPHILIPSHUESETTINGSPAGE_H
#define OPENRGBPHILIPSHUESETTINGSPAGE_H

#include "ui_OpenRGBPhilipsHueSettingsPage.h"
#include <QWidget>

#include "OpenRGBPhilipsHueSettingsEntry.h"

namespace Ui {
class OpenRGBPhilipsHueSettingsPage;
}

class Ui::OpenRGBPhilipsHueSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPhilipsHueSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBPhilipsHueSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddPhilipsHueDeviceButton_clicked();

    void on_RemovePhilipsHueDeviceButton_clicked();

    void on_SavePhilipsHueConfigurationButton_clicked();

private:
    Ui::OpenRGBPhilipsHueSettingsPageUi *ui;
    std::vector<OpenRGBPhilipsHueSettingsEntry*> entries;

};

#endif // OPENRGBPHILIPSHUESETTINGSPAGE_H
