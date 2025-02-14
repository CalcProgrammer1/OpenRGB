/**
    \file test_UPnP.cpp
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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "iostream"
#include "testhelper.h"

#include "hueplusplus/LibConfig.h"
#include "hueplusplus/UPnP.h"
#include <nlohmann/json.hpp>
#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;

const std::vector<std::pair<std::string, std::string>> expected_uplug_dev
    = {{"http://192.168.2.1:1900/gatedesc.xml", "Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19"},
        {"http://192.168.2.116:80/description.xml", "Linux/3.14.0 UPnP/1.0 IpBridge/1.21.0"}};

TEST(UPnP, getDevices)
{
    std::shared_ptr<MockHttpHandler> handler = std::make_shared<MockHttpHandler>();
    EXPECT_CALL(*handler,
        sendMulticast("M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: "
                      "\"ssdp:discover\"\r\nMX: 5\r\nST: ssdp:all\r\n\r\n",
            "239.255.255.250", 1900, Config::instance().getUPnPTimeout()))
        .Times(1)
        .WillRepeatedly(::testing::Return(getMulticastReply()));

    UPnP uplug;
    std::vector<std::pair<std::string, std::string>> foundDevices = uplug.getDevices(handler);

    EXPECT_EQ(foundDevices, expected_uplug_dev);
}
