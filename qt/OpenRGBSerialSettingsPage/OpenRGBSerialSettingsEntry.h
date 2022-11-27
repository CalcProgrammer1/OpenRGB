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

private slots:
    void changeEvent(QEvent *event);

    void on_ProtocolComboBox_currentIndexChanged(int index);

public:
    explicit OpenRGBSerialSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBSerialSettingsEntry();
    Ui::OpenRGBSerialSettingsEntryUi *ui;
};

#endif // OPENRGBSERIALSETTINGSENTRY_H
