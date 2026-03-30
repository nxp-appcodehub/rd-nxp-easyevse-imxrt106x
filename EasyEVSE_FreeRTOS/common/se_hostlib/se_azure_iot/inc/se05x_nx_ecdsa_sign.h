/*
 *
 * Copyright 2019-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SE_AZURE_IOT_INC_SE05X_NX_ECDSA_SIGN_H_
#define SE_AZURE_IOT_INC_SE05X_NX_ECDSA_SIGN_H_

sss_status_t se05x_ecdsa_sign(uint32_t privateKeyId,
								 uint8_t *inputDigest, size_t inputDigestLength,
								 uint8_t *signature, size_t *signatureLen);

#endif /* SE_AZURE_IOT_INC_SE05X_NX_ECDSA_SIGN_H_ */
