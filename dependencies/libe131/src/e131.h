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

#ifndef _E131_H
#define _E131_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

/* E1.31 Public Constants */
extern const uint16_t E131_DEFAULT_PORT;
extern const uint8_t E131_DEFAULT_PRIORITY;

/* E1.31 Socket Address Type */
typedef struct sockaddr_in e131_addr_t;

/* E1.31 Packet Type */
/* All packet contents shall be transmitted in network byte order (big endian) */
typedef union {
  PACK(struct {
    PACK(struct { /* ACN Root Layer: 38 bytes */
      uint16_t preamble_size;    /* Preamble Size */
      uint16_t postamble_size;   /* Post-amble Size */
      uint8_t  acn_pid[12];      /* ACN Packet Identifier */
      uint16_t flength;          /* Flags (high 4 bits) & Length (low 12 bits) */
      uint32_t vector;           /* Layer Vector */
      uint8_t  cid[16];          /* Component Identifier (UUID) */
    }) root;

    PACK(struct { /* Framing Layer: 77 bytes */
      uint16_t flength;          /* Flags (high 4 bits) & Length (low 12 bits) */
      uint32_t vector;           /* Layer Vector */
      uint8_t  source_name[64];  /* User Assigned Name of Source (UTF-8) */
      uint8_t  priority;         /* Packet Priority (0-200, default 100) */
      uint16_t reserved;         /* Reserved (should be always 0) */
      uint8_t  seq_number;       /* Sequence Number (detect duplicates or out of order packets) */
      uint8_t  options;          /* Options Flags (bit 7: preview data, bit 6: stream terminated) */
      uint16_t universe;         /* DMX Universe Number */
    }) frame;

    PACK(struct { /* Device Management Protocol (DMP) Layer: 523 bytes */
      uint16_t flength;          /* Flags (high 4 bits) / Length (low 12 bits) */
      uint8_t  vector;           /* Layer Vector */
      uint8_t  type;             /* Address Type & Data Type */
      uint16_t first_addr;       /* First Property Address */
      uint16_t addr_inc;         /* Address Increment */
      uint16_t prop_val_cnt;     /* Property Value Count (1 + number of slots) */
      uint8_t  prop_val[513];    /* Property Values (DMX start code + slots data) */
    }) dmp;
  });

  uint8_t raw[638]; /* raw buffer view: 638 bytes */
} e131_packet_t;

/* E1.31 Framing Options Type */
typedef enum {
  E131_OPT_TERMINATED = 6,
  E131_OPT_PREVIEW = 7,
} e131_option_t;

/* E1.31 Validation Errors Type */
typedef enum {
  E131_ERR_NONE,
  E131_ERR_NULLPTR,
  E131_ERR_PREAMBLE_SIZE,
  E131_ERR_POSTAMBLE_SIZE,
  E131_ERR_ACN_PID,
  E131_ERR_VECTOR_ROOT,
  E131_ERR_VECTOR_FRAME,
  E131_ERR_VECTOR_DMP,
  E131_ERR_TYPE_DMP,
  E131_ERR_FIRST_ADDR_DMP,
  E131_ERR_ADDR_INC_DMP,
} e131_error_t;

/* Create a socket file descriptor suitable for E1.31 communication */
extern int e131_socket(void);

/* Bind a socket file descriptor to a port number for E1.31 communication */
extern int e131_bind(int sockfd, const uint16_t port);

/* Initialize a unicast E1.31 destination using a host and port number */
extern int e131_unicast_dest(e131_addr_t *dest, const char *host, const uint16_t port);

/* Initialize a multicast E1.31 destination using a universe and port number */
extern int e131_multicast_dest(e131_addr_t *dest, const uint16_t universe, const uint16_t port);

/* Describe an E1.31 destination into a string (must be at least 22 bytes) */
extern int e131_dest_str(char *str, const e131_addr_t *dest);

/* Configure a socket file descriptor to use a specific network interface for outgoing multicast data */
extern int e131_multicast_iface(int sockfd, const int ifindex);

/* Join a socket file descriptor to an E1.31 multicast group using a universe */
extern int e131_multicast_join(int sockfd, const uint16_t universe);

/* Join a socket file descriptor to an E1.31 multicast group using a universe and a specific network interface */
extern int e131_multicast_join_iface(int sockfd, const uint16_t universe, const int ifindex);

/* Join a socket file descriptor to an E1.31 multicast group using a universe and an IP address to bind to */
extern int e131_multicast_join_ifaddr(int sockfd, const uint16_t universe, const char *ifaddr);

/* Initialize an E1.31 packet using a universe and a number of slots */
extern int e131_pkt_init(e131_packet_t *packet, const uint16_t universe, const uint16_t num_slots);

/* Get the state of a framing option in an E1.31 packet */
extern bool e131_get_option(const e131_packet_t *packet, const e131_option_t option);

/* Set the state of a framing option in an E1.31 packet */
extern int e131_set_option(e131_packet_t *packet, const e131_option_t option, const bool state);

/* Send an E1.31 packet to a socket file descriptor using a destination */
extern ssize_t e131_send(int sockfd, const e131_packet_t *packet, const e131_addr_t *dest);

/* Receive an E1.31 packet from a socket file descriptor */
extern ssize_t e131_recv(int sockfd, e131_packet_t *packet);

/* Validate that an E1.31 packet is well-formed */
extern e131_error_t e131_pkt_validate(const e131_packet_t *packet);

/* Check if an E1.31 packet should be discarded (sequence number out of order) */
extern bool e131_pkt_discard(const e131_packet_t *packet, const uint8_t last_seq_number);

/* Dump an E1.31 packet to a stream (i.e. stdout, stderr) */
extern int e131_pkt_dump(FILE *stream, const e131_packet_t *packet);

/* Return a string describing an E1.31 error */
extern const char *e131_strerror(const e131_error_t error);

#ifdef __cplusplus
}
#endif
#endif
