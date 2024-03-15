#ifndef OPENRGBELGATOLIGHTSTRIPSETTINGSENTRY_H
#define OPENRGBELGATOLIGHTSTRIPSETTINGSENTRY_H

#include "ui_OpenRGBElgatoLightStripSettingsEntry.h"
#include <QDialog>

namespace Ui
{
    class OpenRGBElgatoLightStripSettingsEntry;
}

class Ui::OpenRGBElgatoLightStripSettingsEntry : public QDialog
{
    Q_OBJECT

    public:
        explicit OpenRGBElgatoLightStripSettingsEntry(QWidget *parent = nullptr);
        ~OpenRGBElgatoLightStripSettingsEntry();
        Ui::OpenRGBElgatoLightStripSettingsEntryUi *ui;

    private slots:
        void changeEvent(QEvent *event);
};

#endif // OPENRGBELGATOLIGHTSTRIPSETTINGSENTRY_H
