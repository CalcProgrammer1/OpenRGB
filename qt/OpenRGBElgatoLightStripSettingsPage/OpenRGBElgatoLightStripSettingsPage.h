#ifndef OPENRGBELGATOLIGHTSTRIPSETTINGSPAGE_H
#define OPENRGBELGATOLIGHTSTRIPSETTINGSPAGE_H

#include "ui_OpenRGBElgatoLightStripSettingsPage.h"
#include <QWidget>
#include "OpenRGBElgatoLightStripSettingsEntry.h"

namespace Ui
{
    class OpenRGBElgatoLightStripSettingsPage;
}

class Ui::OpenRGBElgatoLightStripSettingsPage : public QWidget
{
    Q_OBJECT

    public:
        explicit OpenRGBElgatoLightStripSettingsPage(QWidget *parent = nullptr);
        ~OpenRGBElgatoLightStripSettingsPage();

    private slots:
        void changeEvent(QEvent *event);
        void on_AddElgatoLightStripDeviceButton_clicked();

        void on_RemoveElgatoLightStripDeviceButton_clicked();

        void on_SaveElgatoLightStripConfigurationButton_clicked();

    private:
        Ui::OpenRGBElgatoLightStripSettingsPageUi *ui;
        std::vector<OpenRGBElgatoLightStripSettingsEntry*> entries;
};

#endif // OPENRGBELGATOLIGHTSTRIPSETTINGSPAGE_H
