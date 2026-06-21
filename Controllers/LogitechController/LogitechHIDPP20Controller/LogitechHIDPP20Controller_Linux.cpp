/*---------------------------------------------------------*\
| LogitechHIDPP20Controller_Linux.cpp                       |
|                                                           |
|   Linux-specific path-migration and device-name lookup    |
|   for the unified Logitech HID++ 2.0 controller.          |
|                                                           |
|   Uses sysfs (/sys/class/hidraw) to find the same         |
|   physical device on a new hidraw path after a USB <->    |
|   wireless transition, and to read Centurion sub-device   |
|   friendly names from HID_NAME uevent fields.             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <cctype>
#include <cstdio>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <dirent.h>
#include "LogitechHIDPP20Controller.h"
#include "LogManager.h"

#define LOG_TAG log_tag.c_str()

std::string LogitechHIDPP20Controller::GetCenturionSubDeviceName(const std::string& path)
{
    /*--------------------------------------------------------*\
    | Centurion sub-device friendly name comes from sysfs      |
    | HID_NAME=, same field Solaar reads. The `path` arg is a  |
    | hidraw dev path like /dev/hidraw5; extract the hidrawN   |
    | basename and read /sys/class/hidraw/<basename>/device/   |
    | uevent.                                                  |
    \*--------------------------------------------------------*/
    std::string sysfs_name;
    size_t pos = path.rfind("hidraw");

    if(pos == std::string::npos)
    {
        return sysfs_name;
    }

    std::string uevent_path = "/sys/class/hidraw/" + path.substr(pos) + "/device/uevent";
    FILE* f = fopen(uevent_path.c_str(), "r");

    if(!f)
    {
        return sysfs_name;
    }

    char line[256];

    while(fgets(line, sizeof(line), f))
    {
        if(strncmp(line, "HID_NAME=", 9) == 0)
        {
            sysfs_name = line + 9;

            while(!sysfs_name.empty() && (sysfs_name.back() == '\n' || sysfs_name.back() == '\r'))
            {
                sysfs_name.pop_back();
            }

            break;
        }
    }

    fclose(f);
    return sysfs_name;
}

