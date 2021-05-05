#ifndef SUPPORTEDDEVICESPAGE_H
#define SUPPORTEDDEVICESPAGE_H

#include "DetectorTableModel.h"
#include "ui_OpenRGBSupportedDevicesPage.h"
#include <QWidget>
#include <QSortFilterProxyModel>

namespace Ui {
class OpenRGBSupportedDevicesPage;
}

class Ui::OpenRGBSupportedDevicesPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBSupportedDevicesPage(QWidget *parent = nullptr);
    ~OpenRGBSupportedDevicesPage();
    DetectorTableModel* detectorTableModel;
    QSortFilterProxyModel* detectorSortModel;

private slots:
    void on_SaveButton_clicked();

    void on_OpenSettingsFolderButton_clicked();

    void on_Filter_textChanged(const QString &arg1);

    void on_ToggleAllCheckbox_toggled(const bool checked);

private:
    Ui::OpenRGBSupportedDevicesPageUi *ui;
};

#endif // SUPPORTEDDEVICESPAGE_H
