/**
    \file Bridge.h
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

#ifndef INCLUDE_HUEPLUSPLUS_HUE_H
#define INCLUDE_HUEPLUSPLUS_HUE_H

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "APICache.h"
#include "BridgeConfig.h"
#include "BrightnessStrategy.h"
#include "ColorHueStrategy.h"
#include "ColorTemperatureStrategy.h"
#include "Group.h"
#include "HueCommandAPI.h"
#include "HueDeviceTypes.h"
#include "IHttpHandler.h"
#include "Light.h"
#include "ResourceList.h"
#include "Rule.h"
#include "Scene.h"
#include "Schedule.h"
#include "Sensor.h"
#include "SensorList.h"
#include "Utils.h"

#include <nlohmann/json.hpp>

//! \brief Namespace for the hueplusplus library
namespace hueplusplus
{
// forward declarations
class Bridge;

//!
//! Class to find all Hue bridges on the network and create usernames for them.
//!
class BridgeFinder
{
public:
    struct BridgeIdentification
    {
        std::string ip;
        int port = 80;
        std::string mac;
    };

public:
    //! \brief Constructor of BridgeFinder class
    //!
    //! \param handler HttpHandler of type \ref IHttpHandler for communication with the bridge
    BridgeFinder(std::shared_ptr<const IHttpHandler> handler);

    //! \brief Finds all bridges in the network and returns them.
    //!
    //! The user should be given the opportunity to select the correct one based on the mac address.
    //! \return vector containing ip and mac of all found bridges
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    std::vector<BridgeIdentification> findBridges() const;

    //! \brief Gets a Hue bridge based on its identification
    //!
    //! \param identification \ref BridgeIdentification that specifies a bridge
    //! \param sharedState Uses a single, shared cache for all objects on the bridge.
    //! \return \ref Bridge class object
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body or username could not be requested
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    Bridge getBridge(const BridgeIdentification& identification, bool sharedState = false);

    //! \brief Function that adds a username to the usernames map
    //!
    //! \param mac MAC address of Hue bridge
    //! \param username Username that is used to control the Hue bridge
    void addUsername(const std::string& mac, const std::string& username);

    //! \brief Function that adds a client key to the clientkeys map
    //!
    //! The client key is only needed for entertainment mode, otherwise it is optional.
    //! \param mac MAC address of Hue bridge
    //! \param clientkey Client key that is used to control the Hue bridge in entertainment mode
    void addClientKey(const std::string& mac, const std::string& clientkey);

    //! \brief Function that returns a map of mac addresses and usernames.
    //!
    //! Note these should be saved at the end and re-loaded with \ref addUsername
    //! next time, so only one username is generated per bridge. \returns A map
    //! mapping mac address to username for every bridge
    const std::map<std::string, std::string>& getAllUsernames() const;

    //! \brief Normalizes mac address to plain hex number.
    //! \returns \p input without separators and whitespace, in lower case.
    static std::string normalizeMac(std::string input);

private:
    //! \brief Parses mac address from description.xml
    //!
    //! \param description Content of description.xml file as returned by GET request.
    //! \returns Content of xml element \c serialNumber if description matches a Hue bridge, otherwise an empty
    //! string.
    static std::string parseDescription(const std::string& description);

    std::map<std::string, std::string> usernames; //!< Maps all macs to usernames added by \ref
                                                  //!< BridgeFinder::addUsername
    std::map<std::string, std::string> clientkeys; //!< Maps all macs to clientkeys added by \ref
                                                   //!< BridgeFinder::addClientKey
    std::shared_ptr<const IHttpHandler> http_handler;
};

//! \brief Bridge class for a bridge.
//!
//! This is the main class used to interact with the Hue bridge.
class Bridge
{
    friend class BridgeFinder;

public:
    using LightList = SearchableResourceList<Light>;
    using GroupList = GroupResourceList<Group, CreateGroup>;
    using ScheduleList = CreateableResourceList<ResourceList<Schedule, int>, CreateSchedule>;
    using SceneList = CreateableResourceList<ResourceList<Scene, std::string>, CreateScene>;
    using RuleList = CreateableResourceList<ResourceList<Rule, int>, CreateRule>;

public:
    //! \brief Constructor of Bridge class
    //!
    //! \param ip IP address in dotted decimal notation like "192.168.2.1"
    //! \param port Port of the hue bridge
    //! \param username String that specifies the username that is used to control
    //! the bridge. Can be left empty and acquired in \ref requestUsername.
    //! \param handler HttpHandler for communication with the bridge
    //! \param clientkey Optional client key for streaming
    //! \param refreshDuration Time between refreshing the cached state.
    //! \param sharedState Uses a single, shared cache for all objects on the bridge.
    Bridge(const std::string& ip, const int port, const std::string& username,
        std::shared_ptr<const IHttpHandler> handler, const std::string& clientkey = "",
        std::chrono::steady_clock::duration refreshDuration = std::chrono::seconds(10), bool sharedState = false);

    //! \brief Refreshes the bridge state.
    //!
    //! Should only be called rarely, as a full refresh is costly and usually not necessary.
    //! Instead refresh only the parts you are interested in or rely on periodic refreshes
    //! that happen automatically when calling non-const methods.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void refresh();

    //! \brief Sets refresh interval for the whole bridge state.
    //! \param refreshDuration The new minimum duration between refreshes. May be 0 or \ref c_refreshNever.
    //! 
    //! Also sets refresh duration on all resource lists on the bridge, but not on already existing lights.
    //! The resource lists (such as lights()) can have their own durations, but those must be set after calling this function.
    void setRefreshDuration(std::chrono::steady_clock::duration refreshDuration);

    //! \brief Function to get the ip address of the hue bridge
    //!
    //! \return string containing ip
    std::string getBridgeIP() const;

    //! \brief Function to set stream mode to active for entertainment mode
    //!
    //! \return bool - whether stream request was successful
    bool startStreaming(std::string group_identifier);

    //! \brief Function to set stream mode to active for entertainment mode
    //!
    //! \return bool - whether stream request was successful
    bool stopStreaming(std::string group_identifier);

    //! \brief Function to get the port of the hue bridge
    //!
    //! \return integer containing port
    int getBridgePort() const;

    //! \brief Send a username request to the Hue bridge.
    //!
    //! Blocks for about 30 seconds and 5 seconds to prepare.
    //! It automatically sets the username variable according to the username received and returns the username
    //! received. This function should only be called once to acquire a username to control the bridge and the
    //! username should be saved for future use. \return username for API usage \throws std::system_error when
    //! system or socket operations fail \throws HueException when response contained no body \throws
    //! HueAPIResponseException when response contains an error except link button not pressed. \throws
    //! nlohmann::json::parse_error when response could not be parsed
    std::string requestUsername();

    //! \brief Function that returns the username
    //!
    //! \return The username used for API access
    std::string getUsername() const;

    //! \brief Function that returns the client key
    //!
    //! \return The client key used for Entertainment Mode API access
    std::string getClientKey() const;

    //! \brief Function to set the ip address of this class representing a bridge
    //!
    //! \param ip String that specifies the ip in dotted decimal notation like "192.168.2.1"
    void setIP(const std::string& ip);

    //! \brief Function to set the port of this class representing a bridge
    //!
    //! \param port Integer that specifies the port of an address like
    //! "192.168.2.1:8080"
    void setPort(const int port);

    //! \brief Provides access to the configuration of the bridge.
    BridgeConfig& config();
    //! \brief Provides access to the configuration of the bridge.
    //! \note Does not refresh state.
    const BridgeConfig& config() const;

    //! \brief Provides access to the Light%s on the bridge.
    LightList& lights();
    //! \brief Provides access to the Light%s on the bridge.
    //! \note Does not refresh state.
    const LightList& lights() const;

    //! \brief Provides access to the Group%s on the bridge.
    GroupList& groups();
    //! \brief Provides access to the Group%s on the bridge.
    //! \note Does not refresh state.
    const GroupList& groups() const;

    //! \brief Provides access to the Schedule%s on the bridge.
    ScheduleList& schedules();
    //! \brief Provides access to the Schedule%s on the bridge.
    //! \note Does not refresh state.
    const ScheduleList& schedules() const;

    //! \brief Provides access to the Scene%s on the bridge.
    SceneList& scenes();
    //! \brief Provides access to the Scene%s on the bridge.
    //! \note Does not refresh state.
    const SceneList& scenes() const;

    //! \brief Provides access to the Sensor%s on the bridge.
    SensorList& sensors();
    //! \brief Provides access to the Sensor%s on the bridge.
    //! \note Does not refresh state.
    const SensorList& sensors() const;

    //! \brief Provides access to the Rule%s on the bridge.
    RuleList& rules();
    //! \brief Provides access to the Rule%s on the bridge
    //! \note Does not refresh state.
    const RuleList& rules() const;

private:
    //! \brief Function that sets the HttpHandler and updates the HueCommandAPI.
    //! \param handler a HttpHandler of type \ref IHttpHandler
    //!
    //! The HttpHandler and HueCommandAPI are used for bridge communication.
    //! Resetting the HttpHandler should only be done when the username is first set,
    //! before Bridge is used.
    //! Resets all caches and resource lists.
    void setHttpHandler(std::shared_ptr<const IHttpHandler> handler);

private:
    std::string ip; //!< IP-Address of the hue bridge in dotted decimal notation
                    //!< like "192.168.2.1"
    std::string username; //!< Username that is ussed to access the hue bridge
    std::string clientkey; //!< Client key that is used for entertainment mode
    int port;

    std::shared_ptr<const IHttpHandler> http_handler; //!< A IHttpHandler that is used to communicate with the
                                                      //!< bridge
    std::chrono::steady_clock::duration refreshDuration;
    std::shared_ptr<APICache> stateCache;
    detail::MakeCopyable<LightList> lightList;
    detail::MakeCopyable<GroupList> groupList;
    detail::MakeCopyable<ScheduleList> scheduleList;
    detail::MakeCopyable<SceneList> sceneList;
    detail::MakeCopyable<SensorList> sensorList;
    detail::MakeCopyable<RuleList> ruleList;
    detail::MakeCopyable<BridgeConfig> bridgeConfig;
    bool sharedState;
};
} // namespace hueplusplus

#endif
