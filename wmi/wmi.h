/*---------------------------------------------------------*\
| wmi.h                                                     |
|                                                           |
|   WMI interface functionality                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <regex>

#include <comdef.h>
#include <Wbemidl.h>
#include <Windows.h>

#define WMI_WAIT_TIME 2000 /// Max time to wait for next object in when enumerating next object in wmi query in milliseconds

#pragma comment(lib, "wbemuuid.lib")


typedef std::map<std::string, std::string> QueryObj;
typedef std::map<std::string, std::regex> AdditionalFilters;
typedef std::pair<std::string, std::regex> AdditionalFilter;

// Wmi Base class
class Wmi
{
public:
    Wmi();
    ~Wmi();

    // Initialises connection to WMI host
    static HRESULT init();

    HRESULT query(std::string queryStr,
                  std::vector<QueryObj>& queryVectorOut,
                  const AdditionalFilters* filters = nullptr);

private:
    static IWbemLocator *pLoc;
    static IWbemServices *pSvc;
};
