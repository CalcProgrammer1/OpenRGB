/**
    \file ResourceList.h
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

#ifndef INCLUDE_HUEPLUSPLUS_RESOURCE_LIST_H
#define INCLUDE_HUEPLUSPLUS_RESOURCE_LIST_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "APICache.h"
#include "HueException.h"
#include "NewDeviceList.h"
#include "Utils.h"

namespace hueplusplus
{
//! \brief Handles a list of a certain API resource
//! \tparam Resource Resource type that is in the list
//! \tparam IdT Type of the resource id. int or std::string
//!
//! The resources are assumed to be in an object with ids as keys.
//! The Resource class needs a constructor that accepts \c id, HueCommandAPI, \c refreshDuration and \c state;
//! otherwise a factory function needs to be provided that takes \c id, \c state 
//! and a base cache that is null when shared state is disabled.
template <typename Resource, typename IdT>
class ResourceList
{
public:
    using ResourceType = Resource;
    using IdType = IdT;
    static_assert(std::is_integral<IdType>::value || std::is_same<std::string, IdType>::value,
        "IdType must be integral or string");

    //! \brief Construct ResourceList using a base cache and optional factory function
    //! \param baseCache Base cache which holds the parent state, not nullptr
    //! \param cacheEntry Entry name of the list state in the base cache
    //! \param refreshDuration Interval between refreshing the cache
    //! \param sharedState Whether created resources should share the same base cache.
    //! \param factory Optional factory function to create Resources.
    //! Necessary if Resource is not constructible as described above.
    ResourceList(std::shared_ptr<APICache> baseCache, const std::string& cacheEntry,
        std::chrono::steady_clock::duration refreshDuration, bool sharedState = false,
        const std::function<Resource(IdType, const nlohmann::json&, const std::shared_ptr<APICache>&)>& factory
        = nullptr)
        : stateCache(std::make_shared<APICache>(baseCache, cacheEntry, refreshDuration)),
          factory(factory),
          path(stateCache->getRequestPath() + '/'),
          sharedState(sharedState)
    { }
    //! \brief Construct ResourceList with a separate cache and optional factory function
    //! \param commands HueCommandAPI for requests
    //! \param path Path of the resource list
    //! \param refreshDuration Interval between refreshing the cache
    //! \param factory Optional factory function to create Resources.
    //! Necessary if Resource is not constructible as described above.
    ResourceList(const HueCommandAPI& commands, const std::string& path,
        std::chrono::steady_clock::duration refreshDuration,
        const std::function<Resource(IdType, const nlohmann::json&, const std::shared_ptr<APICache>&)>& factory
        = nullptr)
        : stateCache(std::make_shared<APICache>(path, commands, refreshDuration, nullptr)),
          factory(factory),
          path(path + '/'),
          sharedState(false)
    { }

    //! \brief Deleted copy constructor
    ResourceList(const ResourceList&) = delete;
    //! \brief Deleted copy assignment
    ResourceList& operator=(const ResourceList&) = delete;

    //! \brief Refreshes internal state now
    void refresh() { stateCache->refresh(); }

    //! \brief Sets custom refresh interval for this list and all resources created.
    //! \param refreshDuration The new minimum duration between refreshes. May be 0 or \ref c_refreshNever.
    void setRefreshDuration(std::chrono::steady_clock::duration refreshDuration)
    {
        stateCache->setRefreshDuration(refreshDuration);
    }

    //! \brief Get all resources that exist
    //! \returns A vector of references to every Resource
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    std::vector<Resource> getAll()
    {
        nlohmann::json& state = stateCache->getValue();
        std::vector<Resource> result;
        result.reserve(state.size());
        for (auto it = state.begin(); it != state.end(); ++it)
        {
            result.emplace_back(construct(maybeStoi(it.key()), it.value()));
        }
        return result;
    }

    //! \brief Get resource specified by id
    //! \param id Identifier of the resource
    //! \returns The resource matching the id
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when id does not exist
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    Resource get(const IdType& id)
    {
        const nlohmann::json& state = stateCache->getValue();
        std::string key = maybeToString(id);
        if (!state.count(key))
        {
            throw HueException(FileInfo {__FILE__, __LINE__, __func__}, "Resource id is not valid");
        }
        return construct(id, state[key]);
    }

    //! \brief Checks whether resource with id exists
    //! \param id Identifier of the resource to check
    //! \returns true when the resource with given id exists
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    bool exists(const IdType& id) { return stateCache->getValue().count(maybeToString(id)) != 0; }

    //! \brief Checks whether resource with id exists
    //! \param id Identifier of the resource to check
    //! \returns true when the resource with given id exists
    //! \note This will not update the cache
    //! \throws HueException when the cache is empty
    bool exists(const IdType& id) const { return stateCache->getValue().count(maybeToString(id)) != 0; }

    //! \brief Removes the resource
    //! \param id Identifier of the resource to remove
    //! \returns true on success
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    //!
    //! If successful, invalidates references to the Resource removed.
    bool remove(const IdType& id)
    {
        std::string requestPath = path + maybeToString(id);
        nlohmann::json result = stateCache->getCommandAPI().DELETERequest(
            requestPath, nlohmann::json::object(), FileInfo {__FILE__, __LINE__, __func__});
        bool success = utils::safeGetMember(result, 0, "success") == requestPath + " deleted";
        return success;
    }

protected:
    //! \brief Calls std::stoi if IdType is int
    static IdType maybeStoi(const std::string& key) { return maybeStoi(key, std::is_integral<IdType> {}); }

    //! \brief Calls std::to_string if IdType is int
    static std::string maybeToString(const IdType& id) { return maybeToString(id, std::is_integral<IdType> {}); }

    //! \brief Constructs resource using factory or constructor, if available
    //! \throws HueException when factory is nullptr and Resource cannot be constructed as specified above.
    Resource construct(const IdType& id, const nlohmann::json& state)
    {
        return construct(id, state,
            std::is_constructible<Resource, IdType, HueCommandAPI, std::chrono::steady_clock::duration,
                const nlohmann::json&> {});
    }

    //! \brief Protected defaulted move constructor
    ResourceList(ResourceList&&) = default;
    //! \brief Protected defaulted move assignment
    ResourceList& operator=(ResourceList&&) = default;

private:
    // Resource is constructible
    Resource construct(const IdType& id, const nlohmann::json& state, std::true_type)
    {
        if (factory)
        {
            return factory(id, state, sharedState ? stateCache : std::shared_ptr<APICache>());
        }
        else
        {
            if (sharedState)
            {
                return Resource(id, stateCache);
            }
            else
            {
                return Resource(id, stateCache->getCommandAPI(), stateCache->getRefreshDuration(), state);
            }
        }
    }
    // Resource is not constructible
    Resource construct(const IdType& id, const nlohmann::json& state, std::false_type)
    {
        if (!factory)
        {
            throw HueException(FileInfo {__FILE__, __LINE__, __func__},
                "Resource is not constructable with default parameters, but no factory given");
        }
        return factory(id, state, sharedState ? stateCache : std::shared_ptr<APICache>());
    }

private:
    static IdType maybeStoi(const std::string& key, std::true_type) { return std::stoi(key); }
    static IdType maybeStoi(const std::string& key, std::false_type) { return key; }
    static std::string maybeToString(IdType id, std::true_type) { return std::to_string(id); }
    static std::string maybeToString(const IdType& id, std::false_type) { return id; }

protected:
    std::shared_ptr<APICache> stateCache;
    std::function<Resource(IdType, const nlohmann::json&, const std::shared_ptr<APICache>&)> factory;
    std::string path;
    bool sharedState;
};

//! \brief Handles a ResourceList of physical devices which can be searched for
//! \tparam Resource Resource type that is in the list
template <typename Resource>
class SearchableResourceList : public ResourceList<Resource, int>
{
public:
    using ResourceList<Resource, int>::ResourceList;

    //! \brief Start search for new devices
    //! \param deviceIds Serial numbers of the devices to search for (max. 10)
    //!
    //! Takes more than 40s. If many devices were found a second search command might be necessary.
    void search(const std::vector<std::string>& deviceIds = {})
    {
        std::string requestPath = this->path;
        // Remove trailing slash
        requestPath.pop_back();
        if (deviceIds.empty())
        {
            this->stateCache->getCommandAPI().POSTRequest(
                requestPath, nlohmann::json::object(), FileInfo {__FILE__, __LINE__, __func__});
        }
        else
        {
            this->stateCache->getCommandAPI().POSTRequest(
                requestPath, nlohmann::json {{"deviceid", deviceIds}}, FileInfo {__FILE__, __LINE__, __func__});
        }
    }

    //! \brief Get devices found in last search
    NewDeviceList getNewDevices() const
    {
        nlohmann::json response = this->stateCache->getCommandAPI().GETRequest(
            this->path + "new", nlohmann::json::object(), FileInfo {__FILE__, __LINE__, __func__});
        return NewDeviceList::parse(response);
    }

protected:
    //! \brief Protected defaulted move constructor
    SearchableResourceList(SearchableResourceList&&) = default;
    //! \brief Protected defaulted move assignment
    SearchableResourceList& operator=(SearchableResourceList&&) = default;
};

//! \brief Handles a ResourceList where Resources can be added by the user
//! \tparam BaseResourceList Base resource list type (ResourceList or SearchableResourceList).
//! \tparam CreateType Type that provides parameters for creation.
//! Must have a const getRequest() function returning the JSON for the POST request.
template <typename BaseResourceList, typename CreateType>
class CreateableResourceList : public BaseResourceList
{
public:
    using BaseResourceList::BaseResourceList;

    //! \brief Create a new resource
    //! \param params Parameters for the new resource
    //! \returns The id of the created resource or 0/an empty string if failed.
    //! \throws std::system_error when system or socket operations fail
    //! \throws HueException when response contains no body
    //! \throws HueAPIResponseException when response contains an error
    //! \throws nlohmann::json::parse_error when response could not be parsed
    //! \throws std::invalid_argument when IdType is int and std::stoi fails
    typename BaseResourceList::IdType create(const CreateType& params)
    {
        std::string requestPath = this->path;
        // Remove slash
        requestPath.pop_back();
        nlohmann::json response = this->stateCache->getCommandAPI().POSTRequest(
            requestPath, params.getRequest(), FileInfo {__FILE__, __LINE__, __func__});
        nlohmann::json id = utils::safeGetMember(response, 0, "success", "id");
        if (id.is_string())
        {
            std::string idStr = id.get<std::string>();
            if (idStr.find(this->path) == 0)
            {
                idStr.erase(0, this->path.size());
            }
            this->stateCache->refresh();
            return this->maybeStoi(idStr);
        }
        return typename BaseResourceList::IdType {};
    }

protected:
    //! \brief Protected defaulted move constructor
    CreateableResourceList(CreateableResourceList&&) = default;
    //! \brief Protected defaulted move assignment
    CreateableResourceList& operator=(CreateableResourceList&&) = default;
};

//! \brief Handles a group list with the special group 0
//! \tparam Resource Resource type that is in the list
//! \tparam CreateType Type that provides parameters for creation.
//! Must have a const getRequest() function returning the JSON for the POST request.
template <typename Resource, typename CreateType>
class GroupResourceList : public CreateableResourceList<ResourceList<Resource, int>, CreateType>
{
    using Base = CreateableResourceList<ResourceList<Resource, int>, CreateType>;

public:
    using Base::Base;
    //! \brief Get group, specially handles group 0
    //! \see ResourceList::get
    Resource get(const int& id)
    {
        const nlohmann::json& state = this->stateCache->getValue();
        std::string key = this->maybeToString(id);
        if (!state.count(key) && id != 0)
        {
            throw HueException(FileInfo {__FILE__, __LINE__, __func__}, "Resource id is not valid");
        }
        return this->construct(id, id == 0 ? nlohmann::json {nullptr} : state[key]);
    }
    //! \brief Get group, specially handles group 0
    //! \see ResourceList::exists
    bool exists(int id) const { return id == 0 || Base::exists(id); }

protected:
    //! \brief Protected defaulted move constructor
    GroupResourceList(GroupResourceList&&) = default;
    //! \brief Protected defaulted move assignment
    GroupResourceList& operator=(GroupResourceList&&) = default;
};
} // namespace hueplusplus

#endif
