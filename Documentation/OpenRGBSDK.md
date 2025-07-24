# OpenRGB SDK Documentation

OpenRGB provides a network-based Software Development Kit (SDK) interface for third-party software applications to integrate with OpenRGB to control lighting on OpenRGB-supported devices.  This protocol is a binary, packet-based protocol designed for efficient, lightweight transfer of lighting data over a TCP/IP connection.  It may be used locally or over a physical network between computers.  The protocol is versioned.  Client and server must negotiate a minimum supported protocol version upon connection.  The selected protocol version determines what capabilities are available and can change packet format for certain packets as new information is added to the protocol.

The protocol mimics the [RGBController API](The-RGBController-API) closely.  It can be thought of as "RGBController over IP" in that the protocol is designed so that a network RGBController object can be created on the client that is a direct copy of the real RGBController object on the server.  Calls to the network client RGBController object send packets to the server which trigger calls to the real object, updating the necessary object data before the call.

# Protocol Versions

| Protocol Version | OpenRGB Release | Description                                                                                                    |
| ---------------- | --------------- | -------------------------------------------------------------------------------------------------------------- |
| 0                | 0.3             | Initial (unversioned) protocol                                                                                 |
| 1                | 0.5             | Add versioning, add vendor string                                                                              |
| 2                | 0.6             | Add profile controls                                                                                           |
| 3                | 0.7             | Add brightness field to modes, add SaveMode()                                                                  |
| 4                | 0.9             | Add segments field to zones, plugin interface                                                                  |
| 5                | 1.0             | Add zone flags, controller flags, effects-only zones, alternative LED names, add ClearSegments and AddSegments |

\* Denotes unreleased version, reflects status of current pipeline

# Protocol Basics

The default port for the OpenRGB SDK server is 6742.  This is "ORGB" on a telephone keypad.

Each packet starts with a header that indicates the packet is an OpenRGB SDK packet and provides the device and packet IDs.  The header format is described in the following table.

### NetPacketHeader structure

| Size | Format       | Name        | Description         |
| ---- | ------------ | ----------- | ------------------- |
| 4    | char[4]      | pkt_magic   | Magic value, "ORGB" |
| 4    | unsigned int | pkt_dev_idx | Device Index        |
| 4    | unsigned int | pkt_id      | Packet ID           |
| 4    | unsigned int | pkt_size    | Packet Size         |

`pkt_magic`: Always set this to the literal value "ORGB".

`pkt_dev_idx`: The device index that the command is targeting.

`pkt_id`: The command ID, see IDs table below

`pkt_size`: The size, in bytes, of the packet data

### Packet IDs

The following IDs represent different SDK commands.  Each ID packet has a certain format of data associated with it, which will be explained under each ID's section of this document.  Gaps have been left in the ID values to allow for future expansion.  The same ID values are often used for both request and response packets.

