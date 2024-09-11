#ifndef LWIP_PING_H
#define LWIP_PING_H

#include "lwip/ip_addr.h"
#include "lwip/err.h"

/**
 * PING_USE_SOCKETS: Set to 1 to use sockets, otherwise the raw api is used
 */
#ifndef PING_USE_SOCKETS
#define PING_USE_SOCKETS    LWIP_SOCKET
#endif

void ping_init(const ip_addr_t* ping_addr);

err_t ping_send(int s, const ip_addr_t *addr);
err_t ping_recv(int s);

#if !PING_USE_SOCKETS
void ping_send_now(void);
#endif /* !PING_USE_SOCKETS */

#endif /* LWIP_PING_H */
