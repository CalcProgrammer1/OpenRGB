/**
    \file HueException.cpp
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

#include "hueplusplus/HueException.h"

namespace hueplusplus
{
HueException::HueException(FileInfo fileInfo, const std::string& message)
    : HueException("HueException", std::move(fileInfo), message)
{}

const char* HueException::what() const noexcept
{
    return whatMessage.c_str();
}

const FileInfo& HueException::GetFile() const noexcept
{
    return fileInfo;
}

HueException::HueException(const char* exceptionName, FileInfo fileInfo, const std::string& message)
    : fileInfo(std::move(fileInfo))
{
    whatMessage = exceptionName;
    whatMessage.append(" from ");
    whatMessage.append(this->fileInfo.ToString());
    whatMessage.append(" ");
    whatMessage.append(message);
}

HueAPIResponseException::HueAPIResponseException(
    FileInfo fileInfo, int error, std::string address, std::string description)
    : HueException("HueApiResponseException", std::move(fileInfo), GetMessage(error, address, description)),
      error(error),
      address(std::move(address)),
      description(std::move(description))
{}

int HueAPIResponseException::GetErrorNumber() const noexcept
{
    return error;
}

const std::string& HueAPIResponseException::GetAddress() const noexcept
{
    return address;
}

const std::string& HueAPIResponseException::GetDescription() const noexcept
{
    return description;
}

HueAPIResponseException HueAPIResponseException::Create(FileInfo fileInfo, const nlohmann::json& response)
{
    const nlohmann::json error = response.at("error");
    int errorCode = -1;
    if (error.count("type"))
    {
        if (error["type"].is_number_integer())
        {
            errorCode = error["type"].get<int>();
        }
        else if (error["type"].is_string())
        {
            errorCode = std::stoi(error["type"].get<std::string>());
        }
    }
    std::string address = error.value("address", "");
    std::string description = error.value("description", "");
    return HueAPIResponseException(std::move(fileInfo), errorCode, std::move(address), std::move(description));
}

std::string HueAPIResponseException::GetMessage(int error, const std::string& addr, const std::string& description)
{
    std::string result = std::to_string(error);
    result.append(" ");
    result.append(addr);
    result.append(" ");
    result.append(description);
    return result;
}

std::string FileInfo::ToString() const
{
    if (filename.empty() || line < 0)
    {
        return "Unknown file";
    }
    std::string result = func;
    result.append(" in ");
    result.append(filename);
    result.append(":");
    result.append(std::to_string(line));
    return result;
}
} // namespace hueplusplus
