/*---------------------------------------------------------*\
| RobobloqRangeMerger.h                                     |
|                                                           |
|   Helper for merging LED ranges for Robobloq              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"

std::vector<unsigned char> MergeRobobloqRanges(const std::vector<RGBColor>& colors, int tuple_count);
