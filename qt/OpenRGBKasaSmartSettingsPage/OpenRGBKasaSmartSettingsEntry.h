#ifndef OPENRGBKASASMARTSETTINGSENTRY_H
#define OPENRGBKASASMARTSETTINGSENTRY_H

#include "ui_OpenRGBKasaSmartSettingsEntry.h"
#include <QWidget>

namespace Ui {
class OpenRGBKasaSmartSettingsEntry;
}

class Ui::OpenRGBKasaSmartSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBKasaSmartSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBKasaSmartSettingsEntry();
    Ui::OpenRGBKasaSmartSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};

#endif // OPENRGBKASASMARTSETTINGSENTRY_H
