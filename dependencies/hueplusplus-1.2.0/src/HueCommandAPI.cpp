/**
    \file HueCommandAPI.h
    Copyright Notice\n
    Copyright (C) 2018  Jan Rogall		- developer\n
    Copyright (C) 2018  Moritz Wirger	- developer\n

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

#include "hueplusplus/HueCommandAPI.h"

#include <thread>

#include "hueplusplus/LibConfig.h"
#include "hueplusplus/HueExceptionMacro.h"

namespace hueplusplus
{
namespace
{
// Runs functor with appropriate timeout and retries when timed out or connection reset
template <typename Timeout, typename Fun>
nlohmann::json RunWithTimeout(std::shared_ptr<Timeout> timeout, std::chrono::steady_clock::duration minDelay, Fun fun)
{
    auto now = std::chrono::steady_clock::now();
    std::lock_guard<std::mutex> lock(timeout->mutex);
    if (timeout->timeout > now)
    {
        std::this_thread::sleep_until(timeout->timeout);
    }
    try
    {
        nlohmann::json response = fun();
        timeout->timeout = now + minDelay;
        return response;
    }
    catch (const std::system_error& e)
    {
        if (e.code() == std::errc::connection_reset || e.code() == std::errc::timed_out)
        {
            // Happens when hue is too busy, wait and try again (once)
            std::this_thread::sleep_for(minDelay);
            nlohmann::json v = fun();
            timeout->timeout = std::chrono::steady_clock::now() + minDelay;
            return v;
        }
        // Cannot recover from other types of errors
        throw;
    }
}
} // namespace

HueCommandAPI::HueCommandAPI(
    const std::string& ip, const int port, const std::string& username, std::shared_ptr<const IHttpHandler> httpHandler)
    : ip(ip),
      port(port),
      username(username),
      httpHandler(std::move(httpHandler)),
      timeout(new TimeoutData {std::chrono::steady_clock::now(), {}})
{}

nlohmann::json HueCommandAPI::PUTRequest(const std::string& path, const nlohmann::json& request) const
{
    return PUTRequest(path, request, CURRENT_FILE_INFO);
}

nlohmann::json HueCommandAPI::PUTRequest(
    const std::string& path, const nlohmann::json& request, FileInfo fileInfo) const
{
    return HandleError(std::move(fileInfo), RunWithTimeout(timeout, Config::instance().getBridgeRequestDelay(), [&]() {
        return httpHandler->PUTJson(combinedPath(path), request, ip, port);
    }));
}

nlohmann::json HueCommandAPI::GETRequest(const std::string& path, const nlohmann::json& request) const
{
    return GETRequest(path, request, CURRENT_FILE_INFO);
}

nlohmann::json HueCommandAPI::GETRequest(
    const std::string& path, const nlohmann::json& request, FileInfo fileInfo) const
{
    return HandleError(std::move(fileInfo), RunWithTimeout(timeout, Config::instance().getBridgeRequestDelay(), [&]() {
        return httpHandler->GETJson(combinedPath(path), request, ip, port);
    }));
}

nlohmann::json HueCommandAPI::DELETERequest(const std::string& path, const nlohmann::json& request) const
{
    return DELETERequest(path, request, CURRENT_FILE_INFO);
}

nlohmann::json HueCommandAPI::DELETERequest(
    const std::string& path, const nlohmann::json& request, FileInfo fileInfo) const
{
    return HandleError(std::move(fileInfo), RunWithTimeout(timeout, Config::instance().getBridgeRequestDelay(), [&]() {
        return httpHandler->DELETEJson(combinedPath(path), request, ip, port);
    }));
}

nlohmann::json HueCommandAPI::POSTRequest(const std::string& path, const nlohmann::json& request) const
{
    return POSTRequest(path, request, CURRENT_FILE_INFO);
}

nlohmann::json HueCommandAPI::POSTRequest(
    const std::string& path, const nlohmann::json& request, FileInfo fileInfo) const
{
    return HandleError(std::move(fileInfo), RunWithTimeout(timeout, Config::instance().getBridgeRequestDelay(), [&]() {
        return httpHandler->POSTJson(combinedPath(path), request, ip, port);
    }));
}

nlohmann::json HueCommandAPI::HandleError(FileInfo fileInfo, const nlohmann::json& response) const
{
    if (response.count("error"))
    {
        throw HueAPIResponseException::Create(std::move(fileInfo), response);
    }
    else if (response.is_array())
    {
        // Check if array contains error response
        auto it
            = std::find_if(response.begin(), response.end(), [](const nlohmann::json& v) { return v.count("error"); });
        if (it != response.end())
        {
            throw HueAPIResponseException::Create(std::move(fileInfo), it.value());
        }
    }
    return response;
}

std::string HueCommandAPI::combinedPath(const std::string& path) const
{
    std::string result = "/api/";
    result.append(username);
    // If path does not begin with '/', insert it unless it is empty
    if (!path.empty() && path.front() != '/')
    {
        result.append("/");
    }
    result.append(path);
    return result;
}
} // namespace hueplusplus
