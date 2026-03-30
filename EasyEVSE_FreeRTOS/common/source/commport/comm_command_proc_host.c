/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "message_builder.h"
#include "message_handler.h"
#include "message_raw.h"
#include "message_transport_pal.h"

#include "message_transport_uart.h"
#include "message_builder_metrology.h"
#include "message_pool.h"

#include "comm_command_proc_host.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"

#include "EVSE_Utils.h"
#include "EVSE_Metering.h"

#define DEVICE_TYPE       DEVICE_TYPE_DEVICE_HOST_CONTROLLER

/* Average packet size is around ~55 bytes. On a baudrate of 115200 this means arround ~5ms
Double it to accomodate almost all messages. */
#define MS_TO_WAIT_FOR_RESPONSE_DATA_TRANSFER         10
/*Average response time for the EVSE-Emeter*/
#define MS_TO_WAIT_FOR_PROCESS                        5
/* Metrology itself is around 150 bytes. this means almost 100 bytes more. So let's add 10ms to be safe */
#define METROLOGY_ALL_EXTRA_TIMEOUT_MS  10
#define MS_TO_WAIT_FOR_RETRANSMISSION   (MS_TO_WAIT_FOR_RESPONSE_DATA_TRANSFER + MS_TO_WAIT_FOR_PROCESS)
#define MAX_RETRANSMISSION_RETRIES      3

typedef struct _mirror_configuration
{
    uint16_t pp_state;
    uint16_t cp_state;
    uint16_t gfci_state;
    uint32_t sw_version_major;
    uint32_t sw_version_minor;
    uint32_t sw_version_hotfix;
    uint32_t hw_version;
    uint16_t pwm;
    uint16_t pwm_ms;
    uint8_t relay_state;
    uint32_t cp_value;
    metrology_values_t current;
    metrology_values_t voltage;
    metrology_values_t active_power;
    metrology_values_t reactive_power;
    metrology_values_t apparent_power;
    power_direction_t power_direction;
    uint32_t wh_t;
    uint32_t varh_t;
} mirror_configuration_t;

typedef struct _lpuart_message_format
{
    uint32_t size;
    char *buffer_addr;
} lpuart_message_format_t;

static mirror_configuration_t mirror_config = {0};

static lpuart_state_t lpuart_state     = LPUART_STATE_IDLE;
static uint32_t iso15118_tick_delay_ms = 0;
static bool s_init                     = false;

static EventGroupHandle_t s_evse_uart_event = NULL;
static EventGroupHandle_t s_evse_emeter_event = NULL;
static SemaphoreHandle_t s_uart_sem = NULL;

static relay_state_t relay_state = RELAY_STATE_RELAY_OPEN;
static uint16_t duty_cycle = 0;
static uint16_t duty_cycle_ms = 0;
static volatile uint8_t req_flags = 0x00;
static uint32_t expected_msg_id_respond = 0;

char print_metrology_data[1024];

/**
 * @brief Sends a message with automatic retry on timeout
 * 
 * @param msg_crc Pointer to the message to send
 * @param max_retries Maximum number of retransmission attempts
 * @param timeout_ms Timeout in milliseconds to wait for response
 * @return MessageStatus indicating success or failure
 */
static MessageStatus Host_SendMessageWithRetry(e_meter_message_crc_t *msg_crc, uint8_t max_retries, uint32_t timeout_ms)
{
    MessageStatus status = MessageHandler_Success;
    uint8_t retries = 0;
    bool response_received = false;

    if (msg_crc == NULL)
    {
        return MessageHandler_InvalidParameter;
    }

    // Acquire UART mutex
    if (xSemaphoreTake(s_uart_sem, portMAX_DELAY) != pdTRUE)
    {
        configPRINTF((error("Failed to acquire UART semaphore")));
        return MessageHandler_Fail;
    }

    expected_msg_id_respond = msg_crc->message.msg_id;
 
    /* We are the sole owners of the UART. Clear the RCV bit */
    xEventGroupClearBits(s_evse_emeter_event, EVSE_EMETER_RESPONSE_RECEIVED);

    // Send initial message
    MessageTransport_Send(msg_crc);

    // Retry loop
    while (retries < max_retries)
    {
        EventBits_t uxBits = xEventGroupWaitBits(
            s_evse_emeter_event,
            EVSE_EMETER_RESPONSE_RECEIVED,
            pdTRUE,
            pdFALSE,
            pdMS_TO_TICKS(timeout_ms));

        if ((uxBits & EVSE_EMETER_RESPONSE_RECEIVED) == EVSE_EMETER_RESPONSE_RECEIVED)
        {
            response_received = true;
            break;
        }

        MessageTransport_Send(msg_crc);
        retries++;     
    }

    // Release UART mutex
    xSemaphoreGive(s_uart_sem);

    if (!response_received)
    {
        if (retries == max_retries)
        {
            configPRINTF((error("Max retransmission retries exceeded for message cmd ID: 0x%x"), msg_crc->message.msg_id));           
        }
        status = MessageHandler_Fail;
    }

    return status;
}

