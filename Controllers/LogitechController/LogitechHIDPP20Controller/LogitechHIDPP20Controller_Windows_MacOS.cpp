/*---------------------------------------------------------*\
| LogitechHIDPP20Controller_Windows_MacOS.cpp               |
|                                                           |
|   Path-migration and device-name lookup for the unified   |
|   Logitech HID++ 2.0 controller on Windows and macOS.     |
|                                                           |
|   Uses hidapi's hid_enumerate + hid_device_info fields    |
|   (serial_number, product_string, product_id, usage_page) |
|   on platforms with no /sys/class/hidraw equivalent.      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <chrono>
#include <cstring>
#include <string>
#include <vector>
#include "LogitechHIDPP20Controller.h"
#include "LogManager.h"
#include "StringUtils.h"

#define LOG_TAG log_tag.c_str()

std::string LogitechHIDPP20Controller::GetCenturionSubDeviceName(const std::string& path)
{
    /*---------------------------------------------------------*\
    | On Windows, hidapi's hid_device_info carries a            |
    | product_string field (wchar_t*). Enumerate all Logitech   |
    | devices and find the one whose path matches `path`.       |
    |                                                           |
    | Caveat: Windows hidapi typically returns the parent       |
    | product string on every (interface, usage_page, usage)    |
    | entry that maps to the same USB device, so Centurion      |
    | sub-devices may share a name with the parent dongle.      |
    | That's a less specific name than Linux's HID_NAME, but    |
    | still better than "Logitech Centurion Device".            |
    \*---------------------------------------------------------*/
    std::string friendly;

    hid_device_info* devs = hid_enumerate(0x046D, 0x0000);

    for(hid_device_info* d = devs; d != nullptr; d = d->next)
    {
        if(d->path == nullptr)
        {
            continue;
        }

        if(std::string(d->path) != path)
        {
            continue;
        }

        friendly = StringUtils::wchar_to_string(d->product_string);
        break;
    }

    hid_free_enumeration(devs);
    return friendly;
}

