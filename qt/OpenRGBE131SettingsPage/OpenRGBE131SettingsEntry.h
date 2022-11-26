#ifndef OPENRGBE131SETTINGSENTRY_H
#define OPENRGBE131SETTINGSENTRY_H

#include "ui_OpenRGBE131SettingsEntry.h"
#include <QWidget>

namespace Ui {
class OpenRGBE131SettingsEntry;
}

class Ui::OpenRGBE131SettingsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBE131SettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBE131SettingsEntry();
    Ui::OpenRGBE131SettingsEntryUi *ui;

private:
    void HideMatrixSettings();
    void ShowMatrixSettings();

private slots:
    void changeEvent(QEvent *event);
    void on_TypeComboBox_currentIndexChanged(int index);
};

#endif // OPENRGBE131SETTINGSENTRY_H
