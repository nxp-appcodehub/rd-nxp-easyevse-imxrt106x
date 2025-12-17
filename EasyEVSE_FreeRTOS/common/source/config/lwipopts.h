/**
 ******************************************************************************
 * @file    lwipopts.h
 * This file is based on \src\include\lwip\opt.h
 ******************************************************************************
 * Copyright (c) 2013-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018, 2022, 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__


#define LWIP_CHECKSUM_ON_COPY 1

#define LWIP_DNS_SECURE 0

#define CONFIG_NETWORK_HIGH_PERF 1
/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT 1

/**
 * NO_SYS==0: Use RTOS
 */
#define NO_SYS 0
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN 1
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET 1

/**
 * LWIP_SO_RCVTIMEO==1: Enable receive timeout for sockets/netconns and
 * SO_RCVTIMEO processing.
 */
#define LWIP_SO_RCVTIMEO 1

/**
 * LWIP_SO_SNDTIMEO==1: Enable send timeout for sockets/netconns and
 * SO_SNDTIMEO processing.
 */
#define LWIP_SO_SNDTIMEO 1

/**
 * Needed for network connection check using ping
 */
#define LWIP_RAW 1


/* ---------- Multicast DNS options ---------- */
/* Multicast DNS module */
#ifndef LWIP_MDNS_RESPONDER
#define LWIP_MDNS_RESPONDER 1
#endif

/**
 * The maximum number of services per netif
 * recommended to be two times greater than number of matter fabrics supported
 */
#ifndef MDNS_MAX_SERVICES
#define MDNS_MAX_SERVICES 10
#endif

/* Search over multicast DNS module */
#ifndef LWIP_MDNS_SEARCH
#define LWIP_MDNS_SEARCH 0
#endif

/* Provide strncasecmp to lwIP so it does not provide it's own lwip_strnicmp implementation along the strncasecmp from
 * toolchain */
#ifndef lwip_strnicmp
#define lwip_strnicmp strncasecmp
#endif

/* ---------- Core locking ---------- */

#define LWIP_TCPIP_CORE_LOCKING 1

void sys_lock_tcpip_core(void);
#define LOCK_TCPIP_CORE() sys_lock_tcpip_core()

void sys_unlock_tcpip_core(void);
#define UNLOCK_TCPIP_CORE() sys_unlock_tcpip_core()

void sys_check_core_locking(void);
#define LWIP_ASSERT_CORE_LOCKED() sys_check_core_locking()

void sys_mark_tcpip_thread(void);
#define LWIP_MARK_TCPIP_THREAD() sys_mark_tcpip_thread()

/* ---------- Memory options ---------- */

#define MEMP_USE_CUSTOM_POOLS 0 //1

/**
 * MEM_ALIGNMENT: should be set to the alignment of the CPU
 *    4 byte alignment -> #define MEM_ALIGNMENT 4
 *    2 byte alignment -> #define MEM_ALIGNMENT 2
 */
#ifndef MEM_ALIGNMENT
#define MEM_ALIGNMENT 4
#endif

/**
 * MEM_SIZE: the size of the heap memory. If the application will send
 * a lot of data that needs to be copied, this should be set high.
 */
#ifndef MEM_SIZE
#define MEM_SIZE (22 * 1024)
#endif

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#ifndef MEMP_NUM_PBUF
#if CONFIG_NETWORK_HIGH_PERF
#define MEMP_NUM_PBUF 30
#else
#define MEMP_NUM_PBUF 10
#endif
#endif

/**
 * MEMP_NUM_TCPIP_MSG_INPKT: the number of struct tcpip_msg, which are used
 * for incoming packets.
 * (only needed if you use tcpip.c)
 */
#ifndef MEMP_NUM_TCPIP_MSG_INPKT
#if CONFIG_NETWORK_HIGH_PERF
#define MEMP_NUM_TCPIP_MSG_INPKT 32
#else
#define MEMP_NUM_TCPIP_MSG_INPKT 16
#endif
#endif

/** MEMP_NUM_TCPIP_MSG_*: the number of struct tcpip_msg, which is used
   for sequential API communication and incoming packets. Used in
   src/api/tcpip.c. */
#ifndef MEMP_NUM_TCPIP_MSG_API
#if CONFIG_NETWORK_HIGH_PERF
#define MEMP_NUM_TCPIP_MSG_API 64
#else
#define MEMP_NUM_TCPIP_MSG_API 16
#endif
#endif

/**
 * MEMP_NUM_NETBUF: the number of struct netbufs.
 * (only needed if you use the sequential API, like api_lib.c)
 */
#ifndef MEMP_NUM_NETBUF
#if CONFIG_NETWORK_HIGH_PERF
#define MEMP_NUM_NETBUF 32
#else
#define MEMP_NUM_NETBUF 16
#endif
#endif

