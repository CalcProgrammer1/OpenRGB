/*---------------------------------------------------------*\
| AutoStart-MacOS.cpp                                       |
|                                                           |
|   Autostart implementation for MacOS                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <mach-o/dyld.h>
#include "AutoStart-MacOS.h"
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
        std::string     desktop_file                = GenerateLaunchAgentFile(autostart_info);
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
    char     exepath[ PATH_MAX ];
    uint32_t exesize = PATH_MAX;

    int ret_val = _NSGetExecutablePath(exepath, &exesize);

    return(std::string(exepath, (ret_val == 0) ? strlen(exepath) : 0));

    return("");
}

/*---------------------------------------------------------*\
| MacOS AutoStart Implementation                            |
| Private Methods                                           |
\*---------------------------------------------------------*/

std::string AutoStart::GenerateLaunchAgentFile(AutoStartInfo autostart_info)
{
    /*-----------------------------------------------------*\
    | Generate a .plist file from the AutoStart             |
    | parameters                                            |
    \*-----------------------------------------------------*/
    std::stringstream fileContents;

    fileContents << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"                                                                    << std::endl;
    fileContents << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"    << std::endl;
    fileContents << "<plist version=\"1.0\">"                                                                                       << std::endl;
    fileContents << "<dict>"                                                                                                        << std::endl;
    fileContents << "    <key>Label</key>"                                                                                          << std::endl;
    fileContents << "    <string>org.openrgb</string>"                                                                              << std::endl;
    fileContents << "    <key>ProgramArguments</key>"                                                                               << std::endl;
    fileContents << "    <array>"                                                                                                   << std::endl;
    fileContents << "        <string>" << autostart_info.path << "</string>"                                                        << std::endl;

    if(autostart_info.args != "")
    {
        std::istringstream arg_parser(autostart_info.args);
        std::string arg;

        while(arg_parser >> arg)
        {
            fileContents << "        <string>" << arg << "</string>"                                                                << std::endl;
        }
    }

    fileContents << "    </array>"                                                                                                  << std::endl;
    fileContents << "    <key>RunAtLoad</key><true/>"                                                                               << std::endl;
    fileContents << "</dict>"                                                                                                       << std::endl;
    fileContents << "</plist>"                                                                                                      << std::endl;

    return(fileContents.str());
}

void AutoStart::InitAutoStart(std::string name)
{
    std::string autostart_dir;

    autostart_name = name;

    /*-----------------------------------------------------*\
    | Determine where the autostart .desktop files are      |
    | kept                                                  |
    \*-----------------------------------------------------*/
    autostart_dir = getenv("HOME");
    autostart_dir = autostart_dir + "/Library/LaunchAgents/";

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
            autostart_file = autostart_dir + autostart_name + ".plist";
        }
    }
}

