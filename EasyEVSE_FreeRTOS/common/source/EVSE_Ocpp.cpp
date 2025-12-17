/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */




#include <filesystem>
#include <cinttypes>

#include "EVSE_Ocpp.h"

#include "charge_point.hpp"
#include <ocpp/common/cistring.hpp>
#include <ocpp/v2/connectivity_manager.hpp>
#include "composed_device_model.hpp"
#include "types.hpp"
#include "conversions.hpp"
#include "transaction_handler.hpp"

#include <future>

#ifdef __cplusplus
extern "C" {
#endif

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "task_config.h"

#include "EVSE_ChargingProtocol.h"
#include "EVSE_Metering.h"
#include "EVSE_UI.h"
#include "EVSE_NFC.h"
#include "EVSE_Connectivity.h"
#include "ISO15118.h"

#ifdef __cplusplus
}
#endif

#if ENABLE_OCPP

#define EVSE_DELAY_OCPP_START 500
#define EVSE_ID 1
#define EVSE_CONNECTOR_ID 1
#define EVSE_OCPP_AUTH_TIMEOUT_S 3    // 3 seconds
#define EVSE_OCPP_SESSION_ID_MAX 40
#define EVSE_OCPP_MAX_RECONNECTS 15

using namespace ocpp::v2;
using namespace module;

static ocppConnectionState_t s_ocppConnectionState = EVSE_Ocpp_NotConnected;
static const char *ocppConnectionStateString[EVSE_Ocpp_Last + 1] = {
    [EVSE_Ocpp_NotConnected]      = "Ocpp client not connected",
    [EVSE_Ocpp_Connected]         = "Ocpp client connected",
	[EVSE_Ocpp_Error]             = "Failed connection to ocpp server",
    [EVSE_Ocpp_Last]              = "LAST",
};

static EventGroupHandle_t s_evse_ocpp_event = NULL;

static SemaphoreHandle_t s_ocppAuthRespSemaphore = NULL;
static bool is_EV_authorized = false;
static uint32_t last_auth_time = 0U;

static ocpp::Powermeter ocppCurrentPowerData;
static uint32_t ocppTelemetryCounter = 0U;

static float totalCost = 0.0f;
static double runningCost = 0.0f;

static bool EV_plugged_in = false;
static bool session_started = false;
static bool disconnect_requested = false;

static bool iso15118_pnc_enabled = false;
static int32_t ev_connection_timeout = 0U;
static std::vector<std::string> ocppDisplayMessagesStr;
static ocpp::v2::NetworkConnectionProfile ocppConnectionProfile;
static int32_t ocppHeartbeatInterval = 0U;
static uint32_t prng_state = 1;

static uint8_t ocpp_reconnection_attempts = 0U;



namespace OCPPConfigCheck{
    /* Helper: Check if two strings are equal at compile time */
    constexpr bool strings_equal(const char* a, const char* b) {
        return (*a == *b) && (*a == '\0' || strings_equal(a + 1, b + 1));
    }
}

static_assert(!OCPPConfigCheck::strings_equal(EVSE_OCPP_SERVER_URL, "paste your ocpp server address here"), "Please configure EVSE_OCPP_SERVER_URL");
static_assert(!OCPPConfigCheck::strings_equal(CHARGE_POINT_ID, "paste your charge point id here"), "Please configure CHARGE_POINT_ID");

/**
 * Set state and advertise change state.
 * @param connectionState new ocpp state;
 */
static void prvEVSE_OCPP_SetConnectionState(ocppConnectionState_t ocppConnectionState)
{
    if (s_ocppConnectionState == ocppConnectionState)
    {
        return;
    }

    s_ocppConnectionState = ocppConnectionState;

#if (ENABLE_LCD)
    EVSE_UI_SetEvent(EVSE_UI_OcppStatus);
#endif /* ENABLE_LCD */
}

static uint32_t simple_random(void) {
    // Linear congruential generator
    prng_state = (prng_state * 1103515245 + 12345) & 0x7fffffff;
    return prng_state;
}

static void generate_session_id(char* buffer, size_t buffer_size)
{
	uint32_t tick = xTaskGetTickCount();

    uint32_t random1, random2;

    //taskENTER_CRITICAL();
    // Seed with tick count for better randomness
    prng_state ^= tick;
    random1 = simple_random();
    random2 = simple_random();
    //taskEXIT_CRITICAL();

    snprintf(buffer, buffer_size, "%08" PRIX32 "-%04" PRIX32 "-%04" PRIX32 "-%04" PRIX32 "-%08" PRIX32 "%04" PRIX32,
    		random1,
			(random2 >> 16) & 0xFFFF,
			random2 & 0xFFFF,
			(tick >> 16) & 0xFFFF,
			tick,
			(random1 >> 16) & 0xFFFF);

    configPRINTF(("session_id is %s", buffer));
}

static void prvEVSE_OCPP_InitCurrentPowerData(void)
{
	ocppCurrentPowerData.timestamp = ocpp::DateTime().to_rfc3339();
	ocppCurrentPowerData.energy_Wh_import.total = 0.0f;
	ocppCurrentPowerData.energy_Wh_import.L1 = 0.0f;
	ocppCurrentPowerData.energy_Wh_import.L2 = 0.0f;
	ocppCurrentPowerData.energy_Wh_import.L3 = 0.0f;
	ocppCurrentPowerData.meter_id = "MTR553";
	ocppCurrentPowerData.phase_seq_error = false;
	ocppCurrentPowerData.energy_Wh_export = ocpp::Energy{0.0f};
	ocppCurrentPowerData.power_W = ocpp::Power{0.0f};
	ocppCurrentPowerData.voltage_V = ocpp::Voltage{0.0f};
	ocppCurrentPowerData.VAR = ocpp::ReactivePower{0.0f};
	ocppCurrentPowerData.current_A = ocpp::Current{0.0f};
	ocppCurrentPowerData.frequency_Hz = ocpp::Frequency{0.0f};
}

