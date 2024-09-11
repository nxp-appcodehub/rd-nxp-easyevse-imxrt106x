/*
 *
 * Copyright 2019-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef SE05X_GETCERTIFICATE_H_
#define SE05X_GETCERTIFICATE_H_

#include <fsl_sss_api.h>

sss_status_t se05x_GetCertificate(uint32_t certId,  uint8_t *certificate_data, size_t *certLength);

#endif /* SE05X_GETCERTIFICATE_H_ */