static void Convert_Status_To_MessageStatus(status_t sdk_status, MessageStatus *status)
{
    switch (sdk_status)
    {
        case kStatus_Success:
            *status = MessageHandler_Success;
            break;
        case kStatus_Fail:
            *status = MessageHandler_Fail;
            break;
        case kStatus_InvalidArgument:
        case kStatus_NoData:
        case kStatus_OutOfRange:
            *status = MessageHandler_InvalidParameter;
            break;
        default:
            *status = MessageHandler_Fail;
    }
}

MessageStatus metrology_values_copy_present(metrology_values_t *dst, const metrology_values_t *src)
{
    if ((dst == NULL) || (src == NULL))
    {
        return MessageHandler_InvalidParameter;
    }

    if(src->has_data_phase1) dst->data_phase1 = src->data_phase1;
    if(src->has_data_phase2) dst->data_phase2 = src->data_phase2;
    if(src->has_data_phase3) dst->data_phase3 = src->data_phase3;

    return MessageHandler_Success;
}

MessageStatus Host_AsyncSend(command_id_t cmd_id)
{
    MessageStatus message_status = MessageHandler_Success;

    e_meter_message_crc_t *msg_crc = MessagePool_Acquire();
    if (msg_crc == NULL)
    {
        configPRINTF(("Failed to acquire message pool"));
        return MessageHandler_Fail;
    }

    uint32_t timeout = MS_TO_WAIT_FOR_RETRANSMISSION;

    request_t *request  = &msg_crc->message.request;
    request->command_id = cmd_id;

    switch (cmd_id)
    {
        case COMMAND_ID_GET_METROLOGY_ALL:
        /* The transmission for this message is around */
            timeout += METROLOGY_ALL_EXTRA_TIMEOUT_MS;
        case COMMAND_ID_GET_PRESENT_VOLTAGE:
        case COMMAND_ID_GET_PRESENT_CURRENT:
        case COMMAND_ID_GET_PRESENT_POWER:
        case COMMAND_ID_GET_PRESENT_ACTIVE_POWER:
        case COMMAND_ID_GET_PRESENT_REACTIVE_POWER:
        case COMMAND_ID_GET_PRESENT_APPARENT_POWER:
        case COMMAND_ID_GET_POWER_DIRECTION:
        case COMMAND_ID_GET_ENERGY_INDEX:
        case COMMAND_ID_GET_METROLOGY_LAST:
        case COMMAND_ID_GET_PP_STATE:
        case COMMAND_ID_GET_CP_STATE:
        case COMMAND_ID_GET_GFCI_STATE:
        case COMMAND_ID_GET_CP_VALUE:
        case COMMAND_ID_GET_PWM_MS:
        case COMMAND_ID_GET_RELAY_STATE:
        case COMMAND_ID_GET_SW_VERSION:
        case COMMAND_ID_GET_HW_VERSION:
        case COMMAND_ID_GET_TIMESTAMP:
        case COMMAND_ID_GET_PWM:
        case COMMAND_ID_GET_CHARGE_STATE:
        case COMMAND_ID_GET_ALLOW_CHARGE:
        case COMMAND_ID_GET_FAULT_VALUE:
        case COMMAND_ID_GET_POWER_HIGH_LIMIT:
        case COMMAND_ID_GET_CURRENT_HIGH_LIMIT:
        case COMMAND_ID_GET_VOLTAGE_HIGH_LIMIT:
        case COMMAND_ID_GET_VOLTAGE_LOW_LIMIT:
        case COMMAND_ID_GET_NOMINAL_CURRENT:
        case COMMAND_ID_GET_FAULT_DETAILED_CLASS_B:
        case COMMAND_ID_GET_FAULT_DETAILED_APP:
        case COMMAND_ID_GET_CONTROL_MODE:
        case COMMAND_ID_GET_ENABLE_ADVERTISE:
        case COMMAND_ID_GET_ADVERTISE_FREQ:
        case COMMAND_ID_GET_TEMPERATURE:
            request->which_request = REQUEST_NO_DATA_TAG;
            break;
        case COMMAND_ID_SET_PWM:
            request->which_request = REQUEST_PWM_VALUE_TAG;
            request->request.pwm_value.pwm_value = (float)duty_cycle;
            break;
        case COMMAND_ID_SET_PWM_MS:
            request->which_request = REQUEST_PWM_MS_VALUE_TAG;
            configPRINTF(("Host_AsyncSend SET_PWM_MS: %u ms", duty_cycle_ms));
            request->request.pwm_ms_value.pwm_value = (uint32_t)duty_cycle_ms;
            break;
        case COMMAND_ID_SET_RELAY_STATE:
            request->which_request = REQUEST_RELAY_STATE_TAG;
            request->request.relay_state = (relay_state_t)relay_state;
            break;
        case COMMAND_ID_SET_TIMESTAMP:
            request->which_request = REQUEST_UNIX_TIME_TAG;
            break;
        case COMMAND_ID_SET_ALLOW_CHARGE: /* This message is ignored if EMETER is in fault state */
            request->which_request = REQUEST_ALLOW_CHARGE_TAG;
            break;
        case COMMAND_ID_SET_CHARGE_STATE: /* Used in Hybrid_Charger or FullHost_Charger control mode. This message is
                                             ignored if EMETER is in fault state */
            request->which_request = REQUEST_CHARGE_STATE_TAG;
            break;
        case COMMAND_ID_SET_POWER_HIGH_LIMIT:
            request->which_request = REQUEST_POWER_HIGH_LIMIT_TAG;
            break;
        case COMMAND_ID_SET_CURRENT_HIGH_LIMIT:
            request->which_request = REQUEST_CURRENT_HIGH_LIMIT_TAG;
            break;
        case COMMAND_ID_SET_VOLTAGE_HIGH_LIMIT:
            request->which_request = REQUEST_VOLTAGE_HIGH_LIMIT_TAG;
            break;
        case COMMAND_ID_SET_VOLTAGE_LOW_LIMIT:
            request->which_request = REQUEST_VOLTAGE_LOW_LIMIT_TAG;
            break;
        case COMMAND_ID_SET_NOMINAL_CURRENT:
            request->which_request = REQUEST_NOMINAL_CURRENT_TAG;
            break;
        case COMMAND_ID_SET_CLEAR_FAULTS: /* This command is mostly for demo purposes. Clearing the faults should be
                                             done in a supervise manner */
            request->which_request = REQUEST_NO_DATA_TAG;
            break;
        case COMMAND_ID_SET_CONTROL_MODE:
            message_status = MessageHandler_NotImplemented;
            break;
        case COMMAND_ID_SET_ENABLE_ADVERTISE:
            message_status = MessageHandler_NotImplemented;
            break;
        case COMMAND_ID_SET_ADVERTISE_FREQ:
            request->which_request = REQUEST_ADVERTISE_FREQ_TAG;
            break;
        case COMMAND_ID_RESET:
            break;
        case COMMAND_ID_FIRMWARE_DOWNLOAD:
            message_status = MessageHandler_NotImplemented;
            break;
        case COMMAND_ID_FIRMWARE_UPDATE:
            request->which_request = REQUEST_FIRMWARE_UPDATE_TAG;
            break;
        default:
            message_status = MessageHandler_Fail;
            break;
    }

    if (MessageHandler_Success == message_status)
    {
        uint8_t retries = 0;
        
        MessageBuilder_PrepareMessage(msg_crc, PAYLOAD_HOST_REQUEST, DEVICE_TYPE);

        message_status = Host_SendMessageWithRetry(msg_crc, MAX_RETRANSMISSION_RETRIES, timeout);
    }

    MessagePool_Release(msg_crc);
    return message_status;
}

