/*---------------------------------------------------------*\
| NanoleafScanningThread.cpp                                |
|                                                           |
|   OpenRGB Nanoleaf scanning thread                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#else
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/select.h>
#endif

#include "mdns.h"

#include "NanoleafScanningThread.h"

static char namebuffer[256];

static struct sockaddr_in service_address_ipv4;
static struct sockaddr_in6 service_address_ipv6;

static int has_ipv4;
static int has_ipv6;

static mdns_string_t ipv4_address_to_string(char* buffer, size_t capacity, const struct sockaddr_in* addr, size_t addrlen)
{
	char host[NI_MAXHOST] = {0};
	char service[NI_MAXSERV] = {0};
	int ret = getnameinfo((const struct sockaddr*)addr, (socklen_t)addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
	int len = 0;

	if(ret == 0)
    {
        len = snprintf(buffer, capacity, "%s", host);
	}

	if(len >= (int)capacity)
	{
		len = (int)capacity - 1;
	}

	mdns_string_t str;
	str.str = buffer;
	str.length = len;

	return str;
}

static mdns_string_t ipv6_address_to_string(char* buffer, size_t capacity, const struct sockaddr_in6* addr, size_t addrlen)
{
	char host[NI_MAXHOST] = {0};
	char service[NI_MAXSERV] = {0};
	int ret = getnameinfo((const struct sockaddr*)addr, (socklen_t)addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
	int len = 0;

	if(ret == 0)
    {
		if(addr->sin6_port != 0)
		{
			len = snprintf(buffer, capacity, "[%s]:%s", host, service);
		}
		else
		{
			len = snprintf(buffer, capacity, "%s", host);
		}
	}

	if(len >= (int)capacity)
	{
		len = (int)capacity - 1;
	}

	mdns_string_t str;
	str.str = buffer;
	str.length = len;

	return str;
}

/*-----------------------------------------------------*\
| Open sockets for sending one-shot multicast queries   |
| from an ephemeral port                                |
\*-----------------------------------------------------*/
static int open_client_sockets(int* sockets, int max_sockets, int port)
{
    /*-----------------------------------------------------*\
    | When sending, each socket can only send to one        |
    | network interface from an ephemeral port, thus we     |
    | need to open one socket for each interface and        |
    | address family                                        |
    \*-----------------------------------------------------*/
	int num_sockets = 0;

#ifdef _WIN32

	IP_ADAPTER_ADDRESSES* adapter_address = 0;
	ULONG address_size = 8000;
	unsigned int ret;
	unsigned int num_retries = 4;
	do
    {
		adapter_address = (IP_ADAPTER_ADDRESSES*)malloc(address_size);
		ret = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_ANYCAST, 0, adapter_address, &address_size);

		if(ret == ERROR_BUFFER_OVERFLOW)
        {
			free(adapter_address);
			adapter_address = 0;
			address_size *= 2;
		}
        else
        {
			break;
		}
	}
    while(num_retries-- > 0);

	if(!adapter_address || (ret != NO_ERROR))
    {
		free(adapter_address);
		return num_sockets;
	}

	int first_ipv4 = 1;
	int first_ipv6 = 1;

	for(PIP_ADAPTER_ADDRESSES adapter = adapter_address; adapter; adapter = adapter->Next)
    {
		if(adapter->TunnelType == TUNNEL_TYPE_TEREDO)
		{
			continue;
		}

		if(adapter->OperStatus != IfOperStatusUp)
		{
			continue;
		}

		for(IP_ADAPTER_UNICAST_ADDRESS* unicast = adapter->FirstUnicastAddress; unicast; unicast = unicast->Next)
        {
			if(unicast->Address.lpSockaddr->sa_family == AF_INET)
            {
				struct sockaddr_in* saddr = (struct sockaddr_in*)unicast->Address.lpSockaddr;
				if((saddr->sin_addr.S_un.S_un_b.s_b1 != 127) ||
				   (saddr->sin_addr.S_un.S_un_b.s_b2 != 0) ||
				   (saddr->sin_addr.S_un.S_un_b.s_b3 != 0) ||
				   (saddr->sin_addr.S_un.S_un_b.s_b4 != 1))
                {
					int log_addr = 0;
					if(first_ipv4)
                    {
						service_address_ipv4 = *saddr;
						first_ipv4 = 0;
						log_addr = 1;
					}
					has_ipv4 = 1;
					if(num_sockets < max_sockets)
                    {
						saddr->sin_port = htons((unsigned short)port);
						int sock = mdns_socket_open_ipv4(saddr);
						if(sock >= 0)
                        {
							sockets[num_sockets++] = sock;
							log_addr = 1;
						}
                        else
                        {
							log_addr = 0;
						}
					}
					if(log_addr)
                    {
						char buffer[128];
						ipv4_address_to_string(buffer, sizeof(buffer), saddr, sizeof(struct sockaddr_in));
					}
				}
			}
            else if(unicast->Address.lpSockaddr->sa_family == AF_INET6)
            {
				struct sockaddr_in6* saddr = (struct sockaddr_in6*)unicast->Address.lpSockaddr;
				static const unsigned char localhost[] = {0, 0, 0, 0, 0, 0, 0, 0,
				                                          0, 0, 0, 0, 0, 0, 0, 1};
				static const unsigned char localhost_mapped[] = {0, 0, 0,    0,    0,    0, 0, 0,
				                                                 0, 0, 0xff, 0xff, 0x7f, 0, 0, 1};
				if((unicast->DadState == NldsPreferred) &&
				   memcmp(saddr->sin6_addr.s6_addr, localhost, 16) &&
				   memcmp(saddr->sin6_addr.s6_addr, localhost_mapped, 16))
                {
					int log_addr = 0;
					if(first_ipv6)
                    {
						service_address_ipv6 = *saddr;
						first_ipv6 = 0;
						log_addr = 1;
					}
					has_ipv6 = 1;
					if(num_sockets < max_sockets)
                    {
						saddr->sin6_port = htons((unsigned short)port);
						int sock = mdns_socket_open_ipv6(saddr);
						if(sock >= 0)
                        {
							sockets[num_sockets++] = sock;
							log_addr = 1;
						}
                        else
                        {
							log_addr = 0;
						}
					}
					if(log_addr)
                    {
						char buffer[128];
						ipv6_address_to_string(buffer, sizeof(buffer), saddr, sizeof(struct sockaddr_in6));
					}
				}
			}
		}
	}

	free(adapter_address);