static void EVSE_VehicleData_to_OcppData(void)
{
	const vehicle_data_t *values = EVSE_ChargingProtocol_GetVehicleData();
	const meter_data_t *m_values = EVSE_Meter_GetMeterData();
	if ((!values) || (!m_values))
	{
		return;
	}

	ocppCurrentPowerData.timestamp = ocpp::DateTime().to_rfc3339(); //"2025-09-16T12:40:37.373781Z";

	ocppCurrentPowerData.phase_seq_error = false;

	ocppCurrentPowerData.energy_Wh_import.total = 0.0f; //grid-to-vehicle
	ocppCurrentPowerData.energy_Wh_import.L1 = 0.0f;
	ocppCurrentPowerData.energy_Wh_import.L2 = 0.0f;
	ocppCurrentPowerData.energy_Wh_import.L3 = 0.0f;
	ocppCurrentPowerData.energy_Wh_export = ocpp::Energy{0.0f}; //VEHICLE-TO-GRID

	charging_directions_t dir = EVSE_ChargingProtocol_ChargingDirection();

	if (dir == EVSE_G2V)
	{
#if ENABLE_ISO15118
		ocppCurrentPowerData.energy_Wh_import.total = float(values->deliveredEnergyHLC);
#else
		ocppCurrentPowerData.energy_Wh_import.total = float(values->deliveredEnergy);
#endif
	}
	else if (dir == EVSE_V2G)
	{
#if ENABLE_ISO15118
		ocppCurrentPowerData.energy_Wh_export = ocpp::Energy{float(values->deliveredEnergyHLC)};
#else
		ocppCurrentPowerData.energy_Wh_export = ocpp::Energy{float(values->deliveredEnergy)};
#endif
	}

	ocppCurrentPowerData.power_W = ocpp::Power{m_values->wh};
	ocppCurrentPowerData.voltage_V = ocpp::Voltage{m_values->vrms};
	ocppCurrentPowerData.VAR = ocpp::ReactivePower{float(m_values->Q)};
	ocppCurrentPowerData.current_A = ocpp::Current{values->chargeCurrent};
//	ocppCurrentPowerData.frequency_Hz = ocpp::Frequency{values[FREQUENCY_HZ]};
}

static std::optional<ocpp::v2::IdToken> prvEVSE_OCPP_GetLocalAuthorizationToken(void)
{
#if ENABLE_ISO15118
    if (EVSE_ISO15118_GetVehicleAuthMethod() == VehicleAuth_PnC)
    {
        const char* emaid = EVSE_ISO15118_GetEmaidValue();
        if (emaid == NULL)
        {
            return std::nullopt;
        }

        configPRINTF(("Packing emaid value %s", emaid));
        std::string eMAIDStr(emaid);
        ocpp::CiString<255> tokenValue(eMAIDStr);
        ocpp::CiString<20> tokenId("eMAID");
        ocpp::v2::IdToken idLocalToken {.idToken = tokenValue, .type = tokenId};
        return idLocalToken;
    }
#endif

#if (ENABLE_CLEV663_NFC == 1)
	// wait for NFC to be ready
	const char *vehicleID = EVSE_NFC_Get_VehicleID();
	if ((vehicleID == NULL) || (strlen(vehicleID) == 0))
	{
		return std::nullopt;
	}

	// Create the ID token
    if (strcmp(vehicleID, "N/A") == 0) {
        ocpp::CiString<255> tokenValue("AUTH_FROM_CLI");
        ocpp::CiString<20> tokenId("Local");
    	// Create OCPP ID token structure
        ocpp::v2::IdToken idLocalToken {.idToken = tokenValue, .type = tokenId};
        return idLocalToken;
    }

    std::string vehicleIDStr(vehicleID);
    ocpp::CiString<255> tokenValue(vehicleIDStr);
	ocpp::CiString<20> tokenId("ISO14443");

#else
	// Create the ID token
	ocpp::CiString<255> tokenValue("AUTH_FROM_CLI");
    ocpp::CiString<20> tokenId("Local");
#endif /* (ENABLE_CLEV663_NFC == 1) */

	// Create OCPP ID token structure
	ocpp::v2::IdToken idLocalToken {.idToken = tokenValue, .type = tokenId};
	return idLocalToken;
}

static std::vector<std::string> extractStartStopPoints(const std::string& point_list)
{
	std::vector<std::string> points;

	if (point_list.empty())
	{
		return points;
	}

	size_t start = 0;
	size_t end = 0;

	while ((end = point_list.find(',', start)) != std::string::npos) {
	    std::string item = point_list.substr(start, end - start);

	    // Trim whitespace
	    item.erase(0, item.find_first_not_of(" \t"));
	    item.erase(item.find_last_not_of(" \t") + 1);

	    if (!item.empty()) {
	        points.push_back(item);
	    }

	    start = end + 1;
	}

	// Handle the last item (after the last comma)
	std::string last_item = point_list.substr(start);
	last_item.erase(0, last_item.find_first_not_of(" \t"));
	last_item.erase(last_item.find_last_not_of(" \t") + 1);

	if (!last_item.empty()) {
	    points.push_back(last_item);
	}

	return points;
}

static TxStartStopPoint get_tx_start_stop_enum(const std::string& tx_start_stop_point)
{
	if (tx_start_stop_point == "ParkingBayOccupancy") {
		return TxStartStopPoint::ParkingBayOccupancy;
	} else if (tx_start_stop_point == "EVConnected") {
		return TxStartStopPoint::EVConnected;
	} else if (tx_start_stop_point == "Authorized") {
		return TxStartStopPoint::Authorized;
	} else if (tx_start_stop_point == "PowerPathClosed") {
		return TxStartStopPoint::PowerPathClosed;
	} else if (tx_start_stop_point == "EnergyTransfer") {
		return TxStartStopPoint::EnergyTransfer;
	} else if (tx_start_stop_point == "DataSigned") {
		return TxStartStopPoint::DataSigned;
	} else {
		// default to PowerPathClosed for now
		return TxStartStopPoint::PowerPathClosed;
	}
}

static void process_tx_event_effect(const int32_t evse_id, module::TxEventEffect tx_event_effect, std::shared_ptr<ocpp::v2::ChargePoint>& charge_point, TransactionHandler& transaction_handler) {
    if (tx_event_effect == module::TxEventEffect::NONE) {
        return;
    }

    const auto transaction_data = transaction_handler.get_transaction_data(evse_id);
    if (transaction_data == nullptr) {
    	configPRINTF(("Could not start transaction because no transaction data is present"));
        throw std::runtime_error("Could not start transaction because no transaction data is present");
    }

    ocpp::DateTime now_timestamp;
    // fix me: this messes the transaction_data->trigger_reason field
    // transaction_data->timestamp = module::conversions::to_ocpp_datetime_or_now(std::string("2025-09-19Z14:44T"));

    if (tx_event_effect == module::TxEventEffect::START_TRANSACTION) {
        transaction_data->started = true;
        transaction_data->meter_value = module::conversions::to_ocpp_meter_value(
            ocppCurrentPowerData, ocpp::v2::ReadingContextEnum::Transaction_Begin,
            std::nullopt);
        charge_point->on_transaction_started(
            evse_id, transaction_data->connector_id, transaction_data->session_id, now_timestamp,
            transaction_data->trigger_reason, transaction_data->meter_value, std::nullopt,
            std::nullopt, std::nullopt, std::nullopt,
            transaction_data->charging_state);
    } else if (tx_event_effect == module::TxEventEffect::STOP_TRANSACTION) {
        transaction_data->meter_value = module::conversions::to_ocpp_meter_value(ocppCurrentPowerData,
                                                                         ocpp::v2::ReadingContextEnum::Transaction_End,
																		 std::nullopt);
        charge_point->on_transaction_finished(evse_id, now_timestamp, transaction_data->meter_value,
                                                    transaction_data->stop_reason, transaction_data->trigger_reason,
                                                    std::nullopt, std::nullopt,
                                                    transaction_data->charging_state);
        transaction_handler.reset_transaction_data(evse_id);
    }
}

