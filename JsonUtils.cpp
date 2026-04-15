/*---------------------------------------------------------*\
| JsonUtils.cpp                                             |
|                                                           |
|   JSON utility functions                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "JsonUtils.h"

bool JsonUtils::JsonGetBool(nlohmann::json& val, std::string key, bool dft)
{
    if((val.contains(key)) && (val[key].type() == nlohmann::json::value_t::boolean))
    {
        return((bool)val[key]);
    }
    else
    {
        return(dft);
    }
}

int JsonUtils::JsonGetInt(nlohmann::json& val, std::string key, int dft)
{
    if((val.contains(key)) &&
       ((val[key].type() == nlohmann::json::value_t::number_integer)  ||
        (val[key].type() == nlohmann::json::value_t::number_unsigned) ||
        (val[key].type() == nlohmann::json::value_t::number_float)))
    {
        return((int)val[key]);
    }
    else
    {
        return(dft);
    }
}

std::string JsonUtils::JsonGetString(nlohmann::json& val, std::string key, std::string dft, bool allow_empty)
{
    if((val.contains(key)) && (val[key].type() == nlohmann::json::value_t::string))
    {
        std::string ret_val = (std::string)val[key];

        if((!allow_empty) && (ret_val == ""))
        {
            return(dft);
        }
        else
        {
            return(ret_val);
        }
    }
    else
    {
        return(dft);
    }
}

bool JsonUtils::JsonParse(std::string json_string, nlohmann::json& json_ref)
{
    try
    {
        json_ref = nlohmann::json::parse(json_string);
        return(true);
    }
    catch(...)
    {
        return(false);
    }
}
