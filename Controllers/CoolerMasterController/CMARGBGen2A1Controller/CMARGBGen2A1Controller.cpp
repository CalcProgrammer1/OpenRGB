/*---------------------------------------------------------*\
| CMARGBGen2A1Controller.cpp                                |
|                                                           |
|   Driver for Cooler Master ARGB Gen 2 A1 controller       |
|                                                           |
|   Morgan Guimard (morg)                       26 Jun 2022 |
|   Fabian R (kderazorback)                     11 Aug 2023 |
|   Daniel Clark (megadjc)                      14 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
#include "CMARGBGen2A1Controller.h"
#include "LogManager.h"
#include "StringUtils.h"

using timeout_milliseconds = std::chrono::duration<int, std::milli>;

CMARGBGen2A1controller::CMARGBGen2A1controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev         = dev_handle;
    location    = info.path;
    name        = dev_name;

    /*-----------------------------------------------------*\
    | The A1 Mini only exposes one physical ARGB output.    |
    | Full-size A1/A1 V2 controllers expose three.          |
    \*-----------------------------------------------------*/
    if(info.product_id == 0x01CB)
    {
        port_count = 1;
    }

    /*-----------------------------------------------------*\
    | Detection must be read-only. Do not force Custom or   |
    | Direct mode here; only discover the current topology. |
    \*-----------------------------------------------------*/
    RefreshTopology();
}

CMARGBGen2A1controller::~CMARGBGen2A1controller()
{
    if(realtime_mode.load())
    {
        ExitRealtimeMode();
    }
    else
    {
        StopRealtimeThread();
    }

    hid_close(dev);
}

std::string CMARGBGen2A1controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CMARGBGen2A1controller::GetNameString()
{
    return(name);
}

std::string CMARGBGen2A1controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string CMARGBGen2A1controller::GetFirmwareVersion()
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_FIRMWARE, CM_ARGB_GEN2_A1_READ, {}, response))
    {
        return("");
    }

    int version = response[3] | (response[4] << 8);

    std::ostringstream stream;
    stream << "0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << version;
    return(stream.str());
}

unsigned int CMARGBGen2A1controller::GetPortCount() const
{
    return(port_count);
}

const CMARGBGen2A1PortInfo& CMARGBGen2A1controller::GetPortInfo(unsigned int port) const
{
    static const CMARGBGen2A1PortInfo invalid_port{};

    if(port >= port_count)
    {
        return(invalid_port);
    }

    return(ports[port]);
}

bool CMARGBGen2A1controller::QueryPortGeneration(unsigned int port, unsigned char& generation)
{
    if(port >= port_count)
    {
        return(false);
    }

    return(ReadPortGeneration(port, generation));
}

unsigned char CMARGBGen2A1controller::PortMask(unsigned int port)
{
    return((unsigned char)(1U << port));
}

bool CMARGBGen2A1controller::RefreshTopology()
{
    bool any_valid = false;

    for(unsigned int port = 0; port < CM_ARGB_GEN2_A1_CHANNEL_COUNT; ++port)
    {
        ports[port] = CMARGBGen2A1PortInfo{};

        if(port < port_count && RefreshPort(port))
        {
            any_valid = true;
        }
    }

    return(any_valid);
}

bool CMARGBGen2A1controller::RefreshPortTopology(unsigned int port)
{
    return(RefreshPort(port));
}

bool CMARGBGen2A1controller::RefreshPort(unsigned int port)
{
    if(port >= port_count)
    {
        return(false);
    }

    CMARGBGen2A1PortInfo info;
    info.outer_mask = PortMask(port);

    if(!ReadPortGeneration(port, info.generation))
    {
        ports[port] = info;
        return(false);
    }

    info.valid = true;

    /*-----------------------------------------------------*\
    | Query only the topology command belonging to the live |
    | generation.  Probing the inactive generation is       |
    | unnecessary and may return an error.                  |
    \*-----------------------------------------------------*/
    if(info.generation == CM_ARGB_GEN2_A1_GENERATION_GEN1)
    {
        ReadGen1Geometry(port, info.gen1_device_count, info.gen1_leds_per_device);
    }
    else
    {
        /*-------------------------------------------------*\
        | A Gen2 port with no attached device may time out. |
        | That means an empty port, not a dead controller.  |
        \*-------------------------------------------------*/
        ReadGen2Topology(port, info.gen2_devices);
    }

    ports[port] = info;
    return(true);
}

bool CMARGBGen2A1controller::ReadPortGeneration(unsigned int port, unsigned char& generation)
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };
    unsigned char outer = PortMask(port);

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_GENERATION, CM_ARGB_GEN2_A1_READ, {outer}, response,
                           CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS, {outer}))
    {
        return(false);
    }

    if(response[3] != outer)
    {
        return(false);
    }

    generation = response[4];
    return(generation == CM_ARGB_GEN2_A1_GENERATION_GEN1 || generation == CM_ARGB_GEN2_A1_GENERATION_GEN2);
}

bool CMARGBGen2A1controller::ReadGen1Geometry(unsigned int port, unsigned char& device_count, unsigned char& leds_per_device)
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };
    unsigned char outer = PortMask(port);

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_GEN1_GEOMETRY, CM_ARGB_GEN2_A1_READ, {outer}, response,
                           CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS, {outer}))
    {
        return(false);
    }

    if(response[3] != outer)
    {
        return(false);
    }

    device_count    = response[4];
    leds_per_device = response[5];
    return(true);
}