/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#ifndef MEMP_NUM_UDP_PCB
#define MEMP_NUM_UDP_PCB (7 + (LWIP_MDNS_RESPONDER))
#endif
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#ifndef MEMP_NUM_TCP_PCB
#define MEMP_NUM_TCP_PCB 10
#endif
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#ifndef MEMP_NUM_TCP_PCB_LISTEN
#define MEMP_NUM_TCP_PCB_LISTEN 2
#endif

#ifndef MEMP_NUM_NETCONN
#define MEMP_NUM_NETCONN (MEMP_NUM_TCP_PCB + MEMP_NUM_TCP_PCB_LISTEN + MEMP_NUM_UDP_PCB)
#endif

/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#ifndef MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG 22
#endif
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */
#ifndef MEMP_NUM_SYS_TIMEOUT
#define MEMP_NUM_SYS_TIMEOUT 10
#endif
/* MEMP_NUM_REASS_DATA: The number of whole IP packets
   queued for reassembly. */
#ifndef MEMP_NUM_REASSDATA
#define MEMP_NUM_REASSDATA 4
#endif

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#ifndef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE 15
#endif

/**
 * PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. The default is
 * designed to accomodate single full size TCP frame in one pbuf, including
 * TCP_MSS, IP header, and link header.
 */
#define PBUF_POOL_BUFSIZE 1580

/**
 * MEMP_NUM_FRAG_PBUF: the number of IP fragments simultaneously sent
 * (fragments, not whole packets!).
 * This is only used with LWIP_NETIF_TX_SINGLE_PBUF==0 and only has to be > 1
 * with DMA-enabled MACs where the packet is not yet sent when netif->output
 * returns.
 */
#define MEMP_NUM_FRAG_PBUF 15


/* ---------- TCP options ---------- */
#ifndef LWIP_TCP
#define LWIP_TCP 1
#endif

#ifndef TCP_TTL
#define TCP_TTL 255
#endif

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#ifndef TCP_QUEUE_OOSEQ
#define TCP_QUEUE_OOSEQ 0
#endif

/* TCP Maximum segment size. */
#ifndef TCP_MSS
#define TCP_MSS (1500 - 40) /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */
#endif

/* TCP sender buffer space (bytes). */
#ifndef TCP_SND_BUF
#define TCP_SND_BUF (6 * TCP_MSS)
#endif

/* TCP sender buffer space (pbufs). This must be at least = 2 *
   TCP_SND_BUF/TCP_MSS for things to work. */
#ifndef TCP_SND_QUEUELEN
#define TCP_SND_QUEUELEN ((3 * TCP_SND_BUF) / TCP_MSS)
#endif

/* TCP receive window. */
#ifndef TCP_WND
#if CONFIG_NETWORK_HIGH_PERF
#define TCP_WND (15 * TCP_MSS)
#else
#define TCP_WND (5 * TCP_MSS)
#endif /* CONFIG_NETWORK_HIGH_PERF */
#endif

/**
 * Enable TCP_KEEPALIVE
 */
#define LWIP_TCP_KEEPALIVE 1
/* Enable keepalive for all sockets */
#define TCP_KEEPALIVE_DEFAULT 1

/* Enable backlog*/
#ifndef TCP_LISTEN_BACKLOG
#define TCP_LISTEN_BACKLOG 1
#endif /* TCP_LISTEN_BACKLOG */

/* ---------- Network Interfaces options ---------- */

/**
 * TCP_RESOURCE_FAIL_RETRY_LIMIT: limit for retrying sending of tcp segment
 * on resource failure error returned by driver.
 */
#define TCP_RESOURCE_FAIL_RETRY_LIMIT 50

/**
 * LWIP_NETIF_HOSTNAME==1: use DHCP_OPTION_HOSTNAME with netif's hostname
 * field.
 */
#define LWIP_NETIF_HOSTNAME 1

/* Support netif api (in netifapi.c). */
#ifndef LWIP_NETIF_API
#define LWIP_NETIF_API 1
#endif /* LWIP_NETIF_API */

/* ---------- ICMP options ---------- */
#ifndef LWIP_ICMP
#define LWIP_ICMP 1
#endif /* LWIP_ICMP */

/* ---------- DHCP options ---------- */
/* Enable DHCP module. */
#ifndef LWIP_DHCP
#define LWIP_DHCP 1
#endif /* LWIP_DHCP */

/* ---------- IGMP options ---------- */
/**
 * LWIP_IGMP==1: Turn on IGMP module.
 */
#define LWIP_IGMP 1

