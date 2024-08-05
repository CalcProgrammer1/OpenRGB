/**
    \file EntertainmentMode.h
    Copyright Notice\n
    Copyright (C) 2020  Adam Honse		- developer\n
    Copyright (C) 2021  Moritz Wirger		- developer\n

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

#ifndef INCLUDE_HUEPLUSPLUS_HUE_ENTERTAINMENT_MODE_H
#define INCLUDE_HUEPLUSPLUS_HUE_ENTERTAINMENT_MODE_H

#include "Bridge.h"
#include "Group.h"

namespace hueplusplus
{
struct TLSContext;

//! \brief Class for Hue Entertainment Mode
//!
//! Provides methods to initialize and control Entertainment groups.
class EntertainmentMode
{
public:
    //! \brief Constructor
    //!
    //! \param b Bridge reference
    //! \param g Group to control in entertainment mode reference
    //! 
    //! \note References are held to both \c b and \c g. 
    //! They must stay valid until EntertainmentMode ist destroyed.
    EntertainmentMode(Bridge& b, Group& g);

    //! \brief Destroy the Entertainment Mode object
    ~EntertainmentMode();

    //! \brief Connect and start streaming
    //!
    //! \return true If conected and ready to receive commands
    //! \return false If an error occured
    bool connect();

    //! \brief Disconnect and stop streaming
    //!
    //! \return true If disconnected successfully
    //! \return false If an error occurred
    bool disconnect();

    //! \brief Set the color of the given light in RGB format
    //!
    //! \param light_index Light index inside the group
    //! \param red Red color value (0-255)
    //! \param green Green color value (0-255)
    //! \param blue Blue color value (0-255)
    //! \return true If light_index was valid
    //! \return false If light_index was invalid
    bool setColorRGB(uint8_t light_index, uint8_t red, uint8_t green, uint8_t blue);

    //! \brief Update all set colors by \ref setColorRGB
    //!
    //! \return true If all color values for all lights have ben written/sent
    //! \return false If there was an error while writing
    bool update();

protected:
    Bridge* bridge; //!< Associated bridge
    Group* group; //!< Associated group

    std::vector<uint8_t> entertainment_msg; //!< buffer containing the entertainment mode packet data
    uint8_t entertainment_num_lights; //!< number of lights in entertainment mode group

    std::unique_ptr<TLSContext> tls_context; //!< tls context
};
} // namespace hueplusplus

#endif
