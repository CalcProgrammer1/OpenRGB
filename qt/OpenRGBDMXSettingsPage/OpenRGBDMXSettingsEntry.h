#ifndef OPENRGBDMXSETTINGSENTRY_H
#define OPENRGBDMXSETTINGSENTRY_H

#include "ui_OpenRGBDMXSettingsEntry.h"
#include <QWidget>

namespace Ui {
class OpenRGBDMXSettingsEntry;
}

class Ui::OpenRGBDMXSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBDMXSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBDMXSettingsEntry();
    Ui::OpenRGBDMXSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
    void on_TypeComboBox_currentIndexChanged(int index);
};

#endif // OPENRGBDMXSETTINGSENTRY_H
