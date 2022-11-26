#ifndef OPENRGBPHILIPSWIZSETTINGSPAGE_H
#define OPENRGBPHILIPSWIZSETTINGSPAGE_H

#include "ui_OpenRGBPhilipsWizSettingsPage.h"
#include <QWidget>

#include "OpenRGBPhilipsWizSettingsEntry.h"

namespace Ui {
class OpenRGBPhilipsWizSettingsPage;
}

class Ui::OpenRGBPhilipsWizSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPhilipsWizSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBPhilipsWizSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddPhilipsWizDeviceButton_clicked();

    void on_RemovePhilipsWizDeviceButton_clicked();

    void on_SavePhilipsWizConfigurationButton_clicked();

private:
    Ui::OpenRGBPhilipsWizSettingsPageUi *ui;
    std::vector<OpenRGBPhilipsWizSettingsEntry*> entries;

};

#endif // OPENRGBPHILIPSWIZSETTINGSPAGE_H
