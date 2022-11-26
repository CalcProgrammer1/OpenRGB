#ifndef OPENRGBYEELIGHTSETTINGSENTRY_H
#define OPENRGBYEELIGHTSETTINGSENTRY_H

#include "ui_OpenRGBYeelightSettingsEntry.h"
#include <QWidget>

namespace Ui {
class OpenRGBYeelightSettingsEntry;
}

class Ui::OpenRGBYeelightSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBYeelightSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBYeelightSettingsEntry();
    Ui::OpenRGBYeelightSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
    void on_HostIPChooserButton_clicked();

};

#endif // OPENRGBYeelightSETTINGSENTRY_H