static void process_charging_started(const int32_t evse_id, const int32_t connector_id, bool remote_start, std::shared_ptr<ocpp::v2::ChargePoint>& charge_point, TransactionHandler& transaction_handler)
{
	auto transaction_data = transaction_handler.get_transaction_data(evse_id);
	if (transaction_data != nullptr) {
		transaction_data->charging_state = ocpp::v2::ChargingStateEnum::Charging;
		transaction_data->trigger_reason = remote_start ? ocpp::v2::TriggerReasonEnum::RemoteStart :
				ocpp::v2::TriggerReasonEnum::ChargingStateChanged;
	}

	const auto tx_event_effect = transaction_handler.submit_event(evse_id, TxEvent::ENERGY_TRANSFER_STARTED);
	process_tx_event_effect(evse_id, tx_event_effect, charge_point, transaction_handler);
	charge_point->on_charging_state_changed(evse_id, ocpp::v2::ChargingStateEnum::Charging);
}

static void process_charging_ended(const int32_t evse_id, const int32_t connector_id, std::shared_ptr<ocpp::v2::ChargePoint>& charge_point, TransactionHandler& transaction_handler, bool remote_stop)
{
	auto transaction_data = transaction_handler.get_transaction_data(evse_id);
	if (transaction_data != nullptr) {
		if (remote_stop)
		{
			transaction_data->trigger_reason = ocpp::v2::TriggerReasonEnum::RemoteStop;
			transaction_data->charging_state = ocpp::v2::ChargingStateEnum::EVConnected;
		}
		else
		{
	     transaction_data->trigger_reason = ocpp::v2::TriggerReasonEnum::StopAuthorized;
	     transaction_data->charging_state = ocpp::v2::ChargingStateEnum::EVConnected;
		}
	}

	const auto tx_event_effect = transaction_handler.submit_event(evse_id, TxEvent::ENERGY_TRANSFER_STOPPED);
	process_tx_event_effect(evse_id, tx_event_effect, charge_point, transaction_handler);
	charge_point->on_charging_state_changed(evse_id, transaction_data->charging_state);
}

static void reset_session_state(void)
{
	EV_plugged_in = false;
	is_EV_authorized = false;
	session_started = false;
	last_auth_time = 0;
}

std::string EVSE_OCPP_GetEVSEIdentity(void)
{
    return std::string(CHARGE_POINT_ID);
}

void EVSE_OCPP_SetEvent(ocpp_events_t event)
{
    if (s_evse_ocpp_event)
    {
        xEventGroupSetBits(s_evse_ocpp_event, event);
    }
}

void EVSE_OCPP_SetReconnectEvent(void)
{
    if (s_evse_ocpp_event)
    {
        xEventGroupSetBits(s_evse_ocpp_event, EVSE_RECONNECT_EVENT);
    }
}

bool EVSE_OCPP_GetAuthResponse(uint32_t timeout_ms, bool* auth_response)
{
    if (!s_ocppAuthRespSemaphore)
    {
        return false;
    }

    TickType_t timeoutTicks = pdMS_TO_TICKS(timeout_ms);
	if (xSemaphoreTake(s_ocppAuthRespSemaphore, timeoutTicks) == pdTRUE)
	{
        if (auth_response)
        {
		    *auth_response = is_EV_authorized;
        }
        last_auth_time = 0;
        return true;
	}

	uint32_t elapsed_time = 0;
    uint32_t current_time = EVSE_Connectivity_GetUnixTime();
    if (last_auth_time > 0)
    {
        elapsed_time = current_time - last_auth_time;
    }

    if ((timeout_ms) || (elapsed_time > EVSE_OCPP_AUTH_TIMEOUT_S))
    {
    	/* Blocking or non-blocking mode, we reached here due to timeout */
        if (auth_response)
        {
            *auth_response = false;
        }
        last_auth_time = 0;
        return true;
    }

    return false;
}
void EVSE_OCPP_SetNetworkConnectionProfile()
{
	ocppConnectionProfile.ocppInterface = OCPPInterfaceEnum::Wireless0;
	ocppConnectionProfile.ocppTransport = OCPPTransportEnum::JSON;
	ocppConnectionProfile.messageTimeout = EVSE_OCPP_TIMEOUT_S;
	ocppConnectionProfile.ocppCsmsUrl = EVSE_OCPP_SERVER_URL;
	ocppConnectionProfile.securityProfile = EVSE_OCPP_SECURITY_LEVEL;
}

ocpp::v2::NetworkConnectionProfile* EVSE_OCPP_GetNetworkConnectionProfile(void)
{
   return &ocppConnectionProfile;
}

const char* EVSE_OCPP_GetDisplayMessageString(void)
{
	if(ocppDisplayMessagesStr.empty())
	{
		return NULL;
	}
	else
	{
		// display only first message
		return ocppDisplayMessagesStr[0].c_str();
	}
}

ocppConnectionState_t EVSE_OCPP_GetConnectionState(void)
{
    configPRINTF((info("OCPP connection:%s."), ocppConnectionStateString[s_ocppConnectionState]));
    return s_ocppConnectionState;
}

const char *EVSE_OCPP_GetStringFromState(ocppConnectionState_t ocppState)
{
    return ocppConnectionStateString[ocppState];
}

const uint32_t EVSE_OCPP_GetTelemetryCount(void)
{
    return ocppTelemetryCounter;
}

//Callbacks
bool OCPP_IsResetAllowed_Cb(const std::optional<const int32_t> evse_id, const ResetEnum& reset_type)
{
	configPRINTF(("-----> OCPP_IsResetAllowed Callback\r\n"));
    if (evse_id.has_value()) {
        return reset_type == ResetEnum::OnIdle;
    }
    return reset_type == ResetEnum::Immediate;
}