bool CMARGBGen2A1controller::ReadGen2Topology(unsigned int port, std::vector<CMARGBGen2A1Subdevice>& devices)
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };
    unsigned char outer = PortMask(port);

    devices.clear();

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_GEN2_TOPOLOGY, CM_ARGB_GEN2_A1_READ, {outer}, response,
                           CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS, {outer}))
    {
        return(false);
    }

    if(response[3] != outer)
    {
        return(false);
    }

    unsigned int count = std::min<unsigned int>(response[4], CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT);

    for(unsigned int idx = 0; idx < count; ++idx)
    {
        unsigned int offset = 5 + (idx * 2);

        if(offset + 1 >= sizeof(response))
        {
            break;
        }

        CMARGBGen2A1Subdevice device;
        device.mask      = response[offset];
        device.led_count = response[offset + 1];

        if(device.mask != 0 && device.led_count != 0)
        {
            devices.push_back(device);
        }
    }

    return(true);
}

bool CMARGBGen2A1controller::ReadGen2TopologyStable(unsigned int port, std::vector<CMARGBGen2A1Subdevice>& devices)
{
    std::vector<CMARGBGen2A1Subdevice> previous;
    bool have_previous = false;
    bool have_any      = false;

    devices.clear();

    for(unsigned int attempt = 0; attempt < CM_ARGB_GEN2_A1_TOPOLOGY_POLL_COUNT; ++attempt)
    {
        std::vector<CMARGBGen2A1Subdevice> current;

        if(ReadGen2Topology(port, current))
        {
            have_any = true;
            devices  = current;

            bool same = have_previous && previous.size() == current.size();

            if(same)
            {
                for(std::size_t idx = 0; idx < current.size(); ++idx)
                {
                    if(previous[idx].mask != current[idx].mask || previous[idx].led_count != current[idx].led_count)
                    {
                        same = false;
                        break;
                    }
                }
            }

            if(same)
            {
                return(true);
            }

            previous      = current;
            have_previous = true;
        }

        if(attempt + 1 < CM_ARGB_GEN2_A1_TOPOLOGY_POLL_COUNT)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_TOPOLOGY_POLL_MS));
        }
    }

    /*-----------------------------------------------------*\
    | A Gen2 port with no attached devices can time out on  |
    | CMD07.  The generation switch itself can still have   |
    | succeeded, so an empty topology is not a fatal error. |
    \*-----------------------------------------------------*/
    return(have_any);
}

bool CMARGBGen2A1controller::SetPortGeneration(unsigned int port, unsigned char generation)
{
    if(port >= port_count || (generation != CM_ARGB_GEN2_A1_GENERATION_GEN1 && generation != CM_ARGB_GEN2_A1_GENERATION_GEN2))
    {
        return(false);
    }

    /*-----------------------------------------------------*\
    | The process-local realtime flag does not describe the |
    | controller state when OpenRGB first attaches. Ensure  |
    | any inherited runtime session is closed before a      |
    | persistent generation handoff begins.                 |
    \*-----------------------------------------------------*/
    if(!ExitRealtimeMode())
    {
        return(false);
    }

    unsigned char outer = PortMask(port);
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    /*-----------------------------------------------------*\
    | Keep the small command gap used by the controller's   |
    | normal generation handoff.                            |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_QUERY_TO_GENERATION_DELAY_MS));

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_GENERATION, CM_ARGB_GEN2_A1_WRITE, {outer, generation}, response,
                           CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS, {outer, generation}))
    {
        return(false);
    }

    if(response[3] != outer || response[4] != generation)
    {
        return(false);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_GENERATION_TO_REFRESH_DELAY_MS));

    /*-----------------------------------------------------*\
    | Refresh the selected output after the generation      |
    | write, leaving the short handoff gap used by the      |
    | controller before B2.                                 |
    \*-----------------------------------------------------*/
    memset(response, 0, sizeof(response));
    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_REFRESH, CM_ARGB_GEN2_A1_WRITE, {outer, 0x00}, response,
                           CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS, {outer}))
    {
        return(false);
    }

    if(response[3] != outer)
    {
        return(false);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_REFRESH_TO_TOPOLOGY_DELAY_MS));

    /*-----------------------------------------------------*\
    | B2 completes the generation handoff. Proceed directly |
    | to the active topology command instead of inserting   |
    | another CMD0B GET that can observe pre-handoff state. |
    \*-----------------------------------------------------*/
    CMARGBGen2A1PortInfo info;
    info.outer_mask = outer;
    info.generation = generation;
    info.valid      = true;

    if(generation == CM_ARGB_GEN2_A1_GENERATION_GEN1)
    {
        ReadGen1Geometry(port, info.gen1_device_count, info.gen1_leds_per_device);
    }
    else
    {
        ReadGen2TopologyStable(port, info.gen2_devices);
    }

    ports[port] = info;

    /*-----------------------------------------------------*\
    | Leave a short quiet interval after the topology read  |
    | before effects or realtime traffic use the new state. |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_GENERATION_READY_DELAY_MS));

    return(true);
}

bool CMARGBGen2A1controller::SetGen1Geometry(unsigned int port, unsigned char device_count, unsigned char leds_per_device)
{
    /*-----------------------------------------------------*\
    | CMD06 carries two Gen1 layout factors rather than a   |
    | literal physical-device count and LED count.          |
    \*-----------------------------------------------------*/
    if(port >= port_count || !ports[port].valid
                          || ports[port].generation != CM_ARGB_GEN2_A1_GENERATION_GEN1
                          || device_count < 1 || device_count > CM_ARGB_GEN2_A1_TARGET_MAX_LEDS
                          || leds_per_device < 1 || leds_per_device > CM_ARGB_GEN2_A1_TARGET_MAX_LEDS
                          || device_count * leds_per_device > CM_ARGB_GEN2_A1_TARGET_MAX_LEDS)
    {
        return(false);
    }

    if(!ExitRealtimeMode())
    {
        return(false);
    }

    unsigned char outer = PortMask(port);
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_GEN1_GEOMETRY, CM_ARGB_GEN2_A1_WRITE, {outer, device_count, leds_per_device},
                           response, CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS, {outer, device_count, leds_per_device}))
    {
        return(false);
    }

    ports[port].gen1_device_count    = device_count;
    ports[port].gen1_leds_per_device = leds_per_device;
    return(true);
}

