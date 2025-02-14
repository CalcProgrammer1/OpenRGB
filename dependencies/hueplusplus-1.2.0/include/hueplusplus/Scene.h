/**
    \file Scene.h
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

#ifndef INCLUDE_HUEPLUSPLUS_SCENE_H
#define INCLUDE_HUEPLUSPLUS_SCENE_H

#include <map>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "APICache.h"
#include "ColorUnits.h"
#include "TimePattern.h"

namespace hueplusplus
{
//! \brief Immutable state of a light
class LightState
{
public:
    //! \brief Create LightState from json
    //! \note Use LightStateBuilder for easier creation.
    explicit LightState(const nlohmann::json& state);

    //! \brief Get whether the light is on
    bool isOn() const;

    //! \brief Get whether a brightness is stored
    bool hasBrightness() const;
    //! \brief Get brightness of the light
    //! \returns Stored brightness, or 0
    int getBrightness() const;

    //! \brief Get whether hue and saturation is stored
    bool hasHueSat() const;
    //! \brief Get hue and saturation of the light
    //! \returns Stored hue and saturation, or {0,0} if not stored
    HueSaturation getHueSat() const;

    //! \brief Get whether xy color is stored
    bool hasXY() const;
    //! \brief Get xy color of the light
    //! \returns Stored x,y and brightness, or zeros if not stored
    XYBrightness getXY() const;

    //! \brief Get whether color temperature is stored
    bool hasCt() const;
    //! \brief Get color temperature of the light
    //! \returns Stored color temperature in mired, or 0 if not stored
    int getCt() const;

    //! \brief Get whether effect is stored
    bool hasEffect() const;
    //! \brief Get whether colorloop effect is active
    //! \returns true when colorloop is enabled, false otherwise or if not stored
    bool getColorloop() const;

    //! \brief Get transition time to this light state
    //! \returns Stored transition time or 4 by default
    int getTransitionTime() const;

    //! \brief Convert to json representation
    nlohmann::json toJson() const;

    //! \brief Equality comparison
    bool operator==(const LightState& other) const;
    //! \brief Inequality comparison
    bool operator!=(const LightState& other) const;

private:
    nlohmann::json state;
};

//! \brief Builder to create LightState
class LightStateBuilder
{
public:
    LightStateBuilder& setOn(bool on);
    LightStateBuilder& setBrightness(int brightness);
    LightStateBuilder& setHueSat(const HueSaturation& hueSat);
    LightStateBuilder& setXY(const XY& xy);
    LightStateBuilder& setCt(int mired);
    LightStateBuilder& setColorloop(bool enabled);
    LightStateBuilder& setTransitionTime(int time);

    LightState create();

private:
    nlohmann::json state;
};

//! \brief Scene stored in the bridge
//!
//! Scenes bundle the state of multiple lights so it can be recalled later.
class Scene
{
public:
    //! \brief Type of the scen
    enum class Type
    {
        lightScene, //!< The scene affects specific lights
        groupScene //!< The scene affects all light of a specific group
    };

public:
    //! \brief Creates scene with shared cache
    //! \param id Scene id in the bridge
    //! \param baseCache Cache of the scene list.
    Scene(const std::string& id, const std::shared_ptr<APICache>& baseCache);
    //! \brief Construct existing Scene
    //! \param id Scene id
    //! \param commands HueCommandAPI for requests
    //! \param refreshDuration Time between refreshing the cached state
    //! \param currentState The current state, may be null.
    Scene(const std::string& id, const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState);

    //! \brief Refreshes internal cached state
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

    //! \brief Get scene identifier
    std::string getId() const;
    //! \brief Get scene name
    //!
    //! The scene name is always unique for the bridge. It defaults to the id.
    std::string getName() const;
    //! \brief Set scene name
    //! \param name New name for the scene.
    //! Must be unique for all schedules, otherwise a number is added.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setName(const std::string& name);
    //! \brief Get scene type
    //!
    //! GroupScenes are deleted when the group is deleted.
    Type getType() const;

    //! \brief Get group id for a GroupScene
    //! \returns Group id or 0 if the scene is a LightScene.
    int getGroupId() const;

    //! \brief Get light ids
    //!
    //! For a GroupScene, the light ids are the lights in the group.
    std::vector<int> getLightIds() const;
    //! \brief Set light ids for LightScene
    //! \param ids New light ids
    //!
    //! Light ids cannot be changed on GroupScene. Change the lights in the group instead.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setLightIds(const std::vector<int>& ids);

    //! \brief Get user that created or last changed the scene.
    std::string getOwner() const;
    //! \brief Get whether the scene can be automatically deleted
    bool getRecycle() const;
    //! \brief Get whether scene is locked by a rule or schedule
    bool isLocked() const;

    //! \brief Get app specific data
    std::string getAppdata() const;
    //! \brief Get version of app specific data
    int getAppdataVersion() const;
    //! \brief Set app specific data
    //! \param data Custom data in any format, max length 16.
    //! \param version Version of the data
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setAppdata(const std::string& data, int version);

    //! \brief Get picture, reserved for future use.
    //!
    //! Currently always an empty string.
    std::string getPicture() const;
    //! \brief Get time the scene was created/updated.
    time::AbsoluteTime getLastUpdated() const;
    //! \brief Get version of the scene
    //! \returns 1 for legacy scene without lightstates
    //! \returns 2 for updated scenes with lightstates
    int getVersion() const;

    //! \brief Get stored states of the lights
    //! \returns LightStates for each light in the scene, or an empty map for legacy scenes.
    std::map<int, LightState> getLightStates() const;
    //! \brief Set light states
    //! \param states New states for each light in the scene.
    //! Should contain exactly the lights in the scene. Additional states might cause an error.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setLightStates(const std::map<int, LightState>& states);

    //! \brief Store current light state of every light in the scene
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void storeCurrentLightState();
    //! \brief Store current light state and update transition time
    //! \param transition The updated transition time to this scene
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void storeCurrentLightState(int transition);

    //! \brief Recall scene, putting every light in the stored state
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void recall();

private:
    //! \brief Send put request to specified sub path
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void sendPutRequest(const std::string& path, const nlohmann::json& request, FileInfo fileInfo);

private:
    std::string id;
    APICache state;
};

//! \brief Parameters for creating a new Scene
//!
//! Can be used like a builder object with chained calls.
class CreateScene
{
public:
    //! \brief Set name
    //! \see Scene::setName
    CreateScene& setName(const std::string& name);
    //! \brief Set group id, making the scene a GroupScene
    //! \param id Group id for the scene, not 0
    //!
    //! The group id cannot be changed after the scene was created.
    //! \throws HueException when used after setLightIds
    CreateScene& setGroupId(int id);
    //! \brief Set light ids, making the scene a LightScene
    //! \param ids Ids of lights in the scene
    //! \throws HueException when used after setGroupId
    CreateScene& setLightIds(const std::vector<int>& ids);
    //! \brief Set whether the scene can be automatically deleted
    //!
    //! Cannot be changed after the scene was created.
    CreateScene& setRecycle(bool recycle);
    //! \brief Set app specific data
    //! \see Scene::setAppdata
    CreateScene& setAppdata(const std::string& data, int version);
    //! \brief Set light states of the scene
    //!
    //! When omitted, the current light states are stored.
    //! \see Scene::setLightStates
    CreateScene& setLightStates(const std::map<int, LightState>& states);

    //! \brief Get request to create the scene.
    //! \returns JSON request for a POST to create the new scene
    nlohmann::json getRequest() const;

private:
    nlohmann::json request;
};
} // namespace hueplusplus

#endif