static MessageStatus Host_Request_Callback(const uint16_t msg_id, const request_t *req, respond_t *respond)
{
    MessageStatus status = MessageHandler_Success;

    if ((respond == NULL) || (req == NULL))
    {
        return MessageHandler_InvalidParameter;
    }
    respond->which_respond = RESPOND_NO_DATA_TAG;

    switch (req->command_id)
    {
        case COMMAND_ID_SET_METROLOGY_ALL:
            // TODO Nothing to do yet, meter does not send this command in current code base
            break;
        case COMMAND_ID_SET_CP_STATE:
            configPRINTF(("CPstate req: %d", req->request.cp_state));
            mirror_config.cp_state = (uint16_t)req->request.cp_state;
            req_flags |= CP_STATE_REQ_MASK;
            break;
        case COMMAND_ID_SET_PP_STATE:
            configPRINTF(("PPstate req: %d", req->request.pp_state));
            break;
        case COMMAND_ID_SET_CAR_PLUGGED_IN:
            configPRINTF(("EV plugged in"));
            break;
        case COMMAND_ID_SET_CAR_REMOVED:
            // TODO Nothing to do yet, meter does not send this command in current code base
            break;
        case COMMAND_ID_SET_CAR_POWER_REQUEST:
            configPRINTF(("CAR_POWER_REQUEST (state C)"));
            break;
        case COMMAND_ID_SET_HEARTBEAT:
        {
            const heartbeat_t *heartbeat_req = &req->request.heartbeat;
            if (heartbeat_req->has_fault_value)
            {
                configPRINTF(("heartbeat app_fault: %d", heartbeat_req->fault_value.app_fault));
                configPRINTF(("heartbeat class_b_fault: %d", heartbeat_req->fault_value.class_b_fault));
            }
            else
            {
//                configPRINTF(("Heartbeat received - no fault"));
            }
            break;
        }
        case COMMAND_ID_SET_ENTER_LPM:
            // TODO Nothing to do yet, meter does not send this command in current code base
            break;
        case COMMAND_ID_SET_GFCI:
        {
            const gfci_state_t *GPCIstate_req = &req->request.gfci_state;
            configPRINTF(("GPCIstate_req: %d", GPCIstate_req->gfci_state));
            break;
        }
        case COMMAND_ID_SET_FAULT_VALUE:
        {
            const fault_value_t *fault_value_req = &req->request.fault_value;
            configPRINTF(("fault_value_req app_fault: %d", fault_value_req->app_fault));
            configPRINTF(("fault_value_req class_b_fault: %d", fault_value_req->class_b_fault));
            break;
        }
        case COMMAND_ID_SET_BOOT_COMPLET:
            configPRINTF(("EMETER booted"));
            break;
        case COMMAND_ID_SET_EMERGENCY_PRESSED:
            configPRINTF(("Emergency button pressed"));
            break;
        default:
            // Handle unknown command
            status = MessageHandler_NotImplemented;
            break;
    }

    return status;
}

