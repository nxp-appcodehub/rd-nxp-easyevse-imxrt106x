/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef _LWS_CONFIG_H_
#define _LWS_CONFIG_H_

#include "etharp.h"

#define LWS_INSTALL_DATADIR "/"
#define LWS_ROLE_H1 1
#define LWS_ROLE_H2 1
#define LWS_WITH_HTTP 1
#define LWS_WITH_HTTP2 1
#define LWS_HTTP_HEADERS_ALL 1
#define LWS_WITH_CLIENT 1
#define LWS_WITHOUT_SERVER 1
#define LWS_WITHOUT_TESTAPPS 1
#define LWS_WITHOUT_EXTENSIONS 1

#define LWS_WITH_FREERTOS 1
#define LWS_WITH_NETWORK 1
#define LWS_WITHOUT_BUILTIN_GETIFADDRS 1
#define LWS_WITHOUT_DAEMONIZE 1
#define LWS_WITHOUT_SYS_STATE 1
#define LWS_WITHOUT_SYS_SMD 1
#define LWS_WITHOUT_SYS_FAULT 1
#define LWS_WITHOUT_SYS_NTPCLIENT 1
#define LWS_WITHOUT_SYS_METRICS 1
#define LWS_WITHOUT_SYS_DHCP_CLIENT 1
#define LWS_WITHOUT_SYS_ASYNC_DNS 1
#define LWS_NO_DAEMONIZE 1

#define LWS_PLAT_FREERTOS 1
#define LWS_AMAZON_RTOS 1

#define LWS_WITH_POLL 1

#define LWS_WITH_TLS 1
#define LWS_WITH_TLS_SESSIONS 1
#define LWS_WITH_GENCRYPTO 1
#define LWS_WITH_TLS_JIT_TRUST
#define LWS_WITH_MBEDTLS 1

#define LWS_WITH_WEBSOCKETS 1

#define LWS_WITH_SYS_STATE

//#define LWS_WITH_SYS_NTPCLIENT
//#define LWS_WITH_SYS_DHCP_CLIENT
//#define LWS_WITH_SYS_SMD
//#define LWS_WITH_SYS_ASYNC_DNS
//#define LWS_WITH_SECURE_STREAMS_PROXY_API
//#define LWS_WITH_SECURE_STREAMS
//#define LWS_WITH_SECURE_STREAMS_AUTH_SIGV4
//#define LWS_WITH_ACCESS_LOG
//#define LWS_WITH_CACHE_NSCOOKIEJAR
//#define LWS_WITH_CGI
//#define LWS_WITH_THREADPOOL
//#define LWS_WITH_DRIVERS
//#define LWS_WITH_SOCKS5
//#define LWS_WITH_CONMON
//#define LWS_WITH_OTA

#define LWS_ROLE_WS
#define LWS_WITH_SECURE_STREAMS_CPP

#define LWS_LIBRARY_VERSION "1.5.1"

#define LWS_MAX_SMP 1 //0 //32
#define LWS_COMPRESSED_BACKTRACES_SNIP_PRE 2
#define LWS_COMPRESSED_BACKTRACES_SNIP_POST 1

#define ETHER_ADDR_LEN ETHARP_HWADDR_LEN //6

#endif /* _LWS_CONFIG_H_ */