bool CMARGBGen2A1controller::EnterRealtimeMode()
{
    if(realtime_mode.load())
    {
        /*-------------------------------------------------*\
        | Keep the realtime worker available while device   |
        | ownership is still uncertain after a failed OFF.  |
        \*-------------------------------------------------*/
        StartRealtimeThread();
        return(true);
    }

    /*-----------------------------------------------------*\
    | Match the captured MasterPlus realtime entry:         |
    | CMD01 ON, two global black frames, CMD01 ON, five     |
    | global black frames, then target-specific streaming.  |
    \*-----------------------------------------------------*/
    if(!SendRealtimePreamble())
    {
        return(false);
    }

    realtime_mode = true;
    StartRealtimeThread();
    return(true);
}

bool CMARGBGen2A1controller::SendRealtimePreamble()
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_REALTIME_MODE, CM_ARGB_GEN2_A1_WRITE, {0x01}, response,
                           CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS, {0x01}))
    {
        return(false);
    }

    CMARGBGen2A1RealtimeFrame black_global;
    black_global.outer_mask = CM_ARGB_GEN2_A1_CHANNEL_ALL;
    black_global.subtarget  = CM_ARGB_GEN2_A1_SUBCHANNEL_ALL;

    for(unsigned int idx = 0; idx < CM_ARGB_GEN2_A1_REALTIME_PREROLL_FIRST; ++idx)
    {
        if(!SendRealtimeFrame(black_global))
        {
            return(false);
        }
    }

    memset(response, 0, sizeof(response));

    /*-----------------------------------------------------*\
    | MasterPlus begins target traffic about 104 ms after   |
    | the second realtime enable. Preserve that phase.      |
    \*-----------------------------------------------------*/
    using clock = std::chrono::steady_clock;
    clock::time_point second_enable_start = clock::now();

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_REALTIME_MODE, CM_ARGB_GEN2_A1_WRITE, {0x01}, response,
                           CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS, {0x01}))
    {
        return(false);
    }

    for(unsigned int idx = 0; idx < CM_ARGB_GEN2_A1_REALTIME_PREROLL_SECOND; ++idx)
    {
        if(!SendRealtimeFrame(black_global))
        {
            return(false);
        }
    }

    std::this_thread::sleep_until(second_enable_start + std::chrono::milliseconds(CM_ARGB_GEN2_A1_REALTIME_SECOND_ON_TO_TARGET_MS));

    return(true);
}

