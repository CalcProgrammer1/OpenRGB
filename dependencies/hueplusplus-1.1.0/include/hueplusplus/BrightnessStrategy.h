/**
    \file BrightnessStrategy.h
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

#ifndef INCLUDE_HUEPLUSPLUS_BRIGHTNESS_STRATEGY_H
#define INCLUDE_HUEPLUSPLUS_BRIGHTNESS_STRATEGY_H

#include <cstdint>

namespace hueplusplus
{
class Light;

//! Virtual base class for all BrightnessStrategies
class BrightnessStrategy
{
public:
    //! \brief Virtual function for changing a lights brightness with a specified
    //! transition.
    //!
    //! \param bri The brightness raning from 0 = off to 255 = fully lit
    //! \param transition The time it takes to fade to the new brightness in
    //! multiples of 100ms, 4 = 400ms and should be seen as the default \param
    //! light A reference of the light
    virtual bool setBrightness(unsigned int bri, uint8_t transition, Light& light) const = 0;
    //! \brief Virtual function that returns the current brightnessof the light
    //!
    //! Should update the lights state by calling refreshState()
    //! \param light A reference of the light
    //! \return Unsigned int representing the brightness
    virtual unsigned int getBrightness(Light& light) const = 0;
    //! \brief Virtual function that returns the current brightness of the light
    //!
    //! \note This should not update the lights state
    //! \param light A const reference of the light
    //! \return Unsigned int representing the brightness
    virtual unsigned int getBrightness(const Light& light) const = 0;
    //! \brief Virtual dtor
    virtual ~BrightnessStrategy() = default;
};
} // namespace hueplusplus

#endif
