/*---------------------------------------------------------*\
| AutoStart-Flatpak.cpp                                     |
|                                                           |
|   Autostart implementation for Linux via Flatpak Portal   |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      04 Sep 2026 |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "AutoStart-Linux.h"
#include <gio/gio.h>

AutoStart::AutoStart(std::string name)
{
    InitAutoStart(name);
}

void AutoStart::InitAutoStart(std::string name)
{
    app_name = name;
}

bool AutoStart::CallBackgroundPortal(bool enable, const AutoStartInfo& autostart_info)
{
    GError*             error           = nullptr;
    GDBusConnection*    connection      = g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, &error);
    
    if(!connection)
    {
        if(error)
        {
            g_error_free(error);
        }

        return false;
    }

    /*-----------------------------------------------------*\
    | FreeDesktop Background Portal parameters              |
    \*-----------------------------------------------------*/
    const char*         bus_name        = "org.freedesktop.portal.Desktop";
    const char*         object_path     = "/org/freedesktop/portal/desktop";
    const char*         interface_name  = "org.freedesktop.portal.Background";
    const char*         method_name     = "RequestBackground";

    /*-----------------------------------------------------*\
    | Build the options dictionary (vardict) required by    |
    | the Portal                                            |
    \*-----------------------------------------------------*/
    GVariantBuilder     options_builder;
    g_variant_builder_init(&options_builder, G_VARIANT_TYPE_VARDICT);

    /*-----------------------------------------------------*\
    | Reason shown to the user in the system settings /     |
    | permission prompt                                     |
    \*-----------------------------------------------------*/
    std::string         reason_str      = "Allow " + app_name + " to launch automatically at startup.";
    g_variant_builder_add(&options_builder, "{sv}", "reason", g_variant_new_string(reason_str.c_str()));
    
    /*-----------------------------------------------------*\
    | Toggle the autostart feature state                    |
    \*-----------------------------------------------------*/
    g_variant_builder_add(&options_builder, "{sv}", "autostart", g_variant_new_boolean(enable));

    if(enable)
    {
        /*-------------------------------------------------*\
        | Pass command-line arguments to the background     |
        | request                                           |
        \*-------------------------------------------------*/
        GVariantBuilder arg_builder;
        g_variant_builder_init(&arg_builder, G_VARIANT_TYPE("as"));
        
        /*-------------------------------------------------*\
        | Add the primary executable path inside the        |
        | Flatpak sandbox                                   |
        \*-------------------------------------------------*/
        g_variant_builder_add(&arg_builder, "s", GetExePath().c_str());

        /*-------------------------------------------------*\
        | Append any additional arguments provided by       |
        | OpenRGB's autostart config                        |
        \*-------------------------------------------------*/
        if(!autostart_info.args.empty())
        {
            g_variant_builder_add(&arg_builder, "s", autostart_info.args.c_str());
        }

        GVariant*   commandline_args    = g_variant_builder_end(&arg_builder);
        g_variant_builder_add(&options_builder, "{sv}", "commandline", commandline_args);
    }

    /*-----------------------------------------------------*\
    | An empty string window handle is used since this is a |
    | background configuration request                      |
    \*-----------------------------------------------------*/
    const char*     parent_window       = ""; 
    GVariant*       options             = g_variant_builder_end(&options_builder);

    /*-----------------------------------------------------*\
    | Bundle parameters: (s a{sv}) -> Parent window handle, |
    | Options dictionary                                    |
    \*-----------------------------------------------------*/
    GVariant*       parameters          = g_variant_new("(sa{sv})", parent_window, options);
    GVariant*       reply;
    
    reply = g_dbus_connection_call_sync(connection, bus_name, object_path, interface_name, method_name, parameters, G_VARIANT_TYPE("(o)"), G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error);

    g_object_unref(connection);

    if(error)
    {
        g_error_free(error);
        return false;
    }

    if(reply)
    {
        g_variant_unref(reply);
        return true;
    }

    return false;
}

bool AutoStart::EnableAutoStart(AutoStartInfo autostart_info)
{
    return CallBackgroundPortal(true, autostart_info);
}

bool AutoStart::DisableAutoStart()
{
    return CallBackgroundPortal(false);
}

bool AutoStart::IsAutoStartEnabled()
{
    /*-----------------------------------------------------*\
    | The FreeDesktop Background Portal does not provide a  |
    | stateless status getter.  Sandbox permissions are     |
    | securely managed and persisted internally by the      |
    | host's portal service.  To implement a true state     |
    | track here, OpenRGB will need to manage a local       |
    | boolean flag inside its own configuration files (e.g. |
    | OpenRGB.json).                                        |
    \*-----------------------------------------------------*/
    return false; 
}

std::string AutoStart::GetExePath()
{
    /*-----------------------------------------------------*\
    | Flatpak applications always run from this specific    |
    | exported path hierarchy                               |
    \*-----------------------------------------------------*/
    return "/app/bin/openrgb";
}