bool CMARGBGen2A1controller::ExitRealtimeMode()
{
    bool was_realtime = realtime_mode.load();

    /*-----------------------------------------------------*\
    | Stop the CMD09 producer before releasing realtime.    |
    | Keep host ownership active until CMD01 OFF is acked.  |
    \*-----------------------------------------------------*/
    StopRealtimeThread();
    ClearRealtimeFrames();

    if(!was_realtime)
    {
        return(true);
    }

    /*-----------------------------------------------------*\
    | Leave a short quiet period before CMD01 OFF and retry |
    | the idempotent transition once if its ACK is missed.  |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_REALTIME_EXIT_QUIET_MS));

    for(unsigned int attempt = 0; attempt < CM_ARGB_GEN2_A1_REALTIME_EXIT_ATTEMPTS; ++attempt)
    {
        unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

        if(SendRegularCommand(CM_ARGB_GEN2_A1_CMD_REALTIME_MODE, CM_ARGB_GEN2_A1_WRITE, {0x00}, response,
                              CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS, {0x00}))
        {
            realtime_mode = false;
            return(true);
        }

        if(attempt + 1 < CM_ARGB_GEN2_A1_REALTIME_EXIT_ATTEMPTS)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_REALTIME_EXIT_QUIET_MS));
        }
    }

    return(false);
}

bool CMARGBGen2A1controller::IsRealtimeMode() const
{
    return(realtime_mode.load());
}

bool CMARGBGen2A1controller::SetRealtimeColors(const std::vector<CMARGBGen2A1RealtimeTarget>& targets)
{
    std::vector<CMARGBGen2A1RealtimeFrame> frames;
    frames.reserve(targets.size());

    for(std::size_t target_idx = 0; target_idx < targets.size(); target_idx++)
    {
        const CMARGBGen2A1RealtimeTarget& target = targets[target_idx];

        if(target.outer_mask == 0)
        {
            continue;
        }

        CMARGBGen2A1RealtimeFrame frame;
        frame.outer_mask = target.outer_mask;
        frame.subtarget  = target.subtarget;

        std::size_t count = std::min<std::size_t>(target.colors.size(), CM_ARGB_GEN2_A1_TARGET_MAX_LEDS);

        for(std::size_t idx = 0; idx < count; ++idx)
        {
            frame.data[(idx * 3) + 0] = RGBGetRValue(target.colors[idx]);
            frame.data[(idx * 3) + 1] = RGBGetGValue(target.colors[idx]);
            frame.data[(idx * 3) + 2] = RGBGetBValue(target.colors[idx]);
        }

        frames.push_back(std::move(frame));
    }

    /*-----------------------------------------------------*\
    | Replace the pending realtime snapshot atomically.     |
    | USB output remains owned by the scheduler thread.     |
    \*-----------------------------------------------------*/
    {
        std::lock_guard<std::mutex> lock(realtime_mutex);
        realtime_frames = std::move(frames);
    }

    if(!EnterRealtimeMode())
    {
        ClearRealtimeFrames();
        return(false);
    }

    return(true);
}

void CMARGBGen2A1controller::StartRealtimeThread()
{
    if(realtime_thread.joinable())
    {
        return;
    }

    realtime_stop = false;
    realtime_thread = std::thread(&CMARGBGen2A1controller::RealtimeWorker, this);
}

void CMARGBGen2A1controller::StopRealtimeThread()
{
    realtime_stop = true;
    realtime_cv.notify_all();

    if(realtime_thread.joinable())
    {
        realtime_thread.join();
    }
}

void CMARGBGen2A1controller::ClearRealtimeFrames()
{
    std::lock_guard<std::mutex> lock(realtime_mutex);
    realtime_frames.clear();
}

void CMARGBGen2A1controller::RealtimeWorker()
{
    /*-----------------------------------------------------*\
    | MasterPlus refreshes every active target once per     |
    | approximately 40 ms scheduler cycle.                  |
    \*-----------------------------------------------------*/
    using clock = std::chrono::steady_clock;
    std::chrono::milliseconds period(CM_ARGB_GEN2_A1_REALTIME_PERIOD_MS);
    clock::time_point cycle_start = clock::now();

    while(!realtime_stop.load())
    {
        std::vector<CMARGBGen2A1RealtimeFrame> frames;

        {
            std::lock_guard<std::mutex> lock(realtime_mutex);
            frames = realtime_frames;
        }

        if(realtime_mode.load() && !frames.empty())
        {
            if(!SendRealtimeCycle(frames))
            {
                LOG_INFO("[CMARGBGen2A1] realtime cycle write failed");
            }
        }

        cycle_start += period;
        clock::time_point now = clock::now();

        /*-------------------------------------------------*\
        | Rebase after a missed cycle instead of bursting.  |
        \*-------------------------------------------------*/
        if(now >= cycle_start + period)
        {
            cycle_start = now + period;
        }

        std::unique_lock<std::mutex> wait_lock(realtime_mutex);
        realtime_cv.wait_until(wait_lock, cycle_start, [this]{ return(realtime_stop.load()); });
    }
}

bool CMARGBGen2A1controller::SendRealtimeCycle(const std::vector<CMARGBGen2A1RealtimeFrame>& frames)
{
    /*-----------------------------------------------------*\
    | Keep every target frame in one uninterrupted HID      |
    | scheduler cycle.                                      |
    \*-----------------------------------------------------*/
    std::lock_guard<std::mutex> io_lock(io_mutex);

    for(std::size_t frame_idx = 0; frame_idx < frames.size(); frame_idx++)
    {
        const CMARGBGen2A1RealtimeFrame& frame = frames[frame_idx];

        if(realtime_stop.load() || !realtime_mode.load())
        {
            break;
        }

        if(!SendRealtimeFrameLocked(frame))
        {
            return(false);
        }
    }

    return(true);
}

bool CMARGBGen2A1controller::SendRealtimeFrame(const CMARGBGen2A1RealtimeFrame& frame)
{
    std::lock_guard<std::mutex> io_lock(io_mutex);
    return(SendRealtimeFrameLocked(frame));
}