static MessageStatus Host_Respond_Callback(const uint16_t msg_id, const uint16_t correlation_id, const respond_t *rsp)
{
    MessageStatus status = MessageHandler_Success;

    if (rsp == NULL)
    {
        return MessageHandler_InvalidParameter;
    }
    else if (expected_msg_id_respond == INVALID_MESSAGE_ID)
    {
       // configPRINTF((error("No Message Expected. Might timeout"));
        return MessageHandler_InvalidParameter;
    }

    expected_msg_id_respond = INVALID_MESSAGE_ID;

    switch (rsp->command_id)
    {
        case COMMAND_ID_GET_METROLOGY_ALL:
        case COMMAND_ID_GET_PRESENT_VOLTAGE:
        case COMMAND_ID_GET_PRESENT_CURRENT:
        case COMMAND_ID_GET_PRESENT_POWER:
        case COMMAND_ID_GET_PRESENT_ACTIVE_POWER:
        case COMMAND_ID_GET_PRESENT_REACTIVE_POWER:
        case COMMAND_ID_GET_PRESENT_APPARENT_POWER:
        case COMMAND_ID_GET_POWER_DIRECTION:
        case COMMAND_ID_GET_ENERGY_INDEX:
            if (rsp->which_respond != RESPOND_METROLOGY_TAG)
            {
                configPRINTF(("Expected RESPOND_METROLOGY_TAG, found %d", rsp->which_respond));
                break;
            }

            const metrology_schema_t *metrology = &rsp->respond.metrology;
            if(metrology->has_voltage && metrology->voltage.has_data)
            {
                metrology_values_copy_present(&mirror_config.voltage, &metrology->voltage.data);
            }
            if(metrology->has_current && metrology->current.has_data)
            {
                metrology_values_copy_present(&mirror_config.current, &metrology->current.data);
            }
            if(metrology->has_power && metrology->power.has_active_power && metrology->power.active_power.has_data)
            {
                metrology_values_copy_present(&mirror_config.active_power, &metrology->power.active_power.data);
            }
            if(metrology->has_power && metrology->power.has_reactive_power && metrology->power.reactive_power.has_data)
            {
                metrology_values_copy_present(&mirror_config.reactive_power, &metrology->power.reactive_power.data);
            }
            if(metrology->has_power && metrology->power.has_apparent_power && metrology->power.apparent_power.has_data)
            {
                metrology_values_copy_present(&mirror_config.apparent_power, &metrology->power.apparent_power.data);
            }
            if(metrology->has_power_direction)
            {
                mirror_config.power_direction = metrology->power_direction;
            }
            if(metrology->has_energy_meter_index)
            {
                mirror_config.varh_t = metrology->energy_meter_index.varh_t;
                mirror_config.wh_t = metrology->energy_meter_index.wh_t;
            }

            if (rsp->command_id == COMMAND_ID_GET_METROLOGY_ALL)
            {
                EVSE_Set_MeterEvent(METER_REFRESH_DATA);
            }
            break;
        case COMMAND_ID_GET_METROLOGY_LAST:
            break;
        case COMMAND_ID_GET_PP_STATE:
            mirror_config.pp_state = (uint16_t)(rsp->respond.pp_state);
            break;
        case COMMAND_ID_GET_CP_STATE:
            mirror_config.cp_state = (uint16_t)(rsp->respond.cp_state);
            break;
        case COMMAND_ID_GET_GFCI_STATE:
            mirror_config.gfci_state = (uint16_t)(rsp->respond.gfci_state.gfci_state);
            break;
        case COMMAND_ID_GET_CP_VALUE:
            mirror_config.cp_value = (uint32_t)(rsp->respond.cp_value.cp_value);
            break;
        case COMMAND_ID_GET_PWM_MS:
            mirror_config.pwm_ms = (uint16_t)(rsp->respond.pwm_ms_value.pwm_value);
            break;
        case COMMAND_ID_GET_PWM:
            mirror_config.pwm = (uint16_t)(rsp->respond.pwm_value.pwm_value);
            break;
        case COMMAND_ID_GET_RELAY_STATE:
            mirror_config.relay_state = (uint8_t)(rsp->respond.relay_state);
            break;
        case COMMAND_ID_GET_SW_VERSION:
            const sw_version_t *sw_version = &rsp->respond.sw_version;
            mirror_config.sw_version_major = sw_version->major;
            mirror_config.sw_version_minor = sw_version->minor;
            mirror_config.sw_version_hotfix = sw_version->hotfix;
            break;
        case COMMAND_ID_GET_HW_VERSION:
            mirror_config.hw_version = rsp->respond.hw_version.version;
            break;
        case COMMAND_ID_GET_TIMESTAMP:
            break;
        case COMMAND_ID_GET_CHARGE_STATE:
            break;
        case COMMAND_ID_GET_ALLOW_CHARGE:
            break;
        case COMMAND_ID_GET_FAULT_VALUE:
            break;
        case COMMAND_ID_GET_POWER_HIGH_LIMIT:
            break;
        case COMMAND_ID_GET_CURRENT_HIGH_LIMIT:
            break;
        case COMMAND_ID_GET_VOLTAGE_HIGH_LIMIT:
            break;
        case COMMAND_ID_GET_VOLTAGE_LOW_LIMIT:
            break;
        case COMMAND_ID_GET_NOMINAL_CURRENT:
            break;
        case COMMAND_ID_GET_FAULT_DETAILED_CLASS_B:
            break;
        case COMMAND_ID_GET_FAULT_DETAILED_APP:
            break;
        case COMMAND_ID_GET_CONTROL_MODE:
            break;
        case COMMAND_ID_GET_ENABLE_ADVERTISE:
            break;
        case COMMAND_ID_GET_ADVERTISE_FREQ:
            break;
        case COMMAND_ID_GET_TEMPERATURE:
            break;
        case COMMAND_ID_SET_PWM:
        case COMMAND_ID_SET_PWM_MS:
        case COMMAND_ID_SET_RELAY_STATE:
        case COMMAND_ID_SET_TIMESTAMP:
        case COMMAND_ID_SET_ALLOW_CHARGE:
        case COMMAND_ID_SET_CHARGE_STATE:
        case COMMAND_ID_SET_POWER_HIGH_LIMIT:
        case COMMAND_ID_SET_CURRENT_HIGH_LIMIT:
        case COMMAND_ID_SET_VOLTAGE_HIGH_LIMIT:
        case COMMAND_ID_SET_VOLTAGE_LOW_LIMIT:
        case COMMAND_ID_SET_NOMINAL_CURRENT:
        case COMMAND_ID_SET_CLEAR_FAULTS:
        case COMMAND_ID_SET_CONTROL_MODE:
        case COMMAND_ID_SET_ENABLE_ADVERTISE:
        case COMMAND_ID_SET_ADVERTISE_FREQ:
            if(rsp->which_respond != RESPOND_NO_DATA_TAG)
            {
//                configPRINTF(("Response for command code: %d. Expected RESPOND_NO_DATA_TAG, received %d.", rsp->command_id, rsp->which_respond));
            }
            break;
        case COMMAND_ID_RESET:
            // TODO Nothing to do yet, meter does not send this command in current code base
            break;
        case COMMAND_ID_FIRMWARE_DOWNLOAD:
            // TODO Nothing to do yet, meter does not send this command in current code base
            break;
        case COMMAND_ID_FIRMWARE_UPDATE:
            // TODO Nothing to do yet, meter does not send this command in current code base
            break;
         default:
            // Handle unknown command
            status = MessageHandler_NotImplemented;
            break;
    }

    EVSE_EMETERComm_SetEvent(EVSE_EMETER_RESPONSE_RECEIVED);

    /* can handle retransmision */
    return status;
}

