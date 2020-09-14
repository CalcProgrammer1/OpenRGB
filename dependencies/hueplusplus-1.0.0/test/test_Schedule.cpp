/**
    \file test_Schedule.cpp
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
#include <hueplusplus/Schedule.h>

#include "testhelper.h"

#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;
using namespace testing;

class ScheduleTest : public Test
{
protected:
    std::shared_ptr<MockHttpHandler> handler;
    HueCommandAPI commands;
    nlohmann::json scheduleState;

    ScheduleTest()
        : handler(std::make_shared<MockHttpHandler>()),
          commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler),
          scheduleState({{"name", "Schedule 1"}, {"description", "nice schedule"},
              {"command", {{"address", "/test"}, {"body", {}}, {"method", "PUT"}}}, {"created", "2020-03-03T23:00:03"},
              {"localtime", "T13:00:00/T14:00:00"}, {"status", "enabled"}, {"autodelete", false},
              {"starttime", "2020-04-01T00:00:00"}})
    {}

    void expectGetState(int id)
    {
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + "/schedules/" + std::to_string(id), nlohmann::json::object(),
                getBridgeIp(), getBridgePort()))
            .WillOnce(Return(scheduleState));
    }
};

TEST_F(ScheduleTest, Constructor)
{
    {
        const int id = 13;
        expectGetState(id);
        const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(id, schedule.getId());
        Mock::VerifyAndClearExpectations(handler.get());
    }
    {
        const int id = 0;
        expectGetState(id);
        const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(id, schedule.getId());
        Mock::VerifyAndClearExpectations(handler.get());
    }
}

TEST_F(ScheduleTest, getName)
{
    const int id = 1;
    const std::string name = "Schedule name";
    scheduleState["name"] = name;
    expectGetState(id);
    const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(name, schedule.getName());
}

TEST_F(ScheduleTest, getDescription)
{
    const int id = 1;
    const std::string description = "Schedule description";
    scheduleState["description"] = description;
    expectGetState(id);
    const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(description, schedule.getDescription());
}

TEST_F(ScheduleTest, getCommand)
{
    const int id = 1;
    const std::string addr = "/api/blabla";
    const nlohmann::json body = {{"test", "value"}};
    scheduleState["command"] = {{"address", addr}, {"body", body}, {"method", "PUT"}};
    expectGetState(id);
    const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
    hueplusplus::Action command = schedule.getCommand();
    EXPECT_EQ(addr, command.getAddress());
    EXPECT_EQ(body, command.getBody());
    EXPECT_EQ(hueplusplus::Action::Method::put, command.getMethod());
}

TEST_F(ScheduleTest, getTime)
{
    const int id = 1;
    const std::string time = "T13:00:00/T14:00:00";
    scheduleState["localtime"] = time;
    expectGetState(id);
    const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
    time::TimePattern pattern = schedule.getTime();
    EXPECT_EQ(time, pattern.toString());
}

TEST_F(ScheduleTest, getStatus)
{
    const int id = 1;
    {
        scheduleState["status"] = "enabled";
        expectGetState(id);
        const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(true, schedule.isEnabled());
    }
    {
        scheduleState["status"] = "disabled";
        expectGetState(id);
        const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
        EXPECT_EQ(false, schedule.isEnabled());
    }
}

TEST_F(ScheduleTest, getAutodelete)
{
    const int id = 1;
    const bool autodelete = true;
    scheduleState["autodelete"] = autodelete;
    expectGetState(id);
    const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(autodelete, schedule.getAutodelete());
}

TEST_F(ScheduleTest, getCreated)
{
    const int id = 1;
    const std::string created = "2020-03-03T08:20:53";
    scheduleState["created"] = created;
    expectGetState(id);
    const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(created, schedule.getCreated().toString());
}

TEST_F(ScheduleTest, getStartTime)
{
    const int id = 1;
    const std::string starttime = "2020-03-03T08:20:53";
    scheduleState["starttime"] = starttime;
    expectGetState(id);
    const Schedule schedule(id, commands, std::chrono::seconds(0), nullptr);
    EXPECT_EQ(starttime, schedule.getStartTime().toString());
}

TEST_F(ScheduleTest, setName)
{
    const int id = 1;
    expectGetState(id);
    Schedule schedule(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    const std::string name = "Test schedule";
    nlohmann::json request = {{"name", name}};
    nlohmann::json response = {{"success", {"/schedules/1/name", name}}};
    EXPECT_CALL(
        *handler, PUTJson("/api/" + getBridgeUsername() + "/schedules/1", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    schedule.setName(name);
}

TEST_F(ScheduleTest, setDescription)
{
    const int id = 1;
    expectGetState(id);
    Schedule schedule(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    const std::string description = "Test schedule description";
    nlohmann::json request = {{"description", description}};
    nlohmann::json response = {{"success", {"/schedules/1/description", description}}};
    EXPECT_CALL(
        *handler, PUTJson("/api/" + getBridgeUsername() + "/schedules/1", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    schedule.setDescription(description);
}

TEST_F(ScheduleTest, setCommand)
{
    const int id = 1;
    expectGetState(id);
    Schedule schedule(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    const hueplusplus::Action command({{"address", "abcd"}, {"body", {}}, {"method", "PUT"}});
    nlohmann::json request = {{"command", command.toJson()}};
    nlohmann::json response = {{"success", {"/schedules/1/command", command.toJson()}}};
    EXPECT_CALL(
        *handler, PUTJson("/api/" + getBridgeUsername() + "/schedules/1", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    schedule.setCommand(command);
}

TEST_F(ScheduleTest, setTime)
{
    const int id = 1;
    expectGetState(id);
    Schedule schedule(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    time::TimePattern time {time::AbsoluteVariedTime(std::chrono::system_clock::now())};
    nlohmann::json request = {{"localtime", time.toString()}};
    nlohmann::json response = {{"success", {"/schedules/1/localtime", time.toString()}}};
    EXPECT_CALL(
        *handler, PUTJson("/api/" + getBridgeUsername() + "/schedules/1", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    schedule.setTime(time);
}

TEST_F(ScheduleTest, setStatus)
{
    const int id = 1;
    expectGetState(id);
    Schedule schedule(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    {
        nlohmann::json request = {{"status", "enabled"}};
        nlohmann::json response = {{"success", {"/schedules/1/status", "enabled"}}};
        EXPECT_CALL(
            *handler, PUTJson("/api/" + getBridgeUsername() + "/schedules/1", request, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        expectGetState(id);
        schedule.setEnabled(true);
    }
    {
        nlohmann::json request = {{"status", "disabled"}};
        nlohmann::json response = {{"success", {"/schedules/1/status", "disabled"}}};
        EXPECT_CALL(
            *handler, PUTJson("/api/" + getBridgeUsername() + "/schedules/1", request, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        expectGetState(id);
        schedule.setEnabled(false);
    }
}

TEST_F(ScheduleTest, setAutodelete)
{
    const int id = 1;
    expectGetState(id);
    Schedule schedule(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    const bool autodelete = false;
    nlohmann::json request = {{"autodelete", autodelete}};
    nlohmann::json response = {{"success", {"/schedules/1/autodelete", autodelete}}};
    EXPECT_CALL(
        *handler, PUTJson("/api/" + getBridgeUsername() + "/schedules/1", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(id);
    schedule.setAutodelete(autodelete);
}

TEST(CreateSchedule, setName)
{
    const std::string name = "New schedule";
    const nlohmann::json request = {{"name", name}};
    EXPECT_EQ(request, CreateSchedule().setName(name).getRequest());
}

TEST(CreateSchedule, setDescription)
{
    const std::string description = "New schedule description";
    {
        const nlohmann::json request = {{"description", description}};
        EXPECT_EQ(request, CreateSchedule().setDescription(description).getRequest());
    }
    {
        const std::string name = "New schedule name";
        const nlohmann::json request = {{"name", name}, {"description", description}};
        EXPECT_EQ(request, CreateSchedule().setName(name).setDescription(description).getRequest());
    }
}

TEST(CreateSchedule, setCommand)
{
    const nlohmann::json commandJson = {{"address", "/api/asdf"}, {"method", "PUT"}, {"body", {}}};
    hueplusplus::Action command {commandJson};
    const nlohmann::json request = {{"command", commandJson}};
    EXPECT_EQ(request, CreateSchedule().setCommand(command).getRequest());
}

TEST(CreateSchedule, setTime)
{
    const time::AbsoluteVariedTime time(std::chrono::system_clock::now());
    const nlohmann::json request = {{"localtime", time.toString()}};
    EXPECT_EQ(request, CreateSchedule().setTime(time::TimePattern(time)).getRequest());
}

TEST(CreateSchedule, setStatus)
{
    {
        const nlohmann::json request = {{"status", "enabled"}};
        EXPECT_EQ(request, CreateSchedule().setStatus(true).getRequest());
    }
    {
        const nlohmann::json request = {{"status", "disabled"}};
        EXPECT_EQ(request, CreateSchedule().setStatus(false).getRequest());
    }
}

TEST(CreateSchedule, setAutodelete)
{
    {
        const nlohmann::json request = { {"autodelete", true} };
        EXPECT_EQ(request, CreateSchedule().setAutodelete(true).getRequest());
    }
}


TEST(CreateSchedule, setRecycle)
{
    {
        const nlohmann::json request = {{"recycle", true}};
        EXPECT_EQ(request, CreateSchedule().setRecycle(true).getRequest());
    }
}