bool CMARGBGen2A1controller::SendRealtimeFrameLocked(const CMARGBGen2A1RealtimeFrame& frame)
{
    unsigned char report[CM_ARGB_GEN2_A1_PACKET_LENGTH] = { 0 };

    /*-----------------------------------------------------*\
    | Packet 0: 00 09 <outer> <target> 3C + 59 RGB bytes    |
    \*-----------------------------------------------------*/
    report[1] = 0x00;
    report[2] = CM_ARGB_GEN2_A1_CMD_REALTIME_RGB;
    report[3] = frame.outer_mask;
    report[4] = frame.subtarget;
    report[5] = CM_ARGB_GEN2_A1_TARGET_MAX_LEDS;
    std::copy_n(frame.data, 59, report + 6);

    if(hid_write(dev, report, sizeof(report)) != CM_ARGB_GEN2_A1_PACKET_LENGTH)
    {
        return(false);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_REALTIME_PACKET_GAP_MS));

    /*-----------------------------------------------------*\
    | Packet 1: 01 09 + 62 continuation bytes               |
    \*-----------------------------------------------------*/
    memset(report, 0, sizeof(report));
    report[1] = 0x01;
    report[2] = CM_ARGB_GEN2_A1_CMD_REALTIME_RGB;
    std::copy_n(frame.data + 59, 62, report + 3);

    if(hid_write(dev, report, sizeof(report)) != CM_ARGB_GEN2_A1_PACKET_LENGTH)
    {
        return(false);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_REALTIME_PACKET_GAP_MS));

    /*-----------------------------------------------------*\
    | Packet 2: 82 09 + final 59 continuation bytes         |
    \*-----------------------------------------------------*/
    memset(report, 0, sizeof(report));
    report[1] = 0x82;
    report[2] = CM_ARGB_GEN2_A1_CMD_REALTIME_RGB;
    std::copy_n(frame.data + 121, 59, report + 3);

    if(hid_write(dev, report, sizeof(report)) != CM_ARGB_GEN2_A1_PACKET_LENGTH)
    {
        return(false);
    }

    /*-----------------------------------------------------*\
    | The 2 ms host delay produces the approximately 3 ms   |
    | USB report spacing observed in MasterPlus captures.   |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_REALTIME_PACKET_GAP_MS));
    return(true);
}

static void DecodeEffectRecord(const unsigned char* data, CMARGBGen2A1EffectRecord& effect)
{
    effect.mode       = data[0];
    effect.speed      = data[1];
    effect.brightness = data[2];
    effect.red        = data[3];
    effect.green      = data[4];
    effect.blue       = data[5];
    effect.random     = data[6];
    effect.smode      = data[7];
}

bool CMARGBGen2A1controller::GetEffect(unsigned char outer_mask, unsigned char subtarget, CMARGBGen2A1EffectRecord& effect)
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_EFFECT, CM_ARGB_GEN2_A1_READ, {outer_mask, subtarget}, response))
    {
        return(false);
    }

    if(response[3] != outer_mask || response[4] != subtarget)
    {
        return(false);
    }

    DecodeEffectRecord(response + 5, effect);
    return(true);
}

bool CMARGBGen2A1controller::GetStoredEffect(unsigned char outer_mask, unsigned char subtarget, unsigned char effect_id,
                                             CMARGBGen2A1EffectRecord& effect)
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_STORED_EFFECT, CM_ARGB_GEN2_A1_READ, {outer_mask, subtarget, effect_id}, response))
    {
        return(false);
    }

    if(response[3] != outer_mask || response[4] != subtarget)
    {
        return(false);
    }

    DecodeEffectRecord(response + 5, effect);
    return(true);
}

bool CMARGBGen2A1controller::GetEffectBank(unsigned char outer_mask, unsigned char subtarget, CMARGBGen2A1EffectRecord* effects)
{
    unsigned char page = CM_ARGB_GEN2_A1_COMMAND_PAGE;

    for(unsigned int page_index = 0; page_index < 2; ++page_index, ++page)
    {
        unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

        if(!SendCommandPage(page, CM_ARGB_GEN2_A1_CMD_EFFECT_BANK, CM_ARGB_GEN2_A1_READ, {outer_mask, subtarget}, response))
        {
            return(false);
        }

        if(response[3] != outer_mask || response[4] != subtarget)
        {
            return(false);
        }

        unsigned int first_record = page_index == 0 ? 0 : 7;
        unsigned int record_count = page_index == 0 ? 7 : 3;

        for(unsigned int idx = 0; idx < record_count; ++idx)
        {
            DecodeEffectRecord(response + 5 + (idx * 8), effects[first_record + idx]);
        }
    }

    return(true);
}

bool CMARGBGen2A1controller::SetEffect(unsigned char outer_mask, unsigned char subtarget, int mode_value, unsigned int speed,
                                       unsigned int brightness, RGBColor color, bool random)
{
    if(mode_value < 0 || mode_value > 0xFF || speed > 0xFF || brightness > 0xFF)
    {
        return(false);
    }

    if(realtime_mode.load() && !ExitRealtimeMode())
    {
        return(false);
    }

    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_EFFECT, CM_ARGB_GEN2_A1_WRITE,
                           {outer_mask, subtarget, (unsigned char)mode_value, (unsigned char)speed, (unsigned char)brightness,
                            (unsigned char)RGBGetRValue(color), (unsigned char)RGBGetGValue(color),
                            (unsigned char)RGBGetBValue(color), random, 0x00}, response))
    {
        return(false);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_EFFECT_SETTLE_MS));
    return(true);
}

bool CMARGBGen2A1controller::GetCustomColors(unsigned char outer_mask, unsigned char subtarget, std::size_t led_count,
                                             std::vector<RGBColor>& colors)
{
    colors.clear();

    if(led_count > CM_ARGB_GEN2_A1_TARGET_MAX_LEDS)
    {
        return(false);
    }

    if(led_count == 0)
    {
        return(true);
    }

    std::size_t rgb_size = led_count * 3;
    std::vector<unsigned char> rgb;
    rgb.reserve(rgb_size);
    unsigned char page = CM_ARGB_GEN2_A1_COMMAND_PAGE;

    for(std::size_t page_index = 0; page_index < (rgb_size + 58) / 59; ++page_index, ++page)
    {
        unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

        if(!SendCommandPage(page, CM_ARGB_GEN2_A1_CMD_CUSTOM_RGB, CM_ARGB_GEN2_A1_READ, {outer_mask, subtarget}, response))
        {
            return(false);
        }

        if(response[3] != outer_mask || response[4] != subtarget)
        {
            return(false);
        }

        rgb.insert(rgb.end(), response + 5, response + 5 + std::min<std::size_t>(59, rgb_size - rgb.size()));
    }

    colors.reserve(led_count);

    for(std::size_t idx = 0; idx < led_count; ++idx)
    {
        std::size_t offset = idx * 3;
        colors.push_back(ToRGBColor(rgb[offset], rgb[offset + 1], rgb[offset + 2]));
    }

    return(true);
}

bool CMARGBGen2A1controller::SendCustomColors(unsigned char outer_mask, unsigned char subtarget, const std::vector<RGBColor>& colors)
{
    if(realtime_mode.load() && !ExitRealtimeMode())
    {
        return(false);
    }

    std::vector<unsigned char> rgb;
    std::size_t count = std::min<std::size_t>(colors.size(), CM_ARGB_GEN2_A1_TARGET_MAX_LEDS);
    rgb.reserve(count * 3);

    for(std::size_t idx = 0; idx < count; ++idx)
    {
        rgb.push_back(RGBGetRValue(colors[idx]));
        rgb.push_back(RGBGetGValue(colors[idx]));
        rgb.push_back(RGBGetBValue(colors[idx]));
    }

    if(rgb.empty())
    {
        return(true);
    }

    std::size_t packet_count = (rgb.size() + 58) / 59;
    std::size_t rgb_offset   = 0;
    std::lock_guard<std::mutex> io_lock(io_mutex);

    /*-----------------------------------------------------*\
    | Each CMD08 packet is acknowledged before the next     |
    | packet is transmitted.  The acknowledgement always    |
    | uses the command page and echoes the target address.  |
    \*-----------------------------------------------------*/
    DrainPendingResponses();

    for(unsigned char packet = 0; packet < packet_count; ++packet)
    {
        unsigned char report[CM_ARGB_GEN2_A1_PACKET_LENGTH] = { 0 };
        report[1] = packet;

        if(packet == packet_count - 1)
        {
            report[1] |= 0x80;
        }
        report[2] = CM_ARGB_GEN2_A1_CMD_CUSTOM_RGB;
        report[3] = CM_ARGB_GEN2_A1_WRITE;
        report[4] = outer_mask;
        report[5] = subtarget;

        std::size_t bytes = std::min<std::size_t>(59, rgb.size() - rgb_offset);
        std::copy_n(rgb.begin() + rgb_offset, bytes, report + 6);
        rgb_offset += bytes;

        if(!WriteReport(report))
        {
            return(false);
        }

        bool acknowledged = false;
        using clock = std::chrono::steady_clock;
        clock::time_point deadline = clock::now() + std::chrono::milliseconds(CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS);

        while(clock::now() < deadline)
        {
            timeout_milliseconds remaining = std::chrono::duration_cast<timeout_milliseconds>(deadline - clock::now());
            int read_timeout = std::max(1, remaining.count());
            unsigned char raw[CM_ARGB_GEN2_A1_PACKET_LENGTH] = { 0 };
            int bytes_read = hid_read_timeout(dev, raw, sizeof(raw), read_timeout);

            if(bytes_read <= 0)
            {
                return(false);
            }

            int offset = 0;

            if(raw[0] == 0x00 && bytes_read > 1 && raw[1] == CM_ARGB_GEN2_A1_COMMAND_PAGE)
            {
                offset = 1;
            }
            else if(raw[0] != CM_ARGB_GEN2_A1_COMMAND_PAGE)
            {
                continue;
            }

            int available = bytes_read - offset;

            if(available < 5 || raw[offset + 1] != CM_ARGB_GEN2_A1_CMD_CUSTOM_RGB)
            {
                continue;
            }

            if(raw[offset + 3] != outer_mask || raw[offset + 4] != subtarget)
            {
                continue;
            }

            if(raw[offset + 2] != CM_ARGB_GEN2_A1_RESPONSE)
            {
                return(false);
            }

            acknowledged = true;
            break;
        }

        if(!acknowledged)
        {
            return(false);
        }
    }

    return(true);
}

