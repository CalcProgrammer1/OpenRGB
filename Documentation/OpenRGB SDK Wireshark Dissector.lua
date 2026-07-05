--[[---------------------------------------------------------
-   openrgb_protocol.lua                                    -
-                                                           -
-   OpenRGB SDK protocol parser for Wireshark               -
-                                                           -
-   Adam Honse <calcprogrammer1@gmail.com>      31 Mar 2026 -
-----------------------------------------------------------]]

--[[---------------------------------------------------------
- Define the protocol                                       -
-----------------------------------------------------------]]
openrgb_protocol = Proto("OpenRGB", "OpenRGB Protocol")

--[[---------------------------------------------------------
- Header fields - NetPacketHeader struct                    -
-----------------------------------------------------------]]
header_pkt_magic                                    = ProtoField.string("openrgb.header_pkt_magic",     "pkt_magic",        base.STRING)
header_pkt_dev_id                                   = ProtoField.uint32("openrgb.header_pkt_dev_id",    "pkt_dev_id",       base.DEC)
header_pkt_id                                       = ProtoField.uint32("openrgb.header_pkt_id",        "pkt_id",           base.DEC)
header_pkt_size                                     = ProtoField.uint32("openrgb.header_pkt_size",      "pkt_size",         base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_REQUEST_CONTROLLER_COUNT fields             -
-----------------------------------------------------------]]
request_controller_count_count                      = ProtoField.uint32("openrgb.count",                "count",            base.DEC)
request_controller_count_id_list                    = ProtoField.uint32("openrgb.id_list",              "id_list",          base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_REQUEST_CONTROLLER_DATA fields              -
-----------------------------------------------------------]]
request_controller_data_protocol_version            = ProtoField.uint32("openrgb.protocol_version",     "protocol_version", base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_ACK fields                                  -
-----------------------------------------------------------]]
ack_acked_pkt_id                                    = ProtoField.uint32("openrgb.acked_pkt_id",         "acked_pkt_id",     base.DEC)
ack_status                                          = ProtoField.uint32("openrgb.ack_status",           "ack_status",       base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_REQUEST_PROTOCOL_VERSION fields             -
-----------------------------------------------------------]]
request_protocol_version_protocol_version           = ProtoField.uint32("openrgb.protocol_version",     "protocol_version", base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_SET_CLIENT_NAME fields                      -
-----------------------------------------------------------]]
set_client_name_client_name                         = ProtoField.string("openrgb.client_name",          "client_name",      base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_SET_SERVER_NAME fields                      -
-----------------------------------------------------------]]
set_server_name_server_name                         = ProtoField.string("openrgb.server_name",          "server_name",      base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_SET_CLIENT_FLAGS fields                     -
-----------------------------------------------------------]]
set_client_flags_client_flags                       = ProtoField.uint32("openrgb.client_flags",         "client_flags",     base.HEX)

--[[---------------------------------------------------------
- NET_PACKET_ID_SET_SERVER_FLAGS fields                     -
-----------------------------------------------------------]]
set_server_flags_server_flags                       = ProtoField.uint32("openrgb.server_flags",         "server_flags",     base.HEX)

--[[---------------------------------------------------------
- NET_PACKET_ID_DETECTION_PROGRESS_CHANGED fields           -
-----------------------------------------------------------]]
detection_progress_changed_data_size                = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
detection_progress_changed_percent                  = ProtoField.uint32("openrgb.detection_percent",    "detection_percent",base.DEC)
detection_progress_changed_string_len               = ProtoField.uint16("openrgb.detection_string_len", "detection_string_len", base.DEC)
detection_progress_changed_string                   = ProtoField.string("openrgb.detection_string",     "detection_string", base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_GET_I2C_BUS_INFO fields                     -
-----------------------------------------------------------]]
get_i2c_bus_info_data_size                          = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
get_i2c_bus_info_bus_count                          = ProtoField.uint32("openrgb.bus_count",            "bus_count",        base.DEC)
get_i2c_bus_info_bus_data                           = ProtoField.bytes( "openrgb.bus_data",             "bus_data")

