/**
    \file test_ResourceList.cpp
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

#include <gtest/gtest.h>

#include "testhelper.h"

#include "hueplusplus/ResourceList.h"
#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;
using namespace testing;

class TestResource
{
public:
    TestResource(int id, std::shared_ptr<APICache> baseCache) {}
    TestResource(int id, HueCommandAPI api, std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState) : id(id) { }

    void refresh(bool force = false) { }

public:
    int id;
};
class TestResourceFactory
{
public:
    void refresh(bool force = false) { }
};
class TestStringResource
{
public:
    TestStringResource(const std::string& id, std::shared_ptr<APICache> baseCache) {}
    TestStringResource(const std::string& id, HueCommandAPI api, std::chrono::steady_clock::duration refreshDuration, const nlohmann::json& currentState)
        : id(id)
    { }
    void refresh(bool force = false) { }

public:
    std::string id;
};

class TestCreateType
{
public:
    MOCK_CONST_METHOD0(getRequest, nlohmann::json());
};

TEST(ResourceList, refresh)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    const std::string path = "/resources";
    {
        ResourceList<TestResource, int> list(commands, path, std::chrono::steady_clock::duration::max());
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(2)
            .WillRepeatedly(Return(nlohmann::json::object()));
        list.refresh();
        list.refresh();
        Mock::VerifyAndClearExpectations(handler.get());
    }
    {
        auto baseCache = std::make_shared<APICache>("", commands, std::chrono::steady_clock::duration::max(), nullptr);
        ResourceList<TestResource, int> list(baseCache, "resources", std::chrono::steady_clock::duration::max());
        InSequence s;
        EXPECT_CALL(
            *handler, GETJson("/api/" + getBridgeUsername(), nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(nlohmann::json {{"resources", nlohmann::json::object()}}));
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .Times(2)
            .WillRepeatedly(Return(nlohmann::json::object()));
        list.refresh();
        list.refresh();
        list.refresh();
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST(ResourceList, get)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    const std::string path = "/resources";
    // No factory
    {
        const int id = 2;
        const nlohmann::json response = {{std::to_string(id), {{"resource", "state"}}}};
        ResourceList<TestResource, int> list(commands, path, std::chrono::steady_clock::duration::max());
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));

        TestResource r = list.get(id);
        EXPECT_EQ(id, r.id);
        TestResource r2 = list.get(id);
        EXPECT_EQ(id, r2.id);
    }
    // With factory
    {
        const int id = 2;
        const nlohmann::json state = {{"resource", "state"}};
        const nlohmann::json response = {{std::to_string(id), state}};

        MockFunction<TestResource(int, const nlohmann::json&, const std::shared_ptr<APICache>&)> factory;
        EXPECT_CALL(factory, Call(id, state, std::shared_ptr<APICache>()))
            .WillOnce(Return(TestResource(id, commands, std::chrono::steady_clock::duration::max(), nullptr)));

        ResourceList<TestResource, int> list(
            commands, path, std::chrono::steady_clock::duration::max(), factory.AsStdFunction());
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));

        TestResource r = list.get(id);
        EXPECT_EQ(id, r.id);
    }
    // String id without factory
    {
        const std::string id = "id-2";
        const nlohmann::json response = {{id, {{"resource", "state"}}}};
        ResourceList<TestStringResource, std::string> list(commands, path, std::chrono::steady_clock::duration::max());
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));

        TestStringResource r = list.get(id);
        EXPECT_EQ(id, r.id);
        TestStringResource r2 = list.get(id);
        EXPECT_EQ(id, r2.id);
    }

    {
        ResourceList<TestResourceFactory, int> list(commands, path, std::chrono::steady_clock::duration::max());

        const int id = 2;
        const nlohmann::json response = {{std::to_string(id), {{"resource", "state"}}}};
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        EXPECT_THROW(list.get(id), HueException);
    }
    {
        ResourceList<TestResourceFactory, int> list(commands, path, std::chrono::steady_clock::duration::max(),
            [](int, const nlohmann::json&, const std::shared_ptr<APICache>&) { return TestResourceFactory(); });

        const int id = 2;
        const nlohmann::json response = {{std::to_string(id), {{"resource", "state"}}}};
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        EXPECT_NO_THROW(list.get(id));
    }
}

TEST(ResourceList, exists)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    const std::string path = "/resources";
    const int id = 2;
    const nlohmann::json response = {{std::to_string(id), {{"resource", "state"}}}};
    ResourceList<TestResource, int> list(commands, path, std::chrono::steady_clock::duration::max());
    EXPECT_CALL(*handler,
        GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    list.refresh();
    EXPECT_TRUE(list.exists(id));
    EXPECT_FALSE(list.exists(4));
}

TEST(ResourceList, getAll)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    const std::string path = "/resources";

    const int id = 2;
    const nlohmann::json response = {{std::to_string(id), {{"resource", "state"}}}};
    ResourceList<TestResource, int> list(commands, path, std::chrono::steady_clock::duration::max());
    EXPECT_CALL(*handler,
        GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));

    auto resources = list.getAll();
    EXPECT_THAT(resources, ElementsAre(testing::Field("id", &TestResource::id, Eq(id))));

    const int id2 = 3;
    const nlohmann::json response2 = {{std::to_string(id), {{"r", "s"}}}, {std::to_string(id2), {{"b", "c"}}}};

    EXPECT_CALL(*handler,
        GETJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response2));
    list.refresh();
    auto resources2 = list.getAll();
    EXPECT_THAT(resources2,
        ElementsAre(testing::Field("id", &TestResource::id, Eq(id)), testing::Field("id", &TestResource::id, Eq(id2))));
}

TEST(ResourceList, remove)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    const std::string path = "/resources";
    const int id = 2;
    const std::string requestPath = path + "/" + std::to_string(id);
    const nlohmann::json response = {{{"success", requestPath + " deleted"}}};
    ResourceList<TestResource, int> list(commands, path, std::chrono::steady_clock::duration::max());
    EXPECT_CALL(*handler,
        DELETEJson(
            "/api/" + getBridgeUsername() + requestPath, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response))
        .WillOnce(Return(nlohmann::json()));
    EXPECT_TRUE(list.remove(id));
    EXPECT_FALSE(list.remove(id));
}

TEST(SearchableResourceList, search)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    const std::string path = "/resources";
    SearchableResourceList<TestResource> list(commands, path, std::chrono::steady_clock::duration::max());
    const nlohmann::json response = {{{"success", {{path, "Searching for new devices"}}}}};
    EXPECT_CALL(*handler,
        POSTJson("/api/" + getBridgeUsername() + path, nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    list.search();

    EXPECT_CALL(*handler,
        POSTJson("/api/" + getBridgeUsername() + path, nlohmann::json({{"deviceid", {"abcd", "def", "fgh"}}}),
            getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    list.search({"abcd", "def", "fgh"});
}

TEST(SearchableResourceList, getNewDevices)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    const std::string path = "/resources";
    SearchableResourceList<TestResource> list(commands, path, std::chrono::steady_clock::duration::max());
    const nlohmann::json response = {{"lastscan", "active"}, {"1", {{"name", "A"}}}};
    EXPECT_CALL(*handler,
        GETJson(
            "/api/" + getBridgeUsername() + path + "/new", nlohmann::json::object(), getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    NewDeviceList newDevices = list.getNewDevices();
    EXPECT_TRUE(newDevices.isScanActive());
    EXPECT_THAT(newDevices.getNewDevices(), ElementsAre(std::make_pair(1, "A")));
}

TEST(CreateableResourceList, create)
{
    auto handler = std::make_shared<MockHttpHandler>();
    HueCommandAPI commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler);

    const std::string path = "/resources";
    const nlohmann::json response = {{{"success", {{"id", path + "/2"}}}}};
    const nlohmann::json request = {{"name", "bla"}};
    CreateableResourceList<ResourceList<TestResource, int>, TestCreateType> list(
        commands, path, std::chrono::steady_clock::duration::max());
    EXPECT_CALL(*handler, POSTJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response))
        .WillOnce(Return(nlohmann::json()));
    EXPECT_CALL(*handler, GETJson("/api/" + getBridgeUsername() + path, _, getBridgeIp(), getBridgePort()))
        .Times(AnyNumber())
        .WillRepeatedly(Return(nlohmann::json::object()));
    TestCreateType params;
    EXPECT_CALL(params, getRequest()).Times(2).WillRepeatedly(Return(request));
    EXPECT_EQ(2, list.create(params));
    EXPECT_EQ(0, list.create(params));
}