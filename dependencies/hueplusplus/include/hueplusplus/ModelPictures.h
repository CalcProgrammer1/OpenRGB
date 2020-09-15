/**
    \file ModelPictures.h
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall		- developer\n

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

#ifndef INCLUDE_HUEPLUSPLUS_MODEL_PICTURES_H
#define INCLUDE_HUEPLUSPLUS_MODEL_PICTURES_H

#include <string>

namespace hueplusplus
{
    //! \brief Get the picture name of a given model id
    //!
    //! \note This function will only return the filename without extension,
    //! because Philips provides different file types.
    //! \param modelId Model Id of a device to get the picture of 
    //! \returns String that either contains the filename of the picture of the device 
    //! or an empty string if it was not found.
    std::string getPictureOfModel(const std::string& modelId);
}


#endif