/* ---------- UDP options ---------- */
#ifndef LWIP_UDP
#define LWIP_UDP 1
#endif /* LWIP_UDP */

#ifndef UDP_TTL
#define UDP_TTL 255
#endif /* UDP_TTL */

/* ---------- Statistics options ---------- */
/**
 * LWIP_STATS==1: Enable statistics collection in lwip_stats.
 */
#define LWIP_STATS 1

/**
 * LWIP_STATS_DISPLAY==1: Compile in the statistics output functions.
 */
#define LWIP_STATS_DISPLAY 1

#ifndef LWIP_PROVIDE_ERRNO
#define LWIP_PROVIDE_ERRNO 1
#endif

#ifndef SNTP_SERVER_DNS
#define SNTP_SERVER_DNS 1
#endif

#ifndef SNTP_SET_SYSTEM_TIME
#include "stdint.h"
extern void EVSE_Connectivity_SetUnixTime(uint32_t sec);
#define SNTP_SET_SYSTEM_TIME EVSE_Connectivity_SetUnixTime
#endif /* SNTP_SET_SYSTEM_TIME */

#ifndef LWIP_DHCP_MAX_NTP_SERVERS
#define LWIP_DHCP_MAX_NTP_SERVERS 2
#endif /* LWIP_DHCP_MAX_NTP_SERVERS */
/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/

/*
Some MCU allow computing and verifying the IP, UDP, TCP and ICMP checksums by hardware:
 - To use this feature let the following define uncommented.
 - To disable it and process by CPU comment the  the checksum.
*/
// #define CHECKSUM_BY_HARDWARE

#ifdef CHECKSUM_BY_HARDWARE
/* CHECKSUM_GEN_IP==0: Generate checksums by hardware for outgoing IP packets.*/
#define CHECKSUM_GEN_IP 0
/* CHECKSUM_GEN_UDP==0: Generate checksums by hardware for outgoing UDP packets.*/
#define CHECKSUM_GEN_UDP 0
/* CHECKSUM_GEN_TCP==0: Generate checksums by hardware for outgoing TCP packets.*/
#define CHECKSUM_GEN_TCP 0
/* CHECKSUM_CHECK_IP==0: Check checksums by hardware for incoming IP packets.*/
#define CHECKSUM_CHECK_IP 0
/* CHECKSUM_CHECK_UDP==0: Check checksums by hardware for incoming UDP packets.*/
#define CHECKSUM_CHECK_UDP 0
/* CHECKSUM_CHECK_TCP==0: Check checksums by hardware for incoming TCP packets.*/
#define CHECKSUM_CHECK_TCP 0
#else
/* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
#define CHECKSUM_GEN_IP    1
/* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
#define CHECKSUM_GEN_UDP   1
/* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
#define CHECKSUM_GEN_TCP   1
/* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
#define CHECKSUM_CHECK_IP  1
/* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
#define CHECKSUM_CHECK_UDP 1
/* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
#define CHECKSUM_CHECK_TCP 1
#endif /* CHECKSUM_BY_HARDWARE */

/**
 * DEFAULT_THREAD_STACKSIZE: The stack size used by any other lwIP thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#ifndef DEFAULT_THREAD_STACKSIZE
#define DEFAULT_THREAD_STACKSIZE 3000
#endif /* DEFAULT_THREAD_STACKSIZE */

/**
 * DEFAULT_THREAD_PRIO: The priority assigned to any other lwIP thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#ifndef DEFAULT_THREAD_PRIO
#define DEFAULT_THREAD_PRIO 3
#endif /* DEFAULT_THREAD_PRIO */

/*
   ------------------------------------
   ---------- Debugging options ----------
   ------------------------------------
*/

