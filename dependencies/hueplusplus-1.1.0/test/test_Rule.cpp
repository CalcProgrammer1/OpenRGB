/**
    \file test_Rule.cpp
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

#include <hueplusplus/Rule.h>

#include <gtest/gtest.h>

#include "testhelper.h"

#include "mocks/mock_HttpHandler.h"

using namespace hueplusplus;
using namespace testing;

TEST(Condition, Constructor)
{
    const std::string address = "/api/abcd/test";
    const std::string value = "test value";
    Condition condition(address, Condition::Operator::eq, value);
    EXPECT_EQ(address, condition.getAddress());
    EXPECT_EQ(Condition::Operator::eq, condition.getOperator());
    EXPECT_EQ(value, condition.getValue());
}

TEST(Condition, toJson)
{
    Condition condition("/abcd", Condition::Operator::lt, "3");
    EXPECT_EQ(nlohmann::json({{"address", "/abcd"}, {"operator", "lt"}, {"value", "3"}}), condition.toJson());
}

TEST(Condition, parse)
{
    Condition condition = Condition::parse(nlohmann::json({{"address", "/abcd"}, {"operator", "lt"}, {"value", "3"}}));
    EXPECT_EQ("/abcd", condition.getAddress());
    EXPECT_EQ(Condition::Operator::lt, condition.getOperator());
    EXPECT_EQ("3", condition.getValue());
    EXPECT_THROW(Condition::parse(nlohmann::json({{"address", "/abcd"}, {"operator", "something"}, {"value", "3"}})),
        HueException);
}

TEST(Condition, operatorString)
{
    using Op = Condition::Operator;
    std::map<Op, std::string> values = {{Op::eq, "eq"}, {Op::gt, "gt"}, {Op::lt, "lt"}, {Op::dx, "dx"},
        {Op::ddx, "ddx"}, {Op::stable, "stable"}, {Op::notStable, "not stable"}, {Op::in, "in"}, {Op::notIn, "not in"}};

    for (const auto& pair : values)
    {
        Condition c("", pair.first, "");
        // Check that correct string is
        EXPECT_EQ(pair.second, c.toJson().at("operator"));
        EXPECT_EQ(pair.first,
            Condition::parse(nlohmann::json {{"address", "/abcd"}, {"operator", pair.second}, {"value", "3"}})
                .getOperator());
    }
}

class RuleTest : public Test
{
protected:
    std::shared_ptr<MockHttpHandler> handler;
    HueCommandAPI commands;
    nlohmann::json ruleState;

    RuleTest()
        : handler(std::make_shared<MockHttpHandler>()),
          commands(getBridgeIp(), getBridgePort(), getBridgeUsername(), handler),
          ruleState({{"name", "Rule 1"}, {"owner", "testOwner"}, {"created", "2020-06-01T10:00:00"},
              {"lasttriggered", "none"}, {"timestriggered", 0}, {"status", "enabled"},
              {"conditions", {{{"address", "testAddress"}, {"operator", "eq"}, {"value", "10"}}}},
              {"actions", {{{"address", "testAction"}, {"method", "PUT"}, {"body", {}}}}}})
    { }

    void expectGetState(int id)
    {
        EXPECT_CALL(*handler,
            GETJson("/api/" + getBridgeUsername() + "/rules/" + std::to_string(id), _, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(ruleState));
    }

    Rule getRule(int id = 1)
    {
        expectGetState(id);
        return Rule(id, commands, std::chrono::steady_clock::duration::max(), nullptr);
    }
};

TEST_F(RuleTest, getName)
{
    const std::string name = "Rule name";
    ruleState["name"] = name;
    const Rule rule = getRule();
    EXPECT_EQ(name, rule.getName());
}

TEST_F(RuleTest, setName)
{
    Rule rule = getRule();
    const std::string name = "Test rule";
    nlohmann::json request = {{"name", name}};
    nlohmann::json response = {{"success", {"/rules/1/name", name}}};
    EXPECT_CALL(*handler, PUTJson("/api/" + getBridgeUsername() + "/rules/1", request, getBridgeIp(), getBridgePort()))
        .WillOnce(Return(response));
    expectGetState(1);
    rule.setName(name);
}

TEST_F(RuleTest, getCreated)
{
    const std::string timestamp = "2020-06-01T10:00:00";
    ruleState["created"] = timestamp;
    const Rule rule = getRule();
    EXPECT_EQ(time::AbsoluteTime::parseUTC(timestamp).getBaseTime(), rule.getCreated().getBaseTime());
}

TEST_F(RuleTest, getLastTriggered)
{
    const std::string timestamp = "2020-06-01T10:00:00";
    ruleState["lasttriggered"] = timestamp;
    const Rule rule = getRule();
    EXPECT_EQ(time::AbsoluteTime::parseUTC(timestamp).getBaseTime(), rule.getLastTriggered().getBaseTime());
    ruleState["lasttriggered"] = "none";
    const Rule rule2 = getRule();
    EXPECT_EQ(std::chrono::system_clock::time_point(std::chrono::seconds(0)), rule2.getLastTriggered().getBaseTime());
}

TEST_F(RuleTest, getTimesTriggered)
{
    const int times = 20;
    ruleState["timestriggered"] = times;
    EXPECT_EQ(times, getRule().getTimesTriggered());
}

TEST_F(RuleTest, isEnabled)
{
    ruleState["status"] = "enabled";
    EXPECT_TRUE(getRule().isEnabled());
    ruleState["status"] = "disabled";
    EXPECT_FALSE(getRule().isEnabled());
}

TEST_F(RuleTest, setEnabled)
{
    Rule rule = getRule();
    {
        nlohmann::json request = {{"status", "enabled"}};
        nlohmann::json response = {{"success", {"/rules/1/status", "enabled"}}};
        EXPECT_CALL(
            *handler, PUTJson("/api/" + getBridgeUsername() + "/rules/1", request, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        expectGetState(1);
        rule.setEnabled(true);
    }
    {
        nlohmann::json request = {{"status", "disabled"}};
        nlohmann::json response = {{"success", {"/rules/1/status", "disabled"}}};
        EXPECT_CALL(
            *handler, PUTJson("/api/" + getBridgeUsername() + "/rules/1", request, getBridgeIp(), getBridgePort()))
            .WillOnce(Return(response));
        expectGetState(1);
        rule.setEnabled(false);
    }
}

TEST_F(RuleTest, getOwner)
{
    const std::string owner = "testowner";
    ruleState["owner"] = owner;
    EXPECT_EQ(owner, getRule().getOwner());
}

TEST_F(RuleTest, getConditions)
{
    std::vector<Condition> conditions
        = {Condition("/a/b/c", Condition::Operator::eq, "12"), Condition("/d/c", Condition::Operator::dx, "")};
    ruleState["conditions"] = {conditions[0].toJson(), conditions[1].toJson()};
    const std::vector<Condition> result = getRule().getConditions();
    ASSERT_EQ(2, result.size());
    EXPECT_EQ(conditions[0].toJson(), result[0].toJson());
    EXPECT_EQ(conditions[1].toJson(), result[1].toJson());
}

TEST_F(RuleTest, getActions)
{
    nlohmann::json action0 {{"address", "/a/b"}, {"method", "PUT"}, {"body", {{"value", "test"}}}};
    nlohmann::json action1 {{"address", "/c/d"}, {"method", "POST"}, {"body", {{"32", 1}}}};

    ruleState["actions"] = {action0, action1};
    const std::vector<hueplusplus::Action> result = getRule().getActions();
    ASSERT_EQ(2, result.size());
    EXPECT_EQ(action0, result[0].toJson());
    EXPECT_EQ(action1, result[1].toJson());
}

TEST_F(RuleTest, setConditions)
{
    std::vector<Condition> conditions
        = {Condition("/a/b/c", Condition::Operator::eq, "12"), Condition("/d/c", Condition::Operator::dx, "")};
    const nlohmann::json request = {{"conditions", {conditions[0].toJson(), conditions[1].toJson()}}};

    Rule rule = getRule();
    EXPECT_CALL(*handler, PUTJson("/api/" + getBridgeUsername() + "/rules/1", request, getBridgeIp(), getBridgePort()));
    expectGetState(1);
    rule.setConditions(conditions);
}

TEST_F(RuleTest, setActions)
{
    using hueplusplus::Action;
    nlohmann::json action0 {{"address", "/a/b"}, {"method", "PUT"}, {"body", {{"value", "test"}}}};
    nlohmann::json action1 {{"address", "/c/d"}, {"method", "POST"}, {"body", {{"32", 1}}}};
    const nlohmann::json request = {{"actions", {action0, action1}}};

    const std::vector<Action> actions = {Action(action0), Action(action1)};

    Rule rule = getRule();
    EXPECT_CALL(*handler, PUTJson("/api/" + getBridgeUsername() + "/rules/1", request, getBridgeIp(), getBridgePort()));
    expectGetState(1);
    rule.setActions(actions);
}

TEST(CreateRule, setName)
{
    const std::string name = "New rule";
    const nlohmann::json request = {{"conditions", nullptr}, {"actions", nullptr}, {"name", name}};
    EXPECT_EQ(request, CreateRule({}, {}).setName(name).getRequest());
}

TEST(CreateRule, setStatus)
{
    {
        const nlohmann::json request = {{"conditions", nullptr}, {"actions", nullptr}, {"status", "enabled"}};
        EXPECT_EQ(request, CreateRule({}, {}).setStatus(true).getRequest());
    }
    {
        const nlohmann::json request = {{"conditions", nullptr}, {"actions", nullptr}, {"status", "disabled"}};
        EXPECT_EQ(request, CreateRule({}, {}).setStatus(false).getRequest());
    }
}

TEST(CreateRule, Constructor)
{
    using hueplusplus::Action;
    std::vector<Condition> conditions
        = {Condition("/a/b/c", Condition::Operator::eq, "12"), Condition("/d/c", Condition::Operator::dx, "")};
    nlohmann::json action0 {{"address", "/a/b"}, {"method", "PUT"}, {"body", {{"value", "test"}}}};
    nlohmann::json action1 {{"address", "/c/d"}, {"method", "POST"}, {"body", {{"32", 1}}}};
    const std::vector<Action> actions = {Action(action0), Action(action1)};
    const nlohmann::json request
        = {{"conditions", {conditions[0].toJson(), conditions[1].toJson()}}, {"actions", {action0, action1}}};
    EXPECT_EQ(request, CreateRule(conditions, actions).getRequest());
}
