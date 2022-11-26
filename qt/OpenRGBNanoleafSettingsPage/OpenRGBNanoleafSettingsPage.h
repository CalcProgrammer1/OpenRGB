#ifndef OPENRGBNanoleafSETTINGSPAGE_H
#define OPENRGBNanoleafSETTINGSPAGE_H

#include "ui_OpenRGBNanoleafSettingsPage.h"
#include <QWidget>

#include "OpenRGBNanoleafSettingsEntry.h"

namespace Ui
{
class OpenRGBNanoleafSettingsPage;
}

class Ui::OpenRGBNanoleafSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBNanoleafSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBNanoleafSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_ScanForNanoleafDevicesButton_clicked();
    void on_DeviceFound(QString address, int port);

private:
    Ui::OpenRGBNanoleafSettingsPageUi *ui;
    std::map<std::string, OpenRGBNanoleafSettingsEntry*> entries;
};

#endif // OPENRGBNanoleafSETTINGSPAGE_H