--[[---------------------------------------------------------
- NET_PACKET_ID_GET_HID_DEVICE_INFO fields                  -
-----------------------------------------------------------]]
get_hid_device_info_data_size                       = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
get_hid_device_info_device_count                    = ProtoField.uint32("openrgb.device_count",         "device_count",     base.DEC)
get_hid_device_info_vendor_id                       = ProtoField.uint32("openrgb.vendor_id",            "vendor_id",        base.HEX)
get_hid_device_info_product_id                      = ProtoField.uint32("openrgb.product_id",           "product_id",       base.HEX)
get_hid_device_info_release_number                  = ProtoField.uint32("openrgb.release_number",       "release_number",   base.HEX)
get_hid_device_info_usage_page                      = ProtoField.uint32("openrgb.usage_page",           "usage_page",       base.HEX)
get_hid_device_info_usage                           = ProtoField.uint32("openrgb.usage",                "usage",            base.HEX)
get_hid_device_info_interface_number                = ProtoField.uint32("openrgb.interface_number",     "interface_number", base.DEC)
get_hid_device_info_serial_number_size              = ProtoField.uint16("openrgb.serial_number_size",   "serial_number_size", base.DEC)
get_hid_device_info_serial_number                   = ProtoField.string("openrgb.serial_number",        "serial_number",    base.STRING)
get_hid_device_info_manufacturer_size               = ProtoField.uint16("openrgb.manufacturer_size",    "manufacturer_size", base.DEC)
get_hid_device_info_manufacturer_string             = ProtoField.string("openrgb.manufacturer_string",  "manufacturer_string", base.STRING)
get_hid_device_info_product_string_size             = ProtoField.uint16("openrgb.product_string_size",  "product_string_size", base.DEC)
get_hid_device_info_product_string                  = ProtoField.string("openrgb.product_string",       "product_string",   base.STRING)
get_hid_device_info_path_size                       = ProtoField.uint16("openrgb.path_size",            "path_size",        base.DEC)
get_hid_device_info_path                            = ProtoField.string("openrgb.path",                 "path",             base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_GET_USB_DEVICE_INFO fields                  -
-----------------------------------------------------------]]
get_usb_device_info_data_size                       = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
get_usb_device_info_device_count                    = ProtoField.uint32("openrgb.device_count",         "device_count",     base.DEC)
get_usb_device_info_vendor_id                       = ProtoField.uint32("openrgb.vendor_id",            "vendor_id",        base.HEX)
get_usb_device_info_product_id                      = ProtoField.uint32("openrgb.product_id",           "product_id",       base.HEX)
get_usb_device_info_serial_number_size              = ProtoField.uint16("openrgb.serial_number_size",   "serial_number_size", base.DEC)
get_usb_device_info_serial_number                   = ProtoField.string("openrgb.serial_number",        "serial_number",    base.STRING)
get_usb_device_info_manufacturer_size               = ProtoField.uint16("openrgb.manufacturer_size",    "manufacturer_size", base.DEC)
get_usb_device_info_manufacturer_string             = ProtoField.string("openrgb.manufacturer_string",  "manufacturer_string", base.STRING)
get_usb_device_info_product_string_size             = ProtoField.uint16("openrgb.product_string_size",  "product_string_size", base.DEC)
get_usb_device_info_product_string                  = ProtoField.string("openrgb.product_string",       "product_string",   base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_GET_SERIAL_PORTS fields                     -
-----------------------------------------------------------]]
get_serial_ports_data_size                          = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
get_serial_ports_port_count                         = ProtoField.uint32("openrgb.port_count",           "port_count",       base.DEC)
get_serial_ports_port_string_size                   = ProtoField.uint16("openrgb.port_string_size",     "port_string_size", base.DEC)
get_serial_ports_port_string                        = ProtoField.string("openrgb.port_string",          "port_string",      base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_GET_PROFILE_LIST fields     -
-----------------------------------------------------------]]
profilemanager_get_profile_list_data_size           = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
profilemanager_get_profile_list_num_profiles        = ProtoField.uint16("openrgb.num_profiles",         "num_profiles",     base.DEC)
profilemanager_get_profile_list_profile_name_len    = ProtoField.uint16("openrgb.profile_name_len",     "profile_name_len", base.DEC)
profilemanager_get_profile_list_profile_name        = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_SAVE_PROFILE fields         -
-----------------------------------------------------------]]
profilemanager_save_profile_profile_name            = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_LOAD_PROFILE fields         -
-----------------------------------------------------------]]
profilemanager_load_profile_profile_name            = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_DELETE_PROFILE fields       -
-----------------------------------------------------------]]
profilemanager_delete_profile_profile_name          = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_UPLOAD_PROFILE fields       -
-----------------------------------------------------------]]
profilemanager_upload_profile_profile_json          = ProtoField.string("openrgb.profile_json",         "profile_json",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_DOWNLOAD_PROFILE fields     -
-----------------------------------------------------------]]
profilemanager_download_profile_profile_name        = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)
profilemanager_download_profile_profile_json        = ProtoField.string("openrgb.profile_json",         "profile_json",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_GET_ACTIVE_PROFILE fields   -
-----------------------------------------------------------]]
profilemanager_get_active_profile_profile_name      = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_ACTIVE_PROFILE_CHANGED fields-
-----------------------------------------------------------]]
profilemanager_active_profile_changed_profile_name  = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_PROFILE_LOADED fields       -
-----------------------------------------------------------]]
profilemanager_profile_loaded_profile_json          = ProtoField.string("openrgb.profile_json",         "profile_json",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_PROFILE_LIST_UPDATED fields -
-----------------------------------------------------------]]
profilemanager_profile_list_updated_data_size       = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
profilemanager_profile_list_updated_num_profiles    = ProtoField.uint16("openrgb.num_profiles",         "num_profiles",     base.DEC)
profilemanager_profile_list_updated_profile_name_len= ProtoField.uint16("openrgb.profile_name_len",     "profile_name_len", base.DEC)
profilemanager_profile_list_updated_profile_name    = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PLUGINMANAGER_GET_PLUGIN_LIST fields       -
-----------------------------------------------------------]]
pluginmanager_get_plugin_list_data_size             = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
pluginmanager_get_plugin_list_num_plugins           = ProtoField.uint16("openrgb.num_plugins",          "num_plugins",      base.DEC)
pluginmanager_get_plugin_list_plugin_name_len       = ProtoField.uint16("openrgb.plugin_name_len",      "plugin_name_len",  base.DEC)
pluginmanager_get_plugin_list_plugin_name           = ProtoField.string("openrgb.plugin_name",          "plugin_name",      base.STRING)
pluginmanager_get_plugin_list_plugin_desc_len       = ProtoField.uint16("openrgb.plugin_desc_len",      "plugin_desc_len",  base.DEC)
pluginmanager_get_plugin_list_plugin_desc           = ProtoField.string("openrgb.plugin_desc",          "plugin_desc",      base.STRING)
pluginmanager_get_plugin_list_plugin_ver_len        = ProtoField.uint16("openrgb.plugin_ver_len",       "plugin_ver_len",   base.DEC)
pluginmanager_get_plugin_list_plugin_ver            = ProtoField.string("openrgb.plugin_ver",           "plugin_ver",       base.STRING)
pluginmanager_get_plugin_list_plugin_index          = ProtoField.uint32("openrgb.plugin_index",         "plugin_index",     base.DEC)
pluginmanager_get_plugin_list_plugin_proto_ver      = ProtoField.uint32("openrgb.plugin_proto_ver",     "plugin_proto_ver", base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_PLUGINMANAGER_PLUGIN_SPECIFIC fields       -
-----------------------------------------------------------]]
pluginmanager_plugin_specific_plugin_pkt_type       = ProtoField.uint32("openrgb.plugin_pkt_type",      "plugin_pkt_type",  base.DEC)
pluginmanager_plugin_specific_data                  = ProtoField.bytes( "openrgb.plugin_specific_data",  "plugin_specific_data")

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS fields        -
-----------------------------------------------------------]]
settingsmanager_get_settings_settings_key           = ProtoField.string("openrgb.settings_key",         "settings_key",     base.STRING)
settingsmanager_get_settings_settings_json          = ProtoField.string("openrgb.settings_json",        "settings_json",    base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS_SCHEMA fields -
-----------------------------------------------------------]]
settingsmanager_get_settings_schema_settings_key    = ProtoField.string("openrgb.settings_key",         "settings_key",     base.STRING)
settingsmanager_get_settings_schema_settings_json   = ProtoField.string("openrgb.settings_json",        "settings_json",    base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_MODIFY_SETTINGS fields     -
-----------------------------------------------------------]]
settingsmanager_modify_settings_settings_json       = ProtoField.string("openrgb.settings_json",        "settings_json",    base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_SET_SETTINGS fields        -
-----------------------------------------------------------]]
settingsmanager_set_settings_settings_json          = ProtoField.string("openrgb.settings_json",        "settings_json",    base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_LOGMANAGER_GET_LOG_LEVEL fields            -
-----------------------------------------------------------]]
logmanager_get_log_level_log_level                  = ProtoField.uint32("openrgb.log_level",            "log_level",        base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_LOGMANAGER_SET_LOG_LEVEL fields            -
-----------------------------------------------------------]]
logmanager_set_log_level_log_level                  = ProtoField.uint32("openrgb.log_level",            "log_level",        base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_LOGMANAGER_LOGGED_ENTRY fields             -
-----------------------------------------------------------]]
logmanager_logged_entry_data_size                   = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
logmanager_logged_entry_log_level                   = ProtoField.uint32("openrgb.log_level",            "log_level",        base.DEC)
logmanager_logged_entry_line                        = ProtoField.uint32("openrgb.line",                 "line",             base.DEC)
logmanager_logged_entry_timestamp                   = ProtoField.uint64("openrgb.timestamp",            "timestamp",        base.DEC)
logmanager_logged_entry_filename_size               = ProtoField.uint16("openrgb.filename_size",        "filename_size",    base.DEC)
logmanager_logged_entry_filename                    = ProtoField.string("openrgb.filename",             "filename",         base.STRING)
logmanager_logged_entry_text_size                   = ProtoField.uint16("openrgb.text_size",            "text_size",        base.DEC)
logmanager_logged_entry_text                        = ProtoField.string("openrgb.text",                 "text",             base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE fields             -
-----------------------------------------------------------]]
rgbcontroller_resizezone_zone_idx                   = ProtoField.int32( "openrgb.zone_idx",             "zone_idx",         base.DEC)
rgbcontroller_resizezone_new_size                   = ProtoField.int32( "openrgb.new_size",             "new_size",         base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_CLEARSEGMENTS fields          -
-----------------------------------------------------------]]
rgbcontroller_clearsegments_zone_idx                = ProtoField.int32( "openrgb.zone_idx",             "zone_idx",         base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT fields             -
-----------------------------------------------------------]]
rgbcontroller_addsegment_data_size                  = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
rgbcontroller_addsegment_zone_idx                   = ProtoField.uint32("openrgb.zone_idx",             "zone_idx",         base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_CONFIGUREZONE fields          -
-----------------------------------------------------------]]
rgbcontroller_configurezone_data_size               = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
rgbcontroller_configurezone_zone_idx                = ProtoField.uint32("openrgb.zone_idx",             "zone_idx",         base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_CONFIGUREDEVICE fields        -
-----------------------------------------------------------]]
rgbcontroller_configuredevice_data_size             = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
rgbcontroller_configuredevice_flags                 = ProtoField.uint32("openrgb.flags",                "flags",            base.HEX)
rgbcontroller_configuredevice_name_len              = ProtoField.uint16("openrgb.name_len",             "name_len",         base.DEC)
rgbcontroller_configuredevice_name                  = ProtoField.string("openrgb.name",                 "name",             base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_SETHIDDEN fields              -
-----------------------------------------------------------]]
rgbcontroller_sethidden_hidden                      = ProtoField.bool(  "openrgb.hidden",               "hidden",           base.NONE)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS fields             -
-----------------------------------------------------------]]
rgbcontroller_updateleds_data_size                  = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
rgbcontroller_updateleds_num_colors                 = ProtoField.uint16("openrgb.num_colors",           "num_colors",       base.DEC)
rgbcontroller_updateleds_color                      = ProtoField.bytes( "openrgb.led_color",            "led_color")

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS fields         -
-----------------------------------------------------------]]
rgbcontroller_updatezoneleds_data_size              = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
rgbcontroller_updatezoneleds_zone_idx               = ProtoField.uint32("openrgb.zone_idx",             "zone_idx",         base.DEC)
rgbcontroller_updatezoneleds_num_colors             = ProtoField.uint16("openrgb.num_colors",           "num_colors",       base.DEC)
rgbcontroller_updatezoneleds_color                  = ProtoField.bytes( "openrgb.led_color",            "led_color")

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED fields        -
-----------------------------------------------------------]]
rgbcontroller_updatesingleled_led_idx               = ProtoField.int32( "openrgb.led_idx",              "led_idx",          base.DEC)
rgbcontroller_updatesingleled_color                 = ProtoField.bytes( "openrgb.led_color",            "led_color")

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE fields             -
-----------------------------------------------------------]]
rgbcontroller_updatemode_data_size                  = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
rgbcontroller_updatemode_mode_idx                   = ProtoField.int32( "openrgb.mode_idx",             "mode_idx",         base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_UPDATEZONEMODE fields         -
-----------------------------------------------------------]]
rgbcontroller_updatezonemode_data_size              = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
rgbcontroller_updatezonemode_zone_idx               = ProtoField.int32( "openrgb.zone_idx",             "zone_idx",         base.DEC)
rgbcontroller_updatezonemode_mode_idx               = ProtoField.int32( "openrgb.mode_idx",             "mode_idx",         base.DEC)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_SETDEVICESPECIFICCONFIGURATION fields -
-----------------------------------------------------------]]
rgbcontroller_setdevicespecific_config              = ProtoField.string("openrgb.configuration",        "configuration",    base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_SETDEVICESPECIFICZONECONFIGURATION fields -
-----------------------------------------------------------]]
rgbcontroller_setdevicezoneconfig_zone_idx           = ProtoField.int32( "openrgb.zone_idx",             "zone_idx",        base.DEC)
rgbcontroller_setdevicezoneconfig_config_size        = ProtoField.uint32("openrgb.config_size",          "config_size",     base.DEC)
rgbcontroller_setdevicezoneconfig_config             = ProtoField.string("openrgb.configuration",        "configuration",   base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_SIGNALUPDATE fields           -
-----------------------------------------------------------]]
rgbcontroller_signalupdate_data_size                = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
rgbcontroller_signalupdate_update_reason            = ProtoField.uint32("openrgb.update_reason",        "update_reason",    base.DEC)

