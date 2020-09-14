/**
    \file mock_HueLight.h
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

#include "../hueplusplus/include/HueLight.h"
#include "../hueplusplus/include/json/json.hpp"
#include "../testhelper.h"

//! Mock Class
class MockHueLight : public HueLight
{
public:
    MockHueLight(std::shared_ptr<const IHttpHandler> handler)
        : HueLight(1, HueCommandAPI(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler)) {};

    nlohmann::json& getState() { return state; };

    MOCK_METHOD1(On, bool(uint8_t transition));

    MOCK_METHOD1(Off, bool(uint8_t transition));

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

    MOCK_METHOD1(setName, bool(std::string& name));

    MOCK_CONST_METHOD0(getColorType, ColorType());

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

    MOCK_METHOD3(setColorHueSaturation, bool(uint16_t hue, uint8_t sat, uint8_t transition));

    MOCK_CONST_METHOD0(getColorHueSaturation, std::pair<uint16_t, uint8_t>());

    MOCK_METHOD0(getColorHueSaturation, std::pair<uint16_t, uint8_t>());

    MOCK_METHOD3(setColorXY, bool(float x, float y, uint8_t transition));

    MOCK_CONST_METHOD0(getColorXY, std::pair<float, float>());

    MOCK_METHOD0(getColorXY, std::pair<float, float>());

    MOCK_METHOD4(setColorRGB, bool(uint8_t r, uint8_t g, uint8_t b, uint8_t transition));

    MOCK_METHOD0(alert, bool());

    MOCK_METHOD1(alertTemperature, bool(unsigned int mired));

    MOCK_METHOD2(alertHueSaturation, bool(uint16_t hue, uint8_t sat));

    MOCK_METHOD2(alertXY, bool(float x, float y));

    MOCK_METHOD3(alertRGB, bool(uint8_t r, uint8_t g, uint8_t b));

    MOCK_METHOD1(setColorLoop, bool(bool on));

    MOCK_METHOD1(OnNoRefresh, bool(uint8_t transition));

    MOCK_METHOD1(OffNoRefresh, bool(uint8_t transition));

    MOCK_METHOD3(
        SendPutRequest, nlohmann::json(const nlohmann::json& request, const std::string& subPath, FileInfo fileInfo));

    MOCK_METHOD0(refreshState, void());
};

#endif
