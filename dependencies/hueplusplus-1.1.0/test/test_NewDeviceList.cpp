/**
    \file test_NewDeviceList.cpp
    Copyright Notice\n
    Copyright (C) 2020  Jan Rogall		- developer\n

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

#include <hueplusplus/HueException.h>
#include <hueplusplus/NewDeviceList.h>

#include <gtest/gtest.h>

using namespace hueplusplus;
using namespace testing;

TEST(NewDeviceList, Constructor)
{
    {
        NewDeviceList list("none", {});
        EXPECT_TRUE(list.getNewDevices().empty());
        EXPECT_FALSE(list.hasLastScanTime());
        EXPECT_FALSE(list.isScanActive());
        EXPECT_THROW(list.getLastScanTime(), HueException);
    }
    {
        const std::map<int, std::string> devices = {{1, "a"}, {2, "b"}, {3, "c"}};
        NewDeviceList list("active", devices);
        EXPECT_FALSE(list.hasLastScanTime());
        EXPECT_TRUE(list.isScanActive());
        EXPECT_EQ(devices, list.getNewDevices());
        EXPECT_THROW(list.getLastScanTime(), HueException);
    }
    {
        const std::string timestamp = "2020-03-01T00:10:00";
        NewDeviceList list(timestamp, {});
        EXPECT_TRUE(list.hasLastScanTime());
        EXPECT_FALSE(list.isScanActive());
        EXPECT_EQ(time::AbsoluteTime::parseUTC(timestamp).getBaseTime(), list.getLastScanTime().getBaseTime());
    }
}

TEST(NewDeviceList, parse)
{
    {
        NewDeviceList list = NewDeviceList::parse({});
        EXPECT_FALSE(list.hasLastScanTime());
        EXPECT_FALSE(list.isScanActive());
        EXPECT_TRUE(list.getNewDevices().empty());
        EXPECT_THROW(list.getLastScanTime(), HueException);
    }
    {
        const std::map<int, std::string> devices = {{1, "a"}, {2, "b"}, {3, "c"}};
        const std::string timestamp = "2020-03-01T00:10:00";
        NewDeviceList list = NewDeviceList::parse(
            {{"1", {{"name", "a"}}}, {"2", {{"name", "b"}}}, {"3", {{"name", "c"}}}, {"lastscan", timestamp}});
        EXPECT_TRUE(list.hasLastScanTime());
        EXPECT_FALSE(list.isScanActive());
        EXPECT_EQ(time::AbsoluteTime::parseUTC(timestamp).getBaseTime(), list.getLastScanTime().getBaseTime());
        EXPECT_EQ(devices, list.getNewDevices());
    }
}