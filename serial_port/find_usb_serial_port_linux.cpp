/*---------------------------------------------------------*\
| find_usb_serial_port_linux.cpp                            |
|                                                           |
|   Finds the serial port path(s) of USB serial port devices|
|   given the USB VID and PID of the device                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "find_usb_serial_port.h"

#include <unistd.h>
#include <dirent.h>

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

std::vector<std::string *> find_usb_serial_port(unsigned short vid, unsigned short pid)
{
    std::vector<std::string *>  ret_vector;
    std::string *               tmp_string;
    DIR*                        dir;
    char                        symlink_path[1024]  = {0};
    struct dirent*              ent;
    char                        vid_pid[10]         = {0}; //Store VID/PID

    /*-----------------------------------------------------------------*\
    | Open /sys/class/tty                                               |
    \*-----------------------------------------------------------------*/
    dir = opendir("/sys/class/tty");

    if(dir == NULL)
    {
        return ret_vector;
    }

    /*-----------------------------------------------------------------*\
    | Loop through all symlinks in /sys/class/tty directory to find     |
    | paths with "usb" in them.  These links should have the USB device |
    | index which can be used to find the VID/PID                       |
    \*-----------------------------------------------------------------*/
    ent = readdir(dir);

    while(ent != NULL)
    {
        if(ent->d_type == DT_LNK)
        {
            char tty_path[1024];
            strcpy(tty_path, "/sys/class/tty/");
            strcat(tty_path, ent->d_name);

            /*-----------------------------------------------------------------*\
            | readlink() does not null-terminate, so manually terminate it      |
            \*-----------------------------------------------------------------*/
            ssize_t link_path_size = readlink(tty_path, symlink_path, 1024);
            symlink_path[link_path_size] = '\0';

            char * usb_string = strstr(symlink_path, "usb");

            if(usb_string != NULL)
            {
                char * usb_dev = strstr(usb_string, "/");
                usb_dev++;
                char * usb_end = strstr(usb_dev, "/tty");
                *usb_end = '\0';

                usb_end = strrchr(usb_dev, '/');
                *usb_end = '\0';

                char usb_path[1024];

                strcpy(usb_path, "/sys/bus/usb/devices/");
                strcat(usb_path, usb_dev);

                char vendor_path[1024];
                char product_path[1024];

                strcpy(vendor_path, usb_path);
                strcat(vendor_path, "/idVendor");

                strcpy(product_path, usb_path);
                strcat(product_path, "/idProduct");

                std::ifstream vendor_file;
                std::ifstream product_file;
                std::string vendor_string;
                std::string product_string;

                vendor_file.open(vendor_path);
                product_file.open(product_path);

                std::getline(vendor_file, vendor_string);
                std::getline(product_file, product_string);

                snprintf(vid_pid, 10, "%04x", vid);

                if(strcmp(vid_pid, vendor_string.c_str()) == 0)
                {
                    snprintf(vid_pid, 10, "%04x", pid);
                    if(strcmp(vid_pid, product_string.c_str()) == 0)
                    {
                        char* port_string = NULL;
                        for(int i = strlen(tty_path); i > 0; i--)
                        {
                            if(tty_path[i] == '/')
                            {
                                port_string = &tty_path[i + 1];
                                break;
                            }
                        }
                        tmp_string = new std::string("/dev/");
                        tmp_string->append(port_string);

                        ret_vector.push_back(tmp_string);
                    }
                }
            }
        }

        ent = readdir(dir);
    }


    closedir(dir);
    return ret_vector;

}   /* find_usb_serial_port() */