--[[---------------------------------------------------------
- Generic field for packet data bytes                       -
-----------------------------------------------------------]]
pkt_data                                            = ProtoField.bytes( "openrgb.pkt_data",             "pkt_data")

--[[---------------------------------------------------------
- Version fields                                            -
-----------------------------------------------------------]]
version_client                                      = ProtoField.uint32("openrgb.version_client",       "version_client",   base.DEC)
version_negotiated                                  = ProtoField.uint32("openrgb.version_negotiated",   "version_negotiated", base.DEC)
version_server                                      = ProtoField.uint32("openrgb.version_server",       "version_server",   base.DEC)

openrgb_protocol.fields =
{
    header_pkt_magic,
    header_pkt_dev_id,
    header_pkt_id,
    header_pkt_size,
    request_controller_count_count,
    request_controller_count_id_list,
    request_controller_data_protocol_version,
    ack_acked_pkt_id,
    ack_status,
    request_protocol_version_protocol_version,
    set_client_name_client_name,
    set_server_name_server_name,
    set_client_flags_client_flags,
    set_server_flags_server_flags,
    detection_progress_changed_data_size,
    detection_progress_changed_percent,
    detection_progress_changed_string_len,
    detection_progress_changed_string,
    get_i2c_bus_info_data_size,
    get_i2c_bus_info_bus_count,
    get_i2c_bus_info_bus_data,
    get_hid_device_info_data_size,
    get_hid_device_info_device_count,
    get_hid_device_info_vendor_id,
    get_hid_device_info_product_id,
    get_hid_device_info_release_number,
    get_hid_device_info_usage_page,
    get_hid_device_info_usage,
    get_hid_device_info_interface_number,
    get_hid_device_info_serial_number_size,
    get_hid_device_info_serial_number,
    get_hid_device_info_manufacturer_size,
    get_hid_device_info_manufacturer_string,
    get_hid_device_info_product_string_size,
    get_hid_device_info_product_string,
    get_hid_device_info_path_size,
    get_hid_device_info_path,
    get_usb_device_info_data_size,
    get_usb_device_info_device_count,
    get_usb_device_info_vendor_id,
    get_usb_device_info_product_id,
    get_usb_device_info_serial_number_size,
    get_usb_device_info_serial_number,
    get_usb_device_info_manufacturer_size,
    get_usb_device_info_manufacturer_string,
    get_usb_device_info_product_string_size,
    get_usb_device_info_product_string,
    get_serial_ports_data_size,
    get_serial_ports_port_count,
    get_serial_ports_port_string_size,
    get_serial_ports_port_string,
    profilemanager_get_profile_list_data_size,
    profilemanager_get_profile_list_num_profiles,
    profilemanager_get_profile_list_profile_name_len,
    profilemanager_get_profile_list_profile_name,
    profilemanager_save_profile_profile_name,
    profilemanager_load_profile_profile_name,
    profilemanager_delete_profile_profile_name,
    profilemanager_upload_profile_profile_json,
    profilemanager_download_profile_profile_name,
    profilemanager_download_profile_profile_json,
    profilemanager_get_active_profile_profile_name,
    profilemanager_active_profile_changed_profile_name,
    profilemanager_profile_loaded_profile_json,
    profilemanager_profile_list_updated_data_size,
    profilemanager_profile_list_updated_num_profiles,
    profilemanager_profile_list_updated_profile_name_len,
    profilemanager_profile_list_updated_profile_name,
    pluginmanager_get_plugin_list_data_size,
    pluginmanager_get_plugin_list_num_plugins,
    pluginmanager_get_plugin_list_plugin_name_len,
    pluginmanager_get_plugin_list_plugin_name,
    pluginmanager_get_plugin_list_plugin_desc_len,
    pluginmanager_get_plugin_list_plugin_desc,
    pluginmanager_get_plugin_list_plugin_ver_len,
    pluginmanager_get_plugin_list_plugin_ver,
    pluginmanager_get_plugin_list_plugin_index,
    pluginmanager_get_plugin_list_plugin_proto_ver,
    pluginmanager_plugin_specific_plugin_pkt_type,
    pluginmanager_plugin_specific_data,
    settingsmanager_get_settings_settings_key,
    settingsmanager_get_settings_settings_json,
    settingsmanager_get_settings_schema_settings_key,
    settingsmanager_get_settings_schema_settings_json,
    settingsmanager_modify_settings_settings_json,
    settingsmanager_set_settings_settings_json,
    logmanager_get_log_level_log_level,
    logmanager_set_log_level_log_level,
    logmanager_logged_entry_data_size,
    logmanager_logged_entry_log_level,
    logmanager_logged_entry_line,
    logmanager_logged_entry_timestamp,
    logmanager_logged_entry_filename_size,
    logmanager_logged_entry_filename,
    logmanager_logged_entry_text_size,
    logmanager_logged_entry_text,
    rgbcontroller_resizezone_zone_idx,
    rgbcontroller_resizezone_new_size,
    rgbcontroller_clearsegments_zone_idx,
    rgbcontroller_addsegment_data_size,
    rgbcontroller_addsegment_zone_idx,
    rgbcontroller_configurezone_data_size,
    rgbcontroller_configurezone_zone_idx,
    rgbcontroller_configuredevice_data_size,
    rgbcontroller_configuredevice_flags,
    rgbcontroller_configuredevice_name_len,
    rgbcontroller_configuredevice_name,
    rgbcontroller_sethidden_hidden,
    rgbcontroller_updateleds_data_size,
    rgbcontroller_updateleds_num_colors,
    rgbcontroller_updateleds_color,
    rgbcontroller_updatezoneleds_data_size,
    rgbcontroller_updatezoneleds_zone_idx,
    rgbcontroller_updatezoneleds_num_colors,
    rgbcontroller_updatezoneleds_color,
    rgbcontroller_updatesingleled_led_idx,
    rgbcontroller_updatesingleled_color,
    rgbcontroller_updatemode_data_size,
    rgbcontroller_updatemode_mode_idx,
    rgbcontroller_updatezonemode_data_size,
    rgbcontroller_updatezonemode_zone_idx,
    rgbcontroller_updatezonemode_mode_idx,
    rgbcontroller_setdevicespecific_config,
    rgbcontroller_setdevicezoneconfig_zone_idx,
    rgbcontroller_setdevicezoneconfig_config_size,
    rgbcontroller_setdevicezoneconfig_config,
    rgbcontroller_signalupdate_data_size,
    rgbcontroller_signalupdate_update_reason,
    pkt_data,
    version_client,
    version_negotiated,
    version_server
}

