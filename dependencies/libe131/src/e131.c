/**
 * E1.31 (sACN) library for C/C++
 * Hugo Hromic - http://github.com/hhromic
 *
 * Some content of this file is based on:
 * https://github.com/forkineye/E131/blob/master/E131.h
 * https://github.com/forkineye/E131/blob/master/E131.cpp
 *
 * Copyright 2016 Hugo Hromic
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <errno.h>
#include <inttypes.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2ipdef.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include "e131.h"

/* E1.31 Public Constants */
const uint16_t E131_DEFAULT_PORT = 5568;
const uint8_t E131_DEFAULT_PRIORITY = 0x64;

/* E1.31 Private Constants */
const uint16_t _E131_PREAMBLE_SIZE = 0x0010;
const uint16_t _E131_POSTAMBLE_SIZE = 0x0000;
const uint8_t _E131_ACN_PID[] = {0x41, 0x53, 0x43, 0x2d, 0x45, 0x31, 0x2e, 0x31, 0x37, 0x00, 0x00, 0x00};
const uint32_t _E131_ROOT_VECTOR = 0x00000004;
const uint32_t _E131_FRAME_VECTOR = 0x00000002;
const uint8_t _E131_DMP_VECTOR = 0x02;
const uint8_t _E131_DMP_TYPE = 0xa1;
const uint16_t _E131_DMP_FIRST_ADDR = 0x0000;
const uint16_t _E131_DMP_ADDR_INC = 0x0001;

/* Create a socket file descriptor suitable for E1.31 communication */
int e131_socket(void) {
#ifdef _WIN32
  WSADATA WsaData;
  WSAStartup(MAKEWORD(2, 2), &WsaData);
#endif
  return socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

/* Bind a socket file descriptor to a port number for E1.31 communication */
int e131_bind(int sockfd, const uint16_t port) {
  e131_addr_t addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);
  memset(addr.sin_zero, 0, sizeof addr.sin_zero);
  return bind(sockfd, (struct sockaddr *)&addr, sizeof addr);
}

/* Initialize a unicast E1.31 destination using a host and port number */
int e131_unicast_dest(e131_addr_t *dest, const char *host, const uint16_t port) {
  if (dest == NULL || host == NULL) {
    errno = EINVAL;
    return -1;
  }

  // get the address info of the host (the results are a linked list)
  struct addrinfo *ai, hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  if (getaddrinfo(host, NULL, &hints, &ai) != 0) {
    errno = EADDRNOTAVAIL;
    return -1;
  }

  // for now, only use the first address info result
  dest->sin_family = AF_INET;
  dest->sin_addr = ((struct sockaddr_in *)ai->ai_addr)->sin_addr;
  dest->sin_port = htons(port);
  memset(dest->sin_zero, 0, sizeof dest->sin_zero);

  freeaddrinfo(ai);
  return 0;
}

/* Initialize a multicast E1.31 destination using a universe and port number */
int e131_multicast_dest(e131_addr_t *dest, const uint16_t universe, const uint16_t port) {
  if (dest == NULL || universe < 1 || universe > 63999) {
    errno = EINVAL;
    return -1;
  }
  dest->sin_family = AF_INET;
  dest->sin_addr.s_addr = htonl(0xefff0000 | universe);
  dest->sin_port = htons(port);
  memset(dest->sin_zero, 0, sizeof dest->sin_zero);
  return 0;
}

/* Describe an E1.31 destination into a string */
int e131_dest_str(char *str, const e131_addr_t *dest) {
  if (str == NULL || dest == NULL) {
    errno = EINVAL;
    return -1;
  }
  sprintf(str, "%s:%d", inet_ntoa(dest->sin_addr), ntohs(dest->sin_port));
  return 0;
}

/* Configure a socket file descriptor to use a specific network interface for outgoing multicast data */
int e131_multicast_iface(int sockfd, const int ifindex) {
#ifdef _WIN32
  if (ifindex != 0) {
    errno = ENOSYS;
    return -1;
  }
  struct ip_mreq mreq;
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
#else
  struct ip_mreqn mreq;
  mreq.imr_address.s_addr = htonl(INADDR_ANY);
  mreq.imr_ifindex = ifindex;
#endif
  return setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, (const void *) &mreq, sizeof mreq);
}

