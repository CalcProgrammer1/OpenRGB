/*---------------------------------------------------------*\
| AutoStart-Windows.cpp                                     |
|                                                           |
|   Autostart implementation for Windows                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <shlobj.h>
#include "AutoStart-Windows.h"
#include "LogManager.h"
#include "filesystem.h"
#include "windows.h"

AutoStart::AutoStart(std::string name)
{
    InitAutoStart(name);
}

bool AutoStart::DisableAutoStart()
{
    std::error_code autostart_file_remove_errcode;
    bool success                                    = false;

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
    bool success                                    = false;

    /*-----------------------------------------------------*\
    | Check if the filename is valid                        |
    \*-----------------------------------------------------*/
    if(autostart_file != "")
    {
        bool            weInitialised               = false;
        HRESULT         result;
        IShellLinkW*    shellLink                   = NULL;

        std::wstring    exepathw                    = utf8_decode(autostart_info.path);
        std::wstring    argumentsw                  = utf8_decode(autostart_info.args);
        std::wstring    startupfilepathw            = utf8_decode(autostart_file);
        std::wstring    descriptionw                = utf8_decode(autostart_info.desc);
        std::wstring    iconw                       = utf8_decode(autostart_info.path);

        result                                      = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLinkW, (void**)&shellLink);

        /*-------------------------------------------------*\
        | If not initialized, initialize                    |
        \*-------------------------------------------------*/
        if(result == CO_E_NOTINITIALIZED)
        {
            weInitialised                           = true;
            CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
            result                                  = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLinkW, (void**)&shellLink);
        }

        /*-------------------------------------------------*\
        | If successfully initialized, save a shortcut      |
        | from the AutoStart parameters                     |
        \*-------------------------------------------------*/
        if(SUCCEEDED(result))
        {
            shellLink->SetPath(exepathw.c_str());
            shellLink->SetArguments(argumentsw.c_str());
            shellLink->SetDescription(descriptionw.c_str());
            shellLink->SetIconLocation(iconw.c_str(), 0);

            IPersistFile* persistFile;

            result                                  = shellLink->QueryInterface(IID_IPersistFile, (void**)&persistFile);

            if(SUCCEEDED(result))
            {
                result                              = persistFile->Save(startupfilepathw.c_str(), TRUE);
                success                             = SUCCEEDED(result);
                persistFile->Release();
            }

            shellLink->Release();
        }

        /*-------------------------------------------------*\
        | Uninitialize when done                            |
        \*-------------------------------------------------*/
        if(weInitialised)
        {
            CoUninitialize();
        }
    }
    else
    {
        LOG_ERROR("[AutoStart] Could not establish correct autostart file path.");
    }

    return success;
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
    char exepath[MAX_PATH] = "";

    DWORD count = GetModuleFileNameA(NULL, exepath, MAX_PATH);

    return(std::string(exepath, (count > 0) ? count : 0));
}

/*---------------------------------------------------------*\
| Windows AutoStart Implementation                          |
| Private Methods                                           |
\*---------------------------------------------------------*/

void AutoStart::InitAutoStart(std::string name)
{
    char startMenuPath[MAX_PATH];

    autostart_name = name;

    /*-----------------------------------------------------*\
    | Get startup applications path                         |
    \*-----------------------------------------------------*/
    HRESULT result = SHGetFolderPathA(NULL, CSIDL_PROGRAMS, NULL, 0, startMenuPath);

    if(SUCCEEDED(result))
    {
        autostart_file = std::string(startMenuPath);

        autostart_file += "\\Startup\\" + autostart_name + ".lnk";
    }
    else
    {
        autostart_file.clear();
    }
}

/*---------------------------------------------------------*\
| Convert an UTF8 string to a wide Unicode String           |
| (from wmi.cpp)                                            |
\*---------------------------------------------------------*/
std::wstring AutoStart::utf8_decode(const std::string& str)
{
    if(str.empty())
    {
        return std::wstring();
    }

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), nullptr, 0);

    std::wstring wstrTo(size_needed, 0);

    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), &wstrTo[0], size_needed);

    return(wstrTo);
}