bool LogitechHIDPP20Controller::ScanForDevice(bool force)
{
    /*---------------------------------------------------------*\
    | Scan hidapi for a Logitech device with matching unitId.   |
    | Called by the power thread either periodically (normal    |
    | reactive mode — only when device_online==false, USB/      |
    | wireless transition or physical unplug) or on demand      |
    | from the reader thread after a HID++1.0 Device            |
    | Connection notification arrives (force=true, bypasses     |
    | the online gate). Finds the device on its new             |
    | connection path.                                          |
    |                                                           |
    | Reactive-only: we never migrate while the current path    |
    | still works (same rationale as Linux — see the Linux      |
    | companion file for the full explanation).                 |
    |                                                           |
    | Windows implementation: walks hid_enumerate(046D, *) and  |
    | matches on hid_device_info::serial_number, which for      |
    | Logitech devices generally corresponds to the same        |
    | stable identity as the HID++-reported unit_id. If         |
    | serial_number matching yields nothing (hidapi may not     |
    | populate it for some Logitech devices on Windows), we     |
    | fall through to IRoot probing every candidate on the      |
    | matching usage_page/usage so we can still find the        |
    | device albeit more slowly.                                |
    \*---------------------------------------------------------*/
    if(caps.unit_id.empty() || caps.unit_id == "00000000")
    {
        return false;
    }

    if(!force && device_online.load())
    {
        return false;
    }

    std::string target_norm = StringUtils::normalize_hex_id(caps.unit_id);

    /*---------------------------------------------------------*\
    | Candidate = {path, pid, has_serial_match}. Serial matches |
    | sort first so we try the cheapest/most-likely candidate.  |
    \*---------------------------------------------------------*/
    struct Candidate
    {
        std::string  dev_path;
        unsigned int pid;
        bool         serial_match;
    };

    std::vector<Candidate> candidates;
    unsigned int           current_pid = 0;

    hid_device_info* devs = hid_enumerate(0x046D, 0x0000);

    /*---------------------------------------------------------*\
    | First pass: find the entry matching our current path and  |
    | record its product_id so we can skip same-PID candidates. |
    \*---------------------------------------------------------*/
    for(hid_device_info* d = devs; d != nullptr; d = d->next)
    {
        if(d->path != nullptr && std::string(d->path) == location)
        {
            current_pid = d->product_id;
            break;
        }
    }

    /*---------------------------------------------------------*\
    | Second pass: collect candidates that (a) aren't our       |
    | current path, (b) speak the HID++ interface, and (c) have |
    | a plausible identity match — either the serial_number     |
    | matches our unit_id, or at minimum their PID differs from |
    | ours so they can't be another slot on the same dongle.    |
    \*---------------------------------------------------------*/
    for(hid_device_info* d = devs; d != nullptr; d = d->next)
    {
        if(d->path == nullptr)
        {
            continue;
        }

        if(std::string(d->path) == location)
        {
            continue;
        }

        /*-----------------------------------------------------*\
        | Only HID++ interface (usage_page 0xFF00, usage 2).    |
        | Note: on Windows hidapi may or may not populate       |
        | usage / usage_page consistently. If either is zero,   |
        | fall through — we'll probe unconditionally in that    |
        | case rather than dropping a potential candidate.      |
        \*-----------------------------------------------------*/
        bool usage_known = (d->usage_page != 0 || d->usage != 0);

        if(usage_known && (d->usage_page != 0xFF00 || d->usage != 2))
        {
            continue;
        }

        /*-----------------------------------------------------*\
        | Compare serial_number (wchar_t*) against our unit_id. |
        | Normalize both and do an exact-match comparison.      |
        | Empty/missing serial is allowed — falls through as a  |
        | serial_match=false candidate so the probe path can    |
        | still find it via a different-PID filter.             |
        \*-----------------------------------------------------*/
        bool serial_match = false;

        if(d->serial_number != nullptr && d->serial_number[0] != L'\0')
        {
            std::string sn      = StringUtils::wchar_to_string(d->serial_number);
            std::string sn_norm = StringUtils::normalize_hex_id(sn);

            if(!sn_norm.empty() && sn_norm == target_norm)
            {
                serial_match = true;
            }
        }

        /*-----------------------------------------------------*\
        | If we have no serial match AND this path shares the   |
        | current PID, skip. Same PID with no identity evidence |
        | is probably a sibling slot on the same dongle, not a  |
        | valid migration target.                               |
        \*-----------------------------------------------------*/
        if(!serial_match && d->product_id == current_pid && current_pid != 0)
        {
            continue;
        }

        Candidate c;
        c.dev_path     = d->path;
        c.pid          = d->product_id;
        c.serial_match = serial_match;
        candidates.push_back(c);
    }

    hid_free_enumeration(devs);

    if(candidates.empty())
    {
        return false;
    }

    /*---------------------------------------------------------*\
    | Sort so serial-matched candidates get probed first.       |
    \*---------------------------------------------------------*/
    std::stable_sort(candidates.begin(), candidates.end(),
                     [](const Candidate& a, const Candidate& b)
                     {
                         return a.serial_match && !b.serial_match;
                     });

    /*---------------------------------------------------------*\
    | Probe each candidate with IRoot GetFeature feat 0x0001.   |
    | A live slot returns a long-form (0x11) response; a stale  |
    | slot returns a short-form (0x10) error. See the Linux     |
    | companion file for the wire-level explanation.            |
    \*---------------------------------------------------------*/
    std::string found_path;
    hid_device* found_dev  = nullptr;

    for(size_t c = 0; c < candidates.size(); c++)
    {
        const Candidate& cand = candidates[c];

        LOG_DEBUG("%s Scan: migration candidate at %s (pid=0x%04X, serial_match=%d)",
                  LOG_TAG, cand.dev_path.c_str(), cand.pid,
                  cand.serial_match ? 1 : 0);

        hid_device* test_dev = hid_open_path(cand.dev_path.c_str());

        if(!test_dev)
        {
            LOG_DEBUG("%s Scan: %s failed to open",
                      LOG_TAG, cand.dev_path.c_str());
            continue;
        }

        uint8_t  probe[7]  = {0x10, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00};
        uint8_t  reply[20] = {};
        int      write_rc  = hid_write(test_dev, probe, sizeof(probe));
        bool     probe_ok  = false;

        if(write_rc >= 0)
        {
            std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now() +
                                                             std::chrono::milliseconds(100);

            while(std::chrono::steady_clock::now() < deadline)
            {
                int read_rc = hid_read_timeout(test_dev, reply, sizeof(reply), 50);

                if(read_rc <= 0)
                {
                    continue;
                }

                if(reply[0] == 0x11 && reply[2] == 0x00 && reply[3] == 0x00)
                {
                    probe_ok = true;
                    break;
                }

                if(reply[0] == 0x10 && reply[2] == 0x8F)
                {
                    LOG_DEBUG("%s Scan: %s probe rejected — err=0x%02X (stale slot)",
                              LOG_TAG, cand.dev_path.c_str(), reply[5]);
                    break;
                }
            }
        }

        if(!probe_ok)
        {
            hid_close(test_dev);
            continue;
        }

        LOG_DEBUG("%s Scan: %s probe accepted (feat_idx=0x%02X)",
                  LOG_TAG, cand.dev_path.c_str(), reply[4]);

        found_path = cand.dev_path;
        found_dev  = test_dev;
        break;
    }

    if(!found_dev)
    {
        return false;
    }

    LOG_INFO("%s Device migrated: %s -> %s",
             LOG_TAG, location.c_str(), found_path.c_str());

    SwapHIDHandle(found_dev, found_path);
    return true;
}
