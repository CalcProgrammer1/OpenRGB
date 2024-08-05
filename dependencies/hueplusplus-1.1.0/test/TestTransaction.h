/**
    \file TestTransaction.h
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

#ifndef INCLUDE_HUEPLUSPLUS_TEST_TRANSACTION_H
#define INCLUDE_HUEPLUSPLUS_TEST_TRANSACTION_H

#include <hueplusplus/StateTransaction.h>

#include <gtest/gtest.h>

#include "testhelper.h"

#include "mocks/mock_HttpHandler.h"

class TestTransaction : public hueplusplus::StateTransaction
{
public:
    TestTransaction(hueplusplus::StateTransaction& t) : hueplusplus::StateTransaction(std::move(t)) {}

    nlohmann::json getRequest() const { return request; }
    nlohmann::json getResponse() const
    {
        nlohmann::json response;
        const std::string pathPrefix = path + '/';
        for (auto it = request.begin(); it != request.end(); ++it)
        {
            response.push_back({{"success", {{pathPrefix + it.key(), it.value()}}}});
        }
        return response;
    }
    std::string getPath() const { return path; }

    decltype(auto) expectPut(const std::shared_ptr<MockHttpHandler>& handler) const
    {
        return EXPECT_CALL(
            *handler, PUTJson("/api/" + getBridgeUsername() + path, request, getBridgeIp(), getBridgePort()));
    }
    decltype(auto) expectSuccessfulPut(const std::shared_ptr<MockHttpHandler>& handler,
        const testing::Cardinality& cardinality = testing::AtLeast(1)) const
    {
        return expectPut(handler).Times(cardinality).WillRepeatedly(testing::Return(getResponse()));
    }
};

#endif