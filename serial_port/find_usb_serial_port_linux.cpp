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
    char                        target_vid[10]      = {0};
    char                        target_pid[10]      = {0};

    /*-----------------------------------------------------------------*\
    | Target VID/PID strings                                            |
    \*-----------------------------------------------------------------*/
    snprintf(target_vid, 10, "%04x", vid);
    snprintf(target_pid, 10, "%04x", pid);

    /*-----------------------------------------------------------------*\
    | Open /sys/class/tty                                               |
    \*-----------------------------------------------------------------*/
    //dir = opendir("/sys/class/tty");
    dir = opendir("/sys/bus/usb/devices");

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
            char usb_path[1024];
            strcpy(usb_path, "/sys/bus/usb/devices/");
            strcat(usb_path, ent->d_name);

            /*-----------------------------------------------------------------*\
            | readlink() does not null-terminate, so manually terminate it      |
            \*-----------------------------------------------------------------*/
            ssize_t link_path_size = readlink(usb_path, symlink_path, 1024);
            symlink_path[link_path_size] = '\0';

            if(1)
            {
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

                if(strcmp(target_vid, vendor_string.c_str()) == 0)
                {
                    if(strcmp(target_pid, product_string.c_str()) == 0)
                    {
                        DIR*            usb_dev_dir;
                        struct dirent*  usb_dev_ent;

                        usb_dev_dir = opendir(usb_path);

                        usb_dev_ent = readdir(usb_dev_dir);

                        while(usb_dev_ent != NULL)
                        {
                            if(usb_dev_ent->d_type == DT_DIR)
                            {
                                char tty_path[1024];

                                strcpy(tty_path, usb_path);
                                strcat(tty_path, "/");
                                strcat(tty_path, usb_dev_ent->d_name);
                                strcat(tty_path, "/tty");

                                DIR* tty_dir;

                                tty_dir = opendir(tty_path);

                                if(tty_dir != NULL)
                                {
                                    struct dirent* tty_ent;

                                    tty_ent = readdir(tty_dir);

                                    while(tty_ent != NULL)
                                    {
                                        if(tty_ent->d_type == DT_DIR)
                                        {
                                            if(strncmp(tty_ent->d_name, "tty", 3) == 0)
                                            {
                                                tmp_string = new std::string("/dev/" + std::string(tty_ent->d_name));

                                                ret_vector.push_back(tmp_string);
                                            }
                                        }

                                        tty_ent = readdir(tty_dir);
                                    }
                                }
                            }

                            usb_dev_ent = readdir(usb_dev_dir);
                        }
                    }
                }
            }
        }

        ent = readdir(dir);
    }

    closedir(dir);
    return ret_vector;

    // /*-----------------------------------------------------------------*\
    // | Loop through all symlinks in /sys/class/tty directory to find     |
    // | paths with "usb" in them.  These links should have the USB device |
    // | index which can be used to find the VID/PID                       |
    // \*-----------------------------------------------------------------*/
    // ent = readdir(dir);

    // while(ent != NULL)
    // {
    //     if(ent->d_type == DT_LNK)
    //     {
    //         char tty_path[1024];
    //         strcpy(tty_path, "/sys/class/tty/");
    //         strcat(tty_path, ent->d_name);

    //         /*-----------------------------------------------------------------*\
    //         | readlink() does not null-terminate, so manually terminate it      |
    //         \*-----------------------------------------------------------------*/
    //         ssize_t link_path_size = readlink(tty_path, symlink_path, 1024);
    //         symlink_path[link_path_size] = '\0';

    //         char * usb_string = strstr(symlink_path, "usb");

    //         if(usb_string != NULL)
    //         {
    //             char * usb_dev = strstr(usb_string, "/");
    //             usb_dev++;
    //             char * usb_end = strstr(usb_dev, "/tty");
    //             *usb_end = '\0';

    //             usb_end = strrchr(usb_dev, '/');
    //             *usb_end = '\0';

    //             char usb_path[1024];

    //             strcpy(usb_path, "/sys/bus/usb/devices/");
    //             strcat(usb_path, usb_dev);

    //             char vendor_path[1024];
    //             char product_path[1024];

    //             strcpy(vendor_path, usb_path);
    //             strcat(vendor_path, "/idVendor");

    //             strcpy(product_path, usb_path);
    //             strcat(product_path, "/idProduct");

    //             std::ifstream vendor_file;
    //             std::ifstream product_file;
    //             std::string vendor_string;
    //             std::string product_string;

    //             vendor_file.open(vendor_path);
    //             product_file.open(product_path);

    //             std::getline(vendor_file, vendor_string);
    //             std::getline(product_file, product_string);

    //             snprintf(vid_pid, 10, "%04x", vid);

    //             if(strcmp(vid_pid, vendor_string.c_str()) == 0)
    //             {
    //                 snprintf(vid_pid, 10, "%04x", pid);
    //                 if(strcmp(vid_pid, product_string.c_str()) == 0)
    //                 {
    //                     char* port_string = NULL;
    //                     for(int i = strlen(tty_path); i > 0; i--)
    //                     {
    //                         if(tty_path[i] == '/')
    //                         {
    //                             port_string = &tty_path[i + 1];
    //                             break;
    //                         }
    //                     }
    //                     tmp_string = new std::string("/dev/");
    //                     tmp_string->append(port_string);

    //                     ret_vector.push_back(tmp_string);
    //                 }
    //             }
    //         }
    //     }

    //     ent = readdir(dir);
    // }


    closedir(dir);
    return ret_vector;

}   /* find_usb_serial_port() */
