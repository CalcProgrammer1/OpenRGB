#ifndef OPENRGBNanoleafSETTINGSENTRY_H
#define OPENRGBNanoleafSETTINGSENTRY_H

#include "ui_OpenRGBNanoleafSettingsEntry.h"
#include "OpenRGBNanoleafScanningThread.h"
#include <QWidget>

namespace Ui
{
class OpenRGBNanoleafSettingsEntry;
}

class Ui::OpenRGBNanoleafSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBNanoleafSettingsEntry(QWidget *parent = nullptr);
    OpenRGBNanoleafSettingsEntry(QString a_address, int a_port);
    ~OpenRGBNanoleafSettingsEntry();
    Ui::OpenRGBNanoleafSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
    void on_UnpairButton_clicked();
    void on_PairButton_clicked();

private:
    QString address;
    int port;
    std::string auth_token;
    bool paired;
};

#endif // OPENRGBNanoleafSETTINGSENTRY_H