void Host_Communication_Init()
{
    MessageHandlerCallbacks communication_callback = {
        .on_request = Host_Request_Callback,
        .on_respond = Host_Respond_Callback,
    };
    MessageHandler_Init(&communication_callback, DEVICE_TYPE);
    Uart_ModuleInit();
}

static void EVSE_SetEvent(EventGroupHandle_t event_group_handler, const EventBits_t event)
{
    BaseType_t xResult                  = pdFAIL;

    if (event_group_handler == NULL)
    {
        return;
    }

    if (xPortIsInsideInterrupt())
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xResult = xEventGroupSetBitsFromISR(event_group_handler, event, &xHigherPriorityTaskWoken);
        if (xResult != pdFAIL)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    else
    {    
        /* Task context: use the standard API. */
        xResult = (xEventGroupSetBits(event_group_handler, event) != 0) ? pdPASS : pdFAIL;
    }
}

void EVSE_UARTComm_SetEvent(uart_comm_events_t event)
{
    EVSE_SetEvent(s_evse_uart_event, (EventBits_t)event);
}

void EVSE_EMETERComm_SetEvent(emeter_comm_events_t event)
{
    EVSE_SetEvent(s_evse_emeter_event, (EventBits_t)event);
}

static void SIGBRD_InitData()
{
    mirror_config.sw_version_major = INVALID_SIGBOARD_SW_VERSION;
    mirror_config.sw_version_minor = INVALID_SIGBOARD_SW_VERSION;
    mirror_config.sw_version_hotfix = INVALID_SIGBOARD_SW_VERSION;
    mirror_config.hw_version = INVALID_SIGBOARD_HW_VERSION;
    // Ini­tialize other mirror_config fields to default/invalid values
}

