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

std::string exec(const char* cmd)
{
    char        buffer[128];
    std::string result      = "";
    FILE*       pipe        = popen(cmd, "r");

    if(!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    try
    {
        while(fgets(buffer, sizeof(buffer), pipe) != NULL)
        {
            result += buffer;
        }
    }
    catch(...)
    {
        pclose(pipe);
        throw;
    }

    pclose(pipe);

    return result;
}

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
    /*-----------------------------------------------------*\
    | Strings to search for in ioreg output                 |
    \*-----------------------------------------------------*/
    #define IO_CALLOUT_STR  "\"IOCalloutDevice\" ="
    #define ID_VENDOR_STR   "\"idVendor\" = "
    #define ID_PRODUCT_STR  "\"idProduct\" = "

    /*-----------------------------------------------------*\
    | Return variables                                      |
    \*-----------------------------------------------------*/
    std::vector<std::string *>  ret_vector;
    std::string *               tmp_string;

    /*-----------------------------------------------------*\
    | Execute command to list USB devices                   |
    |                                                       |
    | Top level entry lines in ioreg output start with      |
    | "+-o".  Start the string with an extra newline so     |
    | that we can search for "\n+-0" to identify only hits  |
    | at the beginning of a line.                           |
    \*-----------------------------------------------------*/
    std::string out_string = "\n" + exec("ioreg -r -c IOUSBHostDevice -l");

    /*-----------------------------------------------------*\
    | Append desired VID and PID to search strings          |
    \*-----------------------------------------------------*/
    std::string vid_string = ID_VENDOR_STR + std::to_string(vid);
    std::string pid_string = ID_PRODUCT_STR + std::to_string(pid);

    /*-----------------------------------------------------*\
    | Start position counter at 0                           |
    \*-----------------------------------------------------*/
    std::size_t pos = 0;

    /*-----------------------------------------------------*\
    | Loop through ioreg output, loop exits when "\n+-o"    |
    | string cannot be found.                               |
    \*-----------------------------------------------------*/
    while(1)
    {
        /*-------------------------------------------------*\
        | Variables to store positions in string            |
        \*-------------------------------------------------*/
        std::size_t next_pos;
        std::size_t vid_pos;
        std::size_t pid_pos;

        /*-------------------------------------------------*\
        | Search for the next 2 iterations of "\n+-o" so    |
        | that we can check if hits are in between them     |
        \*-------------------------------------------------*/
        pos         = out_string.find("\n+-o", pos);
        next_pos    = out_string.find("\n+-o", pos + 1);

        /*-------------------------------------------------*\
        | Search for the vendor and product ID strings in   |
        | and verify that they are between pos and next_pos |
        \*-------------------------------------------------*/
        vid_pos     = out_string.find(vid_string, pos);
        pid_pos     = out_string.find(pid_string, pos);

        /*-------------------------------------------------*\
        | Verify that VID/PID matches are within this       |
        | device block by checking that their positions are |
        | less than next_pos.  If next_pos is invalid,      |
        | this is the last block, in which case check if    |
        | VID/PID positions are valid.                      |
        \*-------------------------------------------------*/
        if(((vid_pos < next_pos) && (pid_pos < next_pos)) || ((pos == std::string::npos) && (vid_pos != std::string::npos) && (pid_pos != std::string::npos)))
        {
            /*---------------------------------------------*\
            | Variables to store positions in string        |
            \*---------------------------------------------*/
            std::size_t dev_pos;
            std::size_t start_pos;
            std::size_t end_pos;

            /*---------------------------------------------*\
            | Look for the IO callout device tag and then   |
            | get the start and end positions of its value  |
            \*---------------------------------------------*/
            dev_pos     = out_string.find(IO_CALLOUT_STR, pos + 1);
            start_pos   = out_string.find("\"", dev_pos + sizeof(IO_CALLOUT_STR)) + 1;
            end_pos     = out_string.find("\"\n", start_pos);

            /*---------------------------------------------*\
            | Ensure the IO callout device tag is within    |
            | this device's section                         |
            \*---------------------------------------------*/
            if(dev_pos < next_pos)
            {
                tmp_string = new std::string(out_string.substr(start_pos, end_pos-start_pos));
                ret_vector.push_back(tmp_string);
            }
        }

        /*-------------------------------------------------*\
        | If we've reached the end of the string, break out |
        | of the loop                                       |
        \*-------------------------------------------------*/
        if(pos == std::string::npos)
        {
            break;
        }

        /*-------------------------------------------------*\
        | Increment position                                |
        \*-------------------------------------------------*/
        pos++;
    }

    /*-----------------------------------------------------*\
    | Return vector of detected strings                     |
    \*-----------------------------------------------------*/
    return(ret_vector);
}
