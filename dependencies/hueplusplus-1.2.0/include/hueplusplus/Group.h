/**
    \file Group.h
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

#ifndef INCLUDE_HUEPLUSPLUS_GROUP_H
#define INCLUDE_HUEPLUSPLUS_GROUP_H

#include <string>
#include <vector>

#include "APICache.h"
#include "Action.h"
#include "HueCommandAPI.h"
#include "StateTransaction.h"

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! \brief Class for Groups of lights.
//!
//! Provides methods to control groups.
class Group
{
public:
    //! \brief Creates group with shared cache
    //! \param id Group id in the bridge
    //! \param baseCache Cache of the group list.
    Group(int id, const std::shared_ptr<APICache>& baseCache);
    //! \brief Creates group with id
    //! \param id Group id in the bridge
    //! \param commands HueCommandAPI for requests
    //! \param refreshDuration Time between refreshing the cached state.
    //! \param currentState The current state, may be null.
    Group(int id, const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState);

    //! \brief Refreshes internal cached state.
    //! \param force \c true forces a refresh, regardless of how long the last refresh was ago.
    //! \c false to only refresh when enough time has passed (needed e.g. when calling only const methods).
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void refresh(bool force = false);

    //! \brief Sets custom refresh interval for this group.
    //! \param refreshDuration The new minimum duration between refreshes. May be 0 or \ref c_refreshNever.
    void setRefreshDuration(std::chrono::steady_clock::duration refreshDuration);

    //! \name General information
    ///@{

    //! \brief Get the group id.
    int getId() const;

    //! \brief Get the group name.
    std::string getName() const;

    //! \brief Get the group type.
    //!
    //! The type is specified on creation and cannot be changed.
    //!
    //! Possible types:
    //! \li <code>0</code>: Special group containing all lights, cannot be modified.
    //! \li <code>Luminaire</code>, <code>Lightsource</code>: Automatically created groups for multisource luminaires.
    //! \li <code>LightGroup</code>: Standard, user created group, not empty.
    //! \li <code>Room</code>: User created room, has room type.
    //! \li <code>Entertainment</code>: User created entertainment setup.
    //! \li <code>Zone</code>: User created Zone.
    std::string getType() const;

    //! \brief Get lights in the group.
    //! \returns Ids of the lights in the group.
    std::vector<int> getLightIds() const;

    //! \brief Set group name.
    //! \param name New name for the group.
    //! Must be unique for all groups, otherwise a number is added.
    void setName(const std::string& name);
    //! \brief Set group lights.
    //! \param ids New light ids. May or may not be empty depending on type.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setLights(const std::vector<int>& ids);

    //! \brief Get room type, only for type room.
    //! \returns Room type/class of the group.
    std::string getRoomType() const;
    //! \brief Set room type, only for type room.
    //! \param type New room class, case sensitive.
    //! Only specific values are allowed.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setRoomType(const std::string& type);

    //! \brief Get luminaire model id, only for type luminaire.
    //! \returns Unique id for the hardware model.
    std::string getModelId() const;

    //! \brief Get luminaire model id, only for type luminaire or lightsource.
    //! \returns Unique id in <code>AA:BB:CC:DD</code> format for luminaire groups
    //! or <code>AA:BB:CC:DD-XX</code> for Lightsource groups.
    std::string getUniqueId() const;

    //! \brief Get whether all lights are on.
    //! \returns true when all lights are on.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    bool getAllOn();

    //! \brief Get whether all lights are on.
    //! \returns true when all lights are on.
    //! \note Does not refresh the state.
    bool getAllOn() const;

    //! \brief Get whether any light is on.
    //! \returns true when any light is on.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    bool getAnyOn();

    //! \brief Get whether any light is on.
    //! \returns true when any light is on.
    //! \note Does not refresh the state.
    bool getAnyOn() const;

    ///@}
    //! \name Query Action
    //! The action is the state of one light in the group.
    //! It can be accessed using these methods.
    ///@{

    //! \brief Get on state of one light in the group.
    //! \returns True if the light is on.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    bool getActionOn();

    //! \brief Get on state of one light in the group.
    //! \returns True if the light is on.
    //! \note Does not refresh the state.
    bool getActionOn() const;

    //! \brief Get hue and saturation of one light in the group.
    //! \returns Pair of hue, saturation.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    std::pair<uint16_t, uint8_t> getActionHueSaturation();

    //! \brief Get hue and saturation of one light in the group.
    //! \returns Pair of hue, saturation.
    //! \note Does not refresh the state.
    std::pair<uint16_t, uint8_t> getActionHueSaturation() const;

    //! \brief Get brightness of one light in the group.
    //! \returns Brightness (0-254).
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    unsigned int getActionBrightness();

    //! \brief Get brightness of one light in the group.
    //! \returns Brightness (0-254).
    //! \note Does not refresh the state.
    unsigned int getActionBrightness() const;

    //! \brief Get color temperature of one light in the group.
    //! \returns Color temperature in mired.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    unsigned int getActionColorTemperature();

    //! \brief Get color temperature of one light in the group.
    //! \returns Color temperature in mired.
    //! \note Does not refresh the state.
    unsigned int getActionColorTemperature() const;

    //! \brief Get color coordinates of one light in the group.
    //! \returns Pair of x and y color coordinates.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    std::pair<float, float> getActionColorXY();

    //! \brief Get color coordinates of one light in the group.
    //! \returns Pair of x and y color coordinates.
    //! \note Does not refresh the state.
    std::pair<float, float> getActionColorXY() const;

    //! \brief Get color mode of one light in the group.
    //!
    //! The color mode is the currently used way to specify the color (hs,ct or xy).
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    std::string getActionColorMode();

    //! \brief Get color mode of one light in the group.
    //!
    //! The color mode is the currently used way to specify the color (hs,ct or xy).
    //! \note Does not refresh the state.
    std::string getActionColorMode() const;

    ///@}

    //! \name Change lights
    ///@{

    //! \brief Create a transaction for this group.
    //!
    //! The transaction can be used to change more than one value in one request.
    //!
    //! Example usage: \code
    //! group.transaction().setBrightness(240).setColorHue(5000).commit();
    //! \endcode
    StateTransaction transaction();

    //! \brief Convenience function to turn lights on.
    //! \see StateTransaction::setOn
    void setOn(bool on, uint8_t transition = 4);
    //! \brief Convenience function to set brightness.
    //! \see StateTransaction::setBrightness
    void setBrightness(uint8_t brightness, uint8_t transition = 4);
    //! \brief Convenience function to set hue and saturation.
    //! \see StateTransaction::setColor(const HueSaturation&)
    void setColor(const HueSaturation& hueSat, uint8_t transition = 4);
    //! \brief Convenience function to set color xy.
    //! \see StateTransaction::setColor(const XYBrightness&)
    void setColor(const XYBrightness& xy, uint8_t transition = 4);
    //! \brief Convenience function to set color temperature.
    //! \see StateTransaction::setColorTemperature
    void setColorTemperature(unsigned int mired, uint8_t transition = 4);
    //! \brief Convenience function to set color loop.
    //! \see StateTransaction::setColorLoop
    void setColorLoop(bool on, uint8_t transition = 4);

    //! \brief Recall scene for the group.
    //!
    //! Scenes are saved configurations for the lights in a group.
    //! \param scene Scene name.
    void setScene(const std::string& scene);

    //! \brief Get Action to set scene
    //! \param scene Scene name
    //! \returns A Action that can be used to set the scene on a Schedule
    //!
    //! To set other light properties in a scene, use transaction().
    Action createSceneAction(const std::string& scene) const;

    ///@}

protected:
    //! \brief Utility function to send a put request to the group.
    //!
    //! \param request The request to send
    //! \param subPath A path that is appended to the uri, note it should always start with a slash ("/")
    //! \param fileInfo FileInfo from calling function for exception details.
    //! \returns The parsed reply
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    nlohmann::json sendPutRequest(const std::string& subPath, const nlohmann::json& request, FileInfo fileInfo);

protected:
    int id;
    APICache state;
};

//! \brief Parameters necessary for creating a new Group.
//!
//! Provides static functions for each group type that can be created by the user.
//! \note These are not all types that Group::getType() can return,
//! because some types cannot be created manually.
class CreateGroup
{
public:
    //! \brief Create a LightGroup.
    //!
    //! LightGroup is the default type for groups. Empty LightGroups will be deleted.
    //! \param lights List of light ids, must not be empty.
    //! \param name Name of the new group, optional.
    static CreateGroup LightGroup(const std::vector<int>& lights, const std::string& name = "");
    //! \brief Create a Room group.
    //!
    //! Rooms can have a room class and can be empty. Every light can only be in one room.
    //! \param lights List of light ids, may be empty.
    //! \param name Name of the room, optional.
    //! \param roomType Class of the room (case sensitive), optional.
    //! Refer to Hue developer documentation for a list of possible room classes.
    static CreateGroup Room(
        const std::vector<int>& lights, const std::string& name = "", const std::string& roomType = "");
    //! \brief Create an Entertainment group.
    //!
    //! The lights are used in an entertainment setup and can have relative positions.
    //! The group can be empty.
    //! \param lights List of light ids, may be empty.
    //! \param name Name of the group, optional.
    static CreateGroup Entertainment(const std::vector<int>& lights, const std::string& name = "");

    //! \brief Create a Zone.
    //!
    //! Zones can be empty, a light can be in multiple zones.
    //! \param lights List of light ids, may be empty.
    //! \param name Name of the Zone, optional.
    static CreateGroup Zone(const std::vector<int>& lights, const std::string& name = "");

    //! \brief Get request to create the group.
    //! \returns JSON request for a POST to create the new group
    nlohmann::json getRequest() const;

protected:
    //! \brief Protected constructor, should not be called directly.
    //! \param lights List of light ids for the group.
    //! \param name Name of the group, empty for default name.
    //! \param type Type of the group, empty for default type.
    //! \param roomType Room class if type is room, empty for default class or if type is not room.
    CreateGroup(
        const std::vector<int>& lights, const std::string& name, const std::string& type, const std::string& roomType);

private:
    std::vector<int> lights;
    std::string name;
    std::string type;
    std::string roomType;
};
} // namespace hueplusplus

#endif