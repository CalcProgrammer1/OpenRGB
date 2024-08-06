/*---------------------------------------------------------*\
| find_usb_serial_port_macos.cpp                            |
|                                                           |
|   Finds the serial port path(s) of USB serial port devices|
|   given the USB VID and PID of the device                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "find_usb_serial_port.h"

/*---------------------------------------------------------------------*\
|                                                                       |
|   find_usb_serial_port                                                |
|                                                                       |
|   This function returns the name of the first USB serial port matching|
|   the given USB product and vendor ID.                                |
|                                                                       |
|   vid: Vendor ID code                                                 |
|   pid: Product ID code                                                |
|                                                                       |
|   returns: std::string containing port name "COMx" or "/dev/ttyX"     |
|                                                                       |
\*---------------------------------------------------------------------*/

std::vector<std::string *> find_usb_serial_port(unsigned short /*vid*/, unsigned short /*pid*/)
{
    std::vector<std::string *>  ret_vector;

    /*-----------------------------------------------------------------*\
    | TODO: Implement for MacOS                                         |
    \*-----------------------------------------------------------------*/
    return(ret_vector);
}