/* Define list logs enable */
#define MEMP_OVERFLOW_CHECK (0)
#define MEMP_SANITY_CHECK   (0)
#define MDNS_DEBUG          (LWIP_DBG_OFF)
#define MEM_DEBUG           (LWIP_DBG_OFF)
#define MEMP_DEBUG          (LWIP_DBG_OFF)
#define PBUF_DEBUG          (LWIP_DBG_OFF)
#define API_LIB_DEBUG       (LWIP_DBG_OFF)
#define API_MSG_DEBUG       (LWIP_DBG_OFF)
#define TCPIP_DEBUG         (LWIP_DBG_OFF)
#define NETIF_DEBUG         (LWIP_DBG_OFF)
#define SOCKETS_DEBUG       (LWIP_DBG_OFF)
#define DEMO_DEBUG          (LWIP_DBG_OFF)
#define DHCP_DEBUG          (LWIP_DBG_OFF)
#define AUTOIP_DEBUG        (LWIP_DBG_OFF)
#define ETHARP_DEBUG        (LWIP_DBG_OFF)
#define IP_DEBUG            (LWIP_DBG_OFF)
#define IP_REASS_DEBUG      (LWIP_DBG_OFF)
#define IP6_DEBUG           (LWIP_DBG_OFF)
#define RAW_DEBUG           (LWIP_DBG_OFF)
#define ICMP_DEBUG          (LWIP_DBG_OFF)
#define UDP_DEBUG           (LWIP_DBG_OFF)
#define TCP_DEBUG           (LWIP_DBG_OFF)
#define TCP_INPUT_DEBUG     (LWIP_DBG_OFF)
#define TCP_OUTPUT_DEBUG    (LWIP_DBG_OFF)
#define TCP_RTO_DEBUG       (LWIP_DBG_OFF)
#define TCP_CWND_DEBUG      (LWIP_DBG_OFF)
#define TCP_WND_DEBUG       (LWIP_DBG_OFF)
#define TCP_FR_DEBUG        (LWIP_DBG_OFF)
#define TCP_QLEN_DEBUG      (LWIP_DBG_OFF)
#define TCP_RST_DEBUG       (LWIP_DBG_OFF)
#define PPP_DEBUG           (LWIP_DBG_OFF)
#define SNTP_DEBUG          (LWIP_DBG_OFF)

//#define LWIP_DEBUG
#ifdef LWIP_DEBUG
#ifndef LWIP_PLATFORM_DIAG
#include "FreeRTOSConfig.h"
#define LWIP_PLATFORM_DIAG(x) configPRINTF(x)
#endif /* LWIP_PLATFORM_DIAG */
#endif /* LWIP_DEBUG */

#ifdef LWIP_DEBUG
#define U8_F  "c"
#define S8_F  "c"
#define X8_F  "02x"
#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "u"
#define S32_F "d"
#define X32_F "x"
#define SZT_F "u"
#endif /* LWIP_DEBUG */


/**
 * Loopback demo related options.
 */
#define LWIP_NETIF_LOOPBACK                1
#define LWIP_HAVE_LOOPIF                   1
#define LWIP_NETIF_LOOPBACK_MULTITHREADING 1
#define LWIP_LOOPBACK_MAX_PBUFS            8

#define TCPIP_THREAD_NAME      "tcpip_thread"
#define TCPIP_THREAD_STACKSIZE 1024
#define TCPIP_THREAD_PRIO      8
#ifndef TCPIP_MBOX_SIZE
#define TCPIP_MBOX_SIZE 32
#endif /* TCPIP_MBOX_SIZE */

/**
 * DEFAULT_RAW_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_RAW. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_RAW_RECVMBOX_SIZE 12

/**
 * DEFAULT_UDP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_UDP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_UDP_RECVMBOX_SIZE 12

/**
 * DEFAULT_TCP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_TCP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_TCP_RECVMBOX_SIZE 12

/**
 * DEFAULT_ACCEPTMBOX_SIZE: The mailbox size for the incoming connections.
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when the acceptmbox is created.
 */
#define DEFAULT_ACCEPTMBOX_SIZE 12

#define DNS_TABLE_SIZE      2   // Sufficient for OCPP + NTP + updates
#define DNS_MAX_NAME_LENGTH 128  // Adequate for typical server hostnames
#define DNS_DOES_NAME_CHECK 1   // Security validation
#define DNS_MAX_SERVERS     2   // Primary + secondary DNS

#if (LWIP_DNS || LWIP_IGMP || LWIP_IPV6) && !defined(LWIP_RAND)
/* TODO make it random */
/* When using IGMP or IPv6, LWIP_RAND() needs to be defined to a random-function returning an u32_t random value*/
#include "stdint.h"
uint32_t EVSE_Random(void);
#define LWIP_RAND() EVSE_Random()
#endif /* (LWIP_DNS || LWIP_IGMP || LWIP_IPV6) && !defined(LWIP_RAND) */

/**
 * LWIP_NETIF_EXT_STATUS_CALLBACK==1: Support an extended callback function
 * for several netif related event that supports multiple subscribers.
 * @see netif_ext_status_callback
 */
#define LWIP_NETIF_EXT_STATUS_CALLBACK 1

/**
 * LWIP_NETIF_LINK_CALLBACK==1: Support a callback function from an interface
 * whenever the link changes (i.e., link down)
 */
#define LWIP_NETIF_LINK_CALLBACK 1

/**
 * IP_REASS_MAX_PBUFS: Number of buffers reserved for IP fragment reassembly.
 */
#ifndef IP_REASS_MAX_PBUFS
#define IP_REASS_MAX_PBUFS 4
#endif /* IP_REASS_MAX_PBUFS */

#endif /* __LWIPOPTS_H__ */

/*****END OF FILE****/
