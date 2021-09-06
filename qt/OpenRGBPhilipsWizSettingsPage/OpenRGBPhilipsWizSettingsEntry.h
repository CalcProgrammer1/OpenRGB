#ifndef OPENRGBPHILIPSWIZSETTINGSENTRY_H
#define OPENRGBPHILIPSWIZSETTINGSENTRY_H

#include "ui_OpenRGBPhilipsWizSettingsEntry.h"
#include <QWidget>

namespace Ui {
class OpenRGBPhilipsWizSettingsEntry;
}

class Ui::OpenRGBPhilipsWizSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPhilipsWizSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBPhilipsWizSettingsEntry();
    Ui::OpenRGBPhilipsWizSettingsEntryUi *ui;
};

#endif // OPENRGBPHILIPSWIZSETTINGSENTRY_H