bool CMARGBGen2A1controller::Identify(unsigned char outer_mask, unsigned char subtarget)
{
    if(realtime_mode.load() && !ExitRealtimeMode())
    {
        return(false);
    }

    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_IDENTIFY, CM_ARGB_GEN2_A1_WRITE, {outer_mask, subtarget}, response))
    {
        return(false);
    }

    return(response[3] == outer_mask && response[4] == subtarget);
}

bool CMARGBGen2A1controller::RemapGen2Subports(unsigned int port, const unsigned char* old_masks, const unsigned char* new_masks)
{
    if(port >= port_count)
    {
        return(false);
    }

    if(realtime_mode.load() && !ExitRealtimeMode())
    {
        return(false);
    }

    unsigned char outer = PortMask(port);
    std::vector<unsigned char> payload;
    payload.reserve(1 + (CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT * 2));
    payload.push_back(outer);

    for(unsigned int idx = 0; idx < CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT; ++idx)
    {
        payload.push_back(old_masks[idx]);
        payload.push_back(new_masks[idx]);
    }

    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_GEN2_REMAP, CM_ARGB_GEN2_A1_WRITE, payload, response))
    {
        return(false);
    }

    if(response[3] != outer)
    {
        return(false);
    }

    for(unsigned int idx = 0; idx < CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT; ++idx)
    {
        unsigned int offset = 4 + (idx * 2);

        if(response[offset] != old_masks[idx] || response[offset + 1] != new_masks[idx])
        {
            return(false);
        }
    }

    return(RefreshPort(port));
}

