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
    QColor color();
    
    void setController(RGBController * controller_ptr);
signals:
    void colorChanged(const QColor color);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *);
private:
    QSize initSize;
    QImage wheelImage;
    QImage squareImage;
    QPixmap wheel;
    bool mouseDown;
    QPoint lastPos;
    int margin;
    int wheelWidth;
    QRegion wheelRegion;
    QRegion squareRegion;
    QColor current;
    bool inWheel;
    bool inSquare;
    int x_offset;
    int y_offset;

    RGBController* controller;

    QColor posColor(const QPoint &point);
};

#endif // DEVICEVIEW_H
