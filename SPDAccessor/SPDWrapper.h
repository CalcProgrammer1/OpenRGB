/*---------------------------------------------------------*\
| SPDWrapper.h                                              |
|                                                           |
|   Wrapper for DRAM modules using SPD information          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "SPDAccessor.h"
#include "SPDCommon.h"
#include "SPDDetector.h"

class SPDWrapper
{
  public:
    SPDWrapper(const SPDWrapper &wrapper);
    SPDWrapper(const SPDDetector &detector);
    ~SPDWrapper();

    SPDMemoryType memory_type();
    int index();
    uint16_t jedec_id();
    uint8_t manufacturer_data(uint16_t index);

  private:
    SPDAccessor *accessor = nullptr;
    uint8_t address;
    uint16_t jedec_id_val;
    SPDMemoryType mem_type;
};

/*-------------------------------------------------------------------------*\
| Helper functions for easier collection handling.                          |
\*-------------------------------------------------------------------------*/

bool is_jedec_in_slots(std::vector<SPDWrapper> &slots, uint16_t jedec_id);
std::vector<SPDWrapper*> slots_with_jedec(std::vector<SPDWrapper> &slots, uint16_t jedec_id);
