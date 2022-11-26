#ifndef OPENRGBELGATOKEYLIGHTSETTINGSENTRY_H
#define OPENRGBELGATOKEYLIGHTSETTINGSENTRY_H

#include "ui_OpenRGBElgatoKeyLightSettingsEntry.h"
#include <QDialog>

namespace Ui {
class OpenRGBElgatoKeyLightSettingsEntry;
}

class Ui::OpenRGBElgatoKeyLightSettingsEntry : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBElgatoKeyLightSettingsEntry(QWidget *parent = nullptr);
    ~OpenRGBElgatoKeyLightSettingsEntry();
    Ui::OpenRGBElgatoKeyLightSettingsEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
};

#endif // OPENRGBELGATOKEYLIGHTSETTINGSENTRY_H
