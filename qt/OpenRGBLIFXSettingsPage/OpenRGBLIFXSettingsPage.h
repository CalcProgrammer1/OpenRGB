#ifndef OPENRGBLIFXSETTINGSPAGE_H
#define OPENRGBLIFXSETTINGSPAGE_H

#include "ui_OpenRGBLIFXSettingsPage.h"
#include <QWidget>

#include "OpenRGBLIFXSettingsEntry.h"

namespace Ui {
class OpenRGBLIFXSettingsPage;
}

class Ui::OpenRGBLIFXSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBLIFXSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBLIFXSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddLIFXDeviceButton_clicked();

    void on_RemoveLIFXDeviceButton_clicked();

    void on_SaveLIFXConfigurationButton_clicked();

private:
    Ui::OpenRGBLIFXSettingsPageUi *ui;
    std::vector<OpenRGBLIFXSettingsEntry*> entries;

};

#endif // OPENRGBLIFXSETTINGSPAGE_H
