/**
    \file ZLLSensors.h
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

#ifndef INCLUDE_HUEPLUSPLUS_ZLL_SENSORS_H
#define INCLUDE_HUEPLUSPLUS_ZLL_SENSORS_H

#include "Sensor.h"

namespace hueplusplus
{
namespace sensors
{
//! \brief ZigBee Green Power sensor for button presses
class ZGPSwitch : public BaseDevice
{
public:
    //! \brief Construct from generic sensor
    explicit ZGPSwitch(Sensor sensor) : BaseDevice(std::move(sensor)) { }

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

    //! \brief Get the code of the last switch event.
    //!
    //! Possible values are \ref c_button1 etc., or any other value.
    int getButtonEvent() const;

    //! \brief Code for tap button 1
    static constexpr int c_button1 = 34;
    //! \brief Code for tap button 2
    static constexpr int c_button2 = 16;
    //! \brief Code for tap button 3
    static constexpr int c_button3 = 17;
    //! \brief Code for tap button 4
    static constexpr int c_button4 = 18;

    //! \brief ZGPSwitch sensor type name
    static constexpr const char* typeStr = "ZGPSwitch";
};

//! \brief ZigBee sensor reporting button presses
class ZLLSwitch : public BaseDevice
{
public:
    //! \brief Construct from generic sensor
    explicit ZLLSwitch(Sensor sensor) : BaseDevice(std::move(sensor)) { }

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
    int getBatteryState() const;

    //! \brief Get last sent alert
    //! \note This is not cleared when the alert ends.
    Alert getLastAlert() const;
    //! \brief Send alert
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void sendAlert(Alert type);

    //! \brief Check whether the sensor is reachable
    bool isReachable() const;

    //! \brief Get the code of the last switch event.
    //!
    //! Possible values are \ref c_ON_INITIAL_PRESS etc., or any other value.
    int getButtonEvent() const;

    //! \brief Get time of last status update
    //! \returns The last update time, or a time with a zero duration from epoch
    //! if the last update time is not set.
    time::AbsoluteTime getLastUpdated() const;

    //! \brief Button 1 (ON) pressed
    static constexpr int c_ON_INITIAL_PRESS = 1000;
    //! \brief Button 1 (ON) held
    static constexpr int c_ON_HOLD = 1001;
    //! \brief Button 1 (ON) released short press
    static constexpr int c_ON_SHORT_RELEASED = 1002;
    //! \brief Button 1 (ON) released long press
    static constexpr int c_ON_LONG_RELEASED = 1003;
    //! \brief Button 2 (DIM UP) pressed
    static constexpr int c_UP_INITIAL_PRESS = 2000;
    //! \brief Button 2 (DIM UP) held
    static constexpr int c_UP_HOLD = 2001;
    //! \brief Button 2 (DIM UP) released short press
    static constexpr int c_UP_SHORT_RELEASED = 2002;
    //! \brief Button 2 (DIM UP) released long press
    static constexpr int c_UP_LONG_RELEASED = 2003;
    //! \brief Button 3 (DIM DOWN) pressed
    static constexpr int c_DOWN_INITIAL_PRESS = 3000;
    //! \brief Button 3 (DIM DOWN) held
    static constexpr int c_DOWN_HOLD = 3001;
    //! \brief Button 3 (DIM DOWN) released short press
    static constexpr int c_DOWN_SHORT_RELEASED = 3002;
    //! \brief Button 3 (DIM DOWN) released long press
    static constexpr int c_DOWN_LONG_RELEASED = 3003;
    //! \brief Button 4 (OFF) pressed
    static constexpr int c_OFF_INITIAL_PRESS = 4000;
    //! \brief Button 4 (OFF) held
    static constexpr int c_OFF_HOLD = 4001;
    //! \brief Button 4 (OFF) released short press
    static constexpr int c_OFF_SHORT_RELEASED = 4002;
    //! \brief Button 4 (OFF) released long press
    static constexpr int c_OFF_LONG_RELEASED = 4003;

    //! \brief ZLLSwitch sensor type name
    static constexpr const char* typeStr = "ZLLSwitch";
};

//! \brief Sensor detecting presence in the vicinity
class ZLLPresence : public BaseDevice
{
public:
    //! \brief Construct from generic sensor
    explicit ZLLPresence(Sensor sensor) : BaseDevice(std::move(sensor)) { }

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
    int getBatteryState() const;

    //! \brief Get last sent alert
    //! \note This is not cleared when the alert ends.
    Alert getLastAlert() const;
    //! \brief Send alert
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void sendAlert(Alert type);

    //! \brief Check whether the sensor is reachable
    bool isReachable() const;

    //! \brief Get sensor sensitivity
    int getSensitivity() const;
    //! \brief Get maximum sensitivity
    int getMaxSensitivity() const;
    //! \brief Set sensor sensitivity
    //! \param sensitivity Sensitivity from 0 to max sensitivity (inclusive)
    void setSensitivity(int sensitivity);

    //! \brief Get presence status
    bool getPresence() const;

    //! \brief Get time of last status update
    //! \returns The last update time, or a time with a zero duration from epoch
    //! if the last update time is not set.
    time::AbsoluteTime getLastUpdated() const;

    //! \brief ZLLPresence sensor type name
    static constexpr const char* typeStr = "ZLLPresence";
};

//! \brief ZigBee temperature sensor
class ZLLTemperature : public BaseDevice
{
public:
    //! \brief Construct from generic sensor
    explicit ZLLTemperature(Sensor sensor) : BaseDevice(std::move(sensor)) { }

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
    int getBatteryState() const;

    //! \brief Get last sent alert
    //! \note This is not cleared when the alert ends.
    Alert getLastAlert() const;
    //! \brief Send alert
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    void sendAlert(Alert type);

    //! \brief Check whether the sensor is reachable
    bool isReachable() const;

    //! \brief Get recorded temperature
    //! \returns Temperature in 0.01 degrees Celsius.
    int getTemperature() const;

    //! \brief Get time of last status update
    //! \returns The last update time, or a time with a zero duration from epoch
    //! if the last update time is not set.
    time::AbsoluteTime getLastUpdated() const;

    //! \brief ZLLTemperature sensor type name
    static constexpr const char* typeStr = "ZLLTemperature";
};

//! \brief ZigBee sensor detecting ambient light level
class ZLLLightLevel : public BaseDevice
{
public:
    //! \brief Construct from generic sensor
    explicit ZLLLightLevel(Sensor sensor) : BaseDevice(std::move(sensor)) { }

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
    int getBatteryState() const;

    //! \brief Check whether the sensor is reachable
    bool isReachable() const;

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
    //! \brief Check whether light level is below dark threshold
    bool isDark() const;
    //! \brief Check whether light level is above light threshold
    //!
    //! Light threshold is dark threshold + offset
    bool isDaylight() const;

    //! \brief Get time of last status update
    //! \returns The last update time, or a time with a zero duration from epoch
    //! if the last update time is not set.
    time::AbsoluteTime getLastUpdated() const;

    //! \brief ZLLLightLevel sensor type name
    static constexpr const char* typeStr = "ZLLLightLevel";
};

detail::ConditionHelper<bool> makeConditionDark(const ZLLLightLevel& sensor);
detail::ConditionHelper<bool> makeConditionDaylight(const ZLLLightLevel& sensor);
detail::ConditionHelper<int> makeConditionLightLevel(const ZLLLightLevel& sensor);
} // namespace sensors
} // namespace hueplusplus

#endif
