/*
 * Copyright 2024-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef EVSE_EDGELOCK2GOAGENT_H_
#define EVSE_EDGELOCK2GOAGENT_H_

#include "nxp_iot_agent_status.h"

/**
 * Runs the EdgeLock 2GO agent, in order to get the secure objects configured in
 * EdgeLock 2GO.
 * @return IOT_AGENT_SUCCESS on succeeded operation or IOT_AGENT_FAILURE on failed operation
 */
iot_agent_status_t EVSE_EdgeLock_RunClient(void);


/**
 * @brief Creates the EdgeLock2Go task, initializes internal components.
 */
void EVSE_EdgeLock_Init(void);

/**
 * @brief Returns true when EdgeLock2Go agent finishes provisioning CPO key or if it is already provisioned.
 */
bool EVSE_EdgeLock_IsReady(void);

#endif /* EVSE_EDGELOCK2GOAGENT_H_ */