#else

	struct ifaddrs* ifaddr = 0;
	struct ifaddrs* ifa = 0;

    getifaddrs(&ifaddr);

	int first_ipv4 = 1;
	int first_ipv6 = 1;

	for(ifa = ifaddr; ifa; ifa = ifa->ifa_next)
    {
		if(!ifa->ifa_addr)
		{
			continue;
		}

		if(ifa->ifa_addr->sa_family == AF_INET)
        {
			struct sockaddr_in* saddr = (struct sockaddr_in*)ifa->ifa_addr;
			if(saddr->sin_addr.s_addr != htonl(INADDR_LOOPBACK))
            {
				int log_addr = 0;
				if(first_ipv4)
                {
					service_address_ipv4 = *saddr;
					first_ipv4 = 0;
					log_addr = 1;
				}
				has_ipv4 = 1;
				if(num_sockets < max_sockets)
                {
					saddr->sin_port = htons(port);
					int sock = mdns_socket_open_ipv4(saddr);
					if(sock >= 0)
                    {
						sockets[num_sockets++] = sock;
						log_addr = 1;
					}
                    else
                    {
						log_addr = 0;
					}
				}
				if(log_addr)
                {
					char buffer[128];
					ipv4_address_to_string(buffer, sizeof(buffer), saddr, sizeof(struct sockaddr_in));
				}
			}
		}
        else if(ifa->ifa_addr->sa_family == AF_INET6)
        {
			struct sockaddr_in6* saddr = (struct sockaddr_in6*)ifa->ifa_addr;
			static const unsigned char localhost[] = {0, 0, 0, 0, 0, 0, 0, 0,
			                                          0, 0, 0, 0, 0, 0, 0, 1};
			static const unsigned char localhost_mapped[] = {0, 0, 0,    0,    0,    0, 0, 0,
			                                                 0, 0, 0xff, 0xff, 0x7f, 0, 0, 1};
			if (memcmp(saddr->sin6_addr.s6_addr, localhost, 16) &&
			    memcmp(saddr->sin6_addr.s6_addr, localhost_mapped, 16))
            {
				int log_addr = 0;
				if(first_ipv6)
                {
					service_address_ipv6 = *saddr;
					first_ipv6 = 0;
					log_addr = 1;
				}
				has_ipv6 = 1;
				if(num_sockets < max_sockets)
                {
					saddr->sin6_port = htons(port);
					int sock = mdns_socket_open_ipv6(saddr);
					if (sock >= 0)
                    {
						sockets[num_sockets++] = sock;
						log_addr = 1;
					}
                    else
                    {
						log_addr = 0;
					}
				}
				if(log_addr)
                {
					char buffer[128];
					ipv6_address_to_string(buffer, sizeof(buffer), saddr, sizeof(struct sockaddr_in6));
				}
			}
		}
	}

	freeifaddrs(ifaddr);