void OCPP_Reset_Cb(const std::optional<const int32_t> evse_id, const ResetEnum& reset_type)
{
	configPRINTF(("-----> OCPP_Reset Callback\r\n"));
	return;
}

RequestStartStopStatusEnum OCPP_StopTransaction_Cb(const int32_t evse_id, const ReasonEnum& stop_reason)
{
	configPRINTF(("-----> OCPP_StopTransaction Callback\r\n"));

	if (evse_id != EVSE_ID)
	{
		configPRINTF(("Received stop transaction request for a different EVSE"));
		return RequestStartStopStatusEnum::Rejected;
	}

	if (stop_reason == ReasonEnum::Remote)
	{
		configPRINTF(("Received remote stop transaction request"));
		xEventGroupSetBits(s_evse_ocpp_event, EVSE_CHARGING_STOP_REMOTE_EVENT);
		return RequestStartStopStatusEnum::Accepted;
	}

    return RequestStartStopStatusEnum::Accepted;
}

void OCPP_PauseCharging_Cb(const int32_t evse_id)
{
	configPRINTF(("-----> OCPP_PauseCharging Callback\r\n"));
	configPRINTF(("OCPP asked to pause charging, maybe due to failed authorization.\r\n"));
    return;
}

void OCPP_ConnectorEffectiveOperativeStatusChanged_Cb(const int32_t evse_id, const int32_t connector_id, const OperationalStatusEnum new_status)
{
	configPRINTF(("-----> OCPP_ConnectorEffectiveOperativeStatusChanged Callback\r\n"));
	return;
}

GetLogResponse OCPP_GetLogRequest_Cb(const GetLogRequest& request)
{
	GetLogResponse resp;
	configPRINTF(("-----> OCPP_GetLogRequest Callback\r\n"));
	return resp;
}

UnlockConnectorResponse OCPP_UnlockConnector_Cb(const int32_t evse_id, const int32_t connecor_id)
{
	UnlockConnectorResponse resp;
	configPRINTF(("-----> OCPP_UnlockConnector Callback\r\n"));
	return resp;
}

// A transaction start request was received by the CSMS
RequestStartStopStatusEnum OCPP_RemoteStartTransaction_Cb(const RequestStartTransactionRequest& request,
                                             const bool authorize_remote_start)
{
	configPRINTF(("-----> OCPP_RemoteStartTransaction Callback\r\n"));
	if ((request.evseId.has_value()) && (request.evseId.value() != EVSE_ID))
	{
		configPRINTF(("Received transaction start request for a different EVSE"));
		return RequestStartStopStatusEnum::Rejected;
	}

	if (!EV_plugged_in)
	{
		configPRINTF(("Received transaction start request but EV is not plugged in"));
		return RequestStartStopStatusEnum::Rejected;
	}

	configPRINTF(("Received remote start transaction request"));
	if (authorize_remote_start)
	{
		configPRINTF(("Authorization needed before starting to charge."));
		//TODO: authorize token
	}
	else
	{
		// token already authorized by the CSMS
		configPRINTF(("Authorization not needed, proceed with charging"));
		xEventGroupSetBits(s_evse_ocpp_event, EVSE_CHARGING_START_REMOTE_EVENT);
	}

	return RequestStartStopStatusEnum::Accepted;
}

ocpp::ReservationCheckStatus OCPP_IsReservationForToken_Cb(const int32_t evse_id, const ocpp::CiString<255> idToken,
                                               const std::optional<ocpp::CiString<255>> groupIdToken)
{
	configPRINTF(("-----> OCPP_IsReservationForToken Callback\r\n"));
	return ocpp::ReservationCheckStatus::NotReserved;
}

UpdateFirmwareResponse OCPP_UpdateFirmwareRequest_Cb(const UpdateFirmwareRequest& request)
{
	UpdateFirmwareResponse resp;
	configPRINTF(("-----> OCPP_UpdateFirmwareRequest Callback\r\n"));
	return resp;
}

void OCPP_SecurityEvent_Cb(const ocpp::CiString<50>& event_type, const std::optional<ocpp::CiString<255>>& tech_info)
{
	configPRINTF(("-----> OCPP_SecurityEvent Callback\r\n"));
	return;
}

void OCPP_SetChargingProfiles_Cb()
{
	configPRINTF(("-----> OCPP_SetChargingProfiles Callback\r\n"));
	return;
}

// Calles when the CSMS wants to remotely configure the EVSE
void OCPP_VariableChanged_Cb(const SetVariableData& set_variable_data)
{
	configPRINTF(("-----> OCPP_VariableChanged Callback\r\n"));
	if (set_variable_data.component == ocpp::v2::ControllerComponents::TxCtrlr)
	{
        if (set_variable_data.variable.name.get() == "EVConnectionTimeOut")
        {
            ev_connection_timeout = std::stoi(set_variable_data.attributeValue.get());
        }
        else if (set_variable_data.variable.name.get() == "TxStartPoint")
        {
        	std::string start_point_list = set_variable_data.attributeValue.get();
        	std::vector<std::string> start_points_vec = extractStartStopPoints(start_point_list);
        	if (!start_points_vec.empty())
        	{
//        		for (const auto& point : start_points_vec) {
//        			start_points.insert(get_tx_start_stop_enum(point));
//        		}
        	}
        }
        else if (set_variable_data.variable.name.get() == "TxStopPoint")
        {
        	std::string stop_point_list = set_variable_data.attributeValue.get();
        	std::vector<std::string> stop_points_vec = extractStartStopPoints(stop_point_list);
        	if (!stop_points_vec.empty())
        	{
//        		for (const auto& point : stop_points_vec) {
//        			stop_points.insert(get_tx_start_stop_enum(point));
//        		}
        	}
        }
	}
	else if (set_variable_data.component == ocpp::v2::ControllerComponents::AuthCtrlr)
	{
        //todo
	}
	else if (set_variable_data.component == ocpp::v2::ControllerComponents::ISO15118Ctrlr)
	{
		if (set_variable_data.variable.name.get() == "PnCEnabled")
		{
			iso15118_pnc_enabled = ocpp::conversions::string_to_bool(set_variable_data.attributeValue.get());
		}
		else if (set_variable_data.variable.name.get() == "CentralContractValidationAllowed")
		{
			//todo
		}
		else if (set_variable_data.variable.name.get() == "ContractCertificateInstallationEnabled")
		{
			//todo
		}
	}

	return;
}

SetNetworkProfileStatusEnum OCPP_ValidateNetworkProfile_Cb(
        const int32_t configuration_slot, const NetworkConnectionProfile& network_connection_profile)
{
	configPRINTF(("-----> OCPP_ValidateNetworkProfile Callback\r\n"));
	return SetNetworkProfileStatusEnum::Accepted;
}