/* Join a socket file descriptor to an E1.31 multicast group using a universe */
int e131_multicast_join(int sockfd, const uint16_t universe) {
  return e131_multicast_join_iface(sockfd, universe, 0);
}

/* Join a socket file descriptor to an E1.31 multicast group using a universe and a specific network interface */
int e131_multicast_join_iface(int sockfd, const uint16_t universe, const int ifindex) {
  if (universe < 1 || universe > 63999) {
    errno = EINVAL;
    return -1;
  }
#ifdef _WIN32
  if (ifindex != 0) {
    errno = ENOSYS;
    return -1;
  }
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = htonl(0xefff0000 | universe);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
#else
  struct ip_mreqn mreq;
  mreq.imr_multiaddr.s_addr = htonl(0xefff0000 | universe);
  mreq.imr_address.s_addr = htonl(INADDR_ANY);
  mreq.imr_ifindex = ifindex;
#endif
  return setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void *) &mreq, sizeof mreq);
}

/* Join a socket file descriptor to an E1.31 multicast group using a universe and an IP address to bind to */
extern int e131_multicast_join_ifaddr(int sockfd, const uint16_t universe, const char *ifaddr) {
  if (universe < 1 || universe > 63999) {
    errno = EINVAL;
    return -1;
  }
#ifdef _WIN32
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = htonl(0xefff0000 | universe);
  mreq.imr_interface.s_addr = inet_addr(ifaddr);
#else
  struct ip_mreqn mreq;
  mreq.imr_multiaddr.s_addr = htonl(0xefff0000 | universe);
  mreq.imr_address.s_addr = inet_addr(ifaddr);
  mreq.imr_ifindex = 0;
#endif
  return setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void *) &mreq, sizeof mreq);
}

/* Initialize an E1.31 packet using a universe and a number of slots */
int e131_pkt_init(e131_packet_t *packet, const uint16_t universe, const uint16_t num_slots) {
  if (packet == NULL || universe < 1 || universe > 63999 || num_slots < 1 || num_slots > 512) {
    errno = EINVAL;
    return -1;
  }

  // compute packet layer lengths
  uint16_t prop_val_cnt = num_slots + 1;
  uint16_t dmp_length = prop_val_cnt +
    sizeof packet->dmp - sizeof packet->dmp.prop_val;
  uint16_t frame_length = sizeof packet->frame + dmp_length;
  uint16_t root_length = sizeof packet->root.flength +
    sizeof packet->root.vector + sizeof packet->root.cid + frame_length;

  // clear packet
  memset(packet, 0, sizeof *packet);

  // set Root Layer values
  packet->root.preamble_size = htons(_E131_PREAMBLE_SIZE);
  packet->root.postamble_size = htons(_E131_POSTAMBLE_SIZE);
  memcpy(packet->root.acn_pid, _E131_ACN_PID, sizeof packet->root.acn_pid);
  packet->root.flength = htons(0x7000 | root_length);
  packet->root.vector = htonl(_E131_ROOT_VECTOR);

  // set Framing Layer values
  packet->frame.flength = htons(0x7000 | frame_length);
  packet->frame.vector = htonl(_E131_FRAME_VECTOR);
  packet->frame.priority = E131_DEFAULT_PRIORITY;
  packet->frame.universe = htons(universe);

  // set Device Management Protocol (DMP) Layer values
  packet->dmp.flength = htons(0x7000 | dmp_length);
  packet->dmp.vector = _E131_DMP_VECTOR;
  packet->dmp.type = _E131_DMP_TYPE;
  packet->dmp.first_addr = htons(_E131_DMP_FIRST_ADDR);
  packet->dmp.addr_inc = htons(_E131_DMP_ADDR_INC);
  packet->dmp.prop_val_cnt = htons(prop_val_cnt);

  return 0;
}