static void SIGBRD_UART_comm_task(void *pvParameters)
{
    SIGBRD_InitData();
    while (1)
    {
        EventBits_t uxBits = 0;
        uxBits             = xEventGroupWaitBits(s_evse_uart_event,    /* The event group being tested. */
                                                 EVSE_UART_ALL_EVENTS, /* The bits within the event group to wait for. */
                                                 pdTRUE, pdFALSE, /* Don't wait for all bits, either bit will do. */
                                                 portMAX_DELAY);  /* Wait until event */

        if ((uxBits & EVSE_UART_MESSAGE_RECEIVED) == EVSE_UART_MESSAGE_RECEIVED)
        {
            Comm_Process();
        }
    }

    /* Shouldn't reach this point */
    vTaskDelete(NULL);
}

static void reset_sigbrd_state_machine()
{
    lpuart_state        = LPUART_STATE_IDLE;
}

bool SIGBRD_PushNotificationReceived(uint8_t mask, uint16_t *value)
{
    if(value == NULL)
    {
        return false;
    }

    if((req_flags & mask) == mask) // if we received a specific request
    {
        req_flags &= ~mask; // delete flag for this request
        switch(mask)
        {
            case CP_STATE_REQ_MASK:
                *value = mirror_config.cp_state;
                break;
            case PP_STATE_REQ_MASK:
                break;
            case EV_PLUGGED_REQ_MASK:
                break;
            case EV_REMOVED_REQ_MASK:
                break;
            case EV_PWR_REQ_MASK:
                break;
            default:
                break;
        }
        return true;
    }
    else
    {
        return false;
    }

}

void SIGBRD_GetMetrologyAll(meter_data_t *meter_data)
{
    if (meter_data == NULL)
    {
        return;
    }

    Host_AsyncSend(COMMAND_ID_GET_METROLOGY_ALL);

    meter_data->currentPh1 = mirror_config.current.data_phase1;
    meter_data->currentPh2 = mirror_config.current.data_phase2;
    meter_data->currentPh3  = mirror_config.current.data_phase3;
    meter_data->voltagePh1 = mirror_config.voltage.data_phase1;
    meter_data->voltagePh2 = mirror_config.voltage.data_phase2;
    meter_data->voltagePh3 = mirror_config.voltage.data_phase3;
    meter_data->PPh1 = mirror_config.active_power.data_phase1;
    meter_data->PPh2 = mirror_config.active_power.data_phase2;
    meter_data->PPh3 = mirror_config.active_power.data_phase3;
    meter_data->QPh1 = mirror_config.reactive_power.data_phase1;
    meter_data->QPh2 = mirror_config.reactive_power.data_phase2;
    meter_data->QPh3 = mirror_config.reactive_power.data_phase3;
    meter_data->SPh1 = mirror_config.apparent_power.data_phase1;
    meter_data->SPh2 = mirror_config.apparent_power.data_phase2;
    meter_data->SPh3 = mirror_config.apparent_power.data_phase3;
    meter_data->energy_varht = mirror_config.varh_t;
    meter_data->energy_wht = mirror_config.wh_t;
    if(mirror_config.power_direction == POWER_DIRECTION_GRID2_VEHICLE)
    {
        meter_data->power_direction = EVSE_G2V;
    }
    else if(mirror_config.power_direction == POWER_DIRECTION_VEHICLE2_GRID)
    {
        meter_data->power_direction = EVSE_V2G;
    }
    else
    {
        meter_data->power_direction = EVSE_G2V;
    }
}

