#ifndef DEVICEVIEW_H
#define DEVICEVIEW_H

#include <QWidget>
#include "RGBController.h"

class DeviceView : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceView(QWidget *parent = 0);

    virtual QSize sizeHint () const;
    virtual QSize minimumSizeHint () const;
    
    void setController(RGBController * controller_ptr);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *);
private:
    QSize initSize;
    bool mouseDown;
    QPoint lastPos;

    RGBController* controller;

    QColor posColor(const QPoint &point);
};

#endif // DEVICEVIEW_H