/* Get the state of a framing option in an E1.31 packet */
bool e131_get_option(const e131_packet_t *packet, const e131_option_t option) {
  if (packet != NULL && packet->frame.options & (1 << (option % 8)))
    return true;
  return false;
}

/* Set the state of a framing option in an E1.31 packet */
int e131_set_option(e131_packet_t *packet, const e131_option_t option, const bool state) {
  if (packet == NULL) {
    errno = EINVAL;
    return -1;
  }
  packet->frame.options ^= (-state ^ packet->frame.options) & (1 << (option % 8));
  return 0;
}

/* Send an E1.31 packet to a socket file descriptor using a destination */
ssize_t e131_send(int sockfd, const e131_packet_t *packet, const e131_addr_t *dest) {
  if (packet == NULL || dest == NULL) {
    errno = EINVAL;
    return -1;
  }
  const size_t packet_length = sizeof packet->raw -
    sizeof packet->dmp.prop_val + htons(packet->dmp.prop_val_cnt);
  return sendto(sockfd, (const void *) packet->raw, packet_length, 0,
    (const struct sockaddr *)dest, sizeof *dest);
}

/* Receive an E1.31 packet from a socket file descriptor */
ssize_t e131_recv(int sockfd, e131_packet_t *packet) {
  if (packet == NULL) {
    errno = EINVAL;
    return -1;
  }
  return recv(sockfd, (void *) packet->raw, sizeof packet->raw, 0);
}

/* Validate that an E1.31 packet is well-formed */
e131_error_t e131_pkt_validate(const e131_packet_t *packet) {
  if (packet == NULL)
    return E131_ERR_NULLPTR;
  if (ntohs(packet->root.preamble_size) != _E131_PREAMBLE_SIZE)
    return E131_ERR_PREAMBLE_SIZE;
  if (ntohs(packet->root.postamble_size) != _E131_POSTAMBLE_SIZE)
    return E131_ERR_POSTAMBLE_SIZE;
  if (memcmp(packet->root.acn_pid, _E131_ACN_PID, sizeof packet->root.acn_pid) != 0)
    return E131_ERR_ACN_PID;
  if (ntohl(packet->root.vector) != _E131_ROOT_VECTOR)
    return E131_ERR_VECTOR_ROOT;
  if (ntohl(packet->frame.vector) != _E131_FRAME_VECTOR)
    return E131_ERR_VECTOR_FRAME;
  if (packet->dmp.vector != _E131_DMP_VECTOR)
    return E131_ERR_VECTOR_DMP;
  if (packet->dmp.type != _E131_DMP_TYPE)
    return E131_ERR_TYPE_DMP;
  if (htons(packet->dmp.first_addr) != _E131_DMP_FIRST_ADDR)
    return E131_ERR_FIRST_ADDR_DMP;
  if (htons(packet->dmp.addr_inc) != _E131_DMP_ADDR_INC)
    return E131_ERR_ADDR_INC_DMP;
  return E131_ERR_NONE;
}

/* Check if an E1.31 packet should be discarded (sequence number out of order) */
bool e131_pkt_discard(const e131_packet_t *packet, const uint8_t last_seq_number) {
  if (packet == NULL)
    return true;
  int8_t seq_num_diff = packet->frame.seq_number - last_seq_number;
  if (seq_num_diff > -20 && seq_num_diff <= 0)
    return true;
  return false;
}

