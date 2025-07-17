/*---------------------------------------------------------*\
| AutoStart-Linux.cpp                                       |
|                                                           |
|   Autostart implementation for Linux                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <linux/limits.h>
#include "AutoStart-Linux.h"
#include "LogManager.h"
#include "filesystem.h"

AutoStart::AutoStart(std::string name)
{
    InitAutoStart(name);
}

bool AutoStart::DisableAutoStart()
{
    std::error_code autostart_file_remove_errcode;
    bool            success                         = false;

    /*-----------------------------------------------------*\
    | Check if the filename is valid                        |
    \*-----------------------------------------------------*/
    if(autostart_file != "")
    {
        /*-------------------------------------------------*\
        | If file doesn't exist, disable is successful      |
        \*-------------------------------------------------*/
        if(!filesystem::exists(autostart_file))
        {
            success = true;
        }
        /*-------------------------------------------------*\
        | Otherwise, delete the file                        |
        \*-------------------------------------------------*/
        else
        {
            success = filesystem::remove(autostart_file, autostart_file_remove_errcode);

            if(!success)
            {
                LOG_ERROR("[AutoStart] An error occurred removing the auto start file.");
            }
        }
    }
    else
    {
        LOG_ERROR("[AutoStart] Could not establish correct autostart file path.");
    }

    return(success);
}

bool AutoStart::EnableAutoStart(AutoStartInfo autostart_info)
{
    bool        success                             = false;

    /*-----------------------------------------------------*\
    | Check if the filename is valid                        |
    \*-----------------------------------------------------*/
    if(autostart_file != "")
    {
        std::string     desktop_file                = GenerateDesktopFile(autostart_info);
        std::ofstream   autostart_file_stream(autostart_file, std::ios::out | std::ios::trunc);

        /*-------------------------------------------------*\
        | Error out if the file could not be opened         |
        \*-------------------------------------------------*/
        if(!autostart_file_stream)
        {
            LOG_ERROR("[AutoStart] Could not open %s for writing.", autostart_file.c_str());
            success = false;
        }
        /*-------------------------------------------------*\
        | Otherwise, write the file                         |
        \*-------------------------------------------------*/
        else
        {
            autostart_file_stream << desktop_file;
            autostart_file_stream.close();
            success = !autostart_file_stream.fail();

            if (!success)
            {
                LOG_ERROR("[AutoStart] An error occurred writing the auto start file.");
            }
        }
    }
    else
    {
        LOG_ERROR("[AutoStart] Could not establish correct autostart file path.");
    }

    return(success);
}

bool AutoStart::IsAutoStartEnabled()
{
    /*-----------------------------------------------------*\
    | Check if the filename is valid                        |
    \*-----------------------------------------------------*/
    if(autostart_file != "")
    {
        return(filesystem::exists(autostart_file));
    }
    else
    {
        return(false);
    }
}

std::string AutoStart::GetExePath()
{
    /*-----------------------------------------------------*\
    | Create the OpenRGB executable path                    |
    \*-----------------------------------------------------*/
    char exepath[ PATH_MAX ];

    ssize_t count = readlink("/proc/self/exe", exepath, PATH_MAX);

    return(std::string(exepath, (count > 0) ? count : 0));
}

/*---------------------------------------------------------*\
| Linux AutoStart Implementation                            |
| Private Methods                                           |
\*---------------------------------------------------------*/

std::string AutoStart::GenerateDesktopFile(AutoStartInfo autostart_info)
{
    /*-----------------------------------------------------*\
    | Generate a .desktop file from the AutoStart           |
    | parameters                                            |
    \*-----------------------------------------------------*/
    std::stringstream fileContents;

    fileContents << "[Desktop Entry]"                               << std::endl;
    fileContents << "Categories="        << autostart_info.category << std::endl;
    fileContents << "Comment="           << autostart_info.desc     << std::endl;
    fileContents << "Icon="              << autostart_info.icon     << std::endl;
    fileContents << "Name="              << GetAutoStartName()      << std::endl;
    fileContents << "StartupNotify=true"                            << std::endl;
    fileContents << "Terminal=false"                                << std::endl;
    fileContents << "Type=Application"                              << std::endl;

    /*-----------------------------------------------------*\
    | Add the executable path and arguments                 |
    \*-----------------------------------------------------*/
    fileContents << "Exec="              << autostart_info.path;

    if (autostart_info.args != "")
    {
        fileContents << " " << autostart_info.args;
    }

    fileContents << std::endl;

    return(fileContents.str());
}

void AutoStart::InitAutoStart(std::string name)
{
    std::string autostart_dir;

    autostart_name = name;

    /*-----------------------------------------------------*\
    | Get home and config paths                             |
    \*-----------------------------------------------------*/
    const char *xdg_config_home = getenv("XDG_CONFIG_HOME");
    const char *home = getenv("HOME");

    /*-----------------------------------------------------*\
    | Determine where the autostart .desktop files are      |
    | kept                                                  |
    \*-----------------------------------------------------*/
    if(xdg_config_home != NULL)
    {
        autostart_dir = xdg_config_home;
        autostart_dir = autostart_dir + "/autostart/";
    }
    else if(home != NULL)
    {
        autostart_dir = home;
        autostart_dir = autostart_dir + "/.config/autostart/";
    }

    /*-----------------------------------------------------*\
    | Check if the filename is valid                        |
    \*-----------------------------------------------------*/
    if(autostart_dir != "")
    {
        std::error_code ec;

        bool success = true;

        if(!filesystem::exists(autostart_dir))
        {
            success = filesystem::create_directories(autostart_dir, ec);
        }

        if(success)
        {
            autostart_file = autostart_dir + autostart_name + ".desktop";
        }
    }
}

