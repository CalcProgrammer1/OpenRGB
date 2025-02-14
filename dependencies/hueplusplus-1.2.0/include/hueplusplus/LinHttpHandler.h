/**
    \file LinHttpHandler.h
    Copyright Notice\n
    Copyright (C) 2017  Jan Rogall		- developer\n
    Copyright (C) 2017  Moritz Wirger	- developer\n

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

#ifndef INCLUDE_HUEPLUSPLUS_LINHTTPHANDLER_H
#define INCLUDE_HUEPLUSPLUS_LINHTTPHANDLER_H

#include <string>
#include <vector>

#include "BaseHttpHandler.h"

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! Class to handle http requests and multicast requests on linux systems
class LinHttpHandler : public BaseHttpHandler
{
public:
    //! \brief Function that sends a given message to the specified host and
    //! returns the response.
    //!
    //! \param msg String that contains the message that is sent to the specified
    //! address \param adr String that contains an ip or hostname in dotted
    //! decimal notation like "192.168.2.1" \param port Optional integer that
    //! specifies the port to which the request is sent to. Default is 80 \return
    //! String containing the response of the host
    virtual std::string send(const std::string& msg, const std::string& adr, int port = 80) const override;

    //! \brief Function that sends a multicast request with the specified message.
    //!
    //! \param msg String that contains the request that is sent to the specified
    //! address \param adr Optional String that contains an ip or hostname in
    //! dotted decimal notation, default is "239.255.255.250" \param port Optional
    //! integer that specifies the port to which the request is sent. Default is
    //! 1900 \param timeout Optional The timeout of the
    //! request. Default is 5 seconds \return Vector containing strings of each
    //! answer received
    std::vector<std::string> sendMulticast(const std::string& msg, const std::string& adr = "239.255.255.250",
        int port = 1900, std::chrono::steady_clock::duration timeout = std::chrono::seconds(5)) const override;
};
} // namespace hueplusplus

#endif
