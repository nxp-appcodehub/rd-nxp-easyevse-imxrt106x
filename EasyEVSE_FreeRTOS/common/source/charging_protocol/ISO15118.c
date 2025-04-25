/*
 * Copyright 2024-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <ISO15118.h>
#include <stdio.h>
#include <math.h>
#include "EVSE_UI.h"

#if ENABLE_ISO15118
#include "appl-main.h"
#include "appl-v2g.h"
#include "v2g_cp_service.h"
#endif /* ENABLE_ISO15118 */

#if (PKCS11_SUPPORTED == 1)
#include "EVSE_Secure_Element.h"
#endif /* (PKCS11_SUPPORTED == 1) */

static uint32_t power_requests_cntr = 0;

static vehicle_auth_methods_t vehicle_auth_method = LAST_AUTH_METHOD;
static V2G_status_t V2G_status                    = NOT_CONNECTED;

static evse_iso15118_state_t s_currentState = EVSE_ISO15118_NotEnable;

const char *iso15118AuthStateToString[LAST_AUTH_METHOD + 1] = {
    [VehicleAuth_EIM]  = "EIM",
    [VehicleAuth_PnC]  = "PnC",
    [VehicleAuth_None] = "",
    [LAST_AUTH_METHOD] = "",
};

const char *iso15118StateToString[EVSE_ISO15118_LAST] = {
    [EVSE_ISO15118_NotEnable] = "ISO15118 not activated", [EVSE_ISO15118_InitStack] = "ISO15118 init",
    [EVSE_ISO15118_FWDownload] = "Firmware download",     [EVSE_ISO15118_WaitConnect] = "Wait car",
    [EVSE_ISO15118_SLACStart] = "SLAC in progress",       [EVSE_ISO15118_SLACFailed] = "SLAC failed",
    [EVSE_ISO15118_SLACSucces] = "SLAC Success",          [EVSE_ISO15118_AuthWait] = "Wait for auth",
    [EVSE_ISO15118_AuthFailed] = "Auth failed",           [EVSE_ISO15118_V2GRunnig] = "Demo running",
};

static uint8_t getEnergyDeliveryStatus(uint32_t requestedEnergy, uint32_t deliveredEnergy)
{
    uint8_t percentage = 0;

    if ((requestedEnergy > 0) && (deliveredEnergy > 0) && (deliveredEnergy <= requestedEnergy))
    {
        percentage = (deliveredEnergy * 100) / requestedEnergy;
    }
    if (deliveredEnergy > requestedEnergy)
    {
        percentage = 100;
    }
    return percentage;
}

static void EVSE_ISO15118_SetUIEvent(ui_events_t event)
{
#ifndef EASYEVSE_EV
    EVSE_UI_SetEvent(event);
#endif
}

const char *EVSE_ISO15118_GetStringFromState(evse_iso15118_state_t state)
{
    if (state < EVSE_ISO15118_LAST)
        return iso15118StateToString[state];
    else
        return NULL;
}

evse_iso15118_state_t EVSE_ISO15118_GetState()
{
    return s_currentState;
}

vehicle_auth_methods_t EVSE_ISO15118_GetVehicleAuthMethod(void)
{
    if (s_currentState < EVSE_ISO15118_AuthWait)
    {
        return VehicleAuth_None;
    }
    else
    {
        return vehicle_auth_method;
    }
}

const char *EVSE_ISO15118_GetVehicleAuthMethodString(void)
{
    return iso15118AuthStateToString[EVSE_ISO15118_GetVehicleAuthMethod()];
}

#if ENABLE_ISO15118

charging_directions_t EVSE_ISO15118_GetEnergyDirection(void)
{
    uint8_t direction = 0;
    bool result       = false;

    stxV2GApplExt_EVSEGetEnergyTransferDir(&direction, &result);
    if (result == true)
    {
        if (direction == 0)
        {
            return EVSE_G2V;
        }
        else if (direction == 1)
        {
            return EVSE_V2G;
        }
    }

    return EVSE_NoChargingDirection;
}

void EVSE_ISO15118_ChangeEnergyDirection(void)
{
    stxV2GApplExt_EVChangeChargingDirection();
}


void EVSE_ISO15118_SetState(evse_iso15118_state_t state)
{
	if (state != s_currentState)
	{
	    if (state == EVSE_ISO15118_V2GRunnig)
	    {
	        EVSE_ISO15118_SetV2GStatus(DEMO);
	    }
	    else
	    {
	    	EVSE_ISO15118_SetV2GStatus(NOT_CONNECTED);
	    }

		s_currentState = state;
		EVSE_ISO15118_SetUIEvent(EVSE_UI_ISO15118_Stack_Status);

	}
}

void EVSE_ISO15118_SetVehicleAuthMethod(vehicle_auth_methods_t auth_method)
{
    vehicle_auth_method = auth_method;
}