#if 0
// A helper function that returns a default ConfigNetworkResult
ConfigNetworkResult EmptyConfig() {
    return ConfigNetworkResult{};
}

std::future<ConfigNetworkResult> OCPP_ConfigureNetworkConnectionProfile_Cb(
    const int32_t configuration_slot, const NetworkConnectionProfile& network_connection_profile)
{

//	std::promise<ConfigNetworkResult> promise;
//	ConfigNetworkResult result;
//	promise.set_value(result);
//	return promise.get_future();
//    return std::nullopt;
    return std::async(std::launch::deferred, EmptyConfig);
}
#endif

void OCPP_TimeSync_Cb(const ocpp::DateTime& currentTime)
{
	configPRINTF(("-----> OCPP_TimeSync Callback\r\n"));
	return;
}

void OCPP_BootNotification_Cb(const ocpp::v2::BootNotificationResponse& boot_notification_response)
{
	if (boot_notification_response.status == RegistrationStatusEnum::Accepted)
	{
		prvEVSE_OCPP_SetConnectionState(EVSE_Ocpp_Connected);
		ocppHeartbeatInterval = boot_notification_response.interval;
		configPRINTF(("The EVSE has been successfully registered within the CSMS. Heartbeat interval is %d(seconds). Server current time is %s.",
				ocppHeartbeatInterval, boot_notification_response.currentTime.to_rfc3339().c_str()));
        disconnect_requested = false;
        ocpp_reconnection_attempts = 0;
	}
	else
	{
		configPRINTF(("The EVSE has failed registration with the CSMS. Wait minimum %d seconds before contacting the CSMS again.",
				boot_notification_response.interval));
	}

    return;
}

void OCPP_OcppMessages_Cb(const std::string& message, ocpp::MessageDirection direction)
{
	configPRINTF(("-----> OCPP_OcppMessages Callback\r\n"));
	return;
}

void OCPP_CsEffectiveOperativeStatusChanged_Cb(const OperationalStatusEnum new_status)
{
	configPRINTF(("-----> OCPP_CsEffectiveOperativeStatusChanged Callback\r\n"));
	return;
}

void OCPP_EvseEffectiveOperativeStatusChanged_Cb(const int32_t evse_id, const OperationalStatusEnum new_status)
{
	configPRINTF(("-----> OCPP_EvseEffectiveOperativeStatusChanged Callback\r\n"));
	return;
}

std::string OCPP_GetCustomerInformation_Cb(const std::optional<CertificateHashDataType> customer_certificate,
                                            const std::optional<IdToken> id_token,
                                            const std::optional<ocpp::CiString<64>> customer_identifier)
{
	configPRINTF(("-----> OCPP_GetCustomerInformation Callback\r\n"));
	return "CustomerInfoNotImplemented";
}

void OCPP_ClearCustomerInfo_Cb(const std::optional<CertificateHashDataType> customer_certificate,
                                     const std::optional<IdToken> id_token,
                                     const std::optional<ocpp::CiString<64>> customer_identifier)
{
	configPRINTF(("-----> OCPP_ClearCustomerInfo Callback\r\n"));
	return;
}

void OCPP_AllConnectorsAvailable_Cb()
{
	configPRINTF(("-----> OCPP_AllConnectorsAvailable Callback\r\n"));
	return;
}

DataTransferResponse OCPP_DataTransfer_Cb(const DataTransferRequest& request)
{
	DataTransferResponse resp;
	configPRINTF(("-----> OCPP_DataTransfer Callback\r\n"));
	return resp;
}

void OCPP_TransactionEvent_Cb(const TransactionEventRequest& transaction_event)
{
    configPRINTF(("-----> OCPP_TransactionEvent Callback\r\n"));
    std::string reason = ocpp::v2::conversions::trigger_reason_enum_to_string(transaction_event.triggerReason);
    std::string event = ocpp::v2::conversions::transaction_event_enum_to_string(transaction_event.eventType);
    configPRINTF(("Received Transaction Event Request for event %s and reason %s ", event.c_str(), reason.c_str()));
	return;
}

void OCPP_TransactionEventResponse_Cb(const TransactionEventRequest& transaction_event,
		const TransactionEventResponse& transaction_event_response)
{
    configPRINTF(("-----> OCPP_TransactionEventResponse Callback\r\n"));
    std::string reason = ocpp::v2::conversions::trigger_reason_enum_to_string(transaction_event.triggerReason);
    std::string event = ocpp::v2::conversions::transaction_event_enum_to_string(transaction_event.eventType);
    configPRINTF(("Received Transaction Event Response for event %s and reason %s ", event.c_str(), reason.c_str()));

    if ((transaction_event.eventType == TransactionEventEnum::Updated) &&
			(transaction_event.triggerReason == TriggerReasonEnum::MeterValuePeriodic))
	{
		ocppTelemetryCounter++;
		configPRINTF(("Received confirmation of meter value update"));
        /* Trigger UI update */
        EVSE_UI_SetEvent(EVSE_UI_TelemetryData);
	}
	else if ((transaction_event.eventType == TransactionEventEnum::Ended) &&
			(transaction_event_response.totalCost.has_value()))
	{
		totalCost = transaction_event_response.totalCost.value();
		configPRINTF(("Total cost of charging: %.2f", totalCost));
	}

    // Process authorization response from CSMS:
	if (transaction_event_response.idTokenInfo.has_value())
	{
		AuthorizationStatusEnum validation_result = transaction_event_response.idTokenInfo.value().status;

		if ((validation_result == AuthorizationStatusEnum::Accepted))
		{
			configPRINTF(("Local authorization accepted by the CSMS"));
			is_EV_authorized = true;
		}
		else
		{
			configPRINTF(("Local authorization refused by the CSMS"));
			is_EV_authorized = false;
		}

        if (s_ocppAuthRespSemaphore)
        {
            xSemaphoreGive(s_ocppAuthRespSemaphore);
        }

        EVSE_OCPP_SetEvent(EV_AUTHORIZED_EVENT);
	}

	return;
}

// This is a ONE-WAY callback - CSMS sends cost to charge point
// No response is sent back to CSMS
void OCPP_SetRunningCost_Cb(const ocpp::RunningCost& running_cost, const uint32_t number_of_decimals,
        std::optional<std::string> currency_code)
{
	configPRINTF(("-----> OCPP_SetRunningCost Callback\r\n"));
	if ((running_cost.state == ocpp::RunningCostState::Charging) || (running_cost.state == ocpp::RunningCostState::Idle))
	{
		if (running_cost.timestamp.has_value())
		{
			configPRINTF(("Running cost for transaction %s, at time %s is %.2f",
					running_cost.transaction_id.c_str(), running_cost.timestamp.value().to_rfc3339().c_str(),
					running_cost.cost));
		}
		else
		{
			configPRINTF(("Running cost for transaction %s is %.2f",
					running_cost.transaction_id.c_str(), running_cost.cost));
		}
	}
	else
	{
		totalCost = running_cost.cost;
	}

	runningCost = running_cost.cost;

}