--[[---------------------------------------------------------
- Version tracking tables                                   -
-----------------------------------------------------------]]
local client_version_table      = {}
local server_version_table      = {}

function openrgb_protocol.dissector(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Get length                                            -
    -------------------------------------------------------]]
    local length                = buffer:len()

    --[[-----------------------------------------------------
    - Skip if not enough data for OpenRGB header            -
    -------------------------------------------------------]]
    if length < 0 then
        return
    end

    --[[-----------------------------------------------------
    - Determine if packet sent by client or server          -
    -------------------------------------------------------]]
    local client                = false
    local client_ip             = nil
    local client_port           = nil
    local server                = false
    local server_ip             = nil
    local server_port           = nil

    if pinfo.src_port == 6742 then
        client                  = false
        client_ip               = pinfo.net_dst
        client_port             = pinfo.dst_port
        server                  = true
        server_ip               = pinfo.net_src
        server_port             = pinfo.src_port
    elseif pinfo.dst_port == 6742 then
        client                  = true
        client_ip               = pinfo.net_src
        client_port             = pinfo.src_port
        server                  = false
        server_ip               = pinfo.net_dst
        server_port             = pinfo.dst_port
    end

    --[[-----------------------------------------------------
    - Get version data for this conversation and default to -
    - 0 if invalid                                          -
    -------------------------------------------------------]]
    local conversation_key      = tostring(client_ip) .. ":" .. tostring(client_port) .. "-" .. tostring(server_ip) .. ":" .. tostring(server_port)

    if not client_version_table[conversation_key] then
        client_version_table[conversation_key] = 0
    end

    if not server_version_table[conversation_key] then
        server_version_table[conversation_key] = 0
    end

    local negotiated_version    = 0

    if client_version_table[conversation_key] < server_version_table[conversation_key] then
        negotiated_version = client_version_table[conversation_key]
    else
        negotiated_version = server_version_table[conversation_key]
    end

    --[[-----------------------------------------------------
    - Ensure we have the full packet, both header and data  -
    -------------------------------------------------------]]
    local pkt_size              = buffer(12, 4):le_uint()

    if (pkt_size + 16) > length then
        pinfo.desegment_len     = DESEGMENT_ONE_MORE_SEGMENT
        pinfo.desegment_offset  = 0
        return
    end

    --[[-----------------------------------------------------
    - Parse the OpenRGB protocol into a subtree             -
    -------------------------------------------------------]]
    pinfo.cols.protocol = openrgb_protocol.name
    local subtree = tree:add(openrgb_protocol, buffer(), "OpenRGB Protocol Data")

    if pinfo.src_port == 6742 then
        server = true
        subtree:add("Server")
    elseif pinfo.dst_port == 6742 then
        server = false
        subtree:add("Client")
    end

    --[[-----------------------------------------------------
    - Parse the header into a subtree                       -
    -------------------------------------------------------]]
    local headertree    = subtree:add("OpenRGB Protocol Header")
    local pkt_id        = buffer(8,4):le_uint()
    local pkt_id_name   = get_pkt_id_name(pkt_id)

    headertree:add_le(header_pkt_magic,    buffer(0,4))
    headertree:add_le(header_pkt_dev_id,   buffer(4,4))
    headertree:add_le(header_pkt_id,       pkt_id):append_text(" (" .. pkt_id_name .. ")")
    headertree:add_le(header_pkt_size,     pkt_size)

    if pkt_id == 0 then
        request_controller_count(buffer(16), pinfo, subtree)
    elseif pkt_id == 1 then
        request_controller_data(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 10 then
        ack(buffer(16), pinfo, subtree)
    elseif pkt_id == 40 then
        request_protocol_version(buffer(16), pinfo, subtree, conversation_key)
    elseif pkt_id == 50 then
        set_client_name(buffer(16), pinfo, subtree)
    elseif pkt_id == 51 then
        set_server_name(buffer(16), pinfo, subtree)
    elseif pkt_id == 52 then
        set_client_flags(buffer(16), pinfo, subtree)
    elseif pkt_id == 53 then
        set_server_flags(buffer(16), pinfo, subtree)
    elseif pkt_id == 100 then
        device_list_updated(buffer(16), pinfo, subtree)
    elseif pkt_id == 101 then
        detection_started(buffer(16), pinfo, subtree)
    elseif pkt_id == 102 then
        detection_progress_changed(buffer(16), pinfo, subtree)
    elseif pkt_id == 103 then
        detection_complete(buffer(16), pinfo, subtree)
    elseif pkt_id == 120 then
        get_i2c_bus_info(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 121 then
        get_hid_device_info(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 122 then
        get_usb_device_info(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 123 then
        get_serial_ports(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 140 then
        request_rescan_devices(buffer(16), pinfo, subtree)
    elseif pkt_id == 150 then
        profilemanager_get_profile_list(buffer(16), pinfo, subtree)
    elseif pkt_id == 151 then
        profilemanager_save_profile(buffer(16), pinfo, subtree)
    elseif pkt_id == 152 then
        profilemanager_load_profile(buffer(16), pinfo, subtree)
    elseif pkt_id == 153 then
        profilemanager_delete_profile(buffer(16), pinfo, subtree)
    elseif pkt_id == 154 then
        profilemanager_upload_profile(buffer(16), pinfo, subtree)
    elseif pkt_id == 155 then
        profilemanager_download_profile(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 156 then
        profilemanager_get_active_profile(buffer(16), pinfo, subtree)
    elseif pkt_id == 157 then
        profilemanager_active_profile_changed(buffer(16), pinfo, subtree)
    elseif pkt_id == 158 then
        profilemanager_profile_loaded(buffer(16), pinfo, subtree)
    elseif pkt_id == 159 then
        profilemanager_profile_about_to_load(buffer(16), pinfo, subtree)
    elseif pkt_id == 160 then
        profilemanager_profile_list_updated(buffer(16), pinfo, subtree)
    elseif pkt_id == 161 then
        profilemanager_clear_active_profile(buffer(16), pinfo, subtree)
    elseif pkt_id == 200 then
        pluginmanager_get_plugin_list(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 201 then
        pluginmanager_plugin_specific(buffer(16), pinfo, subtree)
    elseif pkt_id == 250 then
        settingsmanager_get_settings(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 251 then
        settingsmanager_get_settings_schema(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 252 then
        settingsmanager_modify_settings(buffer(16), pinfo, subtree)
    elseif pkt_id == 253 then
        settingsmanager_set_settings(buffer(16), pinfo, subtree)
    elseif pkt_id == 254 then
        settingsmanager_save_settings(buffer(16), pinfo, subtree)
    elseif pkt_id == 300 then
        logmanager_clear_log_buffer(buffer(16), pinfo, subtree)
    elseif pkt_id == 301 then
        logmanager_get_log_buffer(buffer(16), pinfo, subtree)
    elseif pkt_id == 302 then
        logmanager_get_log_level(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 303 then
        logmanager_set_log_level(buffer(16), pinfo, subtree)
    elseif pkt_id == 304 then
        logmanager_logged_entry(buffer(16), pinfo, subtree)
    elseif pkt_id == 1000 then
        rgbcontroller_resizezone(buffer(16), pinfo, subtree)
    elseif pkt_id == 1001 then
        rgbcontroller_clearsegments(buffer(16), pinfo, subtree)
    elseif pkt_id == 1002 then
        rgbcontroller_addsegment(buffer(16), pinfo, subtree)
    elseif pkt_id == 1003 then
        rgbcontroller_configurezone(buffer(16), pinfo, subtree)
    elseif pkt_id == 1004 then
        rgbcontroller_configuredevice(buffer(16), pinfo, subtree)
    elseif pkt_id == 1005 then
        rgbcontroller_sethidden(buffer(16), pinfo, subtree)
    elseif pkt_id == 1050 then
        rgbcontroller_updateleds(buffer(16), pinfo, subtree)
    elseif pkt_id == 1051 then
        rgbcontroller_updatezoneleds(buffer(16), pinfo, subtree)
    elseif pkt_id == 1052 then
        rgbcontroller_updatesingleled(buffer(16), pinfo, subtree)
    elseif pkt_id == 1100 then
        rgbcontroller_setcustommode(buffer(16), pinfo, subtree)
    elseif pkt_id == 1101 then
        rgbcontroller_updatemode(buffer(16), pinfo, subtree)
    elseif pkt_id == 1102 then
        rgbcontroller_savemode(buffer(16), pinfo, subtree)
    elseif pkt_id == 1103 then
        rgbcontroller_updatezonemode(buffer(16), pinfo, subtree)
    elseif pkt_id == 1130 then
        rgbcontroller_setdevicespecificconfiguration(buffer(16), pinfo, subtree)
    elseif pkt_id == 1131 then
        rgbcontroller_setdevicespecificzoneconfiguration(buffer(16), pinfo, subtree)
    elseif pkt_id == 1150 then
        rgbcontroller_signalupdate(buffer(16), pinfo, subtree)

    --[[-----------------------------------------------------
    - If no dedicated parser for this packet exists, add    -
    - raw data field                                        -
    -------------------------------------------------------]]
    elseif pkt_size > 0 then
        subtree:add(pkt_data,               buffer(16, pkt_size))
    end

    local versiontree   = subtree:add("Protocol Version Negotiation")

    versiontree:add(version_client,     client_version_table[conversation_key])
    versiontree:add(version_server,     server_version_table[conversation_key])
    versiontree:add(version_negotiated, negotiated_version)
end

--[[---------------------------------------------------------
- Register protocol dissector for OpenRGB SDK port 6742     -
-----------------------------------------------------------]]
local tcp_port = DissectorTable.get("tcp.port")
tcp_port:add(6742, openrgb_protocol)

--[[---------------------------------------------------------
- NET_PACKET_ID_REQUEST_CONTROLLER_COUNT                    -
-----------------------------------------------------------]]
function request_controller_count(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("Request Controller Count")

    subtree:add_le(request_controller_count_count,  buffer(0,4))

    if length > 4 then
        local offset            = 4

        while offset + 4 <= length do
            subtree:add_le(request_controller_count_id_list,    buffer(offset,4))
            offset              = offset + 4
        end
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_REQUEST_CONTROLLER_DATA                     -
-----------------------------------------------------------]]
function request_controller_data(buffer, pinfo, tree, client)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    if client == true then
        local subtree               = tree:add("Request Controller Data")

        subtree:add_le(request_controller_data_protocol_version,    buffer(0,4))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_ACK                                         -
-----------------------------------------------------------]]
function ack(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()
    local pkt_id                = buffer(0,4):le_uint()
    local pkt_id_name           = get_pkt_id_name(pkt_id)
    local status                = buffer(4,4):le_uint()
    local status_name           = get_status_name(status)
    local subtree               = tree:add("Ack")
    subtree:add_le(ack_acked_pkt_id,    pkt_id):append_text(" (" .. pkt_id_name .. ")")
    subtree:add_le(ack_status,          buffer(4,4)):append_text(" (" .. status_name .. ")")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_REQUEST_PROTOCOL_VERSION                    -
-----------------------------------------------------------]]
function request_protocol_version(buffer, pinfo, tree, conversation_key)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()
    local protocol_version      = buffer(0,4)
    local subtree               = tree:add("Request Protocol Version")
    subtree:add_le(request_protocol_version_protocol_version, protocol_version)

    if pinfo.src_port == 6742 then
        server_version_table[conversation_key] = protocol_version:le_uint()
    elseif pinfo.dst_port == 6742 then
        client_version_table[conversation_key] = protocol_version:le_uint()
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_SET_CLIENT_NAME                             -
-----------------------------------------------------------]]
function set_client_name(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()
    local client_name           = buffer(0,length)
    local subtree               = tree:add("Set Client Name")
    subtree:add(set_client_name_client_name, client_name)
end

--[[---------------------------------------------------------
- NET_PACKET_ID_SET_SERVER_NAME                             -
-----------------------------------------------------------]]
function set_server_name(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()
    local server_name           = buffer(0,length)
    local subtree               = tree:add("Set Server Name")
    subtree:add(set_server_name_server_name, server_name)
end

--[[---------------------------------------------------------
- NET_PACKET_ID_SET_CLIENT_FLAGS                            -
-----------------------------------------------------------]]
function set_client_flags(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()
    local subtree               = tree:add("Set Client Flags")

    subtree:add_le(set_client_flags_client_flags,   buffer(0,4))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_SET_SERVER_FLAGS                            -
-----------------------------------------------------------]]
function set_server_flags(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()
    local subtree               = tree:add("Set Server Flags")

    subtree:add_le(set_server_flags_server_flags,   buffer(0,4))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_DEVICE_LIST_UPDATED                         -
-----------------------------------------------------------]]
function device_list_updated(buffer, pinfo, tree)
    local subtree               = tree:add("Device List Updated")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_DETECTION_STARTED                           -
-----------------------------------------------------------]]
function detection_started(buffer, pinfo, tree)
    local subtree               = tree:add("Detection Started")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_DETECTION_PROGRESS_CHANGED                  -
-----------------------------------------------------------]]
function detection_progress_changed(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("Detection Progress Changed")

    subtree:add_le(detection_progress_changed_data_size,    buffer(0,4))
    subtree:add_le(detection_progress_changed_percent,      buffer(4,4))

    if length > 8 then
        local string_len        = buffer(8,2):le_uint()
        subtree:add_le(detection_progress_changed_string_len, buffer(8,2))

        if length >= (10 + string_len) then
            subtree:add(detection_progress_changed_string,  buffer(10, string_len))
        end
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_DETECTION_COMPLETE                          -
-----------------------------------------------------------]]
function detection_complete(buffer, pinfo, tree)
    local subtree               = tree:add("Detection Complete")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_GET_I2C_BUS_INFO                            -
