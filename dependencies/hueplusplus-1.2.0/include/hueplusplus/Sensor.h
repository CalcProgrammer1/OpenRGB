/**
    \file Sensor.h
    Copyright Notice\n
    Copyright (C) 2020  Stefan Herbrechtsmeier	- developer\n
    Copyright (C) 2020  Jan Rogall          	- developer\n

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

#ifndef INCLUDE_HUEPLUSPLUS_HUE_SENSOR_H
#define INCLUDE_HUEPLUSPLUS_HUE_SENSOR_H

#include <memory>

#include "BaseDevice.h"
#include "Condition.h"
#include "HueCommandAPI.h"
#include "TimePattern.h"

#include <nlohmann/json.hpp>

namespace hueplusplus
{
//! \brief Specifies light alert modes
enum class Alert
{
    none, //!< No alert
    select, //!< Select alert (breathe cycle)
    lselect //!< Long select alert (15s breathe)
};

//! \brief Convert alert to string form
//! \param alert Enum value
//! \returns "none", "select" or "lselect"
std::string alertToString(Alert alert);

//! \brief Convert string to Alert enum
//! \param s String representation
//! \returns Alert::select or Alert::lselect when \c s matches, otherwise Alert::none
Alert alertFromString(const std::string& s);

//! \brief Class for generic or unknown sensor types
//!
//! It is recommended to instead use the classes for specific types in \ref sensors.
//! This class should only be used if the type cannot be known or is not supported.
class Sensor : public BaseDevice
{
public:
    //! \brief Construct Sensor with shared cache
    //! \param id Integer that specifies the id of this sensor
    //! \param baseCache Cache of the SensorList.
    Sensor(int id, const std::shared_ptr<APICache>& baseCache);

    //! \brief Construct Sensor.
    //! \param id Integer that specifies the id of this sensor
    //! \param commands HueCommandAPI for communication with the bridge
    //! \param refreshDuration Time between refreshing the cached state.
    //! \param currentState The current state, may be null.
    Sensor(int id, const HueCommandAPI& commands, std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState);

    //!\name Config attributes
    ///@{

    //! \brief Check whether the sensor has an on attribute
    bool hasOn() const;
    //! \brief check whether the sensor is turned on
    //!
    //! Sensors which are off do not change their status
    //! \throws nlohmann::json::out_of_range when on attribute does not exist.
    bool isOn() const;
    //! \brief Turn sensor on or off
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setOn(bool on);

    //! \brief Check whether the sensor has a battery state
    bool hasBatteryState() const;
    //! \brief Get battery state
    //! \returns Battery state in percent
    //! \throws nlohmann::json::out_of_range when sensor has no battery status.
    int getBatteryState() const;
    //! \brief Set battery state
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setBatteryState(int percent);

    //! \brief Check whether the sensor has alerts
    bool hasAlert() const;
    //! \brief Get last sent alert
    //! \note This is not cleared when the alert ends.
    //! \throws nlohmann::json::out_of_range when sensor has no alert.
    Alert getLastAlert() const;
    //! \brief Send alert
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void sendAlert(Alert type);

    //! \brief Check whether the sensor has reachable validation
    bool hasReachable() const;
    //! \brief Get whether sensor is reachable
    //! \throws nlohmann::json::out_of_range when sensor has no reachable validation
    bool isReachable() const;

    //! \brief Check whether the sensor has a user test mode
    bool hasUserTest() const;
    //! \brief Enable or disable user test mode
    //!
    //! In user test mode, changes are reported more frequently.#
    //! It remains on for 120 seconds or until turned off.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setUserTest(bool enabled);

    //! \brief Check whether the sensor has a URL
    bool hasURL() const;
    //! \brief Get sensor URL
    //!
    //! Only CLIP sensors can have a URL.
    std::string getURL() const;
    //! \brief Set sensor URL
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setURL(const std::string& url);

    //! \brief Get pending config entries, if they exist
    //! \returns The keys of config entries which have been modified,
    //! but were not committed to the device.
    //!
    //! Attempts to set pending config entries may cause errors.
    std::vector<std::string> getPendingConfig() const;

    //! \brief Check whether the sensor has an LED indicator
    bool hasLEDIndication() const;
    //! \brief Get whether the indicator LED is on
    //! \throws nlohmann::json::out_of_range when sensor has no LED
    bool getLEDIndication() const;
    //! \brief Turn LED indicator on or off
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setLEDIndication(bool on);

    //! \brief Get entire config object
    //! \returns A json object with the sensor configuration.
    nlohmann::json getConfig() const;
    //! \brief Set attribute in the sensor config
    //! \param key Key of the config attribute
    //! \param value Any value to set the attribute to
    //!
    //! Can be used to configure sensors with additional config entries.
    void setConfigAttribute(const std::string& key, const nlohmann::json& value);

    ///@}

    //! \brief Get time of last status update
    //! \returns The last update time, or a time with a zero duration from epoch
    //! if the last update time is not set.
    time::AbsoluteTime getLastUpdated() const;

    //! \brief Get state object
    nlohmann::json getState() const;
    //! \brief Set part of the sensor state
    //! \param key Key in the state object
    //! \param value New value
    //!
    //! The state can usually only be set on CLIP sensors, not on physical devices.
    void setStateAttribute(const std::string& key, const nlohmann::json& value);

    //! \brief Get address of the given state attribute, used for conditions
    //! \param key Key in the state object
    //! \returns \c key prefixed with the path to the sensor state
    std::string getStateAddress(const std::string& key) const;

    //! \brief Check if the sensor is Hue certified
    bool isCertified() const;
    //! \brief Check if the sensor is primary sensor of the device
    //!
    //! When there are multiple sensors on one physical device (same MAC address),
    //! the primary device is used for the device information.
    bool isPrimary() const;

    //! \brief Convert sensor to a specific type
    //! \tparam T Sensor type to convert to (from \ref sensors)
    //! \throws HueException when sensor type does not match requested type
    template <typename T>
    T asSensorType() const&
    {
        if (getType() != T::typeStr)
        {
            throw HueException(FileInfo {__FILE__, __LINE__, __func__}, "Sensor type does not match: " + getType());
        }
        return T(*this);
    }
    //! \brief Convert sensor to a specific type
    //! \tparam T Sensor type to convert to (from \ref sensors)
    //! \throws HueException when sensor type does not match requested type
    //!
    //! Move construct \c T to be more efficient when the type is wanted directly.
    template <typename T>
    T asSensorType() &&
    {
        if (getType() != T::typeStr)
        {
            throw HueException(FileInfo {__FILE__, __LINE__, __func__}, "Sensor type does not match: " + getType());
        }
        return T(std::move(*this));
    }
};

//! \brief Parameters for creating a new Sensor
//!
//! Can be used like a builder object with chained calls.
class CreateSensor
{
public:
    //! \brief Construct with necessary parameters
    //! \param name Human readable name
    //! \param modelid Model id of the sensor
    //! \param swversion Software version, may be empty
    //! \param type Sensor type name (see types in \ref sensors)
    //! \param uniqueid Globally unique ID
    //! (MAC address of the device, extended with a unique endpoint id)
    //! \param manufacturername Name of the device manufacturer
    CreateSensor(const std::string& name, const std::string& modelid, const std::string& swversion,
        const std::string& type, const std::string& uniqueid, const std::string& manufacturername);

    //! \brief Set state object
    //! \param state Sensor state, contents depend on the type.
    //! \returns this object for chaining calls
    CreateSensor& setState(const nlohmann::json& state);
    //! \brief Set config object
    //! \param config Sensor config, configs depend on the type. See getters in Sensor for examples.
    //! \returns this object for chaining calls
    CreateSensor& setConfig(const nlohmann::json& config);
    //! \brief Enable recycling, delete automatically when not referenced
    //! \returns this object for chaining calls
    CreateSensor& setRecycle(bool recycle);

    //! \brief Get request to create the sensor
    //! \returns JSON request for a POST to create the new sensor
    nlohmann::json getRequest() const;

protected:
    nlohmann::json request;
};

//! \brief Classes for specific sensor types
//!
//! Classes should have a typeStr member with the type name.
namespace sensors
{
//! \brief Daylight sensor to detect sunrise and sunset
//!
//! Every bridge has a daylight sensor always available.
class DaylightSensor : public BaseDevice
{
public:
    //! \brief Construct from generic sensor
    explicit DaylightSensor(Sensor sensor) : BaseDevice(std::move(sensor)) { }

    //! \brief Check if the sensor is on
    //!
    //! Sensors which are off do not change their status
    bool isOn() const;

    //! \brief Enable or disable sensor
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setOn(bool on);

    //! \brief Check whether the sensor has a battery state
    bool hasBatteryState() const;
    //! \brief Get battery state
    //! \returns Battery state in percent
    //! \throws nlohmann::json::out_of_range when sensor has no battery state.
    int getBatteryState() const;
    //! \brief Set battery state
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setBatteryState(int percent);

    //! \brief Set GPS coordinates for the calculation
    //! \param latitude Decimal latitude coordinate "DDD.DDDD{N|S}" with leading zeros ending with N or S.
    //! "none" to reset. (Empty string is null, which may be used instead of none in the future)
    //! \param longitude Longitude coordinate (same format as latitude), ending with W or E
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setCoordinates(const std::string& latitude, const std::string& longitude);
    //! \brief Check whether coordinates are configured
    //!
    //! There is no way to retrieve the configured coordinates.
    bool isConfigured() const;

    //! \brief Get time offset in minutes to sunrise
    //!
    //! The daylight is true if it is \c offset minutes after sunrise.
    int getSunriseOffset() const;
    //! \brief Set sunrise offset time
    //! \param minutes Minutes from -120 to 120
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setSunriseOffset(int minutes);

    //! \brief Get time offset in minutes to sunset
    //!
    //! The daylight is false if it is \c offset minutes after sunset.
    int getSunsetOffset() const;
    //! \brief Set sunset offset time
    //! \param minutes Minutes from -120 to 120
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setSunsetOffset(int minutes);

    //! \brief Check whether it is daylight or not
    bool isDaylight() const;

    //! \brief Get time of last status update
    //! \returns The last update time, or a time with a zero duration from epoch
    //! if the last update time is not set.
    time::AbsoluteTime getLastUpdated() const;

    //! \brief Daylight sensor type name
    static constexpr const char* typeStr = "Daylight";
};

detail::ConditionHelper<bool> makeCondition(const DaylightSensor& sensor);

template <typename SensorT, detail::void_t<decltype(std::declval<const SensorT>().getLastUpdated())>* = nullptr>
detail::ConditionHelper<time::AbsoluteTime> makeConditionLastUpdate(const SensorT& sensor)
{
    return detail::ConditionHelper<time::AbsoluteTime>(
        "/sensors/" + std::to_string(sensor.getId()) + "/state/lastupdated");
}

template <typename ButtonSensor, detail::void_t<decltype(std::declval<const ButtonSensor>().getButtonEvent())>* = nullptr>
detail::ConditionHelper<int> makeCondition(const ButtonSensor& sensor)
{
    return detail::ConditionHelper<int>(
        "/sensors/" + std::to_string(sensor.getId()) + "/state/buttonevent");
}

template <typename PresenceSensor, detail::void_t<decltype(std::declval<const PresenceSensor>().getPresence())>* = nullptr>
detail::ConditionHelper<bool> makeCondition(const PresenceSensor& sensor)
{
    return detail::ConditionHelper<bool>(
        "/sensors/" + std::to_string(sensor.getId()) + "/state/presence");
}

template <typename TemperatureSensor, detail::void_t<decltype(std::declval<const TemperatureSensor>().getPresence())>* = nullptr>
detail::ConditionHelper<int> makeCondition(const TemperatureSensor& sensor)
{
    return detail::ConditionHelper<int>(
        "/sensors/" + std::to_string(sensor.getId()) + "/state/temperature");
}

} // namespace sensors

} // namespace hueplusplus

#endif
