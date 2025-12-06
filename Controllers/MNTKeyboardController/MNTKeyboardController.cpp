/*---------------------------------------------------------*\
| MNTKeyboardController.cpp                                 |
|                                                           |
|   Driver for the MNT Reform keyboards                     |
|                                                           |
|   Christian Heller <c.heller@plomlompom.de>  7 Aug 2024   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "MNTKeyboardController.h"

MNTKeyboardController::~MNTKeyboardController()
{
    hid_close(dev);
}

void MNTKeyboardController::SendColorMatrix(unsigned char *color_map)
{
    unsigned char row_size    = kbd_cols * KBD_COLOR_SIZE;
    unsigned char cmdbuf_size = CMD_OFFSET + row_size;
    unsigned char *usb_buf    = new unsigned char[cmdbuf_size];
    memcpy(usb_buf, CMD_PREFIX, CMD_PREFIX_LEN);
    for(unsigned int row_idx = 0; row_idx < KBD_ROWS; row_idx++)
    {
        usb_buf[CMD_PREFIX_LEN] = row_idx;
        memcpy(usb_buf + CMD_OFFSET, color_map + row_idx * row_size, row_size);
        hid_write(dev, usb_buf, cmdbuf_size);
    }
    delete[] usb_buf;
}