-----------------------------------------------------------]]
function get_i2c_bus_info(buffer, pinfo, tree, client)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    if client == false then
        local subtree           = tree:add("Get I2C Bus Info")

        subtree:add_le(get_i2c_bus_info_data_size,  buffer(0,4))
        subtree:add_le(get_i2c_bus_info_bus_count,  buffer(4,4))

        if length > 8 then
            subtree:add(get_i2c_bus_info_bus_data,  buffer(8, length - 8))
        end
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_GET_HID_DEVICE_INFO                         -
-----------------------------------------------------------]]
function get_hid_device_info(buffer, pinfo, tree, client)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    if client == false then
        local subtree           = tree:add("Get HID Device Info")

        local data_size         = buffer(0,4):le_uint()
        subtree:add_le(get_hid_device_info_data_size,   buffer(0,4))

        local device_count      = buffer(4,4):le_uint()
        subtree:add_le(get_hid_device_info_device_count, buffer(4,4))

        local offset            = 8

        for i = 1, device_count do
            local device_tree   = subtree:add("HID Device " .. i)

            device_tree:add_le(get_hid_device_info_vendor_id,       buffer(offset,4));     offset = offset + 4
            device_tree:add_le(get_hid_device_info_product_id,      buffer(offset,4));     offset = offset + 4
            device_tree:add_le(get_hid_device_info_release_number,  buffer(offset,4));     offset = offset + 4
            device_tree:add_le(get_hid_device_info_usage_page,      buffer(offset,4));     offset = offset + 4
            device_tree:add_le(get_hid_device_info_usage,           buffer(offset,4));     offset = offset + 4
            device_tree:add_le(get_hid_device_info_interface_number,buffer(offset,4));     offset = offset + 4

            local serial_size   = buffer(offset,2):le_uint()
            device_tree:add_le(get_hid_device_info_serial_number_size, buffer(offset,2));  offset = offset + 2
            if serial_size > 0 then
                device_tree:add(get_hid_device_info_serial_number,   buffer(offset, serial_size)); offset = offset + serial_size
            end

            local manuf_size    = buffer(offset,2):le_uint()
            device_tree:add_le(get_hid_device_info_manufacturer_size, buffer(offset,2));   offset = offset + 2
            if manuf_size > 0 then
                device_tree:add(get_hid_device_info_manufacturer_string, buffer(offset, manuf_size)); offset = offset + manuf_size
            end

            local prod_size     = buffer(offset,2):le_uint()
            device_tree:add_le(get_hid_device_info_product_string_size, buffer(offset,2)); offset = offset + 2
            if prod_size > 0 then
                device_tree:add(get_hid_device_info_product_string,  buffer(offset, prod_size)); offset = offset + prod_size
            end

            local path_size     = buffer(offset,2):le_uint()
            device_tree:add_le(get_hid_device_info_path_size,        buffer(offset,2));   offset = offset + 2
            if path_size > 0 then
                device_tree:add(get_hid_device_info_path,            buffer(offset, path_size)); offset = offset + path_size
            end
        end
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_GET_USB_DEVICE_INFO                         -
-----------------------------------------------------------]]
function get_usb_device_info(buffer, pinfo, tree, client)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    if client == false then
        local subtree           = tree:add("Get USB Device Info")

        local data_size         = buffer(0,4):le_uint()
        subtree:add_le(get_usb_device_info_data_size,   buffer(0,4))

        local device_count      = buffer(4,4):le_uint()
        subtree:add_le(get_usb_device_info_device_count, buffer(4,4))

        local offset            = 8

        for i = 1, device_count do
            local device_tree   = subtree:add("USB Device " .. i)

            device_tree:add_le(get_usb_device_info_vendor_id,       buffer(offset,4));     offset = offset + 4
            device_tree:add_le(get_usb_device_info_product_id,      buffer(offset,4));     offset = offset + 4

            local serial_size   = buffer(offset,2):le_uint()
            device_tree:add_le(get_usb_device_info_serial_number_size, buffer(offset,2));  offset = offset + 2
            if serial_size > 0 then
                device_tree:add(get_usb_device_info_serial_number,   buffer(offset, serial_size)); offset = offset + serial_size
            end

            local manuf_size    = buffer(offset,2):le_uint()
            device_tree:add_le(get_usb_device_info_manufacturer_size, buffer(offset,2));   offset = offset + 2
            if manuf_size > 0 then
                device_tree:add(get_usb_device_info_manufacturer_string, buffer(offset, manuf_size)); offset = offset + manuf_size
            end

            local prod_size     = buffer(offset,2):le_uint()
            device_tree:add_le(get_usb_device_info_product_string_size, buffer(offset,2)); offset = offset + 2
            if prod_size > 0 then
                device_tree:add(get_usb_device_info_product_string,  buffer(offset, prod_size)); offset = offset + prod_size
            end
        end
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_GET_SERIAL_PORTS                            -
-----------------------------------------------------------]]
function get_serial_ports(buffer, pinfo, tree, client)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    if client == false then
        local subtree           = tree:add("Get Serial Ports")

        local data_size         = buffer(0,4):le_uint()
        subtree:add_le(get_serial_ports_data_size,  buffer(0,4))

        local port_count        = buffer(4,4):le_uint()
        subtree:add_le(get_serial_ports_port_count, buffer(4,4))

        local offset            = 8

        for i = 1, port_count do
            local port_tree     = subtree:add("Serial Port " .. i)

            local port_string_size = buffer(offset,2):le_uint()
            port_tree:add_le(get_serial_ports_port_string_size, buffer(offset,2)); offset = offset + 2
            if port_string_size > 0 then
                port_tree:add(get_serial_ports_port_string, buffer(offset, port_string_size)); offset = offset + port_string_size
            end
        end
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_REQUEST_RESCAN_DEVICES                      -
-----------------------------------------------------------]]
function request_rescan_devices(buffer, pinfo, tree)
    local subtree               = tree:add("Request Rescan Devices")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_GET_PROFILE_LIST             -