void EVSE_ISO15118_SetPowerReqCntr(uint32_t cntr)
{
    if (power_requests_cntr != cntr)
    {
        power_requests_cntr = cntr;
        EVSE_ISO15118_SetUIEvent(EVSE_UI_RequestedPower);
    }
}

void EVSE_ISO15118_IncrementPowerReqCntr(void)
{
    EVSE_ISO15118_SetPowerReqCntr(power_requests_cntr + 1);
}

uint32_t EVSE_ISO15118_GetPowerRequestsCntr(void)
{
    return power_requests_cntr;
}

void EVSE_ISO15118_SetV2GStatus(V2G_status_t status)
{
    if (V2G_status != status)
    {
        V2G_status = status;
        EVSE_ISO15118_SetUIEvent(EVSE_UI_ISO15118_V2G_status);
    }
}

V2G_status_t EVSE_ISO15118_GetV2GStatus()
{
    return V2G_status;
}

void EVSE_ISO15118_isCharging(bool *bCharging)
{
    bool result = false;
    if (bCharging != NULL)
    {
        stxV2GApplExt_EVSEGetCharging(bCharging, &result);
    }
}

void EVSE_ISO15118_StopCharging(bool bCharging)
{
#if !EASYEVSE_EV
    if (bCharging == true)
    {
        bool result;
        stxV2GApplExt_EVSEStopCharging(&result);
    }
#endif /* !EASYEVSE_EV */
}

void EVSE_ISO15118_SetMaxCurrent(uint32_t max_current)
{
    bool result = 0;
    stxV2GApplExt_EVSESetMaxACCurrentLimit((float)max_current, &result);
}

char EVSE_ISO15118_GetCpStateString()
{
    char cpStateChar = 'A';
    V2GCPS_STATE cpState;
    bool result;
    stxV2GApplExt_EVSEGetCP(&cpState, &result);
    switch (cpState)
    {
        case V2GCPS_STATE_A:
            cpStateChar = 'A';
            break;
        case V2GCPS_STATE_B:
            cpStateChar = 'B';
            break;
        case V2GCPS_STATE_C:
            cpStateChar = 'C';
            break;
        case V2GCPS_STATE_D:
            cpStateChar = 'D';
            break;
        case V2GCPS_STATE_E:
            cpStateChar = 'E';
            break;
        case V2GCPS_STATE_F:
            cpStateChar = 'F';
            break;
        case V2GCPS_STATE_UNKNOWN:
            cpStateChar = 'A';
            break;
    }
    return cpStateChar;
}

void EVSE_ISO15118_GetVehicleData(vehicle_data_t *vehicle_data)
{
    if (vehicle_data == NULL)
    {
        return;
    }

    double double_variable       = 0.0f;
    uint64_t unsigned64_variable = 0;
    bool result                  = true;

    stxV2GApplExt_EVSEGetDeliveredCurrent(&double_variable, &result);
    double_variable             = fabs(double_variable);
    vehicle_data->chargeCurrent = (float)double_variable;

    stxV2GApplExt_EVSEGetTotalDeliveredEnergy(&double_variable, &result);
    double_variable               = fabs(double_variable);
    vehicle_data->deliveredEnergy = (uint32_t)double_variable;

    stxV2GApplExt_EVSEGetDeliveredEnergyHLC(&double_variable, &result);
    double_variable                  = fabs(double_variable);
    vehicle_data->deliveredEnergyHLC = (uint32_t)double_variable;

    stxV2GApplExt_EVSEGetElapsedTime(&unsigned64_variable, &result);
    vehicle_data->elapsedTime = (uint32_t)unsigned64_variable;

    stxV2GApplExt_EVSEGetEamount(&double_variable, &result);
    double_variable               = fabs(double_variable);
    vehicle_data->requestedEnergy = (uint32_t)double_variable;

    stxV2GApplExt_EVSEGetRemainingTime(&unsigned64_variable, &result);
    vehicle_data->timeToCharge = (uint32_t)unsigned64_variable;

    stxV2GApplExt_EVSEGetAuthenticationStatus(&vehicle_data->vehicleAuth, &result);

    vehicle_data->energyDeliveryStatus =
        getEnergyDeliveryStatus(vehicle_data->requestedEnergy, vehicle_data->deliveredEnergyHLC);
}

void EVSE_ISO15118_Init()
{
#if (PKCS11_SUPPORTED == 1)
    EVSE_SE050_Open_Session();
#endif /* PKCS11_SUPPORTED */
    stxRefApp_Init();
}

void EVSE_ISO15118_Loop(bool *stopCharging)
{
    stxRefApp_Tick();
    if (stopCharging)
    {
        *stopCharging = false;
    }
}

#endif /* ENABLE_ISO15118 */