void SIGBRD_GetPPState(uint16_t *pp_state)
{
    if(pp_state == NULL)
    {
        return;
    }

    Host_AsyncSend(COMMAND_ID_GET_PP_STATE);
    *pp_state = mirror_config.pp_state;
}

void SIGBRD_GetCPState(uint16_t *cp_state)
{
    if(cp_state == NULL)
    {
        return;
    }

    Host_AsyncSend(COMMAND_ID_GET_CP_STATE);
    *cp_state = mirror_config.cp_state;
}

void SIGBRD_GetGFCIState(uint16_t *gfci_state)
{
    if(gfci_state == NULL)
    {
        return;
    }

    Host_AsyncSend(COMMAND_ID_GET_GFCI_STATE);
    *gfci_state = mirror_config.gfci_state;
}

void SIGBRD_CloseRelay(void)
{
    relay_state = RELAY_STATE_RELAY_CLOSED;
    Host_AsyncSend(COMMAND_ID_SET_RELAY_STATE);
}

void SIGBRD_OpenRelay(void)
{
    relay_state = RELAY_STATE_RELAY_OPEN;
    Host_AsyncSend(COMMAND_ID_SET_RELAY_STATE);
}

/*
 * Public meter accessor functions.
 */
void SIGBRD_GetMeterCurrent(double *currentPh1, double *currentPh2, double *currentPh3)
{
    if((currentPh1 == NULL) && (currentPh2 == NULL) && (currentPh3 == NULL))
    {
        return;
    }

    Host_AsyncSend(COMMAND_ID_GET_PRESENT_CURRENT);

    if(currentPh1 != NULL)
    {
        *currentPh1 = (double)mirror_config.current.data_phase1;
    }
    if(currentPh2 != NULL)
    {
        *currentPh2 = (double)mirror_config.current.data_phase2;
    }
    if(currentPh3 != NULL)
    {
        *currentPh3 = (double)mirror_config.current.data_phase3;
    }

}

void SIGBRD_GetMeterVoltage(double *voltagePh1, double *voltagePh2, double *voltagePh3)
{
    if((voltagePh1 == NULL) && (voltagePh2 == NULL) && (voltagePh3 == NULL))
    {
        return;
    }

    Host_AsyncSend(COMMAND_ID_GET_PRESENT_VOLTAGE);

    if(voltagePh1 != NULL)
    {
        *voltagePh1 = (double)mirror_config.voltage.data_phase1;
    }
    if(voltagePh2 != NULL)
    {
        *voltagePh2 = (double)mirror_config.voltage.data_phase2;
    }
    if(voltagePh3 != NULL)
    {
        *voltagePh3 = (double)mirror_config.voltage.data_phase3;
    }
}

void SIGBRD_GetMeterActivePower(double *PPh1, double *PPh2, double *PPh3)
{
    if((PPh1 == NULL) && (PPh2 == NULL) && (PPh3 == NULL))
    {
        return;
    }

    Host_AsyncSend(COMMAND_ID_GET_PRESENT_ACTIVE_POWER);

    if(PPh1 != NULL)
    {
        *PPh1 = (double)mirror_config.active_power.data_phase1;
    }
    if(PPh2 != NULL)
    {
        *PPh2 = (double)mirror_config.active_power.data_phase2;
    }
    if(PPh3 != NULL)
    {
        *PPh3 = (double)mirror_config.active_power.data_phase3;
    }
}

void SIGBRD_GetMeterReactivePower(double *QPh1, double *QPh2, double *QPh3)
{
    if((QPh1 == NULL) && (QPh2 == NULL) && (QPh3 == NULL))
    {
        return;
    }

    Host_AsyncSend(COMMAND_ID_GET_PRESENT_REACTIVE_POWER);

    if(QPh1 != NULL)
    {
        *QPh1 = (double)mirror_config.reactive_power.data_phase1;
    }
    if(QPh2 != NULL)
    {
        *QPh2 = (double)mirror_config.reactive_power.data_phase2;
    }
    if(QPh3 != NULL)
    {
        *QPh3 = (double)mirror_config.reactive_power.data_phase3;
    }
}