-----------------------------------------------------------]]
function profilemanager_get_profile_list(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("ProfileManager Get Profile List")

    subtree:add_le(profilemanager_get_profile_list_data_size,       buffer(0,4))
    subtree:add_le(profilemanager_get_profile_list_num_profiles,    buffer(4,2))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_SAVE_PROFILE                 -
-----------------------------------------------------------]]
function profilemanager_save_profile(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("ProfileManager Save Profile")

    subtree:add(profilemanager_save_profile_profile_name,           buffer(0,length))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_LOAD_PROFILE                 -
-----------------------------------------------------------]]
function profilemanager_load_profile(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("ProfileManager Load Profile")

    subtree:add(profilemanager_load_profile_profile_name,           buffer(0,length))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_DELETE_PROFILE               -
-----------------------------------------------------------]]
function profilemanager_delete_profile(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("ProfileManager Delete Profile")

    subtree:add(profilemanager_delete_profile_profile_name,         buffer(0,length))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_UPLOAD_PROFILE               -
-----------------------------------------------------------]]
function profilemanager_upload_profile(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("ProfileManager Upload Profile")

    subtree:add(profilemanager_upload_profile_profile_json,         buffer(0,length))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_DOWNLOAD_PROFILE             -
-----------------------------------------------------------]]
function profilemanager_download_profile(buffer, pinfo, tree, client)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    if client == true then
        local subtree           = tree:add("ProfileManager Download Profile")

        subtree:add(profilemanager_download_profile_profile_name,   buffer(0,length))
    else
        local subtree           = tree:add("ProfileManager Download Profile")

        subtree:add(profilemanager_download_profile_profile_json,   buffer(0,length))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_GET_ACTIVE_PROFILE           -
-----------------------------------------------------------]]
function profilemanager_get_active_profile(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("ProfileManager Get Active Profile")

    subtree:add(profilemanager_get_active_profile_profile_name,     buffer(0,length))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_ACTIVE_PROFILE_CHANGED       -
-----------------------------------------------------------]]
function profilemanager_active_profile_changed(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("ProfileManager Active Profile Changed")

    subtree:add(profilemanager_active_profile_changed_profile_name, buffer(0,length))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_PROFILE_LOADED               -
-----------------------------------------------------------]]
function profilemanager_profile_loaded(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("ProfileManager Profile Loaded")

    subtree:add(profilemanager_profile_loaded_profile_json,         buffer(0,length))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_PROFILE_ABOUT_TO_LOAD        -
-----------------------------------------------------------]]
function profilemanager_profile_about_to_load(buffer, pinfo, tree)
    local subtree               = tree:add("ProfileManager Profile About To Load")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_PROFILE_LIST_UPDATED         -
-----------------------------------------------------------]]
function profilemanager_profile_list_updated(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("ProfileManager Profile List Updated")

    subtree:add_le(profilemanager_profile_list_updated_data_size,   buffer(0,4))
    subtree:add_le(profilemanager_profile_list_updated_num_profiles,buffer(4,2))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_CLEAR_ACTIVE_PROFILE         -
-----------------------------------------------------------]]
function profilemanager_clear_active_profile(buffer, pinfo, tree)
    local subtree               = tree:add("ProfileManager Clear Active Profile")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PLUGINMANAGER_GET_PLUGIN_LIST               -
-----------------------------------------------------------]]
function pluginmanager_get_plugin_list(buffer, pinfo, tree, client)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    if client == false then
        local subtree           = tree:add("PluginManager Get Plugin List")

        subtree:add_le(pluginmanager_get_plugin_list_data_size,     buffer(0,4))

        local num_plugins       = buffer(4,2):le_uint()
        subtree:add_le(pluginmanager_get_plugin_list_num_plugins,   buffer(4,2))

        local offset            = 6

        for i = 1, num_plugins do
            local plugin_tree   = subtree:add("Plugin " .. i)

            local name_len      = buffer(offset,2):le_uint()
            plugin_tree:add_le(pluginmanager_get_plugin_list_plugin_name_len, buffer(offset,2)); offset = offset + 2
            if name_len > 0 then
                plugin_tree:add(pluginmanager_get_plugin_list_plugin_name, buffer(offset, name_len)); offset = offset + name_len
            end

            local desc_len      = buffer(offset,2):le_uint()
            plugin_tree:add_le(pluginmanager_get_plugin_list_plugin_desc_len, buffer(offset,2)); offset = offset + 2
            if desc_len > 0 then
                plugin_tree:add(pluginmanager_get_plugin_list_plugin_desc, buffer(offset, desc_len)); offset = offset + desc_len
            end

            local ver_len       = buffer(offset,2):le_uint()
            plugin_tree:add_le(pluginmanager_get_plugin_list_plugin_ver_len, buffer(offset,2)); offset = offset + 2
            if ver_len > 0 then
                plugin_tree:add(pluginmanager_get_plugin_list_plugin_ver, buffer(offset, ver_len)); offset = offset + ver_len
            end

            plugin_tree:add_le(pluginmanager_get_plugin_list_plugin_index,    buffer(offset,4)); offset = offset + 4
            plugin_tree:add_le(pluginmanager_get_plugin_list_plugin_proto_ver,buffer(offset,4)); offset = offset + 4
        end
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_PLUGINMANAGER_PLUGIN_SPECIFIC               -
-----------------------------------------------------------]]
function pluginmanager_plugin_specific(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("PluginManager Plugin Specific")

    subtree:add_le(pluginmanager_plugin_specific_plugin_pkt_type,  buffer(0,4))

    if length > 4 then
        subtree:add(pluginmanager_plugin_specific_data,            buffer(4, length - 4))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS                -
-----------------------------------------------------------]]
function settingsmanager_get_settings(buffer, pinfo, tree, client)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("SettingsManager Get Settings")

    if client == true then
        subtree:add(settingsmanager_get_settings_settings_key,      buffer(0, length))
    else
        subtree:add(settingsmanager_get_settings_settings_json,     buffer(0, length))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS_SCHEMA         -
