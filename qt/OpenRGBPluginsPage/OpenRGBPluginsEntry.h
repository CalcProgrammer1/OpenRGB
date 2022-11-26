#ifndef OPENRGBPLUGINSENTRY_H
#define OPENRGBPLUGINSENTRY_H

#include "ui_OpenRGBPluginsEntry.h"
#include <QWidget>

namespace Ui
{
    class OpenRGBPluginsEntry;
}

typedef void (*EnableClickCallback)(void *, void *);

class Ui::OpenRGBPluginsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPluginsEntry(QWidget *parent = nullptr);
    ~OpenRGBPluginsEntry();
    Ui::OpenRGBPluginsEntryUi *ui;

    void RegisterEnableClickCallback(EnableClickCallback new_callback, void * new_callback_arg);

private slots:
    void changeEvent(QEvent *event);
    void on_EnabledCheckBox_stateChanged(int checked);

private:
    EnableClickCallback EnableClickCallbackVal;
    void *              EnableClickCallbackArg;
};

#endif // OPENRGBPLUGINSENTRY_H
