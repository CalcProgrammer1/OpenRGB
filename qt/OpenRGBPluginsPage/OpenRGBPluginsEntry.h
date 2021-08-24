#ifndef OPENRGBPLUGINSENTRY_H
#define OPENRGBPLUGINSENTRY_H

#include "ui_OpenRGBPluginsEntry.h"
#include <QWidget>

namespace Ui {
class OpenRGBPluginsEntry;
}

class Ui::OpenRGBPluginsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPluginsEntry(QWidget *parent = nullptr);
    ~OpenRGBPluginsEntry();
    Ui::OpenRGBPluginsEntryUi *ui;
};

#endif // OPENRGBPLUGINSENTRY_H
