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

#include "include/Utils.h"

#include <iostream>

namespace utils
{
    bool validateReplyForLight(const nlohmann::json& request, const nlohmann::json& reply, int lightId)
    {
        bool success = false;
        std::string path = "/lights/" + std::to_string(lightId) + "/state/";
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
                    if (successPath.find(path) == 0)
                    {
                        const std::string valueKey = successPath.substr(path.size());
                        auto requestIt = request.find(valueKey);
                        success = requestIt != request.end();
                        if (success)
                        {
                            if (valueKey == "xy")
                            {
                                success
                                    = std::abs(requestIt.value()[0].get<float>() - successIt.value()[0].get<float>())
                                        <= 1E-4f
                                    && std::abs(requestIt.value()[1].get<float>() - successIt.value()[1].get<float>())
                                        <= 1E-4f;
                            }
                            else
                            {
                                success = requestIt.value() == successIt.value();
                            }
                            if (!success)
                            {
                                std::cout << "Value " << requestIt.value() << " does not match reply "
                                          << successIt.value() << std::endl;
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
} // namespace utils