bool CMARGBGen2A1controller::GetOverlayTable(unsigned int port, unsigned char* table)
{
    if(port >= port_count)
    {
        return(false);
    }

    unsigned char outer = PortMask(port);
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_OVERLAY_TABLE, CM_ARGB_GEN2_A1_READ, {outer}, response))
    {
        return(false);
    }

    if(response[3] != outer)
    {
        return(false);
    }

    memcpy(table, response + 4, CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH);
    return(true);
}

bool CMARGBGen2A1controller::SetOverlayTable(unsigned int port, const unsigned char* table)
{
    if(port >= port_count)
    {
        return(false);
    }

    if(realtime_mode.load() && !ExitRealtimeMode())
    {
        return(false);
    }

    unsigned char outer = PortMask(port);
    std::vector<unsigned char> payload;
    payload.reserve(1 + CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH);
    payload.push_back(outer);
    payload.insert(payload.end(), table, table + CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH);

    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_OVERLAY_TABLE, CM_ARGB_GEN2_A1_WRITE, payload, response))
    {
        return(false);
    }

    return(response[3] == outer);
}

bool CMARGBGen2A1controller::GetOverlayMode(bool& enabled, unsigned char& interval)
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_OVERLAY_MODE, CM_ARGB_GEN2_A1_READ, {}, response))
    {
        return(false);
    }

    if(response[3] > 1)
    {
        return(false);
    }

    enabled  = response[3] != 0;
    interval = response[4];
    return(true);
}

bool CMARGBGen2A1controller::SetOverlayMode(bool enabled, unsigned char interval)
{
    if(realtime_mode.load() && !ExitRealtimeMode())
    {
        return(false);
    }

    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };
    unsigned char mode = enabled ? 0x01 : 0x00;

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_OVERLAY_MODE, CM_ARGB_GEN2_A1_WRITE, {mode, interval}, response))
    {
        return(false);
    }

    return(response[3] == mode);
}

bool CMARGBGen2A1controller::ResetConfiguration(unsigned char reset_type)
{
    if(reset_type != CM_ARGB_GEN2_A1_RESET_ALL && reset_type != CM_ARGB_GEN2_A1_RESET_LED && reset_type != CM_ARGB_GEN2_A1_RESET_FAN)
    {
        return(false);
    }

    if(realtime_mode.load() && !ExitRealtimeMode())
    {
        return(false);
    }

    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_RESET, CM_ARGB_GEN2_A1_WRITE, {reset_type}, response))
    {
        return(false);
    }

    if(response[3] != reset_type)
    {
        return(false);
    }

    memset(response, 0, sizeof(response));

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_REFRESH, CM_ARGB_GEN2_A1_WRITE, {CM_ARGB_GEN2_A1_CHANNEL_ALL}, response))
    {
        return(false);
    }

    if(response[3] != CM_ARGB_GEN2_A1_CHANNEL_ALL)
    {
        return(false);
    }

    return(RefreshTopology());
}

bool CMARGBGen2A1controller::GetUID(unsigned char* uid)
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_UID, CM_ARGB_GEN2_A1_READ, {}, response))
    {
        return(false);
    }

    memcpy(uid, response + 3, CM_ARGB_GEN2_A1_UID_LENGTH);
    return(true);
}

bool CMARGBGen2A1controller::SetUID(const unsigned char* uid)
{
    if(realtime_mode.load() && !ExitRealtimeMode())
    {
        return(false);
    }

    std::vector<unsigned char> payload(uid, uid + CM_ARGB_GEN2_A1_UID_LENGTH);
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_UID, CM_ARGB_GEN2_A1_WRITE, payload, response))
    {
        return(false);
    }

    for(unsigned int idx = 0; idx < CM_ARGB_GEN2_A1_UID_LENGTH; ++idx)
    {
        if(response[3 + idx] != uid[idx])
        {
            return(false);
        }
    }

    return(true);
}

bool CMARGBGen2A1controller::GetMultilayerMode(unsigned char& mode)
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_MULTILAYER, CM_ARGB_GEN2_A1_READ, {}, response))
    {
        return(false);
    }

    if(response[3] != CM_ARGB_GEN2_A1_MULTILAYER_GLOBAL && response[3] != CM_ARGB_GEN2_A1_MULTILAYER_INDEPENDENT)
    {
        return(false);
    }

    mode = response[3];
    return(true);
}