#endif

	return num_sockets;
}

/*-----------------------------------------------------*\
| Callback handling parsing answers to queries sent     |
\*-----------------------------------------------------*/
static int query_callback(
    int sock,
    [[maybe_unused]] const struct sockaddr* from,
    [[maybe_unused]] size_t addrlen,
    [[maybe_unused]] mdns_entry_type_t entry,
    uint16_t query_id,
    uint16_t rtype,
    [[maybe_unused]] uint16_t rclass,
    [[maybe_unused]] uint32_t ttl,
    const void* data,
    size_t size,
    [[maybe_unused]] size_t name_offset,
    size_t name_length,
    size_t record_offset,
    size_t record_length,
    void* user_data)
{
	(void)sizeof(sock);
	(void)sizeof(query_id);
	(void)sizeof(name_length);
	(void)sizeof(user_data);

    if(rtype == MDNS_RECORDTYPE_A)
    {
		struct sockaddr_in address;
		mdns_record_parse_a(data, size, record_offset, record_length, &address);

        if(address.sin_port == 0)
		{
            address.sin_port = 16021; // Default Nanoleaf port.
		}

        mdns_string_t addrstr = ipv4_address_to_string(namebuffer, sizeof(namebuffer), &address, sizeof(address));

        (static_cast<NanoleafScanningThread*>(user_data))->EmitDeviceFound(addrstr.str, address.sin_port);
	}

	return 0;
}

void NanoleafScanningThread::EmitDeviceFound(QString address, int port)
{
	emit DeviceFound(address, port);
}

/*-----------------------------------------------------*\
| Send a mDNS query                                     |
\*-----------------------------------------------------*/
int NanoleafScanningThread::SendMDNSQuery()
{
    const char* service = "_nanoleafapi._tcp.local.";
	mdns_record_type record = MDNS_RECORDTYPE_PTR;

	int sockets[32];
	int query_id[32];
	int num_sockets = open_client_sockets(sockets, sizeof(sockets) / sizeof(sockets[0]), 0);
	if (num_sockets <= 0)
    {
		return -1;
	}

	size_t capacity = 2048;
	void* buffer = malloc(capacity);
	size_t records;

	//const char* record_name;
	if(record == MDNS_RECORDTYPE_SRV)
	{
		//record_name = "SRV";
	}
	else if(record == MDNS_RECORDTYPE_A)
	{
		//record_name = "A";
	}
	else if(record == MDNS_RECORDTYPE_AAAA)
	{
		//record_name = "AAAA";
	}
	else
	{
		record = MDNS_RECORDTYPE_PTR;
	}

	for(int isock = 0; isock < num_sockets; ++isock)
    {
		query_id[isock] = mdns_query_send(sockets[isock], record, service, strlen(service), buffer, capacity, 0);
	}

    /*-----------------------------------------------------*\
    | This is a simple implementation that loops for        |
    | 5 seconds or as long as we get replies                |
    \*-----------------------------------------------------*/
	int res;
	do
    {
		struct timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		int nfds = 0;
		fd_set readfs;
		FD_ZERO(&readfs);
		for(int isock = 0; isock < num_sockets; ++isock)
        {
			if(sockets[isock] >= nfds)
			{
				nfds = sockets[isock] + 1;
			}

			FD_SET(sockets[isock], &readfs);
		}

		records = 0;
		res = select(nfds, &readfs, 0, 0, &timeout);
		if(res > 0)
        {
			for(int isock = 0; isock < num_sockets; ++isock)
            {
				if(FD_ISSET(sockets[isock], &readfs))
                {
					records += mdns_query_recv(sockets[isock], buffer, capacity, query_callback, this, query_id[isock]);
				}

				FD_SET(sockets[isock], &readfs);
			}
		}
	}
    while (res > 0);

	free(buffer);

	for(int isock = 0; isock < num_sockets; ++isock)
	{
		mdns_socket_close(sockets[isock]);
	}

	return 0;
}

void NanoleafScanningThread::run()
{
    SendMDNSQuery();
}
