#ifndef OPENRGBLIFXSETTINGSENTRY_H
#define OPENRGBLIFXSETTINGSENTRY_H

#include "ui_OpenRGBLIFXSettingsEntry.h"
#include <QWidget>

namespace Ui {
class OpenRGBLIFXSettingsEntry;
}

class Ui::OpenRGBLIFXSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBLIFXSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBLIFXSettingsEntry();
    Ui::OpenRGBLIFXSettingsEntryUi *ui;
};

#endif // OPENRGBLIFXSETTINGSENTRY_H
