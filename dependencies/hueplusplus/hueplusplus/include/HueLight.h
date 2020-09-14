/**
    \file HueLight.h
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

#ifndef _HUE_LIGHT_H
#define _HUE_LIGHT_H

#include <memory>

#include "BrightnessStrategy.h"
#include "ColorHueStrategy.h"
#include "ColorTemperatureStrategy.h"
#include "HueCommandAPI.h"

#include "json/json.hpp"

/*enum ModelType
{
UNDEFINED,	// undefined model
LCT001,		// Hue bulb A19, Color Gamut B, ECL
LCT007,		// Hue bulb A19, Color Gamut B, ECL
LCT010,		// Hue bulb A19, Color Gamut C, ECL
LCT014,		// Hue bulb A19, Color Gamut C, ECL

LCT002,		// Hue Spot BR30, Color Gamut B, ECL
LCT003,		// Hue Spot GU10, Color Gamut B, ECL

LCT011,		// Hue BR30, Color Gamut C, ECL

LST001,		// Hue LightStrips, Color Gamut A, CL
LST002,		// Hue LightStrips Plus, Color Gamut C, ECL

LLC010,		// Hue Living Colors Iris, Color Gamut A, CL
LLC011,		// Hue Living Colors Bloom, Color Gamut A, CL
LLC012,		// Hue Living Colors Bloom, Color Gamut A, CL
LLC006,		// Living Colors Gen3 Iris, Color Gamut A, CL, NO HUE FRIEND
LLC007,		// Living Colors Gen3 Bloom, Aura, Color Gamut A, CL, NO HUE FRIEND
LLC013,		// Disney Living Colors, Color Gamut A, CL

LWB004,		// Hue A19 Lux, Color Gamut -, DL
LWB006,		// Hue A19 Lux, Color Gamut -, DL
LWB007,		// Hue A19 Lux, Color Gamut -, DL
LWB010,		// Hue A19 Lux, Color Gamut -, DL
LWB014,		// Hue A19 Lux, Color Gamut -, DL

LLM001,		// Color Light Module, Color Gamut B, ECL
LLM010,		// Color Temperature Module, Color Gamut 2200K-6500K, CTL
LLM011,		// Color Temperature Module, Color Gamut 2200K-6500K, CTL
LLM012,		// Color Temperature Module, Color Gamut 2200K-6500K, CTL

LTW001,		// Hue Spot BR30, Color Gamut 2200K-6500K, CTL
LTW004,		// Hue Spot BR30, Color Gamut 2200K-6500K, CTL
LTW013,		// Hue Spot BR30, Color Gamut 2200K-6500K, CTL
LTW014,		// Hue Spot BR30, Color Gamut 2200K-6500K, CTL

LLC020		// Hue Go, Color Gamut C, ECL
};*/

//! enum that specifies the color type of all HueLights
enum ColorType
{
    UNDEFINED, //!< ColorType for this light is unknown or undefined
    NONE, //!< light has no specific ColorType
    GAMUT_A,
    GAMUT_B,
    GAMUT_C,
    TEMPERATURE,
    GAMUT_A_TEMPERATURE,
    GAMUT_B_TEMPERATURE,
    GAMUT_C_TEMPERATURE
};

//!
//! Class for Hue Light fixtures
//!
class HueLight
{
    friend class Hue;
    friend class SimpleBrightnessStrategy;
    friend class SimpleColorHueStrategy;
    friend class ExtendedColorHueStrategy;
    friend class SimpleColorTemperatureStrategy;
    friend class ExtendedColorTemperatureStrategy;

public:
    //! \brief std dtor
    ~HueLight() = default;

    //! \brief Function that turns the light on.
    //!
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool On(uint8_t transition = 4);

    //! \brief Function that turns the light off.
    //!
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool Off(uint8_t transition = 4);

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

    //! \brief Const function that returns the id of this light
    //!
    //! \return integer representing the light id
    virtual int getId() const;

    //! \brief Const function that returns the light type
    //!
    //! \return String containing the type
    virtual std::string getType() const;

    //! \brief Function that returns the name of the light.
    //!
    //! \return String containig the name of the light
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual std::string getName();

    //! \brief Const function that returns the name of the light.
    //!
    //! \note This will not refresh the light state
    //! \return String containig the name of the light
    virtual std::string getName() const;

    //! \brief Const function that returns the modelid of the light
    //!
    //! \return String conatining the modelid
    virtual std::string getModelId() const;

    //! \brief Const function that returns the uniqueid of the light
    //!
    //! \note Only working on bridges with versions starting at 1.4
    //! \return String containing the uniqueid or an empty string when the function is not supported
    virtual std::string getUId() const;

