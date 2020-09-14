/**
    \file test_Main.cpp
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

#include <gtest/gtest.h>
#include <hueplusplus/LibConfig.h>

namespace
{
class TestConfig : public hueplusplus::Config
{
public:
    TestConfig()
    {
        preAlertDelay = postAlertDelay = upnpTimeout = bridgeRequestDelay = requestUsernameDelay
            = requestUsernameAttemptInterval = std::chrono::seconds(0);
    }
};

// Environment sets config to disable all delays and speed up tests
class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    void SetUp() override { hueplusplus::Config::instance() = TestConfig(); }

    void TearDown() override {}
};
} // namespace

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new Environment());
    return RUN_ALL_TESTS();
}