-----------------------------------------------------------]]
function settingsmanager_get_settings_schema(buffer, pinfo, tree, client)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("SettingsManager Get Settings Schema")

    if client == true then
        subtree:add(settingsmanager_get_settings_schema_settings_key,   buffer(0, length))
    else
        subtree:add(settingsmanager_get_settings_schema_settings_json,  buffer(0, length))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_MODIFY_SETTINGS             -
-----------------------------------------------------------]]
function settingsmanager_modify_settings(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("SettingsManager Modify Settings")

    subtree:add(settingsmanager_modify_settings_settings_json,      buffer(0, length))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_SET_SETTINGS                -
-----------------------------------------------------------]]
function settingsmanager_set_settings(buffer, pinfo, tree)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("SettingsManager Set Settings")

    subtree:add(settingsmanager_set_settings_settings_json,         buffer(0, length))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_SAVE_SETTINGS               -
-----------------------------------------------------------]]
function settingsmanager_save_settings(buffer, pinfo, tree)
    local subtree               = tree:add("SettingsManager Save Settings")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_LOGMANAGER_CLEAR_LOG_BUFFER                 -
-----------------------------------------------------------]]
function logmanager_clear_log_buffer(buffer, pinfo, tree)
    local subtree               = tree:add("LogManager Clear Log Buffer")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_LOGMANAGER_GET_LOG_BUFFER                   -
-----------------------------------------------------------]]
function logmanager_get_log_buffer(buffer, pinfo, tree)
    local subtree               = tree:add("LogManager Get Log Buffer")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_LOGMANAGER_GET_LOG_LEVEL                    -
-----------------------------------------------------------]]
function logmanager_get_log_level(buffer, pinfo, tree, client)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    if client == false then
        local subtree           = tree:add("LogManager Get Log Level")

        subtree:add_le(logmanager_get_log_level_log_level,          buffer(0,4))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_LOGMANAGER_SET_LOG_LEVEL                    -
