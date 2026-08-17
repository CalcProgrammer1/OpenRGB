/*---------------------------------------------------------*\
| LogitechHIDPP20ReceiverWatcher.cpp                        |
|                                                           |
|   Persistent per-receiver-node listener                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "LogitechHIDPP20ReceiverWatcher.h"
#include "LogitechHIDPP20Controller.h"
#include "LogManager.h"

#define LOG_TAG log_tag.c_str()

/*---------------------------------------------------------*\
| Give up on the node after this many consecutive read      |
| errors; the receiver was unplugged.                       |
\*---------------------------------------------------------*/
static const int WATCHER_READ_ERROR_LIMIT = 10;

/*---------------------------------------------------------*\
| Built sub-device registry. Keyed on node and index so one |
| watcher can poke any of the up-to-six devices paired to   |
| its receiver.                                             |
\*---------------------------------------------------------*/
static std::map<std::pair<std::string, uint8_t>, LogitechHIDPP20Controller*> subdevice_registry;
static std::mutex                                                            subdevice_registry_mutex;

/*---------------------------------------------------------*\
| How to build a device, set by the detector.               |
\*---------------------------------------------------------*/
static LogitechHIDPP20ReceiverWatcher::BuilderFunction       subdevice_builder;

void LogitechHIDPP20ReceiverWatcher::SetBuilder(BuilderFunction builder)
{
    std::lock_guard<std::mutex> lock(subdevice_registry_mutex);
    subdevice_builder = builder;
}

void LogitechHIDPP20ReceiverWatcher::RegisterSubDevice(const std::string& node_path, uint8_t index, LogitechHIDPP20Controller* controller)
{
    std::lock_guard<std::mutex> lock(subdevice_registry_mutex);
    subdevice_registry[std::make_pair(node_path, index)] = controller;
}

