/**
    \file ModelPictures.cpp
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall		- developer\n

    This file is part of hueplusplus.

    hueplusplus is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    hueplusplus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with hueplusplus.  If not, see <http://www.gnu.org/licenses/>.
**/

#include <hueplusplus/ModelPictures.h>

namespace hueplusplus
{
std::string getPictureOfModel(const std::string& modelId)
{
    if (modelId == "LCT001" || modelId == "LCT007" || modelId == "LCT010" || modelId == "LCT014" || modelId == "LTW010"
        || modelId == "LTW001" || modelId == "LTW004" || modelId == "LTW015" || modelId == "LWB004"
        || modelId == "LWB006")
    {
        return "e27_waca";
    }
    else if (modelId == "LWB010" || modelId == "LWB014")
    {
        return "e27_white";
    }
    else if (modelId == "LCT012" || modelId == "LTW012")
    {
        return "e14";
    }
    else if (modelId == "LCT002")
    {
        return "br30";
    }
    else if (modelId == "LCT011" || modelId == "LTW011")
    {
        return "br30_slim";
    }
    else if (modelId == "LCT003")
    {
        return "gu10";
    }
    else if (modelId == "LTW013")
    {
        return "gu10_perfectfit";
    }
    else if (modelId == "LST001" || modelId == "LST002")
    {
        return "lightstrip";
    }
    else if (modelId == "LLC006 " || modelId == "LLC010")
    {
        return "iris";
    }
    else if (modelId == "LLC005" || modelId == "LLC011" || modelId == "LLC012" || modelId == "LLC007")
    {
        return "bloom";
    }
    else if (modelId == "LLC014")
    {
        return "aura";
    }
    else if (modelId == "LLC013")
    {
        return "storylight";
    }
    else if (modelId == "LLC020")
    {
        return "go";
    }
    else if (modelId == "HBL001" || modelId == "HBL002" || modelId == "HBL003")
    {
        return "beyond_ceiling_pendant_table";
    }
    else if (modelId == "HIL001 " || modelId == "HIL002")
    {
        return "impulse";
    }
    else if (modelId == "HEL001 " || modelId == "HEL002")
    {
        return "entity";
    }
    else if (modelId == "HML001" || modelId == "HML002" || modelId == "HML003" || modelId == "HML004"
        || modelId == "HML005")
    {
        return "phoenix_ceiling_pendant_table_wall";
    }
    else if (modelId == "HML006")
    {
        return "phoenix_down";
    }
    else if (modelId == "LTP001" || modelId == "LTP002" || modelId == "LTP003" || modelId == "LTP004"
        || modelId == "LTP005" || modelId == "LTD003")
    {
        return "pendant";
    }
    else if (modelId == "LDF002" || modelId == "LTF001" || modelId == "LTF002" || modelId == "LTC001"
        || modelId == "LTC002" || modelId == "LTC003" || modelId == "LTC004" || modelId == "LTD001"
        || modelId == "LTD002" || modelId == "LDF001")
    {
        return "ceiling";
    }
    else if (modelId == "LDD002 " || modelId == "LFF001")
    {
        return "floor";
    }
    else if (modelId == "LDD001 " || modelId == "LTT001")
    {
        return "table";
    }
    else if (modelId == "LDT001 " || modelId == "MWM001")
    {
        return "recessed";
    }
    else if (modelId == "BSB001")
    {
        return "bridge_v1";
    }
    else if (modelId == "BSB002")
    {
        return "bridge_v2";
    }
    else if (modelId == "SWT001")
    {
        return "tap";
    }
    else if (modelId == "RWL021")
    {
        return "hds";
    }
    else if (modelId == "SML001")
    {
        return "motion_sensor";
    }
    return "";
}
} // namespace hueplusplus