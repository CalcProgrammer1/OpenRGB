/*---------------------------------------------------------*\
| SPDWrapper.cpp                                            |
|                                                           |
|   Wrapper for DRAM modules using SPD information          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "SPDWrapper.h"

SPDWrapper::SPDWrapper(const SPDWrapper &wrapper)
{
    if(wrapper.accessor != nullptr)
    {
        this->accessor = wrapper.accessor->copy();
    }
    this->address = wrapper.address;
    this->mem_type = wrapper.mem_type;
}

SPDWrapper::SPDWrapper(const SPDDetector &detector)
{
    this->address = detector.spd_address();
    this->mem_type = detector.memory_type();

    // Allocate a new accessor
    this->accessor = SPDAccessor::for_memory_type(this->mem_type, detector.smbus(), this->address);
}

SPDWrapper::~SPDWrapper()
{
    delete accessor;
}

SPDMemoryType SPDWrapper::memory_type()
{
    return mem_type;
}

int SPDWrapper::index()
{
    return this->address - 0x50;
}

uint16_t SPDWrapper::jedec_id()
{
    if(accessor == nullptr)
    {
        return 0x0000;
    }
    return accessor->jedec_id();
}

uint8_t SPDWrapper::manufacturer_data(uint16_t index)
{
    if(accessor == nullptr)
    {
        return 0x00;
    }
    return accessor->manufacturer_data(index);
}

/*-------------------------------------------------------------------------*\
| Helper functions for easier collection handling.                          |
\*-------------------------------------------------------------------------*/

bool is_jedec_in_slots(std::vector<SPDWrapper> &slots, uint16_t jedec_id)
{
    for(SPDWrapper &slot : slots)
    {
        if(slot.jedec_id() == jedec_id)
        {
            return true;
        }
    }
    return false;
}

std::vector<SPDWrapper*> slots_with_jedec(std::vector<SPDWrapper> &slots, uint16_t jedec_id)
{
    std::vector<SPDWrapper*> matching_slots;

    for(SPDWrapper &slot : slots)
    {
        if(slot.jedec_id() == jedec_id)
        {
            matching_slots.push_back(&slot);
        }
    }

    return matching_slots;
}
