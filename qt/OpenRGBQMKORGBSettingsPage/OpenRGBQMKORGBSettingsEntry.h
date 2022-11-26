#ifndef OPENRGBQMKORGBSETTINGSENTRY_H
#define OPENRGBQMKORGBSETTINGSENTRY_H

#include "ui_OpenRGBQMKORGBSettingsEntry.h"
#include <QWidget>

namespace Ui {
class OpenRGBQMKORGBSettingsEntry;
}

class Ui::OpenRGBQMKORGBSettingsEntry : public QWidget
{
    Q_OBJECT

private slots:
    void changeEvent(QEvent *event);

public:
    explicit OpenRGBQMKORGBSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBQMKORGBSettingsEntry();
    Ui::OpenRGBQMKORGBSettingsEntryUi *ui;
};

#endif // OPENRGBQMKORGBSETTINGSENTRY_H
