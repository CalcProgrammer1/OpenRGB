#ifndef OPENRGBELGATOKEYLIGHTSETTINGSPAGE_H
#define OPENRGBELGATOKEYLIGHTSETTINGSPAGE_H

#include "ui_OpenRGBElgatoKeyLightSettingsPage.h"
#include <QWidget>

#include "OpenRGBElgatoKeyLightSettingsEntry.h"

namespace Ui {
class OpenRGBElgatoKeyLightSettingsPage;
}

class Ui::OpenRGBElgatoKeyLightSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBElgatoKeyLightSettingsPage(QWidget *parent = nullptr);
    ~OpenRGBElgatoKeyLightSettingsPage();

private slots:
    void changeEvent(QEvent *event);
    void on_AddElgatoKeyLightDeviceButton_clicked();

    void on_RemoveElgatoKeyLightDeviceButton_clicked();

    void on_SaveElgatoKeyLightConfigurationButton_clicked();

private:
    Ui::OpenRGBElgatoKeyLightSettingsPageUi *ui;
    std::vector<OpenRGBElgatoKeyLightSettingsEntry*> entries;

};

#endif // OPENRGBELGATOKEYLIGHTSETTINGSPAGE_H