bool LogitechHIDPP20Controller::ScanForDevice(bool force)
{
    /*---------------------------------------------------------*\
    | Scan sysfs for a hidraw with matching unitId. Called by   |
    | the power thread either periodically (normal reactive     |
    | mode — only when device_online==false, USB/wireless       |
    | transition or physical unplug) or on demand from the      |
    | reader thread after a HID++1.0 Device Connection          |
    | notification arrives (force=true, bypasses the online     |
    | gate). Finds the device on its new connection path.       |
    |                                                           |
    | Reactive-only: we never migrate while the current path    |
    | still works. On devices like the G502 X PLUS that expose  |
    | both a USB-direct hidraw AND a wireless-via-dongle hidraw |
    | simultaneously (when paired to the receiver and plugged   |
    | in at the same time), eager migration would bounce us off |
    | a working path onto one the firmware has actively muted,  |
    | breaking control. The device signals which path is        |
    | active by returning errors/going silent on the inactive   |
    | path — our reader thread picks that up as a hid_read      |
    | failure and flips device_online=false, at which point the |
    | power thread calls this function to pick the new path.    |
    |                                                           |
    | Linux implementation: walks /sys/class/hidraw and matches |
    | on HID_UNIQ. The Windows counterpart (same class method,  |
    | different .cpp file) uses hid_enumerate + serial_number.  |
    |                                                           |
    | Match criteria:                                           |
    |   - device_online == false (caller should already ensure) |
    |   - HID_UNIQ matches our unitId                           |
    |   - Logitech VID (046D)                                   |
    |   - HID++ interface (usage_page 0xFF00, usage 2)          |
    |   - Different from our current path (device moved)        |
    \*---------------------------------------------------------*/
    if(caps.unit_id.empty() || caps.unit_id == "00000000")
    {
        return false;
    }

    /*---------------------------------------------------------*\
    | Online guard: never migrate off a working path unless     |
    | the caller explicitly forces a re-check. The normal       |
    | periodic scan path stays gated on device_online==false    |
    | so it's a no-op when everything is healthy.               |
    |                                                           |
    | The reader thread bypasses this guard (force=true) after  |
    | seeing a HID++1.0 Device Connection Status notification   |
    | from the Lightspeed receiver, which fires BEFORE the      |
    | firmware fully switches its data flow from wireless to    |
    | USB. At that moment device_online is still true (the      |
    | current path hasn't failed yet), but we want the scan to  |
    | run so we can migrate to the new path proactively.        |
    \*---------------------------------------------------------*/
    if(!force && device_online.load())
    {
        return false;
    }

    /*---------------------------------------------------------*\
    | Normalize our unitId to lowercase hex without dashes for  |
    | comparison. Sysfs HID_UNIQ varies between drivers:        |
    |   - Lightspeed virtual: "0d-12-5d-47" (dashes)            |
    |   - USB direct: "0D125D47" (no dashes)                    |
    | We strip dashes and lowercase both sides for matching.    |
    \*---------------------------------------------------------*/
    std::string target_norm;

    for(char c : caps.unit_id)
    {
        if(c != '-')
        {
            target_norm += (char)tolower(c);
        }
    }

    /*---------------------------------------------------------*\
    | Read our current PID from sysfs. Only migrate to paths    |
    | with a DIFFERENT PID — same PID means same receiver,      |
    | just a different pairing slot (e.g., stale pairing).      |
    \*---------------------------------------------------------*/
    unsigned int current_pid = 0;
    {
        std::string cur_hidraw = location.substr(location.rfind('/') + 1);
        std::string cur_uevent = "/sys/class/hidraw/" + cur_hidraw + "/device/uevent";
        std::ifstream cur_file(cur_uevent);
        std::string line;

        while(std::getline(cur_file, line))
        {
            if(line.compare(0, 7, "HID_ID=") == 0)
            {
                size_t lc = line.rfind(':');

                if(lc != std::string::npos)
                {
                    sscanf(line.c_str() + lc + 1, "%x", &current_pid);
                }

                break;
            }
        }
    }

    /*----------------------------------------------------------*\
    | Collect ALL sysfs hidraws with matching unit_id + Logitech |
    | VID + different PID. With multiple Lightspeed dongles in   |
    | the system, several hidraws can share the same HID_UNIQ:   |
    | one is the live virt-slot on the connected dongle, others  |
    | are stale virt-slots on dongles where our device is not    |
    | actually present. We have to probe each one to find out    |
    | which slot is real — sysfs alone can't tell them apart.    |
    \*----------------------------------------------------------*/
    struct Candidate
    {
        std::string  dev_path;
        unsigned int pid;
    };

    std::vector<Candidate> candidates;

    DIR* dir = opendir("/sys/class/hidraw");

    if(!dir)
    {
        return false;
    }

    struct dirent* entry;

    while((entry = readdir(dir)) != nullptr)
    {
        if(strncmp(entry->d_name, "hidraw", 6) != 0)
        {
            continue;
        }

        std::string uevent_path = "/sys/class/hidraw/" +
                                  std::string(entry->d_name) + "/device/uevent";
        std::ifstream uevent(uevent_path);

        if(!uevent.is_open())
        {
            continue;
        }

        std::string line;
        std::string hid_uniq;
        std::string hid_id;

        while(std::getline(uevent, line))
        {
            if(line.compare(0, 9, "HID_UNIQ=") == 0)
            {
                hid_uniq = line.substr(9);
            }
            else if(line.compare(0, 7, "HID_ID=") == 0)
            {
                hid_id = line.substr(7);
            }
        }

        std::string uniq_norm;

        for(char c : hid_uniq)
        {
            if(c != '-')
            {
                uniq_norm += (char)tolower(c);
            }
        }

        if(uniq_norm != target_norm)
        {
            continue;
        }

        if(hid_id.find("0000046D") == std::string::npos &&
           hid_id.find("0000046d") == std::string::npos)
        {
            continue;
        }

        std::string dev_path = "/dev/" + std::string(entry->d_name);

        if(dev_path == location)
        {
            continue;
        }

        size_t last_colon = hid_id.rfind(':');

        if(last_colon == std::string::npos)
        {
            continue;
        }

        unsigned int pid = 0;
        sscanf(hid_id.c_str() + last_colon + 1, "%x", &pid);

        if(pid == current_pid)
        {
            continue;
        }

        candidates.push_back({dev_path, pid});
    }

    closedir(dir);

    if(candidates.empty())
    {
        return false;
    }

    /*----------------------------------------------------------*\
    | Probe each candidate before committing. Two-dongle systems |
    | can expose multiple sysfs hidraws with the same HID_UNIQ:  |
    | one is the live slot, others are stale pairings that       |
    | respond with short-format UNKNOWN_DEVICE errors. Issue a   |
    | cheap IRoot GetFeature (feat 0x0001) to distinguish them.  |
    | A live slot returns a long-form response with feat_idx=0   |
    | and feat_byte matching the sub-index we asked for. A stale |
    | slot returns r[10 xx 8F 00 00 08 ...] (short error, code   |
    | 0x08 UNKNOWN_DEVICE). We accept the first candidate that   |
    | passes; the overall pending_path_check retry loop will     |
    | naturally re-probe all candidates on subsequent scans if   |
    | none pass on the current pass (e.g., mid-transition).      |
    \*----------------------------------------------------------*/
    std::string found_path;
    hid_device* found_dev  = nullptr;

    for(size_t c = 0; c < candidates.size(); c++)
    {
        const Candidate& cand = candidates[c];

        LOG_DEBUG("%s Scan: migration candidate at %s (pid=0x%04X, current=%s pid=0x%04X)",
                  LOG_TAG, cand.dev_path.c_str(), cand.pid,
                  location.c_str(), current_pid);

        /*-----------------------------------------------------*\
        | Multi-dongle caveat: hid_enumerate returns entries    |
        | for ALL dongles with this PID (046D:4099 Lightspeed   |
        | can appear several times in a single machine). Match  |
        | strictly on the sysfs candidate's dev_path so each    |
        | candidate resolves to its OWN dongle's HID++          |
        | interface — not the first match we stumble across.    |
        \*-----------------------------------------------------*/
        hid_device_info* devs      = hid_enumerate(0x046D, (uint16_t)cand.pid);
        std::string      hidpp20_path;

        for(hid_device_info* d = devs; d != nullptr; d = d->next)
        {
            LOG_TRACE("%s Scan: enumerate PID=0x%04X path=%s page=0x%04X usage=%d",
                      LOG_TAG, cand.pid, d->path, d->usage_page, d->usage);

            if(std::string(d->path) == cand.dev_path &&
               d->usage_page == 0xFF00 && d->usage == 2 &&
               std::string(d->path) != location)
            {
                hidpp20_path = d->path;
                break;
            }
        }

        hid_free_enumeration(devs);

        if(hidpp20_path.empty())
        {
            LOG_DEBUG("%s Scan: %s no matching LogitechHID++ interface in enum",
                      LOG_TAG, cand.dev_path.c_str());
            continue;
        }

        hid_device* test_dev = hid_open_path(hidpp20_path.c_str());

        if(!test_dev)
        {
            LOG_DEBUG("%s Scan: %s failed to open", LOG_TAG, hidpp20_path.c_str());
            continue;
        }

        /*------------------------------------------------------*\
        | Probe: HID++2.0 IRoot GetFeature for feat 0x0001       |
        | (IFeatureSet). Wire: w[10 FF 0000 000100]              |
        |   - report_id 0x10 (short)                             |
        |   - device_index 0xFF                                  |
        |   - feat_idx 0x00 (IRoot)                              |
        |   - address 0x00 (func 0 GetFeature, sw_id 0)          |
        |   - payload 00 01 00 (feat_id 0x0001)                  |
        |                                                        |
        | A live device returns r[11 xx 00 0X ...] — long form,  |
        | feat_idx 0x00, the address byte we sent back, and the  |
        | feature index in the payload.                          |
        | A stale slot returns r[10 xx 8F 00 00 08 ...] — short  |
        | error form. Reject anything that starts with 0x10.     |
        \*------------------------------------------------------*/
        uint8_t  probe[7]    = {0x10, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00};
        uint8_t  reply[20]   = {};
        int      write_rc    = hid_write(test_dev, probe, sizeof(probe));
        bool     probe_ok    = false;

        if(write_rc >= 0)
        {
            /*-----------------------------------------------------*\
            | Drain up to ~100ms, looking for a long-form response  |
            | matching our probe. Skip stray reports from unrelated |
            | firmware events that might be queued on the hidraw.   |
            \*-----------------------------------------------------*/
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
                              LOG_TAG, hidpp20_path.c_str(), reply[5]);
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
                  LOG_TAG, hidpp20_path.c_str(), reply[4]);

        found_path = hidpp20_path;
        found_dev  = test_dev;
        break;
    }

    if(!found_dev)
    {
        return false;
    }

    LOG_INFO("%s Device migrated: %s -> %s", LOG_TAG, location.c_str(), found_path.c_str());

    SwapHIDHandle(found_dev, found_path);
    return true;
}
