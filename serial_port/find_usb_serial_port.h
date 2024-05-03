/*---------------------------------------------------------*\
| find_usb_serial_port.h                                    |
|                                                           |
|   Finds the serial port path(s) of USB serial port devices|
|   given the USB VID and PID of the device                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string *> find_usb_serial_port(unsigned short vid, unsigned short pid);