void OCPP_ConnectionStateChanged_Cb(const bool is_connected, const int configuration_slot,
                                     const NetworkConnectionProfile& network_connection_profile,
                                     const ocpp::OcppProtocolVersion ocpp_version)
{
    if (!is_connected)
    {
        prvEVSE_OCPP_SetConnectionState(EVSE_Ocpp_NotConnected);
        configPRINTF(("EVSE is not connected to the OCPP server. Websocket connection lost/retries exhausted."));
    }
    else
    {
        prvEVSE_OCPP_SetConnectionState(EVSE_Ocpp_Connected);
        configPRINTF(("EVSE has (re)connected to the OCPP server."));
        disconnect_requested = false;
        ocpp_reconnection_attempts = 0;
    }
}

SetDisplayMessageResponse OCPP_SetDisplayMessage_Cb(const std::vector<ocpp::DisplayMessage>& display_messages)
{
	SetDisplayMessageResponse response;
	bool valid_message = true;

	response.status = ocpp::v2::DisplayMessageStatusEnum::Accepted;

	for (const ocpp::DisplayMessage& message : display_messages)
	{
		ocpp::DateTime now;

		if ((message.timestamp_from.has_value() && (now < message.timestamp_from.value())) ||
				(message.timestamp_to.has_value() && (now > message.timestamp_to.value())))
		{
		    valid_message = false;
		}

		//todo: check format, language, etc

		if (valid_message)
		{
			configPRINTF(("Received valid message from ocpp server. Only messages with priority 0 are displayed."));
			if (message.priority.has_value())
			{
			    configPRINTF(("Message was received with priority: %d ", message.priority.value()));
			    if (message.priority.value() == ocpp::v2::MessagePriorityEnum::AlwaysFront)
			    {
				    EVSE_UI_SetEvent(EVSE_UI_OcppDisplayMessage);
				    ocppDisplayMessagesStr.push_back(message.message.message);
			    }
			}
			else
			{
				configPRINTF(("Message does not include priority, it will be ignored"));
			}
			configPRINTF(("Message content: %s", message.message.message.c_str()));
		}
		else
		{
			response.status = ocpp::v2::DisplayMessageStatusEnum::Rejected;
		}
	}

	return response;
}

