/**
    \file Hue.h
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

#ifndef _HUE_H
#define _HUE_H

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "BrightnessStrategy.h"
#include "ColorHueStrategy.h"
#include "ColorTemperatureStrategy.h"
#include "HueCommandAPI.h"
#include "HueLight.h"
#include "IHttpHandler.h"

#include "json/json.hpp"

// forward declarations
class Hue;

//!
//! Class to find all Hue bridges on the network and create usernames for them.
//!
class HueFinder
{
public:
    struct HueIdentification
    {
        std::string ip;
        int port = 80;
        std::string mac;
    };

public:
    //! \brief Constructor of HueFinder class
    //!
    //! \param handler HttpHandler of type \ref IHttpHandler for communication with the bridge
    HueFinder(std::shared_ptr<const IHttpHandler> handler);

    //! \brief Finds all bridges in the network and returns them.
    //!
    //! The user should be given the opportunity to select the correct one based on the mac address.
    //! \return vector containing ip and mac of all found bridges
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    std::vector<HueIdentification> FindBridges() const;

    //! \brief Gets a \ref Hue bridge based on its identification
    //!
    //! \param identification \ref HueIdentification that specifies a bridge
    //! \return \ref Hue class object
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body or username could not be requested
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    Hue GetBridge(const HueIdentification& identification);

    //! \brief Function that adds a username to the usernames map
    //!
    //! \param mac MAC address of Hue bridge
    //! \param username Username that is used to control the Hue bridge
    void AddUsername(const std::string& mac, const std::string& username);

    //! \brief Function that returns a map of mac addresses and usernames.
    //!
    //! Note these should be saved at the end and re-loaded with \ref AddUsername
    //! next time, so only one username is generated per bridge. \returns A map
    //! mapping mac address to username for every bridge
    const std::map<std::string, std::string>& GetAllUsernames() const;

    //! \brief Normalizes mac address to plain hex number.
    //! \returns \p input without separators and whitespace, in lower case.
    static std::string NormalizeMac(std::string input);

private:
    //! \brief Parses mac address from description.xml
    //!
    //! \param description Content of description.xml file as returned by GET request.
    //! \returns Content of xml element \c serialNumber if description matches a Hue bridge, otherwise an empty string.
    static std::string ParseDescription(const std::string& description);

    std::map<std::string, std::string> usernames; //!< Maps all macs to usernames added by \ref
                                                  //!< HueFinder::AddUsername
    std::shared_ptr<const IHttpHandler> http_handler;
};

//! Hue class
class Hue
{
    friend class HueFinder;

public:
    //! \brief Constructor of Hue class
    //!
    //! \param ip IP address in dotted decimal notation like "192.168.2.1"
    //! \param port Port of the hue bridge
    //! \param username String that specifies the username that is used to control
    //! the bridge. This needs to be acquired in \ref requestUsername
    //! \param handler HttpHandler for communication with the bridge
    Hue(const std::string& ip, const int port, const std::string& username,
        std::shared_ptr<const IHttpHandler> handler);

    //! \brief Function to get the ip address of the hue bridge
    //!
    //! \return string containing ip
    std::string getBridgeIP();

    //! \brief Function to get the port of the hue bridge
    //!
    //! \return integer containing port
    int getBridgePort();

    //! \brief Send a username request to the Hue bridge.
    //!
    //! Blocks for about 30 seconds and 5 seconds to prepare.
    //! It automatically sets the username variable according to the username received and returns the username
    //! received. This function should only be called once to acquire a username to control the bridge and the username
    //! should be saved for future use.
    //! \return username for API usage
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error except link button not pressed.
    //! \throws nlohmann::json::parse_error when response could not be parsed
    std::string requestUsername();

    //! \brief Function that returns the username
    //!
    //! \return The username used for API access
    std::string getUsername();

    //! \brief Function to set the ip address of this class representing a bridge
    //!
    //! \param ip String that specifies the ip in dotted decimal notation like "192.168.2.1"
    void setIP(const std::string& ip);

    //! \brief Function to set the port of this class representing a bridge
    //!
    //! \param port Integer that specifies the port of an address like
    //! "192.168.2.1:8080"
    void setPort(const int port);

    //! \brief Function that returns a \ref HueLight of specified id
    //!
    //! \param id Integer that specifies the ID of a Hue light
    //! \return \ref HueLight that can be controlled
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when id does not exist or type is unknown
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    HueLight& getLight(int id);

    //! \brief Function to remove a light from the bridge
    //!
    //! \attention Any use of the light after it was successfully removed results in undefined behavior
    //! \param id Id of the light to remove
    //! \return true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    bool removeLight(int id);

    //! \brief Function that returns all light types that are associated with this bridge
    //!
    //! \return A map mapping light id's to light types for every light
    // const std::map<uint8_t, ColorType>& getAllLightTypes();

    //! \brief Function that returns all lights that are associated with this
    //! bridge
    //!
    //! \return A vector containing references to every HueLight
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    std::vector<std::reference_wrapper<HueLight>> getAllLights();

    //! \brief Function that tells whether a given light id represents an existing light
    //!
    //! Calls refreshState to update the local bridge state
    //! \param id Id of a light to check for existance
    //! \return Bool that is true when a light with the given id exists and false when not
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    bool lightExists(int id);

    //! \brief Const function that tells whether a given light id represents an
    //! existing light
    //!
    //! \note This will not update the local state of the bridge
    //! \param id Id of a light to check for existance
    //! \return Bool that is true when a light with the given id exists and false
    //! when not
    bool lightExists(int id) const;

    //! \brief Const function that returns the picture name of a given light id
    //!
    //! \note This will not update the local state of the bridge.
    //! \note This function will only return the filename without extension,
    //! because Philips provides different file types. \param id Id of a light to
    //! get the picture of \return String that either contains the filename of the
    //! picture of the light or if it was not found an empty string
    std::string getPictureOfLight(int id) const;

    //! \brief Const function that returns the picture name of a given model id
    //!
    //! \note This will not update the local state of the bridge.
    //! \note This function will only return the filename without extension,
    //! because Philips provides different file types. \param model_id Model Id of
    //! a device to get the picture of \return String that either contains the
    //! filename of the picture of the device or if it was not found an empty
    //! string
    std::string getPictureOfModel(const std::string& model_id) const;

    //! \brief Function that sets the HttpHandler and updates the HueCommandAPI.
    //!
    //! The HttpHandler and HueCommandAPI are used for bridge communication
    //! \param handler a HttpHandler of type \ref IHttpHandler
    void setHttpHandler(std::shared_ptr<const IHttpHandler> handler)
    {
        http_handler = std::move(handler);
        commands = HueCommandAPI(ip, port, username, http_handler);
    }

private:
    //! \brief Function that refreshes the local \ref state of the Hue bridge
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void refreshState();

private:
    std::string ip; //!< IP-Address of the hue bridge in dotted decimal notation
                    //!< like "192.168.2.1"
    std::string username; //!< Username that is ussed to access the hue bridge
    int port;
    nlohmann::json state; //!< The state of the hue bridge as it is returned from it
    std::map<uint8_t, HueLight> lights; //!< Maps ids to HueLights that are controlled by this bridge

    std::shared_ptr<BrightnessStrategy> simpleBrightnessStrategy; //!< Strategy that is used for controlling the
                                                                  //!< brightness of lights
    std::shared_ptr<ColorHueStrategy> simpleColorHueStrategy; //!< Strategy that is used for controlling the
                                                              //!< color of lights
    std::shared_ptr<ColorHueStrategy> extendedColorHueStrategy; //!< Strategy that is used for controlling the
                                                                //!< color of lights
    std::shared_ptr<ColorTemperatureStrategy> simpleColorTemperatureStrategy; //!< Strategy that is used for controlling
                                                                              //!< the color temperature of lights
    std::shared_ptr<ColorTemperatureStrategy> extendedColorTemperatureStrategy; //!< Strategy that is used for
                                                                                //!< controlling the color temperature
                                                                                //!< of lights
    std::shared_ptr<const IHttpHandler> http_handler; //!< A IHttpHandler that is used to communicate with the
                                                      //!< bridge
    HueCommandAPI commands; //!< A HueCommandAPI that is used to communicate with the bridge
};

#endif
