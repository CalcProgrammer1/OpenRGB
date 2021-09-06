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
};

#endif // OPENRGBYeelightSETTINGSENTRY_H