-----------------------------------------------------------]]
function logmanager_set_log_level(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("LogManager Set Log Level")

    subtree:add_le(logmanager_set_log_level_log_level,              buffer(0,4))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_LOGMANAGER_LOGGED_ENTRY                     -
-----------------------------------------------------------]]
function logmanager_logged_entry(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("LogManager Logged Entry")

    subtree:add_le(logmanager_logged_entry_data_size,   buffer(0,4))
    subtree:add_le(logmanager_logged_entry_log_level,   buffer(4,4))
    subtree:add_le(logmanager_logged_entry_line,        buffer(8,4))
    subtree:add_le(logmanager_logged_entry_timestamp,   buffer(12,8))

    local offset                = 20

    local filename_size         = buffer(offset,2):le_uint()
    subtree:add_le(logmanager_logged_entry_filename_size, buffer(offset,2)); offset = offset + 2
    if filename_size > 0 then
        subtree:add(logmanager_logged_entry_filename,   buffer(offset, filename_size)); offset = offset + filename_size
    end

    local text_size             = buffer(offset,2):le_uint()
    subtree:add_le(logmanager_logged_entry_text_size,   buffer(offset,2)); offset = offset + 2
    if text_size > 0 then
        subtree:add(logmanager_logged_entry_text,       buffer(offset, text_size)); offset = offset + text_size
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE                    -
-----------------------------------------------------------]]
function rgbcontroller_resizezone(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Resize Zone")

    subtree:add_le(rgbcontroller_resizezone_zone_idx,   buffer(0,4))
    subtree:add_le(rgbcontroller_resizezone_new_size,   buffer(4,4))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_CLEARSEGMENTS                 -
-----------------------------------------------------------]]
function rgbcontroller_clearsegments(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Clear Segments")

    subtree:add_le(rgbcontroller_clearsegments_zone_idx,buffer(0,4))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT                    -
-----------------------------------------------------------]]
function rgbcontroller_addsegment(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Add Segment")

    subtree:add_le(rgbcontroller_addsegment_data_size,  buffer(0,4))
    subtree:add_le(rgbcontroller_addsegment_zone_idx,   buffer(4,4))

    if length > 8 then
        subtree:add(pkt_data,                           buffer(8, length - 8))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_CONFIGUREZONE                 -
-----------------------------------------------------------]]
function rgbcontroller_configurezone(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Configure Zone")

    subtree:add_le(rgbcontroller_configurezone_data_size, buffer(0,4))
    subtree:add_le(rgbcontroller_configurezone_zone_idx,  buffer(4,4))

    if length > 8 then
        subtree:add(pkt_data,                           buffer(8, length - 8))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_CONFIGUREDEVICE               -
-----------------------------------------------------------]]
function rgbcontroller_configuredevice(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Configure Device")

    subtree:add_le(rgbcontroller_configuredevice_data_size, buffer(0,4))
    subtree:add_le(rgbcontroller_configuredevice_flags,     buffer(4,4))

    if length > 8 then
        local name_len          = buffer(8,2):le_uint()
        subtree:add_le(rgbcontroller_configuredevice_name_len, buffer(8,2))

        if length >= (10 + name_len) then
            subtree:add(rgbcontroller_configuredevice_name, buffer(10, name_len))
        end
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_SETHIDDEN                     -
-----------------------------------------------------------]]
function rgbcontroller_sethidden(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Set Hidden")

    local hidden_val            = buffer(0,1):le_uint()
    subtree:add(rgbcontroller_sethidden_hidden,         buffer(0,1)):append_text(" (" .. tostring(hidden_val) .. ")")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS                    -
-----------------------------------------------------------]]
function rgbcontroller_updateleds(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Update LEDs")

    subtree:add_le(rgbcontroller_updateleds_data_size,  buffer(0,4))
    subtree:add_le(rgbcontroller_updateleds_num_colors, buffer(4,2))

    if length > 6 then
        subtree:add(rgbcontroller_updateleds_color,     buffer(6, length - 6))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS                -
-----------------------------------------------------------]]
function rgbcontroller_updatezoneleds(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Update Zone LEDs")

    subtree:add_le(rgbcontroller_updatezoneleds_data_size,  buffer(0,4))
    subtree:add_le(rgbcontroller_updatezoneleds_zone_idx,   buffer(4,4))
    subtree:add_le(rgbcontroller_updatezoneleds_num_colors, buffer(8,2))

    if length > 10 then
        subtree:add(rgbcontroller_updatezoneleds_color,     buffer(10, length - 10))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED               -
-----------------------------------------------------------]]
function rgbcontroller_updatesingleled(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Update Single LED")

    subtree:add_le(rgbcontroller_updatesingleled_led_idx, buffer(0,4))
    subtree:add(rgbcontroller_updatesingleled_color,      buffer(4,4))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE                 -
-----------------------------------------------------------]]
function rgbcontroller_setcustommode(buffer, pinfo, tree)
    local subtree               = tree:add("RGBController Set Custom Mode")
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE                    -
-----------------------------------------------------------]]
function rgbcontroller_updatemode(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Update Mode")

    subtree:add_le(rgbcontroller_updatemode_data_size,  buffer(0,4))
    subtree:add_le(rgbcontroller_updatemode_mode_idx,   buffer(4,4))

    if length > 8 then
        subtree:add(pkt_data,                           buffer(8, length - 8))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_SAVEMODE                      -
-----------------------------------------------------------]]
function rgbcontroller_savemode(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Save Mode")

    subtree:add_le(rgbcontroller_updatemode_data_size,  buffer(0,4))
    subtree:add_le(rgbcontroller_updatemode_mode_idx,   buffer(4,4))

    if length > 8 then
        subtree:add(pkt_data,                           buffer(8, length - 8))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_UPDATEZONEMODE                -
-----------------------------------------------------------]]
function rgbcontroller_updatezonemode(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Update Zone Mode")

    subtree:add_le(rgbcontroller_updatezonemode_data_size,  buffer(0,4))
    subtree:add_le(rgbcontroller_updatezonemode_zone_idx,   buffer(4,4))
    subtree:add_le(rgbcontroller_updatezonemode_mode_idx,   buffer(8,4))

    if length > 12 then
        subtree:add(pkt_data,                               buffer(12, length - 12))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_SETDEVICESPECIFICCONFIGURATION -
-----------------------------------------------------------]]
function rgbcontroller_setdevicespecificconfiguration(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Set Device Specific Configuration")

    subtree:add(rgbcontroller_setdevicespecific_config,     buffer(0, length))
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_SETDEVICESPECIFICZONECONFIGURATION -
-----------------------------------------------------------]]
function rgbcontroller_setdevicespecificzoneconfiguration(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Set Device Specific Zone Configuration")

    subtree:add_le(rgbcontroller_setdevicezoneconfig_zone_idx,  buffer(0,4))
    subtree:add_le(rgbcontroller_setdevicezoneconfig_config_size, buffer(4,4))

    if length > 8 then
        subtree:add(rgbcontroller_setdevicezoneconfig_config,   buffer(8, length - 8))
    end
end

--[[---------------------------------------------------------
- NET_PACKET_ID_RGBCONTROLLER_SIGNALUPDATE                  -
-----------------------------------------------------------]]
function rgbcontroller_signalupdate(buffer, pinfo, tree)
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("RGBController Signal Update")

    subtree:add_le(rgbcontroller_signalupdate_data_size,    buffer(0,4))
    subtree:add_le(rgbcontroller_signalupdate_update_reason,buffer(4,4))

    if length > 8 then
        subtree:add(pkt_data,                               buffer(8, length - 8))
    end
end

--[[---------------------------------------------------------
-   get_pkt_id_name                                         -
-                                                           -
-   Get the NET_PACKET_ID name for the given ID             -
-----------------------------------------------------------]]
function get_pkt_id_name(pkt_id)
    local pkt_id_name = "Unknown"

    if      pkt_id ==    0 then pkt_id_name = "NET_PACKET_ID_REQUEST_CONTROLLER_COUNT"
    elseif  pkt_id ==    1 then pkt_id_name = "NET_PACKET_ID_REQUEST_CONTROLLER_DATA"
    elseif  pkt_id ==   10 then pkt_id_name = "NET_PACKET_ID_ACK"
    elseif  pkt_id ==   40 then pkt_id_name = "NET_PACKET_ID_REQUEST_PROTOCOL_VERSION"
    elseif  pkt_id ==   50 then pkt_id_name = "NET_PACKET_ID_SET_CLIENT_NAME"
    elseif  pkt_id ==   51 then pkt_id_name = "NET_PACKET_ID_SET_SERVER_NAME"
    elseif  pkt_id ==   52 then pkt_id_name = "NET_PACKET_ID_SET_CLIENT_FLAGS"
    elseif  pkt_id ==   53 then pkt_id_name = "NET_PACKET_ID_SET_SERVER_FLAGS"
    elseif  pkt_id ==  100 then pkt_id_name = "NET_PACKET_ID_DEVICE_LIST_UPDATED"
    elseif  pkt_id ==  101 then pkt_id_name = "NET_PACKET_ID_DETECTION_STARTED"
    elseif  pkt_id ==  102 then pkt_id_name = "NET_PACKET_ID_DETECTION_PROGRESS_CHANGED"
    elseif  pkt_id ==  103 then pkt_id_name = "NET_PACKET_ID_DETECTION_COMPLETE"
    elseif  pkt_id ==  120 then pkt_id_name = "NET_PACKET_ID_GET_I2C_BUS_INFO"
    elseif  pkt_id ==  121 then pkt_id_name = "NET_PACKET_ID_GET_HID_DEVICE_INFO"
    elseif  pkt_id ==  122 then pkt_id_name = "NET_PACKET_ID_GET_USB_DEVICE_INFO"
    elseif  pkt_id ==  123 then pkt_id_name = "NET_PACKET_ID_GET_SERIAL_PORTS"
    elseif  pkt_id ==  140 then pkt_id_name = "NET_PACKET_ID_REQUEST_RESCAN_DEVICES"
    elseif  pkt_id ==  150 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_GET_PROFILE_LIST"
    elseif  pkt_id ==  151 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_SAVE_PROFILE"
    elseif  pkt_id ==  152 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_LOAD_PROFILE"
    elseif  pkt_id ==  153 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_DELETE_PROFILE"
    elseif  pkt_id ==  154 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_UPLOAD_PROFILE"
    elseif  pkt_id ==  155 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_DOWNLOAD_PROFILE"
    elseif  pkt_id ==  156 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_GET_ACTIVE_PROFILE"
    elseif  pkt_id ==  157 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_ACTIVE_PROFILE_CHANGED"
    elseif  pkt_id ==  158 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_PROFILE_LOADED"
    elseif  pkt_id ==  159 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_PROFILE_ABOUT_TO_LOAD"
    elseif  pkt_id ==  160 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_PROFILE_LIST_UPDATED"
    elseif  pkt_id ==  161 then pkt_id_name = "NET_PACKET_ID_PROFILEMANAGER_CLEAR_ACTIVE_PROFILE"
    elseif  pkt_id ==  200 then pkt_id_name = "NET_PACKET_ID_PLUGINMANAGER_GET_PLUGIN_LIST"
    elseif  pkt_id ==  201 then pkt_id_name = "NET_PACKET_ID_PLUGINMANAGER_PLUGIN_SPECIFIC"
    elseif  pkt_id ==  250 then pkt_id_name = "NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS"
    elseif  pkt_id ==  251 then pkt_id_name = "NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS_SCHEMA"
    elseif  pkt_id ==  252 then pkt_id_name = "NET_PACKET_ID_SETTINGSMANAGER_MODIFY_SETTINGS"
    elseif  pkt_id ==  253 then pkt_id_name = "NET_PACKET_ID_SETTINGSMANAGER_SET_SETTINGS"
    elseif  pkt_id ==  254 then pkt_id_name = "NET_PACKET_ID_SETTINGSMANAGER_SAVE_SETTINGS"
    elseif  pkt_id ==  300 then pkt_id_name = "NET_PACKET_ID_LOGMANAGER_CLEAR_LOG_BUFFER"
    elseif  pkt_id ==  301 then pkt_id_name = "NET_PACKET_ID_LOGMANAGER_GET_LOG_BUFFER"
    elseif  pkt_id ==  302 then pkt_id_name = "NET_PACKET_ID_LOGMANAGER_GET_LOG_LEVEL"
    elseif  pkt_id ==  303 then pkt_id_name = "NET_PACKET_ID_LOGMANAGER_SET_LOG_LEVEL"
    elseif  pkt_id ==  304 then pkt_id_name = "NET_PACKET_ID_LOGMANAGER_LOGGED_ENTRY"
    elseif  pkt_id == 1000 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE"
    elseif  pkt_id == 1001 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_CLEARSEGMENTS"
    elseif  pkt_id == 1002 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT"
    elseif  pkt_id == 1003 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_CONFIGUREZONE"
    elseif  pkt_id == 1004 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_CONFIGUREDEVICE"
    elseif  pkt_id == 1005 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_SETHIDDEN"
    elseif  pkt_id == 1050 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS"
    elseif  pkt_id == 1051 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS"
    elseif  pkt_id == 1052 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED"
    elseif  pkt_id == 1100 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE"
    elseif  pkt_id == 1101 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE"
    elseif  pkt_id == 1102 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_SAVEMODE"
    elseif  pkt_id == 1103 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_UPDATEZONEMODE"
    elseif  pkt_id == 1130 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_SETDEVICESPECIFICCONFIGURATION"
    elseif  pkt_id == 1131 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_SETDEVICESPECIFICZONECONFIGURATION"
    elseif  pkt_id == 1150 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_SIGNALUPDATE"
    end

    return pkt_id_name
end

--[[---------------------------------------------------------
-   get_status_name                                         -
-                                                           -
-   Get the NET_PACKET_STATUS name for the given status     -
-----------------------------------------------------------]]
function get_status_name(status)
    local status_name = "Unknown"

    if      status == 0 then status_name = "NET_PACKET_STATUS_OK"
    elseif  status == 1 then status_name = "NET_PACKET_STATUS_ERROR_GENERIC"
    elseif  status == 2 then status_name = "NET_PACKET_STATUS_ERROR_UNSUPPORTED"
    elseif  status == 3 then status_name = "NET_PACKET_STATUS_ERROR_NOT_ALLOWED"
    elseif  status == 4 then status_name = "NET_PACKET_STATUS_ERROR_INVALID_ID"
    elseif  status == 5 then status_name = "NET_PACKET_STATUS_ERROR_INVALID_DATA"
    end

    return status_name
end
