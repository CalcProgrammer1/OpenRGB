/*---------------------------------------------------------*\
| LogitechHIDPP20ReceiverWatcher.h                          |
|                                                           |
|   Persistent per-receiver-node listener. Decodes the      |
|   dongle's connection notifications and either nudges the |
|   built sub-device controller awake, or builds one for a  |
|   slot that has none yet. Never sends.                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <hidapi.h>

class LogitechHIDPP20Controller;

class LogitechHIDPP20ReceiverWatcher
{
public:
    /*-----------------------------------------------------*\
    | bridge_feat_idx == 0 reads HID++ 1.0 connection       |
    | notifications (standard receiver). Nonzero reads      |
    | CentPPBridge ConnectionStateChangedEvent in the       |
    | Centurion framing given by report_id/addressed.       |
    \*-----------------------------------------------------*/
    LogitechHIDPP20ReceiverWatcher(const std::string& node_path, uint8_t bridge_feat_idx,
                                   uint8_t bridge_report_id, bool bridge_addressed);
    ~LogitechHIDPP20ReceiverWatcher();

    bool        Start();
    bool        IsAlive();

    /*-----------------------------------------------------*\
    | How the watcher builds a device reported as           |
    | connected. Set once by the detector, which owns the   |
    | build; the watcher only decides when.                 |
    \*-----------------------------------------------------*/
    typedef std::function<bool(const std::string& node_path, uint8_t index)> BuilderFunction;
    static void SetBuilder(BuilderFunction builder);

    static bool HasSubDevice(const std::string& node_path, uint8_t index);

    /*-----------------------------------------------------*\
    | Built sub-device registry, shared by all watchers.    |
    | A poke reaches a built controller; a controller       |
    | deregisters at the top of its destructor under the    |
    | same lock, so a poke can never touch a dying one.     |
    \*-----------------------------------------------------*/
    static void RegisterSubDevice(const std::string& node_path, uint8_t index, LogitechHIDPP20Controller* controller);
    static void UnregisterSubDevice(LogitechHIDPP20Controller* controller);

private:
    void        ReaderThreadFunc();
    void        WorkerThreadFunc();
    void        OnConnection(uint8_t index, int direction);
    void        QueueBuild(uint8_t index);
    static bool PokeSubDevice(const std::string& node_path, uint8_t index, int direction);
    static void PokeNode(const std::string& node_path, int direction);

    std::string                     node_path;
    uint8_t                         bridge_feat_idx;
    uint8_t                         bridge_report_id;
    bool                            bridge_addressed;
    std::string                     log_tag;

    hid_device*                     dev;

    std::thread*                    reader_thread;
    std::thread*                    worker_thread;
    std::atomic<bool>               running;
    std::atomic<bool>               alive;

    /*-----------------------------------------------------*\
    | Slots reported connected with no controller yet,      |
    | waiting for the worker. A build is feature discovery  |
    | and a claim, far too long to run on the read loop.    |
    \*-----------------------------------------------------*/
    std::deque<uint8_t>             build_queue;
    std::mutex                      queue_mutex;
    std::condition_variable         queue_cv;
};
