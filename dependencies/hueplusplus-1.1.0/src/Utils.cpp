/**
    \file Utils.cpp
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall		- developer\n
    Copyright (C) 2020  Moritz Wirger	- developer\n

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

#include "hueplusplus/Utils.h"

#include <iostream>

namespace hueplusplus
{
namespace utils
{
bool validatePUTReply(const std::string& path, const nlohmann::json& request, const nlohmann::json& reply)
{
    std::string pathAppend = path;
    if (pathAppend.back() != '/')
    {
        pathAppend.push_back('/');
    }
    bool success = false;
    for (auto it = reply.begin(); it != reply.end(); ++it)
    {
        success = it.value().count("success");
        if (success)
        {
            // Traverse through first object
            nlohmann::json successObject = it.value()["success"];
            for (auto successIt = successObject.begin(); successIt != successObject.end(); ++successIt)
            {
                const std::string successPath = successIt.key();
                if (successPath.find(pathAppend) == 0)
                {
                    const std::string valueKey = successPath.substr(pathAppend.size());
                    auto requestIt = request.find(valueKey);
                    success = requestIt != request.end();
                    if (success)
                    {
                        if (valueKey == "xy")
                        {
                            success = std::abs(requestIt.value()[0].get<float>() - successIt.value()[0].get<float>())
                                    <= 1E-4f
                                && std::abs(requestIt.value()[1].get<float>() - successIt.value()[1].get<float>())
                                    <= 1E-4f;
                        }
                        else
                        {
                            success = requestIt.value() == successIt.value()
                                || (successIt.value().is_string() && successIt.value() == "Updated.");
                        }
                        if (!success)
                        {
                            std::cout << "Value " << requestIt.value() << " does not match reply " << successIt.value()
                                      << std::endl;
                        }
                    }
                }
                else
                {
                    success = false;
                }
            }
        }
        if (!success) // Fail fast
        {
            break;
        }
    }
    return success;
}

bool validateReplyForLight(const nlohmann::json& request, const nlohmann::json& reply, int lightId)
{
    return validatePUTReply("/lights/" + std::to_string(lightId) + "/state/", request, reply);
}
} // namespace utils
} // namespace hueplusplus
