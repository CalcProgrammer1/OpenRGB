/*---------------------------------------------------------*\
| RGBController.h                                           |
|                                                           |
|   OpenRGB's RGB controller hardware abstration layer,     |
|   provides a generic representation of an RGB device      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                02 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include "RGBControllerInterface.h"

class RGBController : public RGBControllerInterface
{
public:
    /*-----------------------------------------------------*\
    | RGBController base class constructor                  |
    \*-----------------------------------------------------*/
    RGBController();
    virtual ~RGBController();

    /*-----------------------------------------------------*\
    | Controller Information                                |
    \*-----------------------------------------------------*/
    std::string             GetName();
    std::string             GetVendor();
    std::string             GetDescription();
    std::string             GetVersion();
    std::string             GetSerial();
    std::string             GetLocation();
    std::string             GetDisplayName();

    device_type             GetDeviceType();
    controller_flags        GetFlags();

    /*-----------------------------------------------------*\
    | Hidden Flag Functions                                 |
    \*-----------------------------------------------------*/
    bool                    GetHidden();
    void                    SetHidden(bool hidden);

    /*-----------------------------------------------------*\
    | Zone Functions                                        |
    \*-----------------------------------------------------*/
    zone                    GetZone(unsigned int zone_idx);
    int                     GetZoneActiveMode(unsigned int zone);
    RGBColor                GetZoneColor(unsigned int zone, unsigned int color_index);
    RGBColor*               GetZoneColorsPointer(unsigned int zone);
    std::size_t             GetZoneCount();
    std::string             GetZoneDisplayName(unsigned int zone);
    zone_flags              GetZoneFlags(unsigned int zone);
    unsigned int            GetZoneLEDsCount(unsigned int zone);
    unsigned int            GetZoneLEDsMax(unsigned int zone);
    unsigned int            GetZoneLEDsMin(unsigned int zone);
    matrix_map_type         GetZoneMatrixMap(unsigned int zone);
    const unsigned int*     GetZoneMatrixMapData(unsigned int zone);
    unsigned int            GetZoneMatrixMapHeight(unsigned int zone);
    unsigned int            GetZoneMatrixMapWidth(unsigned int zone);
    std::size_t             GetZoneModeCount(unsigned int zone);
    unsigned int            GetZoneModeBrightness(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeBrightnessMax(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeBrightnessMin(unsigned int zone, unsigned int mode);
    RGBColor                GetZoneModeColor(unsigned int zone, unsigned int mode, unsigned int color_index);
    unsigned int            GetZoneModeColorMode(unsigned int zone, unsigned int mode);
    std::size_t             GetZoneModeColorsCount(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeColorsMax(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeColorsMin(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeDirection(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeFlags(unsigned int zone, unsigned int mode);
    std::string             GetZoneModeName(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeSpeed(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeSpeedMax(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeSpeedMin(unsigned int zone, unsigned int mode);
    int                     GetZoneModeValue(unsigned int zone, unsigned int mode);
    std::string             GetZoneName(unsigned int zone);
    std::size_t             GetZoneSegmentCount(unsigned int zone);
    segment_flags           GetZoneSegmentFlags(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneSegmentLEDsCount(unsigned int zone, unsigned int segment);
    matrix_map_type         GetZoneSegmentMatrixMap(unsigned int zone, unsigned int segment);
    const unsigned int *    GetZoneSegmentMatrixMapData(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneSegmentMatrixMapHeight(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneSegmentMatrixMapWidth(unsigned int zone, unsigned int segment);
    std::string             GetZoneSegmentName(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneSegmentStartIndex(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneSegmentType(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneStartIndex(unsigned int zone);
    zone_type               GetZoneType(unsigned int zone);

    unsigned int            GetLEDsInZone(unsigned int zone);

    void                    SetZoneActiveMode(unsigned int zone, int mode);
    void                    SetZoneColor(unsigned int zone, unsigned int color_index, RGBColor color);
    void                    SetZoneModeBrightness(unsigned int zone, unsigned int mode, unsigned int brightness);
    void                    SetZoneModeColor(unsigned int zone, unsigned int mode, unsigned int color_index, RGBColor color);
    void                    SetZoneModeColorMode(unsigned int zone, unsigned int mode, unsigned int color_mode);
    void                    SetZoneModeColorsCount(unsigned int zone, unsigned int mode, std::size_t count);
    void                    SetZoneModeDirection(unsigned int zone, unsigned int mode, unsigned int direction);
    void                    SetZoneModeSpeed(unsigned int zone, unsigned int mode, unsigned int speed);

    bool                    SupportsPerZoneModes();

    /*-----------------------------------------------------*\
    | Mode Functions                                        |
    \*-----------------------------------------------------*/
    std::size_t             GetModeCount();
    unsigned int            GetModeBrightness(unsigned int mode);
    unsigned int            GetModeBrightnessMax(unsigned int mode);
    unsigned int            GetModeBrightnessMin(unsigned int mode);
    RGBColor                GetModeColor(unsigned int mode, unsigned int color_index);
    unsigned int            GetModeColorMode(unsigned int mode);
    std::size_t             GetModeColorsCount(unsigned int mode);
    unsigned int            GetModeColorsMax(unsigned int mode);
    unsigned int            GetModeColorsMin(unsigned int mode);
    unsigned int            GetModeDirection(unsigned int mode);
    unsigned int            GetModeFlags(unsigned int mode);
    std::string             GetModeName(unsigned int mode);
    unsigned int            GetModeSpeed(unsigned int mode);
    unsigned int            GetModeSpeedMax(unsigned int mode);
    unsigned int            GetModeSpeedMin(unsigned int mode);
    int                     GetModeValue(unsigned int mode);

    void                    SetModeBrightness(unsigned int mode, unsigned int brightness);
    void                    SetModeColor(unsigned int mode, unsigned int color_index, RGBColor color);
    void                    SetModeColorMode(unsigned int mode, unsigned int color_mode);
    void                    SetModeColorsCount(unsigned int mode, std::size_t count);
    void                    SetModeDirection(unsigned int mode, unsigned int direction);
    void                    SetModeSpeed(unsigned int mode, unsigned int speed);

    int                     GetActiveMode();
    void                    SetActiveMode(int mode);
    void                    SetCustomMode();

    /*-----------------------------------------------------*\
    | LED Functions                                         |
    \*-----------------------------------------------------*/
    std::size_t             GetLEDCount();
    std::string             GetLEDName(unsigned int led);
    unsigned int            GetLEDValue(unsigned int led);

    std::string             GetLEDDisplayName(unsigned int led);

    /*-----------------------------------------------------*\
    | Color Functions                                       |
    \*-----------------------------------------------------*/
    RGBColor                GetColor(unsigned int led);
    RGBColor*               GetColorsPointer();
    void                    SetColor(unsigned int led, RGBColor color);
    void                    SetAllColors(RGBColor color);
    void                    SetAllZoneColors(int zone, RGBColor color);

    /*-----------------------------------------------------*\
    | Device-Specific Configuration Functions               |
    \*-----------------------------------------------------*/
    nlohmann::json          GetDeviceSpecificConfigurationSchema();
    nlohmann::json          GetDeviceSpecificConfiguration();
    void                    SetDeviceSpecificConfiguration(nlohmann::json configuration_json);

    nlohmann::json          GetDeviceSpecificZoneConfigurationSchema(int zone);
    nlohmann::json          GetDeviceSpecificZoneConfiguration(int zone);
    void                    SetDeviceSpecificZoneConfiguration(int zone, nlohmann::json configuration_json);

    /*-----------------------------------------------------*\
    | Update Callback Functions                             |
    \*-----------------------------------------------------*/
    void                    RegisterUpdateCallback(RGBControllerCallback new_callback, void * new_callback_arg);
    void                    UnregisterUpdateCallback(void * callback_arg);
    void                    ClearCallbacks();
    void                    SignalUpdate(unsigned int update_reason);

    /*-----------------------------------------------------*\
    | Device Update Functions                               |
    \*-----------------------------------------------------*/
    void                    Shutdown();

    void                    UpdateLEDs();
    void                    UpdateZoneLEDs(int zone);
    void                    UpdateSingleLED(int led);

    void                    UpdateMode();
    void                    UpdateZoneMode(int zone);
    void                    SaveMode();

    void                    DeviceCallThreadFunction();

    void                    ClearSegments(int zone);
    void                    AddSegment(int zone, segment new_segment);

    void                    ConfigureZone(int zone_idx, zone new_zone);
    void                    ResizeZone(int zone, int new_size);

    void                    ConfigureDevice(controller_flags new_flags, std::string new_name);

    /*-----------------------------------------------------*\
    | Functions to be implemented in device implementation  |
    \*-----------------------------------------------------*/
    virtual void            DeviceConfigureDevice();
    virtual void            DeviceConfigureZone(int zone_idx);

    virtual void            DeviceUpdateLEDs();
    virtual void            DeviceUpdateZoneLEDs(int zone);
    virtual void            DeviceUpdateSingleLED(int led);

    virtual void            DeviceUpdateMode();
    virtual void            DeviceUpdateZoneMode(int zone);
    virtual void            DeviceSaveMode();

    virtual void            DeviceUpdateDeviceSpecificConfiguration();
    virtual void            DeviceUpdateDeviceSpecificZoneConfiguration(int zone);

    /*-----------------------------------------------------*\
    | Static Serialized Description Functions               |
    \*-----------------------------------------------------*/
    static unsigned char *  GetColorDescriptionData(unsigned char* data_ptr, RGBController* controller, unsigned int protocol_version);
    static unsigned int     GetColorDescriptionSize(RGBController* controller, unsigned int protocol_version);
    static unsigned char *  GetDeviceDescriptionData(unsigned char* data_ptr, RGBController* controller, unsigned int protocol_version);
    static unsigned int     GetDeviceDescriptionSize(RGBController* controller, unsigned int protocol_version);
    static unsigned char *  GetLEDDescriptionData(unsigned char* data_ptr, led led, unsigned int protocol_version);
    static unsigned int     GetLEDDescriptionSize(led led, unsigned int protocol_version);
    static unsigned char *  GetMatrixMapDescriptionData(unsigned char* data_ptr, matrix_map_type matrix_map, unsigned int protocol_version);
    static unsigned int     GetMatrixMapDescriptionSize(matrix_map_type matrix_map, unsigned int protocol_version);
    static unsigned char *  GetModeDescriptionData(unsigned char* data_ptr, mode mode, unsigned int protocol_version);
    static unsigned int     GetModeDescriptionSize(mode mode, unsigned int protocol_version);
    static unsigned char *  GetSegmentDescriptionData(unsigned char* data_ptr, segment segment, unsigned int protocol_version);
    static unsigned int     GetSegmentDescriptionSize(segment segment, unsigned int protocol_version);
    static unsigned char *  GetZoneDescriptionData(unsigned char* data_ptr, zone zone, unsigned int protocol_version);
    static unsigned int     GetZoneDescriptionSize(zone zone, unsigned int protocol_version);

    static unsigned char*   SetColorDescription(unsigned char* data_ptr, unsigned int data_size, RGBController* controller, unsigned int protocol_version, bool resize = false);
    static unsigned char*   SetDeviceDescription(unsigned char* data_ptr, unsigned int data_size, RGBController* controller, unsigned int protocol_version);
    static unsigned char*   SetLEDDescription(unsigned char* data_ptr, unsigned int data_size, led* led, unsigned int protocol_version);
    static unsigned char*   SetMatrixMapDescription(unsigned char* data_ptr, unsigned int data_size, matrix_map_type* matrix_map, unsigned int protocol_version);
    static unsigned char*   SetModeDescription(unsigned char* data_ptr, unsigned int data_size, mode* mode, unsigned int protocol_version);
    static unsigned char*   SetSegmentDescription(unsigned char* data_ptr, unsigned int data_size, segment* segment, unsigned int protocol_version);
    static unsigned char*   SetZoneDescription(unsigned char* data_ptr, unsigned int data_size, zone* zone, unsigned int protocol_version);

    /*-----------------------------------------------------*\
    | Static JSON Description Functions                     |
    \*-----------------------------------------------------*/
    static nlohmann::json   GetDeviceDescriptionJSON(RGBController* controller);
    static nlohmann::json   GetLEDDescriptionJSON(led led);
    static nlohmann::json   GetMatrixMapDescriptionJSON(matrix_map_type matrix_map);
    static nlohmann::json   GetModeDescriptionJSON(mode mode);
    static nlohmann::json   GetSegmentDescriptionJSON(segment segment);
    static nlohmann::json   GetZoneDescriptionJSON(zone zone);

    static void             SetDeviceDescriptionJSON(nlohmann::json controller_json, RGBController* controller);
    static led              SetLEDDescriptionJSON(nlohmann::json led_json);
    static matrix_map_type  SetMatrixMapDescriptionJSON(nlohmann::json matrix_map_json);
    static mode             SetModeDescriptionJSON(nlohmann::json mode_json);
    static segment          SetSegmentDescriptionJSON(nlohmann::json segment_json);
    static zone             SetZoneDescriptionJSON(nlohmann::json zone_json);

    static bool             SetModeValuesFromMode(mode& destination, mode& source);

    static std::string      DeviceTypeToString(device_type type);

protected:
    /*-----------------------------------------------------*\
    | Controller information strings                        |
    \*-----------------------------------------------------*/
    std::string             description;    /* controller description   */
    std::string             location;       /* controller location      */
    std::string             name;           /* controller name          */
    std::string             serial;         /* controller serial number */
    std::string             vendor;         /* controller vendor        */
    std::string             version;        /* controller version       */
    std::string             configuration;  /* controller device-       */
                                            /* specific config JSON     */
    std::string             display_name;   /* display name             */

    /*-----------------------------------------------------*\
    | Controller variables                                  |
    \*-----------------------------------------------------*/
    int                     active_mode = 0;/* active mode              */
    controller_flags        flags;          /* controller flags         */
    device_type             type;           /* device type              */

    /*-----------------------------------------------------*\
    | Controller member vectors                             |
    \*-----------------------------------------------------*/
    std::vector<RGBColor>   colors;         /* Color buffer             */
    std::vector<led>        leds;           /* LEDs                     */
    std::vector<std::string>
                            led_display_names;
                                            /* LED display names        */
    std::vector<mode>       modes;          /* Modes                    */
    std::vector<zone>       zones;          /* Zones                    */

    /*-----------------------------------------------------*\
    | Access mutex variables                                |
    \*-----------------------------------------------------*/
    std::shared_mutex       AccessMutex;

    /*-----------------------------------------------------*\
    | Functions not part of interface for internal use only |
    \*-----------------------------------------------------*/
    unsigned int            LEDsInZone(unsigned int zone);
    void                    SetupColors();
    void                    UpdateLEDsInternal();

private:
    /*-----------------------------------------------------*\
    | Device thread variables                               |
    \*-----------------------------------------------------*/
    std::thread*            DeviceCallThread;
    std::atomic<bool>       CallFlag_UpdateLEDs;
    std::atomic<bool>       CallFlag_UpdateMode;
    std::atomic<bool>       DeviceThreadRunning;

    /*-----------------------------------------------------*\
    | Update callback variables                             |
    \*-----------------------------------------------------*/
    std::mutex                          UpdateMutex;
    std::vector<RGBControllerCallback>  UpdateCallbacks;
    std::vector<void *>                 UpdateCallbackArgs;

    /*-----------------------------------------------------*\
    | Private path used internally by DetectionManager      |
    | for HID hotplug path tracking                         |
    \*-----------------------------------------------------*/
    std::string             detection_path;

    /*-----------------------------------------------------*\
    | Certain internal OpenRGB framework classes can modify |
    | protected members                                     |
    \*-----------------------------------------------------*/
    friend class            DetectionManager;
    friend class            NetworkClient;
    friend class            NetworkServer;
    friend class            OpenRGBPluginAPI;
    friend class            ProfileManager;
    friend class            ResourceManager;
};
