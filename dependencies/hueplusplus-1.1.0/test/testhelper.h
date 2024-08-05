/**
    \file testhelper.h
    Copyright Notice\n
    Copyright (C) 2017  Jan Rogall		- developer\n
    Copyright (C) 2017  Moritz Wirger	- developer\n

    This file is part of hueplusplus.

    hueplusplus is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    hueplusplus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with hueplusplus.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _TEST_HELPER_H
#define _TEST_HELPER_H

inline std::string getBridgeIp()
{
    return "192.168.2.116"; //!< IP-Address of the fake hue bridge in dotted
                            //!< decimal notation like "192.168.2.1"
}

inline int getBridgePort()
{
    return 80;
}

inline std::string getBridgeUsername()
{
    return "83b7780291a6ceffbe0bd049104df"; //!< Username that is used to access
                                            //!< the fake hue bridge
}
inline std::string getBridgeId()
{
    return "111111FFFE11E111";
}
inline std::string getBridgeUuid()
{
    return "1f111f11-da11-11e1-1b11-11111111e111";
}
inline std::string getBridgeMac()
{
    return "11111111e111";
}

inline std::string getBridgeXml()
{
    return R"xml(<?xml version="1.0" encoding="UTF-8" ?>
        <root xmlns="urn:schemas-upnp-org:device-1-0">
        <specVersion>
        <major>1</major>
        <minor>0</minor>
        </specVersion>
        <URLBase>http://192.168.2.116:80/</URLBase>
        <device>
        <deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>
        <friendlyName>Philips hue (192.168.2.116)</friendlyName>
        <manufacturer>Royal Philips Electronics</manufacturer>
        <manufacturerURL>http://www.philips.com</manufacturerURL>
        <modelDescription>Philips hue Personal Wireless Lighting</modelDescription>
        <modelName>Philips hue bridge 2015</modelName>
        <modelNumber>BSB002</modelNumber>
        <modelURL>http://www.meethue.com</modelURL>
        <serialNumber>11111111e111</serialNumber>
        <UDN>uuid:1f111f11-da11-11e1-1b11-11111111e111</UDN>
        <presentationURL>index.html</presentationURL>
        <iconList>
        <icon>
        <mimetype>image/png</mimetype>
        <height>48</height>
        <width>48</width>
        <depth>24</depth>
        <url>hue_logo_0.png</url>
        </icon>
        </iconList>
        </device>
        </root>)xml";
}

inline std::vector<std::string> getMulticastReply()
{
    return {"HTTP/1.1 200 OK\r\n"
            "CACHE-CONTROL: max-age=300\r\n"
            "DATE: Wed, 21 Jan 1970 05:42:21 GMT\r\n"
            "EXT:\r\n"
            "LOCATION: http://192.168.2.1:1900/gatedesc.xml\r\n"
            "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
            "01-NLS: 000c0000-0dd0-00b0-0da0-00a000e000c0\r\n"
            "SERVER: Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19\r\n"
            "X-User-Agent: redsonic\r\n"
            "ST: upnp:rootdevice\r\n"
            "USN: uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00::upnp:rootdevice",

        "HTTP/1.1 200 OK\r\n"
        "CACHE-CONTROL: max-age=300\r\n"
        "DATE: Wed, 21 Jan 1970 05:42:21 GMT\r\n"
        "EXT:\r\n"
        "LOCATION: http://192.168.2.1:1900/gatedesc.xml\r\n"
        "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
        "01-NLS: 000c0000-0dd0-00b0-0da0-00a000e000c0\r\n"
        "SERVER: Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19\r\n"
        "X-User-Agent: redsonic\r\n"
        "ST: uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00\r\n"
        "USN: uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00",

        "HTTP/1.1 200 OK\r\n"
        "CACHE-CONTROL: max-age=300\r\n"
        "DATE: Wed, 21 Jan 1970 05:42:21 GMT\r\n"
        "EXT:\r\n"
        "LOCATION: http://192.168.2.1:1900/gatedesc.xml\r\n"
        "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
        "01-NLS: 000c0000-0dd0-00b0-0da0-00a000e000c0\r\n"
        "SERVER: Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19\r\n"
        "X-User-Agent: redsonic\r\n"
        "ST: urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n"
        "USN: "
        "uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00::urn:schemas-upnp-org:device:"
        "InternetGatewayDevice:1",

        "HTTP/1.1 200 OK\r\n"
        "CACHE-CONTROL: max-age=300\r\n"
        "DATE: Wed, 21 Jan 1970 05:42:21 GMT\r\n"
        "EXT:\r\n"
        "LOCATION: http://192.168.2.1:1900/gatedesc.xml\r\n"
        "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
        "01-NLS: 000c0000-0dd0-00b0-0da0-00a000e000c0\r\n"
        "SERVER: Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19\r\n"
        "X-User-Agent: redsonic\r\n"
        "ST: urn:schemas-upnp-org:service:Layer3Forwarding:1\r\n"
        "USN: "
        "uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00::urn:schemas-upnp-org:service:"
        "Layer3Forwarding:1",

        "HTTP/1.1 200 OK\r\n"
        "CACHE-CONTROL: max-age=300\r\n"
        "DATE: Wed, 21 Jan 1970 05:42:21 GMT\r\n"
        "EXT:\r\n"
        "LOCATION: http://192.168.2.1:1900/gatedesc.xml\r\n"
        "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
        "01-NLS: 000c0000-0dd0-00b0-0da0-00a000e000c0\r\n"
        "SERVER: Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19\r\n"
        "X-User-Agent: redsonic\r\n"
        "ST: uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00\r\n"
        "USN: uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00",

        "HTTP/1.1 200 OK\r\n"
        "CACHE-CONTROL: max-age=300\r\n"
        "DATE: Wed, 21 Jan 1970 05:42:21 GMT\r\n"
        "EXT:\r\n"
        "LOCATION: http://192.168.2.1:1900/gatedesc.xml\r\n"
        "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
        "01-NLS: 000c0000-0dd0-00b0-0da0-00a000e000c0\r\n"
        "SERVER: Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19\r\n"
        "X-User-Agent: redsonic\r\n"
        "ST: urn:schemas-upnp-org:device:WANDevice:1\r\n"
        "USN: "
        "uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00::urn:schemas-upnp-org:device:"
        "WANDevice:1",

        "HTTP/1.1 200 OK\r\n"
        "CACHE-CONTROL: max-age=300\r\n"
        "DATE: Wed, 21 Jan 1970 05:42:21 GMT\r\n"
        "EXT:\r\n"
        "LOCATION: http://192.168.2.1:1900/gatedesc.xml\r\n"
        "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
        "01-NLS: 000c0000-0dd0-00b0-0da0-00a000e000c0\r\n"
        "SERVER: Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19\r\n"
        "X-User-Agent: redsonic\r\n"
        "ST: urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1\r\n"
        "USN: "
        "uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00::urn:schemas-upnp-org:service:"
        "WANCommonInterfaceConfig:1",

        "HTTP/1.1 200 OK\r\n"
        "CACHE-CONTROL: max-age=300\r\n"
        "DATE: Wed, 21 Jan 1970 05:42:21 GMT\r\n"
        "EXT:\r\n"
        "LOCATION: http://192.168.2.1:1900/gatedesc.xml\r\n"
        "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
        "01-NLS: 000c0000-0dd0-00b0-0da0-00a000e000c0\r\n"
        "SERVER: Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19\r\n"
        "X-User-Agent: redsonic\r\n"
        "ST: uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00\r\n"
        "USN: uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00",

        "HTTP/1.1 200 OK\r\n"
        "CACHE-CONTROL: max-age=300\r\n"
        "DATE: Wed, 21 Jan 1970 05:42:21 GMT\r\n"
        "EXT:\r\n"
        "LOCATION: http://192.168.2.1:1900/gatedesc.xml\r\n"
        "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
        "01-NLS: 000c0000-0dd0-00b0-0da0-00a000e000c0\r\n"
        "SERVER: Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19\r\n"
        "X-User-Agent: redsonic\r\n"
        "ST: urn:schemas-upnp-org:device:WANConnectionDevice:1\r\n"
        "USN: "
        "uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00::urn:schemas-upnp-org:device:"
        "WANConnectionDevice:1",

        "HTTP/1.1 200 OK\r\n"
        "CACHE-CONTROL: max-age=300\r\n"
        "DATE: Wed, 21 Jan 1970 05:42:21 GMT\r\n"
        "EXT:\r\n"
        "LOCATION: http://192.168.2.1:1900/gatedesc.xml\r\n"
        "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
        "01-NLS: 000c0000-0dd0-00b0-0da0-00a000e000c0\r\n"
        "SERVER: Linux/2.6.36, UPnP/1.0, Portable SDK for UPnP devices/1.6.19\r\n"
        "X-User-Agent: redsonic\r\n"
        "ST: urn:schemas-upnp-org:service:WANIPConnection:1\r\n"
        "USN: "
        "uuid:0f0000b0-f0da-0ad0-00b0-0000000fdf00::urn:schemas-upnp-org:service:"
        "WANIPConnection:1",

        "HTTP/1.1 200 OK\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "EXT:\r\n"
        "CACHE-CONTROL: max-age=100\r\n"
        "LOCATION: http://192.168.2.116:80/description.xml\r\n"
        "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.21.0\r\n"
        "hue-bridgeid: 111111FFFE11E111\r\n"
        "ST: upnp:rootdevice\r\n"
        "USN: uuid:1f111f11-da11-11e1-1b11-11111111e111::upnp:rootdevice",

        "HTTP/1.1 200 OK\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "EXT:\r\n"
        "CACHE-CONTROL: max-age=100\r\n"
        "LOCATION: http://192.168.2.116:80/description.xml\r\n"
        "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.21.0\r\n"
        "hue-bridgeid: 111111FFFE11E111\r\n"
        "ST: uuid:1f111f11-da11-11e1-1b11-11111111e111\r\n"
        "USN: uuid:1f111f11-da11-11e1-1b11-11111111e111",

        "HTTP/1.1 200 OK\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "EXT:\r\n"
        "CACHE-CONTROL: max-age=100\r\n"
        "LOCATION: http://192.168.2.116:80/description.xml\r\n"
        "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.21.0\r\n"
        "hue-bridgeid: 111111FFFE11E111\r\n"
        "ST: urn:schemas-upnp-org:device:basic:1\r\n"
        "USN: uuid:1f111f11-da11-11e1-1b11-11111111e111",

        "HTTP/1.1 200 OK\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "EXT:\r\n"
        "CACHE-CONTROL: max-age=100\r\n"
        "LOCATION: http://192.168.2.116:80/description.xml\r\n"
        "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.21.0\r\n"
        "hue-bridgeid: 111111FFFE11E111\r\n"
        "ST: upnp:rootdevice\r\n"
        "USN: uuid:1f111f11-da11-11e1-1b11-11111111e111::upnp:rootdevice",

        "HTTP/1.1 200 OK\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "EXT:\r\n"
        "CACHE-CONTROL: max-age=100\r\n"
        "LOCATION: http://192.168.2.116:80/description.xml\r\n"
        "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.21.0\r\n"
        "hue-bridgeid: 111111FFFE11E111\r\n"
        "ST: uuid:1f111f11-da11-11e1-1b11-11111111e111\r\n"
        "USN: uuid:1f111f11-da11-11e1-1b11-11111111e111",

        "HTTP/1.1 200 OK\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "EXT:\r\n"
        "CACHE-CONTROL: max-age=100\r\n"
        "LOCATION: http://192.168.2.116:80/description.xml\r\n"
        "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.21.0\r\n"
        "hue-bridgeid: 111111FFFE11E111\r\n"
        "ST: urn:schemas-upnp-org:device:basic:1\r\n"
        "USN: uuid:1f111f11-da11-11e1-1b11-11111111e111"};
}
#endif