void LogitechHIDPP20ReceiverWatcher::UnregisterSubDevice(LogitechHIDPP20Controller* controller)
{
    std::lock_guard<std::mutex> lock(subdevice_registry_mutex);

    for(std::map<std::pair<std::string, uint8_t>, LogitechHIDPP20Controller*>::iterator it = subdevice_registry.begin();
        it != subdevice_registry.end();)
    {
        if(it->second == controller)
        {
            it = subdevice_registry.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool LogitechHIDPP20ReceiverWatcher::PokeSubDevice(const std::string& node_path, uint8_t index, int direction)
{
    std::lock_guard<std::mutex> lock(subdevice_registry_mutex);

    std::map<std::pair<std::string, uint8_t>, LogitechHIDPP20Controller*>::iterator it =
        subdevice_registry.find(std::make_pair(node_path, index));

    if(it == subdevice_registry.end())
    {
        return false;
    }

    it->second->NudgeConnection(direction);

    return true;
}

/*---------------------------------------------------------*\
| A bridge event carries the connected sub-device count,    |
| not an index, so every controller on the node gets the    |
| direction.                                                |
\*---------------------------------------------------------*/
void LogitechHIDPP20ReceiverWatcher::PokeNode(const std::string& node_path, int direction)
{
    std::lock_guard<std::mutex> lock(subdevice_registry_mutex);

    for(std::map<std::pair<std::string, uint8_t>, LogitechHIDPP20Controller*>::iterator it = subdevice_registry.begin();
        it != subdevice_registry.end(); ++it)
    {
        if(it->first.first == node_path)
        {
            it->second->NudgeConnection(direction);
        }
    }
}

bool LogitechHIDPP20ReceiverWatcher::HasSubDevice(const std::string& node_path, uint8_t index)
{
    std::lock_guard<std::mutex> lock(subdevice_registry_mutex);

    return subdevice_registry.count(std::make_pair(node_path, index)) != 0;
}

LogitechHIDPP20ReceiverWatcher::LogitechHIDPP20ReceiverWatcher(const std::string& node_path, uint8_t bridge_feat_idx,
                                                               uint8_t bridge_report_id, bool bridge_addressed)
{
    this->node_path        = node_path;
    this->bridge_feat_idx  = bridge_feat_idx;
    this->bridge_report_id = bridge_report_id;
    this->bridge_addressed = bridge_addressed;
    this->log_tag          = "[Logitech receiver watcher @ " + node_path + "]";
    this->dev              = nullptr;
    this->reader_thread    = nullptr;
    this->worker_thread    = nullptr;
    this->running          = false;
    this->alive            = false;
}

LogitechHIDPP20ReceiverWatcher::~LogitechHIDPP20ReceiverWatcher()
{
    running.store(false);
    queue_cv.notify_all();

    if(reader_thread)
    {
        reader_thread->join();
        delete reader_thread;
    }

    if(worker_thread)
    {
        worker_thread->join();
        delete worker_thread;
    }

    if(dev)
    {
        hid_close(dev);
    }
}

bool LogitechHIDPP20ReceiverWatcher::Start()
{
    dev = hid_open_path(node_path.c_str());

    if(dev == nullptr)
    {
        return false;
    }

    running.store(true);
    alive.store(true);

    reader_thread = new std::thread(&LogitechHIDPP20ReceiverWatcher::ReaderThreadFunc, this);
    worker_thread = new std::thread(&LogitechHIDPP20ReceiverWatcher::WorkerThreadFunc, this);

    LOG_INFO("%s Watching for connection events", LOG_TAG);

    return true;
}

bool LogitechHIDPP20ReceiverWatcher::IsAlive()
{
    return alive.load();
}

/*---------------------------------------------------------*\
| Hand a slot to the worker, once. Repeated connection      |
| events for a slot that cannot answer must not queue       |
| more than one build.                                      |
\*---------------------------------------------------------*/
void LogitechHIDPP20ReceiverWatcher::QueueBuild(uint8_t index)
{
    {
        std::lock_guard<std::mutex> lock(queue_mutex);

        for(std::deque<uint8_t>::iterator it = build_queue.begin(); it != build_queue.end(); ++it)
        {
            if(*it == index)
            {
                return;
            }
        }

        build_queue.push_back(index);
    }

    queue_cv.notify_all();
}

/*---------------------------------------------------------*\
| A slot changed state. A built controller gets the nudge   |
| and its own threads handle the wake; a slot with no       |
| controller was unreachable when the pass ran, so build    |
| it now.                                                   |
\*---------------------------------------------------------*/
void LogitechHIDPP20ReceiverWatcher::OnConnection(uint8_t index, int direction)
{
    if(PokeSubDevice(node_path, index, direction))
    {
        LOG_DEBUG("%s index=0x%02X nudged %+d", LOG_TAG, index, direction);
        return;
    }

    if(direction < 0)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Whether this slot is one we know is the builder's     |
    | answer to give: it holds what every enumerated slot   |
    | needs to be built, and it outlives the pass.          |
    \*-----------------------------------------------------*/
    QueueBuild(index);
}

/*---------------------------------------------------------*\
| Builds happen here, off the read loop, one at a time.     |
\*---------------------------------------------------------*/
void LogitechHIDPP20ReceiverWatcher::WorkerThreadFunc()
{
    while(running.load())
    {
        uint8_t index = 0;
        bool    have  = false;

        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            queue_cv.wait(lock, [this]{ return !build_queue.empty() || !running.load(); });

            if(!running.load())
            {
                return;
            }

            if(!build_queue.empty())
            {
                index = build_queue.front();
                build_queue.pop_front();
                have  = true;
            }
        }

        if(!have)
        {
            continue;
        }

        BuilderFunction builder;

        {
            std::lock_guard<std::mutex> lock(subdevice_registry_mutex);
            builder = subdevice_builder;
        }

        if(!builder)
        {
            continue;
        }

        LOG_INFO("%s index=0x%02X connected, building it", LOG_TAG, index);

        builder(node_path, index);
    }
}

/*---------------------------------------------------------*\
| Only reader of the watcher's handle. Never sends:         |
| decode, flag, continue.                                   |
\*---------------------------------------------------------*/
void LogitechHIDPP20ReceiverWatcher::ReaderThreadFunc()
{
    int errors = 0;

    while(running.load())
    {
        uint8_t buf[64] = {};
        int result = hid_read_timeout(dev, buf, sizeof(buf), 500);

        if(result < 0)
        {
            if(++errors >= WATCHER_READ_ERROR_LIMIT)
            {
                LOG_DEBUG("%s Node is gone, going dormant", LOG_TAG);
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        errors = 0;

        if(result == 0)
        {
            continue;
        }

        if(bridge_feat_idx != 0)
        {
            /*---------------------------------------------*\
            | CentPPBridge event 0:                         |
            | ConnectionStateChangedEvent data[1] = number  |
            | of connected sub-devices for the whole        |
            | bridge, so every slot gets the direction.     |
            \*---------------------------------------------*/
            if(buf[0] != bridge_report_id)
            {
                continue;
            }

            int     hdr  = bridge_addressed ? 1 : 0;
            uint8_t feat = buf[3 + hdr];
            uint8_t func = buf[4 + hdr];

            if(feat != bridge_feat_idx || (func & 0xF0) != 0x00 || (func & 0x0F) == HIDPP20_SW_ID)
            {
                continue;
            }

            uint8_t num_devices = buf[6 + hdr];
            int     direction   = (num_devices > 0) ? 1 : -1;

            LOG_DEBUG("%s Bridge ConnectionStateChanged: %d sub-device(s)", LOG_TAG, num_devices);

            PokeNode(node_path, direction);

            if(direction > 0)
            {
                /*-----------------------------------------*\
                | A bridge event carries no slot index, so  |
                | a sub-device with no controller is the    |
                | dongle's own default index.               |
                \*-----------------------------------------*/
                if(!HasSubDevice(node_path, LOGITECH_DEFAULT_DEVICE_INDEX))
                {
                    QueueBuild(LOGITECH_DEFAULT_DEVICE_INDEX);
                }
            }

            continue;
        }

        /*-------------------------------------------------*\
        | HID++ 1.0 connection notifications from the       |
        | receiver: 0x41 device connection (flags bit       |
        | 0x40 = paired but not linked), 0x40 device        |
        | disconnection. Disconnects are logged only,       |
        | built controllers detect it themselves.           |
        \*-------------------------------------------------*/
        if(buf[0] != LOGITECH_SHORT_MESSAGE && buf[0] != LOGITECH_LONG_MESSAGE)
        {
            continue;
        }

        uint8_t index  = buf[1];
        uint8_t sub_id = buf[2];
        uint8_t flags  = buf[4];

        if(sub_id == 0x41)
        {
            bool link_established = !(flags & 0x40);

            LOG_DEBUG("%s index=0x%02X connection notification flags=0x%02X link=%d",
                      LOG_TAG, index, flags, link_established);

            if(link_established)
            {
                OnConnection(index, 1);
            }
        }
        else if(sub_id == 0x40)
        {
            LOG_DEBUG("%s index=0x%02X disconnection notification", LOG_TAG, index);
        }
        else
        {
            continue;
        }
    }

    alive.store(false);
    queue_cv.notify_all();
}

