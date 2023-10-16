#ifndef OPENRGBSETTINGSMANAGERENTRY_H
#define OPENRGBSETTINGSMANAGERENTRY_H

#include "ui_OpenRGBSettingsManagerEntry.h"
#include <QWidget>
#include <QSignalMapper>
#include "ResourceManager.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Ui {
class OpenRGBSettingsManagerEntry;
}

class Ui::OpenRGBSettingsManagerEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBSettingsManagerEntry(std::string settings_key, json settings_proto, QWidget *parent = nullptr);
    ~OpenRGBSettingsManagerEntry();
    Ui::OpenRGBSettingsManagerEntryUi *ui;

private slots:
    void changeEvent(QEvent *event);
    void onSettingChanged(QObject * arg);

private:
    std::string     key;
    json            data;
    json            proto;
    QSignalMapper * mapper;

    QWidget * CreateWidget(std::string key, json & proto, json & data);
};

#endif // OPENRGBSETTINGSMANAGERENTRY_H