void ocpp_run(void *arg)
{
	EventBits_t ocppBits = 0;
	/* OCPP requires EVSE and Connector numbering starting from 1 counting upwards. */
	int32_t EVSE_id = EVSE_ID;
	int32_t EVSE_connector = EVSE_CONNECTOR_ID;
	std::map<int32_t, int32_t> evse_connector_structure = {{EVSE_id, EVSE_connector}};

	/* Set network connection profile */
	EVSE_OCPP_SetNetworkConnectionProfile();

    /* Initialize device model */
    std::unique_ptr<module::device_model::ComposedDeviceModelStorage> device_model_storage =
            std::make_unique<module::device_model::ComposedDeviceModelStorage>(
                fs::path("device/model/database_path"), true, fs::path("device/model/database_migration_path"),
				fs::path("device/model/config_path"));
    std::shared_ptr<DeviceModel> evse_device_model = std::make_shared<DeviceModel>(std::move(device_model_storage));

    std::shared_ptr<DatabaseHandler> evse_database_handler = std::make_shared<DatabaseHandler>(fs::path("not/implemented")) ;//nullptr;

    std::shared_ptr<ocpp::MessageQueue<MessageType>> evse_message_queue = nullptr;

    std::string message_log_path = "/var/log/ocpp/messages.log";

    /* Set security configuration */
    ocpp::SecurityConfiguration security_config{
        .csms_ca_bundle = fs::path("/path/to/csms_ca_bundle.pem"),
        .mf_ca_bundle = fs::path("/path/to/mf_ca_bundle.pem"),
        .mo_ca_bundle = fs::path("/path/to/mo_ca_bundle.pem"),
        .v2g_ca_bundle = fs::path("/path/to/v2g_ca_bundle.pem"),
        .csms_leaf_cert_directory = fs::path("/path/to/csms_leaf_cert/"),
        .csms_leaf_key_directory = fs::path("/path/to/csms_leaf_key/"),
        .secc_leaf_cert_directory = fs::path("/path/to/secc_leaf_cert/"),
        .secc_leaf_key_directory = fs::path("/path/to/secc_leaf_key/"),
        .secc_leaf_cert_link = fs::path("/path/to/secc_leaf_cert_link.pem"),
        .secc_leaf_key_link = fs::path("/path/to/secc_leaf_key_link.pem"),
        .cpo_cert_chain_link = fs::path("/path/to/cpo_cert_chain_link.pem")
        //.private_key_password = std::optional<std::string>("your_password")
    };

    std::shared_ptr<ocpp::EvseSecurity> evse_security = std::make_shared<ocpp::EvseSecurityImpl>(security_config);

    /* Set callbacks */
    Callbacks callbacks {
    	.is_reset_allowed_callback = OCPP_IsResetAllowed_Cb,
    	.reset_callback = OCPP_Reset_Cb,
		.stop_transaction_callback = OCPP_StopTransaction_Cb,
		.pause_charging_callback = OCPP_PauseCharging_Cb,
		.cs_effective_operative_status_changed_callback = OCPP_CsEffectiveOperativeStatusChanged_Cb,
		.evse_effective_operative_status_changed_callback = OCPP_EvseEffectiveOperativeStatusChanged_Cb,
		.connector_effective_operative_status_changed_callback = OCPP_ConnectorEffectiveOperativeStatusChanged_Cb,
		.get_log_request_callback = OCPP_GetLogRequest_Cb,
		.unlock_connector_callback = OCPP_UnlockConnector_Cb,
		.remote_start_transaction_callback = OCPP_RemoteStartTransaction_Cb,
		.is_reservation_for_token_callback = OCPP_IsReservationForToken_Cb,
		.update_firmware_request_callback = OCPP_UpdateFirmwareRequest_Cb,
		.variable_changed_callback = OCPP_VariableChanged_Cb,
		.validate_network_profile_callback = OCPP_ValidateNetworkProfile_Cb,
		//.configure_network_connection_profile_callback = OCPP_ConfigureNetworkConnectionProfile_Cb,
		.time_sync_callback = OCPP_TimeSync_Cb,
		.ocpp_messages_callback = OCPP_OcppMessages_Cb,
		.security_event_callback = OCPP_SecurityEvent_Cb,
		.set_charging_profiles_callback = OCPP_SetChargingProfiles_Cb,
		.boot_notification_callback = OCPP_BootNotification_Cb,
		.get_customer_information_callback = OCPP_GetCustomerInformation_Cb,
        .clear_customer_information_callback = OCPP_ClearCustomerInfo_Cb,
        .all_connectors_unavailable_callback = OCPP_AllConnectorsAvailable_Cb,
		.data_transfer_callback = OCPP_DataTransfer_Cb,
		.transaction_event_callback = OCPP_TransactionEvent_Cb,
		.transaction_event_response_callback = OCPP_TransactionEventResponse_Cb,
		.connection_state_changed_callback = OCPP_ConnectionStateChanged_Cb,
		.set_display_message_callback = OCPP_SetDisplayMessage_Cb,
		.set_running_cost_callback = OCPP_SetRunningCost_Cb
    };

    /* Create charge point */
	auto charge_point = std::make_shared<ocpp::v2::ChargePoint>(
			evse_connector_structure,
			evse_device_model,
			evse_database_handler,
			evse_message_queue,
			message_log_path,
			evse_security,
			callbacks
	);

	/* Start the ocpp stack and connect to server */
	//charge_point->set_message_queue_resume_delay(std::chrono::seconds(EVSE_RESUME_DELAY_OCPP));
	bool start_connecting = false;
	charge_point->start(BootReasonEnum::PowerUp, start_connecting);
	vTaskDelay(pdMS_TO_TICKS(EVSE_DELAY_OCPP_START));
	charge_point->connect_websocket();

	/* Transaction handler for 1 EVSE */
	std::set<TxStartStopPoint> start_points = {TxStartStopPoint::EVConnected, TxStartStopPoint::Authorized};
	std::set<TxStartStopPoint> stop_points = {TxStartStopPoint::PowerPathClosed};
	TransactionHandler* transactionHandler = nullptr;

	if (EvseTimer::xEvseTimerEventGroup == nullptr)
	{
		EvseTimer::xEvseTimerEventGroup = xEventGroupCreate();
	}

	while(true)
	{
		ocppBits = xEventGroupWaitBits(s_evse_ocpp_event,    /* The event group being tested. */
				EVSE_OCPP_ALL_EVENTS,     /* The bits within the event group to wait for. */
				pdTRUE,                   /* Clear on exit */
				pdFALSE,                  /* Don't wait for all bits, either bit will do. */
				pdMS_TO_TICKS(1000));     /* Wait 1s */

		if (s_ocppConnectionState == EVSE_Ocpp_Connected)
		{
			if ((ocppBits & EV_CONNECTED_EVENT) == EV_CONNECTED_EVENT)
			{
				char session_buff[EVSE_OCPP_SESSION_ID_MAX] = {0,};
				generate_session_id(session_buff, sizeof(session_buff));
				std::string session_id = std::string(session_buff);

				if (transactionHandler == nullptr)
				{
					transactionHandler = new TransactionHandler{1, start_points, stop_points};
					if (transactionHandler != nullptr)
					{
						configPRINTF(("New TransactionHandler created for this session"));

						ocpp::DateTime now_timestamp;
						auto transaction_data = std::make_shared<TransactionData>(
								EVSE_connector,
								session_id,
								now_timestamp,
								ocpp::v2::TriggerReasonEnum::CablePluggedIn,
								ocpp::v2::ChargingStateEnum::EVConnected
						);

						transactionHandler->add_transaction_data(EVSE_id, transaction_data);
						auto tx_event_effect = transactionHandler->submit_event(EVSE_id, module::TxEvent::EV_CONNECTED);

						prvEVSE_OCPP_InitCurrentPowerData();
						process_tx_event_effect(EVSE_id, tx_event_effect, charge_point, *transactionHandler);

						charge_point->on_session_started(EVSE_id, EVSE_connector);
						EV_plugged_in = true;

						configPRINTF((info("EV connected event processed successfully. Notified CSMS")));
					}
					else
					{
						configPRINTF(("Failed to create TransactionHandler for this session"));
					}
				}
		    }

			if ((ocppBits & EV_AUTH_NEEDED_EVENT) == EV_AUTH_NEEDED_EVENT) // the auth token is available
			{
				if (EV_plugged_in == true)
				{
					auto authorized_id_token = prvEVSE_OCPP_GetLocalAuthorizationToken();
					if (authorized_id_token.has_value())
					{
						// Send local authorization to CSMS for validation (sends TransactionEvent(Updated) with id_token
						// and trigger reason ocpp::v2::TriggerReasonEnum::Authorized
						charge_point->on_authorized(EVSE_id, EVSE_connector, authorized_id_token.value());
						last_auth_time = EVSE_Connectivity_GetUnixTime();
						configPRINTF((info("AUTH needed event processed successfully. Notified CSMS")));
					}
					else
					{
						configPRINTF(("WARNING: EV auth needed event received but authorized token is empty"));
					}
				}
				else
				{
					configPRINTF(("WARNING: EV auth needed event received but state is [EV not plugged in]"));
				}
			}

			if ((ocppBits & EV_AUTHORIZED_EVENT) == EV_AUTHORIZED_EVENT)
			{
				if (transactionHandler != nullptr)
				{
					auto tx_event = module::TxEvent::DEAUTHORIZED;
					if (is_EV_authorized == true)
					{
						tx_event = module::TxEvent::AUTHORIZED;
					}
					transactionHandler->submit_event(EVSE_id, tx_event);

				}
				else
				{
					configPRINTF(("WARNING: EV authorized event received but state is [no transaction handler]"));
				}
			}

			if ((ocppBits & EVSE_CHARGING_STARTED_EVENT) == EVSE_CHARGING_STARTED_EVENT)
			{
				if ((EV_plugged_in == true) && (transactionHandler != nullptr))
				{
					process_charging_started(EVSE_id, EVSE_connector, false, charge_point, *transactionHandler);
					session_started = true;
					configPRINTF((info("Charging started [local] event processed successfully. Notified CSMS")));
				}
				else
				{
					configPRINTF(("WARNING: Charging started event received but state is [EV not plugged in | no transaction handler]"));
				}
			}

			if ((ocppBits & EVSE_CHARGING_START_REMOTE_EVENT) == EVSE_CHARGING_START_REMOTE_EVENT)
			{
				if (transactionHandler != nullptr)
				{
					EVSE_ChargingProtocol_StartCharging();

					process_charging_started(EVSE_id, EVSE_connector, true, charge_point, *transactionHandler);
					configPRINTF((info("Charging started [remote] event processed successfully. Notified CSMS")));
				}
				else
				{
					configPRINTF(("WARNING: Charging started [remote] event but no transaction handler exists"));
				}
			}

			if ((ocppBits & EVSE_TELEMETRY_READY_EVENT) == EVSE_TELEMETRY_READY_EVENT)
			{
				if ((EV_plugged_in == true) && (is_EV_authorized == true))
				{
					EVSE_VehicleData_to_OcppData();

					MeterValue meter_value = module::conversions::to_ocpp_meter_value(
							ocppCurrentPowerData, ocpp::v2::ReadingContextEnum::Sample_Periodic,
							std::nullopt);
					//if (SoC updated) (State of Charge)
					// 	auto sampled_soc_value = conversions::to_ocpp_sampled_value(
					//            	ocpp::v2::ReadingContextEnum::Sample_Periodic, ocpp::v2::MeasurandEnum::SoC, "Percent",
					//            	std::nullopt, ocpp::v2::LocationEnum::EV);
					//	sampled_soc_value.value = this->evse_soc_map[evse_id].value();
					//	meter_value.sampledValue.push_back(sampled_soc_value);
					charge_point->on_meter_value(EVSE_id, meter_value);
					configPRINTF((info("Meter data ready event processed successfully. Sent event to the ocpp stack.")));
				}
				else
				{
					configPRINTF(("Telemetry ready event received but state is [EV not plugged in | no session started]"));
				}
			}

			if ((ocppBits & EVSE_CHARGING_STOPPED_EVENT) == EVSE_CHARGING_STOPPED_EVENT)
			{
				if ((EV_plugged_in == true) && (session_started == true) && (transactionHandler != nullptr))
				{
					process_charging_ended(EVSE_id, EVSE_connector, charge_point, *transactionHandler, false);
					session_started = false;
					configPRINTF((info("Charging stopped [local] event processed successfully. Notified CSMS")));
				}
				else
				{
					configPRINTF(("WARNING: Charging stop event received but state is [EV not plugged in | no session started | no transaction handler]"));
				}
			}

			if ((ocppBits & EVSE_CHARGING_STOP_REMOTE_EVENT) == EVSE_CHARGING_STOP_REMOTE_EVENT)
			{
				if (transactionHandler != nullptr)
				{
					EVSE_ChargingProtocol_StopCharging();

					process_charging_ended(EVSE_id, EVSE_connector, charge_point, *transactionHandler, true);
					configPRINTF((info("Charging stopped [remote] event processed successfully. Notified CSMS")));
				}
				else
				{
					configPRINTF(("WARNING: Charging stop remote event received but state is [no transaction handler]"));
				}
			}

			if ((ocppBits & EV_DISCONNECTED_EVENT) == EV_DISCONNECTED_EVENT)
			{
				if ((EV_plugged_in == true) && (transactionHandler != nullptr))
				{
					auto transaction_data = transactionHandler->get_transaction_data(EVSE_id);
					if (transaction_data != nullptr) {
						transaction_data->charging_state = ocpp::v2::ChargingStateEnum::Idle;
						transaction_data->stop_reason = ocpp::v2::ReasonEnum::EVDisconnected;
						transaction_data->trigger_reason = ocpp::v2::TriggerReasonEnum::EVCommunicationLost;
					}

					transactionHandler->submit_event(EVSE_id, TxEvent::DEAUTHORIZED);
					auto tx_event_effect = transactionHandler->submit_event(EVSE_id, TxEvent::EV_DISCONNECTED);
					process_tx_event_effect(EVSE_id, tx_event_effect, charge_point, *transactionHandler);

					charge_point->on_session_finished(EVSE_id, EVSE_connector);

					delete transactionHandler;
					transactionHandler = nullptr;

					reset_session_state();

					configPRINTF((info("EV disconnected event processed successfully. Notified CSMS")));
				}
				else
				{
					if (transactionHandler == nullptr)
					{
						reset_session_state();
					}
					configPRINTF(("WARNING: EV disconnect event received but state is [EV not plugged in | no transaction handler]"));
				}
			}

			if ((ocppBits & EVSE_NETWORK_DOWN_EVENT) == EVSE_NETWORK_DOWN_EVENT)
			{
				if (!disconnect_requested)
				{
					configPRINTF((info("Network disconnect detected. Notifying OCPP stack.")));
					/* Disconnect the websocket and rely on reconnection mechanisms */
					charge_point->on_network_disconnected(ocppConnectionProfile.ocppInterface);
					disconnect_requested = true;
				}
			}
		}
		else
		{
			if (((ocppBits & EVSE_RECONNECT_EVENT) == EVSE_RECONNECT_EVENT)
					&& (ocpp_reconnection_attempts <= EVSE_OCPP_MAX_RECONNECTS))
			{
				if (ocpp_reconnection_attempts == EVSE_OCPP_MAX_RECONNECTS)
				{
					configPRINTF(("Stopping OCPP communication. Subsequent charging sessions will not be authorized!"));
					charge_point->stop();
					break;
				}

				configPRINTF((info("Reconnect event received. Connecting websocket")));
				charge_point->connect_websocket(1);
				ocpp_reconnection_attempts++;
			}

			configPRINTF(("EVSE disconnected from the CSMS, or not yet registered..."));
		}
	}

	while (1)
	{
		vTaskDelay(1000);
	}
}

void EVSE_OCPP_Init(void)
{
	static bool first_time = false;

	if (first_time == false)
	{
		s_evse_ocpp_event = xEventGroupCreate();

		if (s_evse_ocpp_event == NULL)
		{
			configPRINTF((error("%s Failed to create event"), __FUNCTION__));
			while (1)
				;
		}

		s_ocppAuthRespSemaphore = xSemaphoreCreateBinary();

		if (s_ocppAuthRespSemaphore == NULL)
		{
			configPRINTF((error("%s Failed to create auth semaphore"), __FUNCTION__));
			while (1)
				;
		}

		if (xTaskCreate(ocpp_run, "OCPP", APP_EVSE_OCPP_STACK_SIZE, NULL, APP_EVSE_OCPP_PRIORITY, NULL) != pdPASS)
		{
			configPRINTF((error(" %s Failed to create task "), __FUNCTION__));
			while (1)
				;
		}

		first_time = true;
	}

}

int swprintf(wchar_t *wcsbuffer, size_t n, const wchar_t *format, ...)
{
	return 0;
}
#endif
