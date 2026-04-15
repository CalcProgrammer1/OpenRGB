/*---------------------------------------------------------*\
| DeviceView.h                                              |
|                                                           |
|   OpenRGB Device view widget for Qt                       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "RGBController.h"

typedef struct
{
    float                               matrix_x;
    float                               matrix_y;
    float                               matrix_w;
    float                               matrix_h;
} matrix_pos_size_type;

class DeviceView : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceView(QWidget *parent = 0);
    ~DeviceView();

    /*-----------------------------------------------------*\
    | Qt size hints                                         |
    \*-----------------------------------------------------*/
    virtual QSize   minimumSizeHint()               const;
    virtual QSize   sizeHint()                      const;

    /*-----------------------------------------------------*\
    | Selection functions                                   |
    \*-----------------------------------------------------*/
    bool            SelectLED(std::size_t led_idx);
    bool            SelectLEDs(std::vector<std::size_t> leds);
    bool            SelectSegment(std::size_t zone_idx, std::size_t segment_idx, bool add = false);
    bool            SelectZone(std::size_t zone_idx, bool add = false);

    /*-----------------------------------------------------*\
    | Setter functions                                      |
    \*-----------------------------------------------------*/
    void            ClearSelection();
    void            SetChanged();
    void            SetController(RGBController * controller_ptr);
    void            SetDisableKeyExpansion(bool disable_key_expansion);
    void            SetNumericalLabels(bool enable);
    void            SetPerLED(bool per_led_mode);
    void            SetSelectionColor(RGBColor);

protected:
    /*-----------------------------------------------------*\
    | Qt events                                             |
    \*-----------------------------------------------------*/
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *);

private:
    /*-----------------------------------------------------*\
    | State tracking variables                              |
    \*-----------------------------------------------------*/
    bool                                changed;
    bool                                ctrl_down;
    bool                                disable_expansion;
    QPoint                              last_mouse_point;
    bool                                mouse_down;
    bool                                mouse_moved;
    bool                                numerical_labels;
    bool                                per_led;

    /*-----------------------------------------------------*\
    | Size tracking variables                               |
    \*-----------------------------------------------------*/
    QSize                               init_size;
    float                               matrix_h;
    int                                 offset_x;
    int                                 size;

    /*-----------------------------------------------------*\
    | Selection tracking variables                          |
    \*-----------------------------------------------------*/
    std::vector<std::size_t>            previous_selection;
    std::vector<std::size_t>            selected_leds;
    QRect                               selection_rect;
    std::vector<bool>                   selection_flags;
    std::vector<bool>                   previous_flags;

    /*-----------------------------------------------------*\
    | UI element tracking variables                         |
    \*-----------------------------------------------------*/
    std::vector<QString>                led_labels;
    std::vector<matrix_pos_size_type>   led_pos;
    std::vector<matrix_pos_size_type>   segment_pos;
    std::vector<matrix_pos_size_type>   zone_pos;

    /*-----------------------------------------------------*\
    | Pointer to attached RGBController                     |
    \*-----------------------------------------------------*/
    RGBController*                      controller;

    /*-----------------------------------------------------*\
    | Private functions                                     |
    \*-----------------------------------------------------*/
    void InitDeviceView();
    void UpdateSelection();

signals:
    /*-----------------------------------------------------*\
    | Signals                                               |
    \*-----------------------------------------------------*/
    void selectionChanged(int selected_zone, int selected_segment, std::vector<std::size_t>);

};
