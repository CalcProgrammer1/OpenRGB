/*---------------------------------------------------------*\
| DeviceView.h                                              |
|                                                           |
|   OpenRGB Device view widget for Qt                       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "RGBController.h"

typedef struct
{
    float matrix_x;
    float matrix_y;
    float matrix_w;
    float matrix_h;
} matrix_pos_size_type;

class DeviceView : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceView(QWidget *parent = 0);
    ~DeviceView();

    virtual QSize sizeHint () const;
    virtual QSize minimumSizeHint () const;

    void setController(RGBController * controller_ptr);
    void setNumericalLabels(bool enable);
    void setPerLED(bool per_led_mode);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *);

private:
    QSize initSize;
    bool mouseDown;
    bool ctrlDown;
    bool mouseMoved;
    int size;
    int offset_x;
    QRect selectionRect;
    QPoint lastMousePos;
    QVector<int> previousSelection;
    QVector<int> selectedLeds;
    QVector<bool> selectionFlags;
    QVector<bool> previousFlags;
    bool per_led;

    std::vector<matrix_pos_size_type>   zone_pos;
    std::vector<matrix_pos_size_type>   segment_pos;
    std::vector<matrix_pos_size_type>   led_pos;
    std::vector<QString>                led_labels;

    float                               matrix_h;

    bool                                numerical_labels;

    RGBController* controller;

    QColor posColor(const QPoint &point);
    void InitDeviceView();
    void updateSelection();

signals:
    void selectionChanged(QVector<int>);

public slots:
    bool selectLed(int);
    bool selectLeds(QVector<int>);
    bool selectSegment(int zone, int segment, bool add = false);
    bool selectZone(int zone, bool add = false);
    void clearSelection(); // Same as selecting the entire device
    void setSelectionColor(RGBColor);
};
