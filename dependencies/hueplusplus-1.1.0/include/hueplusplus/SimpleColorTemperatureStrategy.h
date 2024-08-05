/**
    \file SimpleColorTemperatureStrategy.h
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

#ifndef INCLUDE_HUEPLUSPLUS_SIMPLE_COLOR_TEMPERATURE_STRATEGY_H
#define INCLUDE_HUEPLUSPLUS_SIMPLE_COLOR_TEMPERATURE_STRATEGY_H

#include "ColorTemperatureStrategy.h"
#include "Light.h"

namespace hueplusplus
{
//! Class implementing the functions of ColorTemperatureStrategy
class SimpleColorTemperatureStrategy : public ColorTemperatureStrategy
{
public:
    //! \brief Function for changing a lights color temperature in mired with a
    //! specified transition.
    //!
    //! The color temperature in mired ranges from 153 to 500 whereas 153 is cold
    //! and 500 is warm. \param mired The color temperature in mired \param
    //! transition The time it takes to fade to the new color in multiples of
    //! 100ms, 4 = 400ms and should be seen as the default \param light A
    //! reference of the light
    bool setColorTemperature(unsigned int mired, uint8_t transition, Light& light) const override;
    //! \brief Function that lets the light perform one breath cycle in the
    //! specified color.
    //!
    //! It uses this_thread::sleep_for to accomodate for the time an \ref
    //! Light::alert() needs The color temperature in mired ranges from 153 to
    //! 500 whereas 153 is cold and 500 is warm. \param mired The color
    //! temperature in mired \param light A reference of the light
    bool alertTemperature(unsigned int mired, Light& light) const override;
    //! \brief Function that returns the current color temperature of the light
    //!
    //! Updates the lights state by calling refreshState()
    //! The color temperature in mired ranges from 153 to 500 whereas 153 is cold
    //! and 500 is warm. \param light A reference of the light \return Unsigned
    //! int representing the color temperature in mired
    unsigned int getColorTemperature(Light& light) const override;
    //! \brief Function that returns the current color temperature of the light
    //!
    //! The color temperature in mired ranges from 153 to 500 whereas 153 is cold
    //! and 500 is warm. \note This does not update the lights state \param light
    //! A const reference of the light \return Unsigned int representing the color
    //! temperature in mired
    unsigned int getColorTemperature(const Light& light) const override;
};
} // namespace hueplusplus

#endif
