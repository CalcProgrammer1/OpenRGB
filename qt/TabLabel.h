#ifndef TABLABEL_H
#define TABLABEL_H

#include <QWidget>
#include "ui_TabLabel.h"

namespace Ui {
class TabLabel;
}

class Ui::TabLabel : public QWidget
{
    Q_OBJECT

public:
    TabLabel(QString icon, QString name);
    ~TabLabel();

private:
    Ui::TabLabelUi *ui;
};

#endif // TABLABEL_H
