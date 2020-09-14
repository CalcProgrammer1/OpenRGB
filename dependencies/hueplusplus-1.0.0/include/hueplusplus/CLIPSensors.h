/**
    \file CLIPSensors.h
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall	- developer\n

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
 */

#ifndef INCLUDE_HUEPLUSPLUS_CLIP_SENSORS_H
#define INCLUDE_HUEPLUSPLUS_CLIP_SENSORS_H

#include "Sensor.h"

namespace hueplusplus
{
namespace sensors
{
//! \brief Common methods for CLIP sensors
class BaseCLIP : public BaseDevice
{
public:
    //! \brief Check if sensor is on
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

    //! \brief Check whether the sensor is reachable
    //! \note Reachable verification is not implemented for CLIP sensors yet
    bool isReachable() const;

    //! \brief Check whether the sensor has a URL
    bool hasURL() const;
    //! \brief Get sensor URL
    std::string getURL() const;
    //! \brief Set sensor URL
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setURL(const std::string& url);

    //! \brief Get time of last status update
    //! \returns The last update time, or a time with a zero duration from epoch
    //! if the last update time is not set.
    time::AbsoluteTime getLastUpdated() const;

protected:
    //! \brief Protected constructor to be used by subclasses
    explicit BaseCLIP(Sensor sensor) : BaseDevice(std::move(sensor)) { }
};

//! \brief CLIP sensor for button presses
class CLIPSwitch : public BaseCLIP
{
public:
    //! \brief Construct from generic sensor
    explicit CLIPSwitch(Sensor sensor) : BaseCLIP(std::move(sensor)) { }

    //! \brief Get the code of the last switch event.
    int getButtonEvent() const;
    //! \brief Set the button event code
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setButtonEvent(int code);

    //! \brief CLIPSwitch sensor type name
    static constexpr const char* typeStr = "CLIPSwitch";
};

//! \brief CLIP sensor detecting whether a contact is open or closed
class CLIPOpenClose : public BaseCLIP
{
public:
    //! \brief Construct from generic sensor
    explicit CLIPOpenClose(Sensor sensor) : BaseCLIP(std::move(sensor)) { }

    //! \brief Check whether the switch is open
    bool isOpen() const;
    //! \brief Set switch state
    //!
    //! The sensor needs to stay in a state for at least 1s.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setOpen(bool open);

    //! \brief CLIPOpenClose sensor type name
    static constexpr const char* typeStr = "CLIPOpenClose";
};

detail::ConditionHelper<bool> makeCondition(const CLIPOpenClose& sensor);

//! \brief CLIP sensor to detect presence
class CLIPPresence : public BaseCLIP
{
public:
    //! \brief Construct from generic sensor
    explicit CLIPPresence(Sensor sensor) : BaseCLIP(std::move(sensor)) { }

    //! \brief Check whether presence was detected
    bool getPresence() const;
    //! \brief Set presence state
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setPresence(bool presence);

    //! \brief CLIPPresence sensor type name
    static constexpr const char* typeStr = "CLIPPresence";
};

//! \brief CLIP sensor for temperature
class CLIPTemperature : public BaseCLIP
{
public:
    //! \brief Construct from generic sensor
    explicit CLIPTemperature(Sensor sensor) : BaseCLIP(std::move(sensor)) { }

    //! \brief Get measured temperature
    //! \returns Temperature in 0.01 degrees Celsius.
    int getTemperature() const;
    //! \brief Set temperature
    //! \param temperature Temperature in 0.01 degreese Celsius.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setTemperature(int temperature);

    //! \brief CLIPTemperature sensor type name
    static constexpr const char* typeStr = "CLIPTemperature";
};

//! \brief CLIP sensor for humidity
class CLIPHumidity : public BaseCLIP
{
public:
    //! \brief Construct from generic sensor
    explicit CLIPHumidity(Sensor sensor) : BaseCLIP(std::move(sensor)) { }

    //! \brief Get measured humidity
    //! \returns Humidity in 0.01% steps
    int getHumidity() const;
    //! \brief Set humidity
    //! \param humidity Humidity in 0.01% steps
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setHumidity(int humidity);

    //! \brief CLIPHumidity sensor type name
    static constexpr const char* typeStr = "CLIPHumidity";
};

detail::ConditionHelper<int> makeCondition(const CLIPHumidity& sensor);

//! \brief CLIP sensor for light level
class CLIPLightLevel : public BaseCLIP
{
public:
    //! \brief Construct from generic sensor
    explicit CLIPLightLevel(Sensor sensor) : BaseCLIP(std::move(sensor)) { }

    //! \brief Get threshold to detect darkness
    int getDarkThreshold() const;
    //! \brief Set threshold to detect darkness
    //! \param threshold Light level as reported by \ref getLightLevel
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setDarkThreshold(int threshold);

    //! \brief Get offset over dark threshold to detect daylight
    int getThresholdOffset() const;
    //! \brief Set offset to detect daylight
    //! \param offset Offset to dark threshold to detect daylight. Must be greater than 1.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setThresholdOffset(int offset);

    //! \brief Get measured light level
    //! \returns Light level in <code>10000*log10(lux)+1</code> (logarithmic scale)
    int getLightLevel() const;
    //! \brief Set measured light level
    //! \param level Light level in <code>10000*log10(lux)+1</code>
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setLightLevel(int level);
    //! \brief Check whether light level is below dark threshold
    bool isDark() const;
    //! \brief Check whether light level is above light threshold
    //!
    //! Light threshold is dark threshold + offset
    bool isDaylight() const;

    //! \brief CLIPLightLevel sensor type name
    static constexpr const char* typeStr = "CLIPLightLevel";
};

//! \brief CLIP sensor for a generic 3rd party sensor.
//!
//! Can be created by POST.
class CLIPGenericFlag : public BaseCLIP
{
public:
    //! \brief Construct from generic sensor
    explicit CLIPGenericFlag(Sensor sensor) : BaseCLIP(std::move(sensor)) { }

    //! \brief Get boolean flag
    bool getFlag() const;
    //! \brief Set flag
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setFlag(bool flag);

    //! \brief CLIPGenericFlag sensor type name
    static constexpr const char* typeStr = "CLIPGenericFlag";
};

detail::ConditionHelper<bool> makeCondition(const CLIPGenericFlag& sensor);

//! \brief CLIP sensor for a generic 3rd party status
//!
//! Can be created by POST.
class CLIPGenericStatus : public BaseCLIP
{
public:
    //! \brief Construct from generic sensor
    explicit CLIPGenericStatus(Sensor sensor) : BaseCLIP(std::move(sensor)) { }

    //! \brief Get sensor status
    int getStatus() const;
    //! \brief Set sensor status
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void setStatus(int status);

    //! \brief CLIPGenericStatus sensor type name
    static constexpr const char* typeStr = "CLIPGenericStatus";
};

detail::ConditionHelper<int> makeCondition(const CLIPGenericStatus& sensor);

} // namespace sensors
} // namespace hueplusplus

#endif
