#ifndef OPENRGBPHILIPSHUESETTINGSENTRY_H
#define OPENRGBPHILIPSHUESETTINGSENTRY_H

#include "ui_OpenRGBPhilipsHueSettingsEntry.h"
#include <QWidget>

namespace Ui {
class OpenRGBPhilipsHueSettingsEntry;
}

class Ui::OpenRGBPhilipsHueSettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPhilipsHueSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBPhilipsHueSettingsEntry();
    Ui::OpenRGBPhilipsHueSettingsEntryUi *ui;
private slots:
    void changeEvent(QEvent *event);
    void on_UnpairButton_clicked();
};

#endif // OPENRGBPHILIPSHUESETTINGSENTRY_H
