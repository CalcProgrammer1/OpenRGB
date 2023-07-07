#ifndef OPENRGBZONESBULKRESIZER_H
#define OPENRGBZONESBULKRESIZER_H

#include "ui_OpenRGBZonesBulkResizer.h"
#include "RGBController.h"
#include <QWidget>

namespace Ui {
class OpenRGBZonesBulkResizer;
}

class Ui::OpenRGBZonesBulkResizer : public QWidget
{
    Q_OBJECT

public:
    static void RunChecks(QWidget *parent);

    explicit OpenRGBZonesBulkResizer(QWidget*, const std::vector<std::tuple<RGBController*, unsigned int>>&);
    ~OpenRGBZonesBulkResizer();

private:
    Ui::OpenRGBZonesBulkResizerUi *ui;

    const std::vector<std::tuple<RGBController*, unsigned int>>& unconfigured_zones;

    void CreateZoneWidget(RGBController*, unsigned int);

    void SaveDoNotRunState();

signals:
    void CloseRequest();

private slots:
    void changeEvent(QEvent *event);
    void on_save_button_clicked();
    void on_ignore_button_clicked();

};

#endif // OPENRGBZONESBULKRESIZER_H