/* Dump an E1.31 packet to a stream (i.e. stdout, stderr) */
int e131_pkt_dump(FILE *stream, const e131_packet_t *packet) {
  if (stream == NULL || packet == NULL) {
    errno = EINVAL;
    return -1;
  }
  fprintf(stream, "[Root Layer]\n");
  fprintf(stream, "  Preamble Size .......... %" PRIu16 "\n", ntohs(packet->root.preamble_size));
  fprintf(stream, "  Post-amble Size ........ %" PRIu16 "\n", ntohs(packet->root.postamble_size));
  fprintf(stream, "  ACN Packet Identifier .. %s\n", packet->root.acn_pid);
  fprintf(stream, "  Flags & Length ......... %" PRIu16 "\n", ntohs(packet->root.flength));
#ifdef _WIN32
  fprintf(stream, "  Layer Vector ........... %lu\n", ntohl(packet->root.vector));
#else
  fprintf(stream, "  Layer Vector ........... %" PRIu32 "\n", ntohl(packet->root.vector));
#endif
  fprintf(stream, "  Component Identifier ... ");
  for (size_t pos=0, total=sizeof packet->root.cid; pos<total; pos++)
    fprintf(stream, "%02x", packet->root.cid[pos]);
  fprintf(stream, "\n");
  fprintf(stream, "[Framing Layer]\n");
  fprintf(stream, "  Flags & Length ......... %" PRIu16 "\n", ntohs(packet->frame.flength));
#ifdef _WIN32
  fprintf(stream, "  Layer Vector ........... %lu\n", ntohl(packet->frame.vector));
#else
  fprintf(stream, "  Layer Vector ........... %" PRIu32 "\n", ntohl(packet->frame.vector));
#endif
  fprintf(stream, "  Source Name ............ %s\n", packet->frame.source_name);
  fprintf(stream, "  Packet Priority ........ %" PRIu8 "\n", packet->frame.priority);
  fprintf(stream, "  Reserved ............... %" PRIu16 "\n", ntohs(packet->frame.reserved));
  fprintf(stream, "  Sequence Number ........ %" PRIu8 "\n", packet->frame.seq_number);
  fprintf(stream, "  Options Flags .......... %" PRIu8 "\n", packet->frame.options);
  fprintf(stream, "  DMX Universe Number .... %" PRIu16 "\n", ntohs(packet->frame.universe));
  fprintf(stream, "[Device Management Protocol (DMP) Layer]\n");
  fprintf(stream, "  Flags & Length ......... %" PRIu16 "\n", ntohs(packet->dmp.flength));
  fprintf(stream, "  Layer Vector ........... %" PRIu8 "\n", packet->dmp.vector);
  fprintf(stream, "  Address & Data Type .... %" PRIu8 "\n", packet->dmp.type);
  fprintf(stream, "  First Address .......... %" PRIu16 "\n", ntohs(packet->dmp.first_addr));
  fprintf(stream, "  Address Increment ...... %" PRIu16 "\n", ntohs(packet->dmp.addr_inc));
  fprintf(stream, "  Property Value Count ... %" PRIu16 "\n", ntohs(packet->dmp.prop_val_cnt));
  fprintf(stream, "[DMP Property Values]\n ");
  for (size_t pos=0, total=ntohs(packet->dmp.prop_val_cnt); pos<total; pos++)
    fprintf(stream, " %02x", packet->dmp.prop_val[pos]);
  fprintf(stream, "\n");
  return 0;
}

/* Return a string describing an E1.31 error */
const char *e131_strerror(const e131_error_t error) {
  switch (error) {
    case E131_ERR_NONE:
      return "Success";
    case E131_ERR_PREAMBLE_SIZE:
      return "Invalid Preamble Size";
    case E131_ERR_POSTAMBLE_SIZE:
      return "Invalid Post-amble Size";
    case E131_ERR_ACN_PID:
      return "Invalid ACN Packet Identifier";
    case E131_ERR_VECTOR_ROOT:
      return "Invalid Root Layer Vector";
    case E131_ERR_VECTOR_FRAME:
      return "Invalid Framing Layer Vector";
    case E131_ERR_VECTOR_DMP:
      return "Invalid Device Management Protocol (DMP) Layer Vector";
    case E131_ERR_TYPE_DMP:
      return "Invalid DMP Address & Data Type";
    case E131_ERR_FIRST_ADDR_DMP:
      return "Invalid DMP First Address";
    case E131_ERR_ADDR_INC_DMP:
      return "Invalid DMP Address Increment";
    default:
      return "Unknown error";
  }
}
