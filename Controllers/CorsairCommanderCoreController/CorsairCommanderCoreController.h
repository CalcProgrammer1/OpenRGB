/*---------------------------------------------------------*\
|  CorsairCommanderCoreController.h                         |
|                                                           |
|  Definitions for Corsair Commander Core                   |
|  Based on code by:                                        |
|  Adam Honse (calcprogrammer1@gmail.com), 8/17/2020        |
|                                                           |
|  Jeff P.                                                  |
\*---------------------------------------------------------*/

#include "RGBController.h"

#include <vector>
#include <chrono>
#include <hidapi/hidapi.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#pragma once

#define CORSAIR_COMMANDER_CORE_PACKET_SIZE_V1   1025 // First bit is the report bit
#define CORSAIR_COMMANDER_CORE_PACKET_SIZE_V2   97   // First bit is the report bit
#define CORSAIR_COMMANDER_CORE_PACKET_SIZE_V3   65   // First bit is the report bit

#define CORSAIR_COMMANDER_CORE_RGB_DATA_LENGTH  699
#define CORSAIR_QL_FAN_ZONE_OFFSET              102
#define CORSAIR_COMMANDER_CORE_NUM_CHANNELS     6

#ifdef _WIN32
#define GLOBAL_CORSAIR_MUTEX_NAME "Global\\CorsairLinkReadWriteGuardMutex"
#endif

enum
{
    CORSAIR_COMMANDER_CORE_MODE_DIRECT          = 0x00,
};

class CorsairCommanderCoreController
{
public:
    CorsairCommanderCoreController(hid_device* dev_handle, const char* path, int pid);
    ~CorsairCommanderCoreController();

    std::string GetFirmwareString();
    std::string GetLocationString();

    void        SetDirectColor
                (
                    std::vector<RGBColor>,
                    std::vector<zone>
                );

    void        KeepaliveThread();
    void        SetFanMode();

private:
    hid_device*             dev;
    std::thread*            keepalive_thread;
    std::atomic<bool>       keepalive_thread_run;
    std::atomic<bool>       controller_ready;
    std::string             location;
    std::vector<RGBColor>   lastcolors;
    std::vector<zone>       lastzones;
    unsigned short int      version[3] = {0, 0, 0};
    int                     packet_size;
    int                     command_res_size;
    int                     pid;
    std::chrono::time_point<std::chrono::steady_clock> last_commit_time;

#ifdef _WIN32
    HANDLE                  global_corsair_access_handle = NULL;
#endif
    
    void        SendCommand(unsigned char command[2], unsigned char data[], unsigned short int data_len, unsigned char res[]);
    void        WriteData(unsigned char endpoint[2], unsigned char data_type[2], unsigned char data[], unsigned short int data_len);

    void        SendCommit();
    void        InitController();
};