    //! \brief Const function that returns the manufacturername of the light
    //!
    //! \note Only working on bridges with versions starting at 1.7
    //! \return String containing the manufacturername or an empty string when the function is not supported
    virtual std::string getManufacturername() const;

    //! \brief Const function that returns the productname of the light
    //!
    //! \note Only working on bridges with versions starting at 1.24
    //! \return String containing the productname or an empty string when the function is not supported
    virtual std::string getProductname() const;

    //! \brief Const function that returns the luminaireuniqueid of the light
    //!
    //! \note Only working on bridges with versions starting at 1.9
    //! \return String containing the luminaireuniqueid or an empty string when the function is not supported
    virtual std::string getLuminaireUId() const;

    //! \brief Function that returns the software version of the light
    //!
    //! \return String containing the software version
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual std::string getSwVersion();

    //! \brief Const function that returns the software version of the light
    //!
    //! \note This will not refresh the light state
    //! \return String containing the software version
    virtual std::string getSwVersion() const;

    //! \brief Function that sets the name of the light
    //!
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setName(const std::string& name);

    //! \brief Const function that returns the color type of the light.
    //!
    //! \return ColorType containig the color type of the light
    virtual ColorType getColorType() const;

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

    //! \brief Const function that converts Kelvin to Mired.
    //!
    //! \param kelvin Unsigned integer value in Kelvin
    //! \return Unsigned integer value in Mired
    unsigned int KelvinToMired(unsigned int kelvin) const;

    //! \brief Const function that converts Mired to Kelvin.
    //!
    //! \param mired Unsigned integer value in Mired
    //! \return Unsigned integer value in Kelvin
    unsigned int MiredToKelvin(unsigned int mired) const;

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

    //! \brief Fucntion that sets the color temperature of this light in mired.
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
    //! \param hue uint16_t that specifies the hue
    //! \param sat uint8_t that specifies the saturation
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms.
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setColorHueSaturation(uint16_t hue, uint8_t sat, uint8_t transition = 4)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->setColorHueSaturation(hue, sat, transition, *this);
        }
        return false;
    };

    //! \brief Const function that returns the current color of the light as hue
    //! and saturation
    //!
    //! \note The color hue and saturation will only be returned when the light
    //! has a reference to a specific \ref ColorHueStrategy.
    //! \note This will not refresh the light state
    //! \return Pair containing the hue as first value and saturation as second value or an empty one when failed
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual std::pair<uint16_t, uint8_t> getColorHueSaturation() const
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
    //! \return Pair containing the hue as first value and saturation as second value or an empty one when failed
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual std::pair<uint16_t, uint8_t> getColorHueSaturation()
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
    //! \param x float that specifies the x coordinate in CIE
    //! \param y float that specifies the y coordinate in CIE
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setColorXY(float x, float y, uint8_t transition = 4)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->setColorXY(x, y, transition, *this);
        }
        return false;
    };

    //! \brief Const function that returns the current color of the light as xy
    //!
    //! \note The color x and y will only be returned when the light has a
    //! reference to a specific \ref ColorHueStrategy.
    //! \note This does not update the lights state
    //! \return Pair containing the x as first value and y as second value or an
    //! empty one when failed
    virtual std::pair<float, float> getColorXY() const
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
    //! \return Pair containing the x as first value and y as second value or an empty one when failed
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual std::pair<float, float> getColorXY()
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->getColorXY(*this);
        }
        return {};
    };

    //! \brief Function to set the color of this light with red green and blue
    //! values.
    //!
    //! \note The color will only be set if the light has a reference to a
    //! specific \ref ColorHueStrategy. The values of red, green and blue are
    //! ranging from 0 to 255. 
    //! \param r uint8_t that specifies the red color value
    //! \param g uint8_t that specifies the green color value
    //! \param b uint8_t that specifies the blue color value
    //! \param transition Optional parameter to set the transition from current state to new, standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool setColorRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t transition = 4)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->setColorRGB(r, g, b, transition, *this);
        }
        return false;
    };

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
    };

    //! \brief Function that lets the light perform one breath cycle in specified
    //! color.
    //!
    //! \note The breath cylce will only be performed if the light has a reference
    //! to a specific \ref ColorHueStrategy.
    //! \param hue uint16_t that specifies the hue
    //! \param sat uint8_t that specifies the saturation
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool alertHueSaturation(uint16_t hue, uint8_t sat)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->alertHueSaturation(hue, sat, *this);
        }
        return false;
    };

    //! \brief Function that lets the light perform one breath cycle in specified
    //! color.
    //!
    //! \note The breath cylce will only be performed if the light has a reference
    //! to a specific \ref ColorHueStrategy. The values of x and y are ranging
    //! from 0 to 1.
    //! \param x float that specifies the x coordinate in CIE
    //! \param y float that specifies the y coordinate in CIE
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool alertXY(float x, float y)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->alertXY(x, y, *this);
        }
        return false;
    };

    //! \brief Function that lets the light perform one breath cycle in specified
    //! color.
    //!
    //! \note The breath cylce will only be performed if the light has a reference
    //! to a specific \ref ColorHueStrategy. The values of red, green and blue are
    //! ranging from 0 to 255.
    //! \param r uint8_t that specifies the red color value
    //! \param g uint8_t that specifies the green color value
    //! \param b uint8_t that specifies the blue color value
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool alertRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        if (colorHueStrategy)
        {
            return colorHueStrategy->alertRGB(r, g, b, *this);
        }
        return false;
    };

    //! \brief Function to turn colorloop effect on/off.
    //!
    //! Notice this function will only be performed light has a reference to a
    //! specific \ref ColorHueStrategy. The colorloop effect will loop through all
    //! colors on current hue and saturation levels. Notice that none of the
    //! setter functions check whether this feature is enabled and the colorloop
    //! can only be disabled with this function or by simply calling
    //! Off()/OffNoRefresh() and then On()/OnNoRefresh(), so you could
    //! alternatively call Off() and then use any of the setter functions. 
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
    };