| Value | Name                                                                                        | Description                                      | Protocol Version |
| ----- | ------------------------------------------------------------------------------------------- | ------------------------------------------------ | ---------------- |
| 0     | [NET_PACKET_ID_REQUEST_CONTROLLER_COUNT](#net_packet_id_request_controller_count)           | Request RGBController device count from server   | 0                |
| 1     | [NET_PACKET_ID_REQUEST_CONTROLLER_DATA](#net_packet_id_request_controller_data)             | Request RGBController data block                 | 0                |
| 40    | [NET_PACKET_ID_REQUEST_PROTOCOL_VERSION](#net_packet_id_request_protocol_version)           | Request OpenRGB SDK protocol version from server | 1*               |
| 50    | [NET_PACKET_ID_SET_CLIENT_NAME](#net_packet_id_set_client_name)                             | Send client name string to server                | 0                |
| 100   | [NET_PACKET_ID_DEVICE_LIST_UPDATED](#net_packet_id_device_list_updated)                     | Indicate to clients that device list has updated | 1                |
| 140   | [NET_PACKET_ID_REQUEST_RESCAN_DEVICES](#net_packet_id_request_rescan_devices)               | Request server to rescan devices                 | 5                |
| 150   | [NET_PACKET_ID_REQUEST_PROFILE_LIST](#net_packet_id_request_profile_list)                   | Request profile list                             | 2                |
| 151   | [NET_PACKET_ID_REQUEST_SAVE_PROFILE](#net_packet_id_request_save_profile)                   | Save current configuration in a new profile      | 2                |
| 152   | [NET_PACKET_ID_REQUEST_LOAD_PROFILE](#net_packet_id_request_load_profile)                   | Load a given profile                             | 2                |
| 153   | [NET_PACKET_ID_REQUEST_DELETE_PROFILE](#net_packet_id_request_delete_profile)               | Delete a given profile                           | 2                |
| 200   | [NET_PACKET_ID_REQUEST_PLUGIN_LIST](#net_packet_id_request_plugin_list)                     | Request plugin list                              | 4                |
| 201   | [NET_PACKET_ID_PLUGIN_SPECIFIC](#net_packet_id_plugin_specific)                             | Plugin specific                                  | 4                |
| 1000  | [NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE](#net_packet_id_rgbcontroller_resizezone)           | RGBController::ResizeZone()                      | 0                |
| 1001  | [NET_PACKET_ID_RGBCONTROLLER_CLEARSEGMENTS](#net_packet_id_rgbcontroller_clearsegments)     | RGBController::ClearSegments()                   | 5                |
| 1002  | [NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT](#net_packet_id_rgbcontroller_addsegment)           | RGBController::AddSegment()                      | 5                |
| 1050  | [NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS](#net_packet_id_rgbcontroller_updateleds)           | RGBController::UpdateLEDs()                      | 0                |
| 1051  | [NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS](#net_packet_id_rgbcontroller_updatezoneleds)   | RGBController::UpdateZoneLEDs()                  | 0                |
| 1052  | [NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED](#net_packet_id_rgbcontroller_updatesingleled) | RGBController::UpdateSingleLED()                 | 0                |
| 1100  | [NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE](#net_packet_id_rgbcontroller_setcustommode)     | RGBController::SetCustomMode()                   | 0                |
| 1101  | [NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE](#net_packet_id_rgbcontroller_updatemode)           | RGBController::UpdateMode()                      | 0                |
| 1102  | [NET_PACKET_ID_RGBCONTROLLER_SAVEMODE](#net_packet_id_rgbcontroller_savemode)               | RGBController::SaveMode()                        | 3                |
        
\* The NET_PACKET_ID_REQUEST_PROTOCOL_VERSION packet was not present in protocol version 0, but clients supporting protocol versions 1+ should always send this packet.  If no response is received, it should be assumed that the server is using protocol 0.

# Packet-Specific Documentation

## NET_PACKET_ID_REQUEST_CONTROLLER_COUNT

### Request [Size: 0]

The client uses this ID to request the number of controllers on the server.  The request contains no data.

### Response [Size: 4]

The server responds to this request with the number of controllers in the device list.  The response contains a single `unsigned int`, size 4, holding this value.

## NET_PACKET_ID_REQUEST_CONTROLLER_DATA

### Request [Protocol 0 Size: 0] [Protocol 1+ Size: 4]

The client uses this ID to request the controller data for a given controller.  For protocol 0, this request contains no data.  For protocol 1 or higher, this request contains a single `unsigned int`, size 4, holding the highest protocol version supported by both the client and the server.  The `pkt_dev_idx` of this request's header indicates which controller you are requesting data for.  Upon connecting, the client should request controller data from 0 to [controller count], where [controller count] is the value from NET_PACKET_ID_REQUEST_CONTROLLER_COUNT.

NOTE: Before sending this request, the client should request the protocol version from the server and determine the value to send, if any.  If the server is using protocol version 0, even if the SDK implementation supports higher, send this packet with no data.

### Response [Size: Variable]

The server responds to this request with a large data block.  The format of the block is shown below.  Portions of this block are omitted if the requested protocol level is below the listed value.  The receiver is expected to parse this data block using the same protocol version sent in the request (or protocol 0 if the request is sent with no data).

| Size                | Format                                | Name                | Protocol Version | Description                                                                                                  |
| ------------------- | ------------------------------------- | ------------------- | ---------------- | ------------------------------------------------------------------------------------------------------------ |
| 4                   | unsigned int                          | data_size           | 0                | Size of all data in packet                                                                                   |
| 4                   | int                                   | type                | 0                | RGBController type field value                                                                               |
| 2                   | unsigned short                        | name_len            | 0                | Length of RGBController name field string, including null termination                                        |
| name_len            | char[name_len]                        | name                | 0                | RGBController name field string value, including null termination                                            |
| 2                   | unsigned short                        | vendor_len          | 1                | Length of RGBController vendor field string, including null termination                                      |
| vendor_len          | char[vendor_len]                      | vendor              | 1                | RGBController vendor field string value, including null termination                                          |
| 2                   | unsigned short                        | description_len     | 0                | Length of RGBController description field string, including null termination                                 |
| description_len     | char[description_len]                 | description         | 0                | RGBController description field string value, including null termination                                     |
| 2                   | unsigned short                        | version_len         | 0                | Length of RGBController version field string, including null termination                                     |
| version_len         | char[version_len]                     | version             | 0                | RGBController version field string value, including null termination                                         |
| 2                   | unsigned short                        | serial_len          | 0                | Length of RGBController serial field string, including null termination                                      |
| serial_len          | char[serial_len]                      | serial              | 0                | RGBController serial field string value, including null termination                                          |
| 2                   | unsigned short                        | location_len        | 0                | Length of RGBController location field string, including null termination                                    |
| location_len        | char[location_len]                    | location            | 0                | RGBController location field string value, including null termination                                        |
| 2                   | unsigned short                        | num_modes           | 0                | Number of modes in RGBController                                                                             |
| 4                   | int                                   | active_mode         | 0                | RGBController active_mode field value                                                                        |
| Variable            | Mode Data[num_modes]                  | modes               | 0                | See [Mode Data](#mode-data) block format table.  Repeat num_modes times                                      |
| 2                   | unsigned short                        | num_zones           | 0                | Number of zones in RGBController                                                                             |
| Variable            | Zone Data[num_zones]                  | zones               | 0                | See [Zone Data](#zone-data) block format table.  Repeat num_zones times                                      |
| 2                   | unsigned short                        | num_leds            | 0                | Number of LEDs in RGBController                                                                              |
| Variable            | LED Data[num_leds]                    | leds                | 0                | See [LED Data](#led-data) block format table.  Repeat num_leds times                                         |
| 2                   | unsigned short                        | num_colors          | 0                | Number of colors in RGBController                                                                            |
| 4 * num_colors      | RGBColor[num_colors]                  | colors              | 0                | RGBController colors field values                                                                            |
| 2                   | unsigned short                        | num_led_alt_names   | 5                | Number of LED alternate name strings                                                                         |
| Variable            | LED Alternate Name[num_led_alt_names] | led_alt_names       | 5                | See [LED Alternate Name Data](#led-alternate-names-data) block format table.  Repeat num_led_alt_names times |
| 4                   | unsigned int                          | flags               | 5                | RGBController flags field value                                                                              |

## Mode Data

The Mode Data block represents one entry in the `RGBController::modes` vector.  Portions of this block are omitted if the requested protocol level is below the listed value.

| Size                | Format                    | Name                | Protocol Version | Description                                            |
| ------------------- | ------------------------- | ------------------- | ---------------- | ------------------------------------------------------ |
| 2                   | unsigned short            | mode_name_len       | 0                | Length of mode name string, including null termination |
| mode_name_len       | char[mode_name_len]       | mode_name           | 0                | Mode name string value, including null termination     |
| 4                   | int                       | mode_value          | 0                | Mode value field value                                 |
| 4                   | unsigned int              | mode_flags          | 0                | Mode flags field value                                 |
| 4                   | unsigned int              | mode_speed_min      | 0                | Mode speed_min field value                             |
| 4                   | unsigned int              | mode_speed_max      | 0                | Mode speed_max field value                             |
| 4                   | unsigned int              | mode_brightness_min | 3                | Mode brightness_min field value                        |
| 4                   | unsigned int              | mode_brightness_max | 3                | Mode brightness_max field value                        |
| 4                   | unsigned int              | mode_colors_min     | 0                | Mode colors_min field value                            |
| 4                   | unsigned int              | mode_colors_max     | 0                | Mode colors_max field value                            |
| 4                   | unsigned int              | mode_speed          | 0                | Mode speed value                                       |
| 4                   | unsigned int              | mode_brightness     | 3                | Mode brightness value                                  |
| 4                   | unsigned int              | mode_direction      | 0                | Mode direction value                                   |
| 4                   | unsigned int              | mode_color_mode     | 0                | Mode color_mode value                                  |
| 2                   | unsigned short            | mode_num_colors     | 0                | Mode number of colors                                  |
| 4 * mode_num_colors | RGBColor[mode_num_colors] | mode_colors         | 0                | Mode color values                                      |

## Zone Data

The Zone Data block represents one entry in the `RGBController::zones` vector.

| Size                   | Format                            | Name                | Protocol Version | Description                                                                                                  |
| ---------------------- | --------------------------------- | ------------------- | ---------------- | ------------------------------------------------------------------------------------------------------------ |
| 2                      | unsigned short                    | zone_name_len       | 0                | Length of zone name string, including null termination                                                       |
| zone_name_len          | char[zone_name_len]               | zone_name           | 0                | Zone name string value, including null termination                                                           |
| 4                      | int                               | zone_type           | 0                | Zone type value                                                                                              |
| 4                      | unsigned int                      | zone_leds_min       | 0                | Zone leds_min value                                                                                          |
| 4                      | unsigned int                      | zone_leds_max       | 0                | Zone leds_max value                                                                                          |
| 4                      | unsigned int                      | zone_leds_count     | 0                | Zone leds_count value                                                                                        |
| 2                      | unsigned short                    | zone_matrix_len     | 0                | Zone matrix length if matrix_map exists: (matrix_map width * height * 4) + 8 OTHERWISE 0 if matrix_map NULL  |
| 4*                     | unsigned int                      | zone_matrix_height  | 0                | Zone matrix_map height (*only if matrix_map exists)                                                          |
| 4*                     | unsigned int                      | zone_matrix_width   | 0                | Zone matrix_map width (*only if matrix_map exists)                                                           |
| (zone_matrix_len - 8)* | unsigned int[zone_matrix_len - 8] | zone_matrix_data    | 0                | Zone matrix_map data (*only if matrix_map exists)                                                            |
| 2                      | unsigned short                    | num_segments        | 4                | Number of segments in zone                                                                                   |
| Variable               | Segment Data[num_segments]        | segments            | 4                | See [Segment Data](#segment-data) block format table.  Repeat num_segments times                             |
| 4                      | unsigned int                      | zone_flags          | 5                | Zone flags value                                                                                             |

## Segment Data

The Segment Data block represents one entry in the `RGBController::zones::segments` vector.  This data block was introduced in protocol version 4.

| Size             | Format                 | Name               | Protocol Version | Description                                               |
| ---------------- | ---------------------- | ------------------ | ---------------- | --------------------------------------------------------- |
| 2                | unsigned short         | segment_name_len   | 4                | Length of segment name string, including null termination |
| segment_name_len | char[segment_name_len] | segment_name       | 4                | Segment name string value, including null termination     |
| 4                | int                    | segment_type       | 4                | Segment type value                                        |
| 4                | unsigned int           | segment_start_idx  | 4                | Segment start_idx value                                   |
| 4                | unsigned int           | segment_leds_count | 4                | Segment leds_count value                                  |

## LED Data

The LED Data block represents one entry in the `RGBController::leds` vector.

| Size                | Format                    | Name                | Protocol Version | Description                                            |
| ------------------- | ------------------------- | ------------------- | ---------------- | ------------------------------------------------------ |
| 2                   | unsigned short            | led_name_len        | 0                | Length of LED name string, including null termination  |
| led_name_len        | char[led_name_len]        | led_name            | 0                | LED name string value, including null termination      |
| 4                   | unsigned int              | led_value           | 0                | LED value field value                                  |

## LED Alternate Name Data

The LED Alternate Name Data block represents one entry in the `RGBController::led_alt_names` vector.  This data block was introduced in protocol version 5.

| Size             | Format                 | Name             | Protocol Version | Description                                                     |
| ---------------- | ---------------------- | ---------------- | ---------------- | --------------------------------------------------------------- |
| 2                | unsigned short         | led_alt_name_len | 5                | Length of LED alternate name string, including null termination |
| led_alt_name_len | char[led_alt_name_len] | led_alt_name     | 5                | LED alternate name string value, including null termination     |

## NET_PACKET_ID_REQUEST_PROTOCOL_VERSION

### Request [Size: 4]

The client uses this ID to request the server's highest supported protocol version as well as to indicate to the server the client's highest supported protocol version.  The request contains a single `unsigned int`, size 4, containing the client's highest supported protocol version.

### Response [Size: 4]

The server responds to this request with a single `unsigned int`, size 4, containing the server's highest supported protocol version.  If the server is using protocol version 0, it will not send a response.  If no response is received, assume the server's highest supported protocol version is version 0.

## NET_PACKET_ID_SET_CLIENT_NAME

### Client Only [Size: Variable]

The client uses this ID to send the client's null-terminated name string to the server.  The size of the packet is the size of the string including the null terminator.  In C, this is strlen() + 1.  There is no response from the server for this packet.

## NET_PACKET_ID_DEVICE_LIST_UPDATED

### Server Only [Size: 0]

The server uses this ID to notify a client that the server's device list has been updated.  Upon receiving this packet, clients should synchronize their local device lists with the server by requesting size and controller data again.  This packet contains no data.

## NET_PACKET_ID_REQUEST_RESCAN_DEVICES

### Client Only [Size: 0]

The client uses this ID to request the server rescan its devices.

## NET_PACKET_ID_REQUEST_PROFILE_LIST

### Request [Size: 0]

The client uses this ID to request the server's profile list.  The request contains no data.

### Response [Size: Variable]

The server responds to this request with a data block.  The format of the block is shown below.

| Size     | Format                     | Name         | Protocol Version | Description                                                                      |
| -------- | -------------------------- | ------------ | ---------------- | -------------------------------------------------------------------------------- |
| 4        | unsigned int               | data_size    | 2                | Size of all data in packet                                                       |
| 2        | unsigned short             | num_profiles | 2                | Number of profiles on server                                                     |
| Variable | Profile Data[num_profiles] | profiles     | 2                | See [Profile Data](#profile-data) block format table.  Repeat num_profiles times |

## Profile Data

The profile data block represents the information of one profile.  This data block was introduced in protocol version 2.

| Size             | Format                 | Name             | Protocol Version | Description                                               |
| ---------------- | ---------------------- | ---------------- | ---------------- | --------------------------------------------------------- |
| 2                | unsigned short         | profile_name_len | 2                | Length of profile name string, including null termination |
| profile_name_len | char[profile_name_len] | profile_name     | 2                | Profile name string value, including null termination     |

## NET_PACKET_ID_REQUEST_SAVE_PROFILE

### Client Only [Size: Variable]

The client uses this ID to command the server to save the current configuration to a profile.  It passes the name of the profile to save as a null-terminated string.  The size of the packet is the size of the string including the null terminator.  In C, this is strlen() + 1.  There is no response from the server for this packet.

## NET_PACKET_ID_REQUEST_LOAD_PROFILE

### Client Only [Size: Variable]

The client uses this ID to command the server to load the given profile.  It passes the name of the profile to load as a null-terminated string.  The size of the packet is the size of the string including the null terminator.  In C, this is strlen() + 1.  There is no response from the server for this packet.

Calling this function will not actually update the controllers.  Instead, the controller states will be updated from the profile on the server side.  After sending this request, the client should re-request all controller states from the server so that the client controller states match the server states loaded from the profile.  After requesting all of the controller data, the client shall call UpdateMode() on all controllers to apply the updated state.

## NET_PACKET_ID_REQUEST_DELETE_PROFILE

### Client Only [Size: Variable]

The client uses this ID to command the server to delete the given profile.  It passes the name of the profile to delete as a null-terminated string.  The size of the packet is the size of the string including the null terminator.  In C, this is strlen() + 1.  There is no response from the server for this packet.

## NET_PACKET_ID_REQUEST_PLUGIN_LIST

### Request [Size: 0]

The client uses this ID to request the server's plugin list.  The request contains no data.

### Response [Size: Variable]

The server responds to this request with a data block.  The format of the block is shown below.

| Size     | Format                   | Name        | Protocol Version | Description                                                                   |
| -------- | ------------------------ | ----------- | ---------------- | ----------------------------------------------------------------------------- |
| 4        | unsigned int             | data_size   | 4                | Size of all data in packet                                                    |
| 2        | unsigned short           | num_plugins | 4                | Number of plugins on server                                                   |
| Variable | Plugin Data[num_plugins] | plugins     | 4                | See [Plugin Data](#plugin-data) block format table.  Repeat num_plugins times |

## Plugin Data

The plugin data block represents the information of one plugin.  This data block was introduced in protocol version 4.

| Size                   | Format                       | Name                    | Protocol Version | Description                                                     |
| ---------------------- | ---------------------------- | ----------------------- | ---------------- | --------------------------------------------------------------- |
| 2                      | unsigned short               | plugin_name_len         | 4                | Length of plugin name string, including null termination        |
| plugin_name_len        | char[plugin_name_len]        | plugin_name             | 4                | Plugin name string value, including null termination            |
| 2                      | unsigned short               | plugin_description_len  | 4                | Length of plugin description string, including null termination |
| plugin_description_len | char[plugin_description_len] | plugin_description      | 4                | Plugin description string value, including null termination     |
| 2                      | unsigned short               | plugin_version_len      | 4                | Length of plugin version string, including null termination     |
| plugin_version_len     | char[plugin_version_len]     | plugin_version          | 4                | Plugin version string value, including null termination         |
| 4                      | unsigned int                 | plugin_index            | 4                | Plugin index value                                              |
| 4                      | unsigned int                 | plugin_protocol_version | 4                | Plugin protocol version value                                   |

## NET_PACKET_ID_PLUGIN_SPECIFIC

### Request [Size: Variable]

This packet is used to send data to a plugin.  The `pkt_dev_idx` field in the header specifies which plugin to send to and corresponds to the `plugin_index` field in the plugin list.  The first 4 bytes of the data is the plugin packet type, the rest of the packet is plugin-specific.

List of plugins that currently support this:

- [Effects plugin](https://gitlab.com/OpenRGBDevelopers/OpenRGBEffectsPlugin/-/blob/master/SDK.md)

### Response [Size: Variable]

The response is optionally generated by the plugin.  The data in the packet is plugin-specific.

## NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE

### Client Only [Size: 8]

The client uses this ID to call the ResizeZone() function of an RGBController device.  The packet data contains a data block.  The format of the block is shown below.  The `pkt_dev_idx` of this request's header indicates which controller you are calling ResizeZone() on.

| Size | Format | Name     | Description          |
| ---- | ------ | -------- | -------------------- |
| 4    | int    | zone_idx | Zone index to resize |
| 4    | int    | new_size | New size of the zone |

## NET_PACKET_ID_RGBCONTROLLER_CLEARSEGMENTS

### Client Only [Size: 4]

The client uses this ID to call the ClearSegments() function of an RGBController device.  The packet contains the index of the zone to clear segments on, type int (size 4).  The `pkt_dev_idx` of this request's header indicates which controller you are calling ClearSegments() on.

## NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT

### Client Only [Size: Variable]

The client uses this ID to call the AddSegment() function of an RGBController device.  The packet contains a data block.  The format of the block is shown below.  The `pkt_dev_idx` of this request's header indicates which controller you are calling AddSegment() on.

| Size             | Format                 | Name             | Description                                               |
| ---------------- | ---------------------- | ---------------- | --------------------------------------------------------- |
| 4                | unsigned int           | data_size        | Size of all data in packet                                |
| 4                | unsigned int           | zone_idx         | Zone index to add segment to                              |
| Variable         | Segment Data           | segment          | See [Segment Data](#segment-data) block format table.     |

## NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS

### Client Only [Size: Variable]

The client uses this ID to call the UpdateLEDs() function of an RGBController device.  The packet data contains a data block.  The format of the block is shown below.  The `pkt_dev_idx` of this request's header indicates which controller you are calling UpdateLEDs() on.

| Size           | Format               | Name       | Description                         |
| -------------- | -------------------- | ---------- | ----------------------------------- |
| 4              | unsigned int         | data_size  | Size of all data in packet          |
| 2              | unsigned short       | num_colors | Number of color values in packet    |
| 4 * num_colors | RGBColor[num_colors] | led_color  | Color values for each LED in device |

## NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS

### Client Only [Size: Variable]

The client uses this ID to call the UpdateZoneLEDs() function of an RGBController device.  The packet data contains a data block.  The format of the data block is shown below.  The `pkt_dev_idx` of this request's header indicates which controller you are calling UpdateZoneLEDs() on.

| Size           | Format               | Name       | Description                       |
| -------------- | -------------------- | ---------- | --------------------------------- |
| 4              | unsigned int         | data_size  | Size of all data in packet        |
| 4              | unsigned int         | zone_idx   | Zone index to update              |
| 2              | unsigned short       | num_colors | Number of color values in packet  |
| 4 * num_colors | RGBColor[num_colors] | led_color  | Color values for each LED in zone |

## NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED

### Client Only [Size: 8]

The client uses this ID to call the UpdateSingleLED() function of an RGBController device.  The packet data contains a data block.  The format of the data block is shown below.  The `pkt_dev_idx` of this request's header indicates which controller you are calling UpdateSingleLED() on.

| Size | Format   | Name      | Description |
| ---- | -------- | --------- | ----------- |
| 4    | int      | led_idx   | LED index   |
| 4    | RGBColor | led_color | LED color   |

## NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE

### Client Only [Size: 0]

The client uses this ID to call the SetCustomMode() function of an RGBController device.  The packet contains no data.  The `pkt_dev_idx` of this request's header indicates which controller you are calling SetCustomMode() on.

## NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE

### Client Only [Size: Variable]

The client uses this ID to call the UpdateMode() function of an RGBController device.  The packet contains a data block.  The format of the data block is shown below.  The `pkt_dev_idx` of this request's header indicates which controller you are calling UpdateMode() on.

| Size                | Format                    | Name                | Protocol Version | Description                                            |
| ------------------- | ------------------------- | ------------------- | ---------------- | ------------------------------------------------------ |
| 4                   | unsigned int              | data_size           | 0                | Size of all data in packet                             |
| 4                   | int                       | mode_idx            | 0                | Mode index to update                                   |
| Variable            | Mode Data                 | mode                | 0                | See [Mode Data](#mode-data) block format table.        |

## NET_PACKET_ID_RGBCONTROLLER_SAVEMODE

### Client Only [Size: Variable]

The client uses this ID to call the SaveMode() function of an RGBController device.  The packet contains a data block.  The format of the data block is the same as for [NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE](#net_packet_id_rgbcontroller_updatemode).  The `pkt_dev_idx` of this request's header indicates which controller you are calling SaveMode() on.
