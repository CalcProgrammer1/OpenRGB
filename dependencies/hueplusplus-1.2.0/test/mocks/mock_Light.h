/**
    \file mock_Light.h
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

#ifndef _MOCK_HUE_LIGHT_H
#define _MOCK_HUE_LIGHT_H

#include <string>
#include <vector>

#include <gmock/gmock.h>

#include "../testhelper.h"
#include "hueplusplus/Light.h"
#include <nlohmann/json.hpp>

//! Mock Class
class MockLight : public hueplusplus::Light
{
public:
    MockLight(std::shared_ptr<const hueplusplus::IHttpHandler> handler)
        : Light(1, hueplusplus::HueCommandAPI(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler), nullptr,
            nullptr, nullptr, std::chrono::steady_clock::duration::max(), nullptr)
    {
        // Set refresh duration to max, so random refreshes do not hinder the test setups
    }

    nlohmann::json& getState() { return state.getValue(); }

    MOCK_METHOD1(on, bool(uint8_t transition));

    MOCK_METHOD1(off, bool(uint8_t transition));

    MOCK_METHOD0(isOn, bool());

    MOCK_CONST_METHOD0(isOn, bool());

    MOCK_CONST_METHOD0(getId, int());

    MOCK_CONST_METHOD0(getType, std::string());

    MOCK_METHOD0(getName, std::string());

    MOCK_CONST_METHOD0(getName, std::string());

    MOCK_CONST_METHOD0(getModelId, std::string());

    MOCK_CONST_METHOD0(getUId, std::string());

    MOCK_CONST_METHOD0(getManufacturername, std::string());

    MOCK_CONST_METHOD0(getLuminaireUId, std::string());

    MOCK_METHOD0(getSwVersion, std::string());

    MOCK_CONST_METHOD0(getSwVersion, std::string());

    MOCK_METHOD1(setName, bool(const std::string& name));

    MOCK_CONST_METHOD0(getColorType, hueplusplus::ColorType());

    MOCK_CONST_METHOD0(hasBrightnessControl, bool());

    MOCK_CONST_METHOD0(hasTemperatureControl, bool());

    MOCK_CONST_METHOD0(hasColorControl, bool());

    MOCK_METHOD2(setBrightness, bool(unsigned int bri, uint8_t transition));

    MOCK_CONST_METHOD0(getBrightness, unsigned int());

    MOCK_METHOD0(getBrightness, unsigned int());

    MOCK_METHOD2(setColorTemperature, bool(unsigned int mired, uint8_t transition));

    MOCK_CONST_METHOD0(getColorTemperature, unsigned int());

    MOCK_METHOD0(getColorTemperature, unsigned int());

    MOCK_METHOD2(setColorHue, bool(uint16_t hue, uint8_t transition));

    MOCK_METHOD2(setColorSaturation, bool(uint8_t sat, uint8_t transition));

    MOCK_METHOD2(setColorHueSaturation, bool(const hueplusplus::HueSaturation& hueSat, uint8_t transition));

    MOCK_CONST_METHOD0(getColorHueSaturation, hueplusplus::HueSaturation());

    MOCK_METHOD0(getColorHueSaturation, hueplusplus::HueSaturation());

    MOCK_METHOD2(setColorXY, bool(const hueplusplus::XYBrightness& xy, uint8_t transition));

    MOCK_CONST_METHOD0(getColorXY, hueplusplus::XYBrightness());

    MOCK_METHOD0(getColorXY, hueplusplus::XYBrightness());

    MOCK_METHOD2(setColorRGB, bool(const hueplusplus::RGB& rgb, uint8_t transition));

    MOCK_METHOD0(alert, bool());

    MOCK_METHOD1(alertTemperature, bool(unsigned int mired));

    MOCK_METHOD1(alertHueSaturation, bool(const hueplusplus::HueSaturation& hueSat));

    MOCK_METHOD1(alertXY, bool(const hueplusplus::XYBrightness& xy));

    MOCK_METHOD1(setColorLoop, bool(bool on));

    MOCK_METHOD3(sendPutRequest,
        nlohmann::json(const std::string& subPath, const nlohmann::json& request, hueplusplus::FileInfo fileInfo));
};

#endif