protected:
    //! \brief Protected ctor that is used by \ref Hue class.
    //!
    //! \param id Integer that specifies the id of this light
    //! \param commands HueCommandAPI for communication with the bridge
    //!
    //! leaves strategies unset
    HueLight(int id, const HueCommandAPI& commands);

    //! \brief Protected ctor that is used by \ref Hue class, also sets
    //! strategies.
    //!
    //! \param id Integer that specifies the id of this light
    //! \param commands HueCommandAPI for communication with the bridge
    //! \param brightnessStrategy Strategy for brightness. May be nullptr.
    //! \param colorTempStrategy Strategy for color temperature. May be nullptr.
    //! \param colorHueStrategy Strategy for color hue/saturation. May be nullptr.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    HueLight(int id, const HueCommandAPI& commands, std::shared_ptr<const BrightnessStrategy> brightnessStrategy,
        std::shared_ptr<const ColorTemperatureStrategy> colorTempStrategy,
        std::shared_ptr<const ColorHueStrategy> colorHueStrategy);

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

    //! \brief Protected function that sets the HueCommandAPI.
    //!
    //! The HueCommandAPI is used for bridge communication
    //! \param commandAPI the new HueCommandAPI
    virtual void setCommandAPI(const HueCommandAPI& commandAPI) { commands = commandAPI; };

    //! \brief Function that turns the light on without refreshing its state.
    //!
    //! \param transition Optional parameter to set the transition from current state to new standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool OnNoRefresh(uint8_t transition = 4);

    //! \brief Function that turns the light off without refreshing its state.
    //!
    //! \param transition Optional parameter to set the transition from current state to new standard is 4 = 400ms
    //! \return Bool that is true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual bool OffNoRefresh(uint8_t transition = 4);

    //! \brief Utility function to send a put request to the light.
    //!
    //! \throws nlohmann::json::parse_error if the reply could not be parsed
    //! \param request A nlohmann::json aka the request to send
    //! \param subPath A path that is appended to the uri, note it should always start with a slash ("/")
    //! \param fileInfo FileInfo from calling function for exception details.
    //! \return The parsed reply
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual nlohmann::json SendPutRequest(const nlohmann::json& request, const std::string& subPath, FileInfo fileInfo);

    //! \brief Virtual function that refreshes the \ref state of the light.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contained no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    virtual void refreshState();

protected:
    int id; //!< holds the id of the light
    nlohmann::json state; //!< holds the current state of the light updated by \ref refreshState
    ColorType colorType; //!< holds the \ref ColorType of the light

    std::shared_ptr<const BrightnessStrategy>
        brightnessStrategy; //!< holds a reference to the strategy that handles brightness commands
    std::shared_ptr<const ColorTemperatureStrategy>
        colorTemperatureStrategy; //!< holds a reference to the strategy that handles colortemperature commands
    std::shared_ptr<const ColorHueStrategy>
        colorHueStrategy; //!< holds a reference to the strategy that handles all color commands
    HueCommandAPI commands; //!< A IHttpHandler that is used to communicate with the bridge
};

#endif
