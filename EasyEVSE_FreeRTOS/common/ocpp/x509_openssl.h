/*
 * x509_openssl.h
 *
 *  Created on: Jul 9, 2025
 *      Author: nxf52905
 */

#ifndef X509_OPENSSL_H_
#define X509_OPENSSL_H_

#include "ssl_types.h"

#define X509_CHECK_FLAG_NO_WILDCARDS			    (1 << 1)
#define X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS		(1 << 2)

#define SSL_FILETYPE_PEM 1
#define SSL_VERIFY_PEER  0x01

typedef char* X509_NAME;

X509* X509_STORE_CTX_get_current_cert(const X509_STORE_CTX* ctx);
int X509_check_host(X509 *, const char *name, size_t namelen, unsigned int flags, char **peername);
X509_NAME *X509_get_subject_name(const X509 *x);
int X509_NAME_get_text_by_NID(const X509_NAME *name, int nid, char *buf, int len);

#endif /* X509_OPENSSL_H_ */
