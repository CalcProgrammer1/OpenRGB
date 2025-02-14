/**
    \file Light.h
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

#ifndef INCLUDE_HUEPLUSPLUS_HUE_LIGHT_H
#define INCLUDE_HUEPLUSPLUS_HUE_LIGHT_H

#include <memory>

#include "APICache.h"
#include "BaseDevice.h"
#include "BrightnessStrategy.h"
#include "ColorHueStrategy.h"
#include "ColorTemperatureStrategy.h"
#include "HueCommandAPI.h"
#include "StateTransaction.h"

#include <nlohmann/json.hpp>

namespace hueplusplus
{

//! enum that specifies the color type of all HueLights
enum class ColorType
{
    UNDEFINED, //!< ColorType for this light is unknown or undefined
    NONE, //!< light has no specific ColorType
    GAMUT_A, //!< light uses Gamut A
    GAMUT_B, //!< light uses Gamut B
    GAMUT_C, //!< light uses Gamut C
    TEMPERATURE, //!< light has color temperature control
    GAMUT_A_TEMPERATURE, //!< light uses Gamut A and has color temperature control
    GAMUT_B_TEMPERATURE, //!< light uses Gamut B and has color temperature control
    GAMUT_C_TEMPERATURE, //!< light uses Gamut C and has color temperature control
    GAMUT_OTHER, //!< light uses capabilities to specify a different gamut
    GAMUT_OTHER_TEMPERATURE //!< light uses capabilities to specify a different gamut and has color temperature control
};

//! \brief Class for Hue Light fixtures
//!
//! Provides methods to query and control lights.
class Light : public BaseDevice
{
    friend class LightFactory;
    friend class SimpleBrightnessStrategy;
    friend class SimpleColorHueStrategy;
    friend class ExtendedColorHueStrategy;
    friend class SimpleColorTemperatureStrategy;
    friend class ExtendedColorTemperatureStrategy;

public:
    //! \name General information
    ///@{

    //! \brief Const function that returns the luminaireuniqueid of the light
    //!
    //! \note Only working on bridges with versions starting at 1.9
    //! \return String containing the luminaireuniqueid or an empty string when the function is not supported
    virtual std::string getLuminaireUId() const;

    //! \brief Const function that returns the color type of the light.
    //!
    //! \return ColorType containig the color type of the light
    virtual ColorType getColorType() const;

    //! \brief Get gamut space of possible light colors
    //! \returns Used gamut, or \ref gamut::maxGamut when unknown.
    ColorGamut getColorGamut() const;

    ///@}
    //! \name Light state
    ///@{

    //! \brief Function that turns the light on.
    //!
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool on(uint8_t transition = 4);

    //! \brief Function that turns the light off.
    //!
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool off(uint8_t transition = 4);

    //! \brief Function to check whether a light is on or off
    //!
    //! \return Bool that is true, when the light is on and false, when off
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool isOn();

    //! \brief Const function to check whether a light is on or off
    //!
    //! \note This will not refresh the light state
    //! \return Bool that is true, when the light is on and false, when off
    virtual bool isOn() const;

    //! \brief Const function to check whether this light has brightness control
    //!
    //! \return Bool that is true when the light has specified abilities and false
    //! when not
    virtual bool hasBrightnessControl() const { return brightnessStrategy != nullptr; };

    //! \brief Const function to check whether this light has color temperature
    //! control
    //!
    //! \return Bool that is true when the light has specified abilities and false
    //! when not
    virtual bool hasTemperatureControl() const { return colorTemperatureStrategy != nullptr; };

    //! \brief Connst function to check whether this light has full color control
    //!
    //! \return Bool that is true when the light has specified abilities and false
    //! when not
    virtual bool hasColorControl() const { return colorHueStrategy != nullptr; };

    //! \brief Function that sets the brightness of this light.
    //!
    //! \note The brightness will only be set if the light has a reference to a
    //! specific \ref BrightnessStrategy. The brightness can range from 0 = off to
    //! 254 = fully lit.
    //! \param bri Unsigned int that specifies the brightness
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setBrightness(unsigned int bri, uint8_t transition = 4)
    {
        if (brightnessStrategy)
        {
            return brightnessStrategy->setBrightness(bri, transition, *this);
        }
        return false;
    };

    //! \brief Const function that returns the brightness of this light.
    //!
    //! \note The brightness will only be returned if the light has a reference to
    //! a specific \ref BrightnessStrategy. \note This will not refresh the light
    //! state The brightness can range from 0 = off to 254 = fully lit. \return
    //! Unsigned int that is 0 when function failed
    virtual unsigned int getBrightness() const
    {
        if (brightnessStrategy)
        {
            return brightnessStrategy->getBrightness(*this);
        }
        return 0;
    };

    //! \brief Function that returns the brightness of this light.
    //!
    //! \note The brightness will only be returned if the light has a reference to
    //! a specific \ref BrightnessStrategy. The brightness can range from 0 = off
    //! to 254 = fully lit.
    //! \return Unsigned int that is 0 when function failed
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual unsigned int getBrightness()
    {
        if (brightnessStrategy)
        {
            return brightnessStrategy->getBrightness(*this);
        }
        return 0;
    };

    //! \brief Function that sets the color temperature of this light in mired.
    //!
    //! \note The color temperature will only be set if the light has a reference
    //! to a specific \ref ColorTemperatureStrategy. The color temperature can
    //! range from 153 to 500.
    //! \param mired Unsigned int that specifies the color temperature in Mired
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setColorTemperature(unsigned int mired, uint8_t transition = 4)
    {
        if (colorTemperatureStrategy)
        {
            return colorTemperatureStrategy->setColorTemperature(mired, transition, *this);
        }
        return false;
    };

    //! \brief Const function that returns the current color temperature of the
    //! light
    //!
    //! \note The color temperature will only be returned when the light has a
    //! reference to a specific \ref ColorTemperatureStrategy.
    //! \note This will not refresh the light state
    //! The color temperature in mired ranges from 153 to 500 whereas 153 is cold
    //! and 500 is warm.
    //! \return Unsigned int representing the color temperature in mired or 0 when failed
    virtual unsigned int getColorTemperature() const
    {
        if (colorTemperatureStrategy)
        {
            return colorTemperatureStrategy->getColorTemperature(*this);
        }
        return 0;
    };

    //! \brief Function that returns the current color temperature of the light
    //!
    //! \note The color temperature will only be returned when the light has a
    //! reference to a specific \ref ColorTemperatureStrategy.
    //! Updates the lights state by calling refreshState()
    //! The color temperature in mired ranges from 153 to 500 whereas 153 is cold
    //! and 500 is warm.
    //! \return Unsigned int representing the color temperature in mired or 0 when failed
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual unsigned int getColorTemperature()
    {
        if (colorTemperatureStrategy)
        {
            return colorTemperatureStrategy->getColorTemperature(*this);
        }
        return 0;
    };

    //! \brief Function to set the color of this light with specified hue.
    //!
    //! \note The color will only be set if the light has a reference to a
    //! specific \ref ColorHueStrategy. The hue can range from 0 to 65535, whereas
    //! 65535 and 0 are red, 25500 is green and 46920 is blue.
    //! \param hue uint16_t that specifies the hue
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setColorHue(uint16_t hue, uint8_t transition = 4)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->setColorHue(hue, transition, *this);
        }
        return false;
    };

    //! \brief Function to set the color of this light with specified saturation.
    //!
    //! \note The color will only be set if the light has a reference to a
    //! specific \ref ColorHueStrategy. The saturation can range from 0 to 254,
    //! whereas 0 is least saturated (white) and 254 is most saturated.
    //! \param sat uint8_t that specifies the saturation
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setColorSaturation(uint8_t sat, uint8_t transition = 4)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->setColorSaturation(sat, transition, *this);
        }
        return false;
    };

    //! \brief Function to set the color of this light with specified hue and
    //! saturation.
    //!
    //! \note The color will only be set if the light has a reference to a
    //! specific \ref ColorHueStrategy.
    //! \param hueSat Color in hue and satuation.
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms.
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setColorHueSaturation(const HueSaturation& hueSat, uint8_t transition = 4)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->setColorHueSaturation(hueSat, transition, *this);
        }
        return false;
    };

    //! \brief Const function that returns the current color of the light as hue
    //! and saturation
    //!
    //! \note The color hue and saturation will only be returned when the light
    //! has a reference to a specific \ref ColorHueStrategy.
    //! \note This will not refresh the light state
    //! \return Current hue and saturation or {0,0} when failed
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual HueSaturation getColorHueSaturation() const
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->getColorHueSaturation(*this);
        }
        return {};
    };

    //! \brief Function that returns the current color of the light as hue and
    //! saturation
    //!
    //! \note The color hue and saturation will only be returned when the light
    //! has a reference to a specific \ref ColorHueStrategy. Updates the lights
    //! state by calling refreshState()
    //! \return Current hue and saturation or {0,0} when failed
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual HueSaturation getColorHueSaturation()
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->getColorHueSaturation(*this);
        }
        return {};
    };

    //! \brief Function to set the color of this light in CIE with specified x y.
    //!
    //! \note The color will only be set if the light has a reference to a
    //! specific \ref ColorHueStrategy. The values of x and y are ranging from 0 to 1.
    //! \param xy The color in XY and brightness
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setColorXY(const XYBrightness& xy, uint8_t transition = 4)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->setColorXY(xy, transition, *this);
        }
        return false;
    };

    //! \brief Const function that returns the current color of the light as xy
    //!
    //! \note The color x and y will only be returned when the light has a
    //! reference to a specific \ref ColorHueStrategy.
    //! \note This does not update the lights state
    //! \return XYBrightness with x, y and brightness or an empty one (all 0) when failed
    virtual XYBrightness getColorXY() const
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->getColorXY(*this);
        }
        return {};
    };

    //! \brief Function that returns the current color of the light as xy
    //!
    //! \note The color x and y will only be returned when the light has a
    //! reference to a specific \ref ColorHueStrategy.
    //! Updates the lights state by calling refreshState()
    //! \return XYBrightness with x, y and brightness or an empty one (all 0) when failed
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual XYBrightness getColorXY()
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->getColorXY(*this);
        }
        return {};
    }

    //! \brief Function to set the color of this light with red green and blue
    //! values.
    //!
    //! \note The color will only be set if the light has a reference to a
    //! specific \ref ColorHueStrategy. The values of red, green and blue are
    //! ranging from 0 to 255.
    //! \param rgb RGB color that will be mapped to the available color space
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setColorRGB(const RGB& rgb, uint8_t transition = 4)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->setColorXY(rgb.toXY(getColorGamut()), transition, *this);
        }
        return false;
    }

    //! \brief Function that lets the light perform one breath cycle.
    //!
    //! Can be used for locating a light.
    //! \return bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool alert();

    //! \brief Function that lets the light perform one breath cycle in specified
    //! color temperature.
    //!
    //! \note The breath cylce will only be performed if the light has a reference
    //! to a specific \ref ColorTemperatureStrategy.
    //! \param mired Color temperature in mired
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool alertTemperature(unsigned int mired)
    {
        if (colorTemperatureStrategy)
        {
            return colorTemperatureStrategy->alertTemperature(mired, *this);
        }
        return false;
    }

    //! \brief Function that lets the light perform one breath cycle in specified
    //! color.
    //!
    //! \note The breath cylce will only be performed if the light has a reference
    //! to a specific \ref ColorHueStrategy.
    //! \param hueSat Color in hue and saturation
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool alertHueSaturation(const HueSaturation& hueSat)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->alertHueSaturation(hueSat, *this);
        }
        return false;
    }

    //! \brief Function that lets the light perform one breath cycle in specified
    //! color.
    //!
    //! \note The breath cylce will only be performed if the light has a reference
    //! to a specific \ref ColorHueStrategy.
    //! \param xy The x,y coordinates in CIE and brightness
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool alertXY(const XYBrightness& xy)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->alertXY(xy, *this);
        }
        return false;
    }

    //! \brief Function to turn colorloop effect on/off.
    //!
    //! Notice this function will only be performed light has a reference to a
    //! specific \ref ColorHueStrategy. The colorloop effect will loop through all
    //! colors on current hue and saturation levels. Notice that none of the
    //! setter functions check whether this feature is enabled and the colorloop
    //! can only be disabled with this function or by simply calling
    //! off() and then on(), so you could
    //! alternatively call off() and then use any of the setter functions.
    //! \param on bool that enables this feature when true and disables it when false
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setColorLoop(bool on)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->setColorLoop(on, *this);
        }
        return false;
    }

    //! \brief Create a transaction for this light.
    //!
    //! The transaction can be used to change more than one value in one request.
    //! Only use the functions supported by the current light type.
    //!
    //! Example usage: \code
    //! light.transaction().setBrightness(240).setColorHue(5000).commit();
    //! \endcode
    virtual StateTransaction transaction();

    ///@}

protected:
    //! \brief Protected ctor that is used by \ref LightFactory.
    //!
    //! \param id Integer that specifies the id of this light
    //! \param commands HueCommandAPI for communication with the bridge
    //!
    //! leaves strategies unset
    Light(int id, const HueCommandAPI& commands);

    //! \brief Protected ctor that is used by \ref LightFactory.
    //!
    //! \param id Integer that specifies the id of this light
    //! \param baseCache Cache of the light list (must not be null).
    //!
    //! leaves strategies unset
    Light(int id, const std::shared_ptr<APICache>& baseCache);

    //! \brief Protected ctor that is used by \ref LightFactory, also sets
    //! strategies.
    //!
    //! \param id Integer that specifies the id of this light
    //! \param commands HueCommandAPI for communication with the bridge
    //! \param brightnessStrategy Strategy for brightness. May be nullptr.
    //! \param colorTempStrategy Strategy for color temperature. May be nullptr.
    //! \param colorHueStrategy Strategy for color hue/saturation. May be nullptr.
    //! \param refreshDuration Time between refreshing the cached state.
    //! Can be 0 to always refresh, or steady_clock::duration::max() to never refresh.
    //! \param currentState The current light state, may be null.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    Light(int id, const HueCommandAPI& commands, std::shared_ptr<const BrightnessStrategy> brightnessStrategy,
        std::shared_ptr<const ColorTemperatureStrategy> colorTempStrategy,
        std::shared_ptr<const ColorHueStrategy> colorHueStrategy, std::chrono::steady_clock::duration refreshDuration,
        const nlohmann::json& currentState);

    //! \brief Protected function that sets the brightness strategy.
    //!
    //! The strategy defines how specific commands that deal with brightness
    //! control are executed \param strat a strategy of type \ref
    //! BrightnessStrategy
    virtual void setBrightnessStrategy(std::shared_ptr<const BrightnessStrategy> strat)
    {
        brightnessStrategy = std::move(strat);
    };

    //! \brief Protected function that sets the colorTemperature strategy.
    //!
    //! The strategy defines how specific commands that deal with colortemperature
    //! control are executed \param strat a strategy of type \ref
    //! ColorTemperatureStrategy
    virtual void setColorTemperatureStrategy(std::shared_ptr<const ColorTemperatureStrategy> strat)
    {
        colorTemperatureStrategy = std::move(strat);
    };

    //! \brief Protected function that sets the colorHue strategy.
    //!
    //! The strategy defines how specific commands that deal with color control
    //! are executed \param strat a strategy of type \ref ColorHueStrategy
    virtual void setColorHueStrategy(std::shared_ptr<const ColorHueStrategy> strat)
    {
        colorHueStrategy = std::move(strat);
    };

protected:
    ColorType colorType; //!< holds the \ref ColorType of the light

    std::shared_ptr<const BrightnessStrategy>
        brightnessStrategy; //!< holds a reference to the strategy that handles brightness commands
    std::shared_ptr<const ColorTemperatureStrategy>
        colorTemperatureStrategy; //!< holds a reference to the strategy that handles colortemperature commands
    std::shared_ptr<const ColorHueStrategy>
        colorHueStrategy; //!< holds a reference to the strategy that handles all color commands
};
} // namespace hueplusplus

#endif