bool CMARGBGen2A1controller::SetMultilayerMode(unsigned char mode)
{
    if(mode != CM_ARGB_GEN2_A1_MULTILAYER_GLOBAL && mode != CM_ARGB_GEN2_A1_MULTILAYER_INDEPENDENT)
    {
        return(false);
    }

    if(realtime_mode.load() && !ExitRealtimeMode())
    {
        return(false);
    }

    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };

    if(!SendRegularCommand(CM_ARGB_GEN2_A1_CMD_MULTILAYER, CM_ARGB_GEN2_A1_WRITE, {mode}, response))
    {
        return(false);
    }

    if(response[3] != mode)
    {
        return(false);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_EFFECT_SCOPE_SETTLE_MS));
    return(true);
}

bool CMARGBGen2A1controller::SaveToFlash()
{
    unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };
    return(SendRegularCommand(CM_ARGB_GEN2_A1_CMD_SAVE, CM_ARGB_GEN2_A1_WRITE, {}, response));
}

bool CMARGBGen2A1controller::SendCommandPage(unsigned char page, unsigned char command, unsigned char operation,
                                             const std::vector<unsigned char>& payload, unsigned char* response, int timeout_ms,
                                             const std::vector<unsigned char>& expected_response_prefix)
{
    if(payload.size() > (CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH - 3))
    {
        return(false);
    }

    unsigned char report[CM_ARGB_GEN2_A1_PACKET_LENGTH] = { 0 };
    report[1] = page;
    report[2] = command;
    report[3] = operation;
    std::copy(payload.begin(), payload.end(), report + 4);

    std::lock_guard<std::mutex> io_lock(io_mutex);

    /*-----------------------------------------------------*\
    | Synchronous commands own the input queue while the    |
    | transport lock is held. Discard replies left behind   |
    | by earlier fire-and-forget runtime commands before    |
    | starting a new request/response transaction.          |
    \*-----------------------------------------------------*/
    if(response != nullptr)
    {
        DrainPendingResponses();
    }

    if(hid_write(dev, report, sizeof(report)) != CM_ARGB_GEN2_A1_PACKET_LENGTH)
    {
        return(false);
    }

    if(response == nullptr)
    {
        return(true);
    }

    using clock = std::chrono::steady_clock;
    clock::time_point deadline = clock::now() + std::chrono::milliseconds(timeout_ms);

    while(clock::now() < deadline)
    {
        timeout_milliseconds remaining = std::chrono::duration_cast<timeout_milliseconds>(deadline - clock::now());
        int read_timeout = std::max(1, remaining.count());
        unsigned char raw[CM_ARGB_GEN2_A1_PACKET_LENGTH] = { 0 };
        int bytes_read = hid_read_timeout(dev, raw, sizeof(raw), read_timeout);

        if(bytes_read <= 0)
        {
            return(false);
        }

        int offset = 0;

        if(raw[0] == 0x00 && bytes_read > 1 && raw[1] == page)
        {
            offset = 1;
        }
        else if(raw[0] != page)
        {
            continue;
        }

        int available = bytes_read - offset;

        if(available < 3)
        {
            continue;
        }

        unsigned char candidate[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = { 0 };
        int to_copy = std::min(available, CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH);
        std::copy_n(raw + offset, to_copy, candidate);

        if(candidate[0] != page || candidate[1] != command)
        {
            continue;
        }

        /*-------------------------------------------------*\
        | Commands are reused across several outer/subport  |
        | targets. Ignore a delayed response for an earlier |
        | target instead of making the current transaction  |
        | fail and requiring another user submission.       |
        \*-------------------------------------------------*/
        if(!expected_response_prefix.empty())
        {
            if(expected_response_prefix.size() > sizeof(candidate) - 3 || !std::equal(expected_response_prefix.begin(),
                                                                                      expected_response_prefix.end(), candidate + 3))
            {
                continue;
            }
        }

        if(candidate[2] != CM_ARGB_GEN2_A1_RESPONSE)
        {
            return(false);
        }

        memcpy(response, candidate, sizeof(candidate));
        return(true);
    }

    return(false);
}

bool CMARGBGen2A1controller::SendRegularCommand(unsigned char command, unsigned char operation,
                                                const std::vector<unsigned char>& payload, unsigned char* response, int timeout_ms,
                                                const std::vector<unsigned char>& expected_response_prefix)
{
    return(SendCommandPage(CM_ARGB_GEN2_A1_COMMAND_PAGE, command, operation, payload, response, timeout_ms,
                           expected_response_prefix));
}

bool CMARGBGen2A1controller::WriteReport(const unsigned char* report)
{
    int written = hid_write(dev, report, CM_ARGB_GEN2_A1_PACKET_LENGTH);
    return(written == CM_ARGB_GEN2_A1_PACKET_LENGTH);
}

void CMARGBGen2A1controller::DrainPendingResponses()
{
    unsigned char raw[CM_ARGB_GEN2_A1_PACKET_LENGTH] = { 0 };
    int bytes_read = 0;

    while((bytes_read = hid_read_timeout(dev, raw, sizeof(raw), 1)) > 0)
    {
    }
}
