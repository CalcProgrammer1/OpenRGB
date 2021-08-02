#ifndef OPENRGBSERIALSETTINGSENTRY_H
#define OPENRGBSERIALSETTINGSENTRY_H

#include "ui_OpenRGBSerialSettingsEntry.h"
#include <QWidget>

namespace Ui {
class OpenRGBSerialSettingsEntry;
}

class Ui::OpenRGBSerialSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBSerialSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBSerialSettingsEntry();
    Ui::OpenRGBSerialSettingsEntryUi *ui;
};

#endif // OPENRGBSERIALSETTINGSENTRY_H