void SIGBRD_GetMeterApparentPower(double *SPh1, double *SPh2, double *SPh3)
{
    if((SPh1 == NULL) && (SPh2 == NULL) && (SPh3 == NULL))
    {
        return;
    }

    Host_AsyncSend(COMMAND_ID_GET_PRESENT_APPARENT_POWER);

    if(SPh1 != NULL)
    {
        *SPh1 = (double)mirror_config.apparent_power.data_phase1;
    }
    if(SPh2 != NULL)
    {
        *SPh2 = (double)mirror_config.apparent_power.data_phase2;
    }
    if(SPh3 != NULL)
    {
        *SPh3 = (double)mirror_config.apparent_power.data_phase3;
    }
}

void SIGBRD_GetADCVal(uint32_t *adcValPtr)
{
    if(adcValPtr == NULL)
    {
        return;
    }
    Host_AsyncSend(COMMAND_ID_GET_CP_VALUE);
    *adcValPtr = mirror_config.cp_value;
}

void SIGBRD_SetPWMDutyInPercent(uint16_t dutyCycle)
{
    duty_cycle = dutyCycle;
    Host_AsyncSend(COMMAND_ID_SET_PWM);
}

void SIGBRD_SetPWMDutyInMilli(uint16_t dutyCycle)
{
    duty_cycle_ms = dutyCycle;
    Host_AsyncSend(COMMAND_ID_SET_PWM_MS);
}

void SIGBRD_GetPWMDutyInPercent(uint16_t *dutyVal)
{
    if(dutyVal == NULL)
    {
        return;
    }
    Host_AsyncSend(COMMAND_ID_GET_PWM);
    *dutyVal = mirror_config.pwm;
}

void SIGBRD_GetPWMDutyInMilli(uint16_t *dutyVal)
{
    if(dutyVal == NULL)
    {
        return;
    }
    Host_AsyncSend(COMMAND_ID_GET_PWM_MS);
    *dutyVal = mirror_config.pwm_ms;
}

void SIGBRD_GetSWVersion(uint32_t *sw_version_major, uint32_t *sw_version_minor, uint32_t *sw_version_hotfix)
{
    if((sw_version_major == NULL) || (sw_version_minor == NULL) || (sw_version_hotfix == NULL))
    {
        return;
    }
    Host_AsyncSend(COMMAND_ID_GET_SW_VERSION);
    *sw_version_major = mirror_config.sw_version_major;
    *sw_version_minor = mirror_config.sw_version_minor;
    *sw_version_hotfix = mirror_config.sw_version_hotfix;
}

void SIGBRD_GetHWVersion(uint32_t *hw_version)
{
    if (hw_version == NULL)
        return;

    Host_AsyncSend(COMMAND_ID_GET_HW_VERSION);

    *hw_version = mirror_config.hw_version;
}

void SIGBRD_GetRelayState(uint8_t *relay_state)
{
    if (relay_state == NULL)
        return;

    Host_AsyncSend(COMMAND_ID_GET_RELAY_STATE);

    *relay_state = mirror_config.relay_state;
}

void SIGBRD_SetChargingProtocol_TickDelayMs(uint32_t delay)
{
    iso15118_tick_delay_ms = delay;
}

void SIGBRD_UART_BridgeEntry(void)
{
    if (s_init == true)
    {
        return;
    }

    reset_sigbrd_state_machine();
#if EASYEVSE
    s_uart_sem = xSemaphoreCreateBinary();
    if (s_uart_sem == NULL)
    {
        configPRINTF((error("%s Failed to create binary sem"), __FUNCTION__));
        while (1)
            ;
    }

    xSemaphoreGive(s_uart_sem);  // Initialize to available state
    
    s_evse_uart_event = xEventGroupCreate();
    if (s_evse_uart_event == NULL)
    {
        configPRINTF((error("%s Failed to create event"), __FUNCTION__));
        while (1)
            ;
    }

    s_evse_emeter_event = xEventGroupCreate();
    if (s_evse_emeter_event == NULL)
        {
            configPRINTF((error("%s Failed to create event"), __FUNCTION__));
            while (1)
                ;
        }

    if (xTaskCreate(SIGBRD_UART_comm_task, "SIGBRD_UART_comm", APP_BRIDGE_MANAGER_STACK_SIZE, NULL,
            APP_BRIDGE_MANAGER_PRIORITY, NULL) != pdPASS)
    {
        configPRINTF((error("%s Failed to create task"), __FUNCTION__));
        while (1)
            ;
    }
#endif /* EASYEVSE */

    Host_Communication_Init();
    s_init = true;
}
