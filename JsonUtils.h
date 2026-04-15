/*---------------------------------------------------------*\
| JsonUtils.h                                               |
|                                                           |
|   JSON utility functions                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "nlohmann/json.hpp"

class JsonUtils
{
public:
    static bool JsonGetBool(nlohmann::json& val, std::string key, bool dft = false);
    static int  JsonGetInt(nlohmann::json& val, std::string key, int dft = 0);
    static std::string JsonGetString(nlohmann::json& val, std::string key, std::string dft = "", bool allow_empty = true);
    static bool JsonParse(std::string json_string, nlohmann::json& json_ref);
};
