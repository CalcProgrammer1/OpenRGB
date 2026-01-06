/*---------------------------------------------------------*\
| RobobloqRangeMerger.cpp                                   |
|                                                           |
|   Helper for merging LED ranges for Robobloq              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RobobloqRangeMerger.h"
#include <cmath>
#include <limits>
#include <LogManager.h>


/***
 * The official application does not send a full set of LED values to the device (i.e. 71 in the
 * 34" case) but rather compresses the 71 RGB values down to exactly 34 ranges. It seems to use
 * pre-configured ranges, but we can do better by calculating the ranges to use that create the
 * least error, preserving single pixel detail.
 *
 * We use a greedy merge algorithm: initially define a 1-length range for each pixel. Try to merge
 * any 2 adjacent ranges and pick the merge that creates the least difference. Repeat until we
 * have 34.
 */
std::vector<unsigned char> MergeRobobloqRanges(const std::vector<RGBColor>& colors, int tuple_count)
{
    if(tuple_count == 0)
    {
        LOG_ERROR("[Robobloq] MergeRobobloqRanges called with tuple_count == 0");
        return {};
    }

    struct LEDRange
    {
        int start;    /* Start LED (1-indexed)             */
        int end;      /* End LED                           */
        int n;        /* Number of LEDs in range           */
        double sum_r; /* Sum of R values                   */
        double sum_g;
        double sum_b;
        double term;  /* = sum_r^2 + sum_g^2 + sum_b^2 / n */
    };

    int num_leds = (int)colors.size();
    std::vector<LEDRange> ranges;
    ranges.reserve(num_leds);

    /*-----------------------------------------------------*\
    | 1. Initialize ranges (one per pixel)                  |
    \*-----------------------------------------------------*/
    for(int i = 0; i < num_leds; i++)
    {
        LEDRange r;
        r.start = i + 1;  /* 1-based index                 */
        r.end   = i + 1;
        r.n     = 1;
        r.sum_r = RGBGetRValue(colors[i]);
        r.sum_g = RGBGetGValue(colors[i]);
        r.sum_b = RGBGetBValue(colors[i]);
        r.term  = (r.sum_r * r.sum_r + r.sum_g * r.sum_g + r.sum_b * r.sum_b);
        ranges.push_back(r);
    }

    /*-----------------------------------------------------*\
    | 2. Merge until we have tuple_count tuples             |
    \*-----------------------------------------------------*/
    while((int)ranges.size() > tuple_count)
    {
        double best_delta = std::numeric_limits<double>::max();
        int best_idx = -1;
        double best_merged_term = 0;

        /*-----------------------------------------------------*\
        | Find best adjacent pair to merge                      |
        \*-----------------------------------------------------*/

        /***
         *  Minimise Sum of Squared Errors (SSE) = sum(pixel - average)^2
         *                                       = sum(pixel^2) - sum(n*average^2)
         *
         * As sum(pixel^2) is constant, we need to maximise sum(n*average^2).
         *
         * Since average = sum / n:
         *   n * average^2 = n * (sum / n)^2 = n * (sum^2 / n^2) = sum^2 / n
         *
         * We need to maximise sum(sum_k^2/n_k) for all ranges k. We cache the
         * sum^2/n value as 'term'.
         **/
        for(size_t i = 0; i < ranges.size() - 1; i++)
        {
            const LEDRange& r1 = ranges[i];
            const LEDRange& r2 = ranges[i+1];

            double sum_r = r1.sum_r + r2.sum_r;
            double sum_g = r1.sum_g + r2.sum_g;
            double sum_b = r1.sum_b + r2.sum_b;
            int n = r1.n + r2.n;

            double term_merged = (sum_r * sum_r + sum_g * sum_g + sum_b * sum_b) / n;
            double delta = r1.term + r2.term - term_merged;

            if(delta < best_delta)
            {
                best_delta = delta;
                best_idx = (int)i;
                best_merged_term = term_merged;
            }
        }

        if(best_idx != -1)
        {
            /*---------------------------------------------*\
            | Merge best_idx and best_idx+1                 |
            \*---------------------------------------------*/
            LEDRange& r_left = ranges[best_idx];
            const LEDRange& r_right = ranges[best_idx+1];

            r_left.end = r_right.end;
            r_left.n += r_right.n;
            r_left.sum_r += r_right.sum_r;
            r_left.sum_g += r_right.sum_g;
            r_left.sum_b += r_right.sum_b;
            r_left.term = best_merged_term;

            ranges.erase(ranges.begin() + best_idx + 1);
        }
        else
        {
            /*---------------------------------------------*\
            | No merge possible                             |
            \*---------------------------------------------*/
            break;
        }
    }

    std::vector<unsigned char> color_bytes;
    color_bytes.reserve(tuple_count * 5);

    for(size_t i = 0; i < ranges.size(); i++)
    {
        const LEDRange& r = ranges[i];
        unsigned char avg_r = (unsigned char)std::round(r.sum_r / r.n);
        unsigned char avg_g = (unsigned char)std::round(r.sum_g / r.n);
        unsigned char avg_b = (unsigned char)std::round(r.sum_b / r.n);

        color_bytes.push_back((unsigned char)r.start);
        color_bytes.push_back(avg_r);
        color_bytes.push_back(avg_g);
        color_bytes.push_back(avg_b);
        color_bytes.push_back((unsigned char)r.end);
    }

    return color_bytes;
}
