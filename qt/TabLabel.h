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
    TabLabel(QString icon, QString name, char* original, char* context);
    ~TabLabel();

private:
    Ui::TabLabelUi *ui;
    char* label;
    char* ctxt;

private slots:
    void changeEvent(QEvent *event);
};

#endif // TABLABEL_H
