/**
    \file HueException.h
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

#ifndef INCLUDE_HUEPLUSPLUS_HUE_EXCEPTION_H
#define INCLUDE_HUEPLUSPLUS_HUE_EXCEPTION_H

#include <exception>
#include <string>

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! \brief Contains information about error location, use \ref CURRENT_FILE_INFO to create
struct FileInfo
{
    //! \brief Current file name from __FILE__. Empty if unknown
    std::string filename;
    //! \brief Current line number from __LINE__. -1 if unknown
    int line = -1;
    //! \brief Current function from __func__. Empty if unknown
    std::string func;

    //! \brief String representation of func, file and line.
    //! \returns "<func> in <filename>:<line>" or "Unknown file" if unknown.
    std::string ToString() const;
};

//! \brief Exception class with file information. Base class of all custom exception classes
class HueException : public std::exception
{
public:
    //! \brief Creates HueException with information about the error and source
    //! \param fileInfo Source of the error. Must not always be the throw location,
    //! can also be a calling function which matches the cause better.
    //! \param message Human readable error message.
    HueException(FileInfo fileInfo, const std::string& message);

    //! \brief What message of the exception
    //! \returns exception name, file info and constructor message as char* into member string
    const char* what() const noexcept override;

    //! \brief Filename and line where the exception was thrown or caused by
    const FileInfo& GetFile() const noexcept;

protected:
    //! \brief Creates HueException with child class name
    //!
    //! Should be used by subclasses which can append additional information to the end of whatMessage.
    //! \param exceptionName class name of the subclass
    //! \param fileInfo Source of the error. Must not always be the throw location,
    //! can also be a calling function which matches the cause better.
    //! \param message Human readable error message
    HueException(const char* exceptionName, FileInfo fileInfo, const std::string& message);

private:
    std::string whatMessage;
    FileInfo fileInfo;
};

//! \brief Exception caused by a Hue API "error" response with additional information
//!
//! Refer to Hue developer documentation for more detail on specific error codes.
class HueAPIResponseException : public HueException
{
public:
    //! \brief Create exception with info from Hue API error
    //! \param fileInfo Source of the error. Must not always be the throw location,
    //! can also be a calling function which matches the cause better.
    //! \param error Hue API error code from error response.
    //! \param address URI the API call referred to from error response.
    //! \param description Error description from response.
    HueAPIResponseException(FileInfo fileInfo, int error, std::string address, std::string description);

    //! \brief Error number from Hue API error response.
    //!
    //! Refer to Hue developer documentation for meaning of error codes.
    int GetErrorNumber() const noexcept;
    //! \brief Address the API call tried to access.
    const std::string& GetAddress() const noexcept;
    //! \brief Error description
    const std::string& GetDescription() const noexcept;

    //! \brief Creates exception from API response.
    //! \param fileInfo Location of the cause
    //! \param response Hue API response. Must contain a member "error" with "type", "address" and "description".
    //! \returns HueAPIResponseException with info from the response.
    //! If response does not contain the required members, they are defaulted to -1 or "".
    static HueAPIResponseException Create(FileInfo fileInfo, const nlohmann::json& response);

private:
    //! \brief Creates exception message containing the given information
    static std::string GetMessage(int error, const std::string& addr, const std::string& description);

private:
    int error;
    std::string address;
    std::string description;
};
} // namespace hueplusplus

#endif