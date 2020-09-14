/**
    \file test_BaseDevice.cpp
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
**/

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testhelper.h"

#include "hueplusplus/BaseDevice.h"
#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;
using namespace testing;

class TestDevice : public BaseDevice
{
public:
    TestDevice(int id, const HueCommandAPI& commands, const std::string& path,
        std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState)
        : BaseDevice(id, commands, path, refreshDuration, currentState)
    { }
};

class BaseDeviceTest : public Test
{
protected:
    std::shared_ptr<MockHttpHandler> handler;
    HueCommandAPI commands;
    nlohmann::json state;
    std::string path = "/test/";

protected:
    BaseDeviceTest()
        : handler(std::make_shared<MockHttpHandler>()),
          commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler),
          state({{"type", "testType"}, {"name", "Test name"}, {"swversion", "1.2.3.4"}, {"modelid", "TEST"},
              {"manufacturername", "testManuf"}, {"uniqueid", "00:00:00:00:00:00:00:00-00"},
              {"productname", "Test type"}})
    { }

    TestDevice getDevice(int id)
    {
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path + std::to_string(id), _, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(state));
        return TestDevice(id, commands, path, std::chrono::steady_clock::duration::max(), nullptr);
    }
};

TEST_F(BaseDeviceTest, getId)
{
    const int id = 1;
    EXPECT_EQ(id, getDevice(id).getId());
}

TEST_F(BaseDeviceTest, getName)
{
    EXPECT_EQ("Test name", getDevice(1).getName());
}

TEST_F(BaseDeviceTest, getType)
{
    EXPECT_EQ("testType", getDevice(1).getType());
}

TEST_F(BaseDeviceTest, getModelId)
{
    EXPECT_EQ("TEST", getDevice(1).getModelId());
}

TEST_F(BaseDeviceTest, getUId)
{
    EXPECT_EQ("00:00:00:00:00:00:00:00-00", getDevice(1).getUId());
    state.erase("uniqueid");
    EXPECT_EQ("", getDevice(1).getUId());
}

TEST_F(BaseDeviceTest, getManufacturername)
{
    EXPECT_EQ("testManuf", getDevice(1).getManufacturername());
    state.erase("manufacturername");
    EXPECT_EQ("", getDevice(1).getManufacturername());
}

TEST_F(BaseDeviceTest, getProductname)
{
    EXPECT_EQ("Test type", getDevice(1).getProductname());
    state.erase("productname");
    EXPECT_EQ("", getDevice(1).getProductname());
}

TEST_F(BaseDeviceTest, getSwVersion)
{
    EXPECT_EQ("1.2.3.4", getDevice(1).getSwVersion());
}

TEST_F(BaseDeviceTest, setName)
{
    const std::string name = "asdbsdakfl";
    const nlohmann::json request = {{"name", name}};
    const nlohmann::json response = { {{"success", {{"/lights/1/name", name}}}} };
    
    TestDevice device = getDevice(1);
    EXPECT_CALL(*handler, PUTJson("/api/" + getBridgeUsername() + path + "1/name", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    EXPECT_TRUE(device.setName(name));
}
