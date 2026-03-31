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
- NET_PACKET_ID_PROFILEMANAGER_GET_PROFILE_LIST             -
-----------------------------------------------------------]]
profilemanager_get_profile_list_data_size           = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
profilemanager_get_profile_list_num_profiles        = ProtoField.uint16("openrgb.num_profiles",         "num_profiles",     base.DEC)
profilemanager_get_profile_list_profile_name_len    = ProtoField.uint16("openrgb.profile_name_len",     "profile_name_len", base.DEC)
profilemanager_get_profile_list_profile_name        = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_SAVE_PROFILE                 -
-----------------------------------------------------------]]
profilemanager_save_profile_profile_name            = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_LOAD_PROFILE                 -
-----------------------------------------------------------]]
profilemanager_load_profile_profile_name            = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_DELETE_PROFILE               -
-----------------------------------------------------------]]
profilemanager_delete_profile_profile_name          = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_UPLOAD_PROFILE               -
-----------------------------------------------------------]]
profilemanager_upload_profile_profile_json          = ProtoField.string("openrgb.profile_json",         "profile_json",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_DOWNLOAD_PROFILE             -
-----------------------------------------------------------]]
profilemanager_download_profile_profile_name        = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)
profilemanager_download_profile_profile_json        = ProtoField.string("openrgb.profile_json",         "profile_json",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_GET_ACTIVE_PROFILE           -
-----------------------------------------------------------]]
profilemanager_get_active_profile_profile_name      = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_ACTIVE_PROFILE_CHANGED       -
-----------------------------------------------------------]]
profilemanager_active_profile_changed_profile_name  = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_PROFILE_LOADED               -
-----------------------------------------------------------]]
profilemanager_profile_loaded_profile_json          = ProtoField.string("openrgb.profile_json",         "profile_json",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_PROFILEMANAGER_PROFILE_LIST_UPDATED         -
-----------------------------------------------------------]]
profilemanager_profile_list_updated_data_size       = ProtoField.uint32("openrgb.data_size",            "data_size",        base.DEC)
profilemanager_profile_list_updated_num_profiles    = ProtoField.uint16("openrgb.num_profiles",         "num_profiles",     base.DEC)
profilemanager_profile_list_updated_profile_name_len= ProtoField.uint16("openrgb.profile_name_len",     "profile_name_len", base.DEC)
profilemanager_profile_list_updated_profile_name    = ProtoField.string("openrgb.profile_name",         "profile_name",     base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS                -
-----------------------------------------------------------]]
settingsmanager_get_settings_settings_key           = ProtoField.string("openrgb.settings_key",         "settings_key",     base.STRING)
settingsmanager_get_settings_settings_json          = ProtoField.string("openrgb.settings_json",        "settings_json",    base.STRING)

--[[---------------------------------------------------------
- NET_PACKET_ID_SETTINGSMANAGER_SET_SETTINGS                -
-----------------------------------------------------------]]
settingsmanager_set_settings_settings_json          = ProtoField.string("openrgb.settings_json",        "settings_json",    base.STRING)

--[[---------------------------------------------------------
- Generic field for packet data bytes                       -
-----------------------------------------------------------]]
pkt_data                                            = ProtoField.bytes( "openrgb.pkt_data",             "pkt_data")

--[[---------------------------------------------------------
- Version fields                                            -
-----------------------------------------------------------]]
version_client                                      = ProtoField.uint32("openrgb.version_client",       "version_client",       base.DEC)
version_negotiated                                  = ProtoField.uint32("openrgb.version_negotiated",   "version_negotiated",   base.DEC)
version_server                                      = ProtoField.uint32("openrgb.version_server",       "version_server",       base.DEC)

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
    settingsmanager_get_settings_settings_key,
    settingsmanager_get_settings_settings_json,
    settingsmanager_set_settings_settings_json,
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
    elseif pkt_id == 156 then
        profilemanager_get_active_profile(buffer(16), pinfo, subtree)
    elseif pkt_id == 157 then
        profilemanager_active_profile_changed(buffer(16), pinfo, subtree)
    elseif pkt_id == 158 then
        profilemanager_profile_loaded(buffer(16), pinfo, subtree)
    elseif pkt_id == 160 then
        profilemanager_profile_list_updated(buffer(16), pinfo, subtree)
    elseif pkt_id == 250 then
        settingsmanager_get_settings(buffer(16), pinfo, subtree, client)
    elseif pkt_id == 251 then
        settingsmanager_set_settings(buffer(16), pinfo, subtree, client)

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
- NET_PACKET_ID_SETTINGSMANAGER_SET_SETTINGS                -
-----------------------------------------------------------]]
function settingsmanager_set_settings(buffer, pinfo, tree, client)
    --[[-----------------------------------------------------
    - Local variables                                       -
    -------------------------------------------------------]]
    local length                = buffer:len()

    if length == 0 then
        return
    end

    local subtree               = tree:add("SettingsManager Set Settings")

    if client == true then
        subtree:add(settingsmanager_set_settings_settings_json,     buffer(0, length))
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
    elseif  pkt_id ==  251 then pkt_id_name = "NET_PACKET_ID_SETTINGSMANAGER_SET_SETTINGS"
    elseif  pkt_id ==  252 then pkt_id_name = "NET_PACKET_ID_SETTINGSMANAGER_SAVE_SETTINGS"
    elseif  pkt_id == 1000 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE"
    elseif  pkt_id == 1001 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_CLEARSEGMENTS"
    elseif  pkt_id == 1002 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT"
    elseif  pkt_id == 1003 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_CONFIGUREZONE"
    elseif  pkt_id == 1050 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS"
    elseif  pkt_id == 1051 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS"
    elseif  pkt_id == 1052 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED"
    elseif  pkt_id == 1100 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE"
    elseif  pkt_id == 1101 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE"
    elseif  pkt_id == 1102 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_SAVEMODE"
    elseif  pkt_id == 1103 then pkt_id_name = "NET_PACKET_ID_RGBCONTROLLER_UPDATEZONEMODE"
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
