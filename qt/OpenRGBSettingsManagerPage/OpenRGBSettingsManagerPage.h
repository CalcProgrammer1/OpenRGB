#ifndef OPENRGBSETTINGSMANAGERPAGE_H
#define OPENRGBSETTINGSMANAGERPAGE_H

#include "ui_OpenRGBSettingsManagerPage.h"
#include <QWidget>

#include "OpenRGBSettingsManagerEntry.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Ui {
class OpenRGBSettingsManagerPage;
}

class Ui::OpenRGBSettingsManagerPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBSettingsManagerPage(QWidget *parent = nullptr);
    ~OpenRGBSettingsManagerPage();

private slots:
    void changeEvent(QEvent *event);

private:
    Ui::OpenRGBSettingsManagerPageUi*           ui;
    std::vector<OpenRGBSettingsManagerEntry*>   entries;

};

#endif // OPENRGBSETTINGSMANAGERPAGE_H
