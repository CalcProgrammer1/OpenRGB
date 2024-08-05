/**
    \file UPnP.h
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

#ifndef INCLUDE_HUEPLUSPLUS_UPNP_H
#define INCLUDE_HUEPLUSPLUS_UPNP_H

#include <memory>
#include <string>
#include <vector>

#include "IHttpHandler.h"

namespace hueplusplus
{
//! Class that looks for UPnP devices using an m-search package
class UPnP
{
public:
    //! \brief Searches for UPnP devices and returns all found ones.
    //!
    //! It does it by sending an m-search packet and waits for all responses.
    //! Since responses can be received multiple times this function conveniently removes all duplicates.
    //! \param handler HttpHandler for communication
    //! \return A vector containing pairs of address and name of all found devices
    //! \throws std::system_error when system or socket operations fail
    std::vector<std::pair<std::string, std::string>> getDevices(std::shared_ptr<const IHttpHandler> handler);
};
} // namespace hueplusplus

#endif
