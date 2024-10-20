/* Copyright (c) Microsoft Corporation.
 * Licensed under the MIT License. */

/**
 * @brief Transport abstraction to be used between TLS or Sockets
 *
 */
#if (EVSE_X509_SE050_AUTH == 0)
#ifndef TRANSPORT_ABSTRACTION_H
#define TRANSPORT_ABSTRACTION_H

typedef struct NetworkContext   NetworkContext_t;

/* SSL Context Handle */
typedef void                    * SSLContextHandle;

/* Socket Context Handle */
typedef void                    * SocketContextHandle;

#endif /* TRANSPORT_ABSTRACTION_H */
#endif
